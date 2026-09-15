#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ============================================================
 *                       可配置宏定义
 * ============================================================ */
#define WIFI_SSID           "CMCC-79Ja"
#define WIFI_PWD            "fd8cy37a"
#define SERVER_IP           "192.168.1.5"
#define SERVER_PORT         "8088"

/* 每个状态的超时时间（单位 ms） */
#define TIMEOUT_AT_MS       3000
#define TIMEOUT_CWJAP_MS    15000
#define TIMEOUT_CIPSTART_MS 8000
#define TIMEOUT_RECV_MS     30000

/* 清理缓冲区大小 */
#define CLEAN_BUF_SIZE      128

/* ============================================================
 *                       全局变量
 * ============================================================ */
volatile int RECFlage = 0;              /* 收到一帧数据标志 */
volatile uint16_t currentLen = 0;       /* 当前帧长度 */

/* ⚠️ 请确认 RecMessage 的定义（在 Serial.c/h 中）：
 *    它应该至少是 uint8_t RecMessage[MAX_BUFFER + 1];
 *    多留 1 字节是为了补 '\0'，避免 strlen 越界。
 *    如果它只有 MAX_BUFFER 大小，请去改大一点。
 */
extern uint8_t RecMessage[];

/* ============================================================
 *                       状态机定义
 * ============================================================ */
typedef enum {
    WIFI_ST_AT_SEND,            /* 发送 AT 测试 */
    WIFI_ST_AT_WAIT,            /* 等待 OK */

    WIFI_ST_CWJAP_SEND,         /* 发送 AT+CWJAP 连接 WiFi */
    WIFI_ST_CWJAP_WAIT,         /* 等待 GOT IP */

    WIFI_ST_CIPSTART_SEND,      /* 发送 AT+CIPSTART 连接 TCP */
    WIFI_ST_CIPSTART_WAIT,      /* 等待 CONNECT */

    WIFI_ST_TCP_RECV,           /* TCP 已连接，接收数据 */

    WIFI_ST_ERROR               /* 错误状态（可用于重试） */
} WifiState;

static WifiState status = WIFI_ST_AT_SEND;
static uint32_t stateTick = 0;          /* 进入当前状态的时间戳 */

/* OLED 非阻塞显示用 */
static char oledLine2[17] = {0};        /* OLED 一行最多 16 字符 */
static uint32_t oledClearTick = 0;
static bool oledNeedClear = false;

/* ============================================================
 *                       工具函数
 * ============================================================ */

/* 判断 src 中是否包含 sub（替代你原来的 compareStr） */
static bool str_contains(const char* src, const char* sub)
{
    if (src == NULL || sub == NULL) return false;
    return strstr(src, sub) != NULL;
}

/* 去除 \r \n，写入调用方提供的缓冲区（避免 malloc 泄漏） */
static void get_message(const char* response, char* clean, uint16_t cleanSize)
{
    uint16_t j = 0;
    if (response == NULL || clean == NULL || cleanSize == 0) return;

    for (uint16_t i = 0; response[i] != '\0' && j < cleanSize - 1; i++) {
        if (response[i] != '\r' && response[i] != '\n') {
            clean[j++] = response[i];
        }
    }
    clean[j] = '\0';
}

/* 获取系统滴答（假设你用的是标准库的 SysTick 或 HAL） */
static uint32_t get_tick(void)
{
    /* ⚠️ 如果你有 HAL，直接用 HAL_GetTick()；
     *    如果是标准库，可以用自己写的毫秒计数，或者用 Delay 里的变量。
     *    这里给出一个最通用的写法：直接用 SysTick 计数器。
     */
    return (uint32_t)(SysTick->VAL);    /* 注意：这是递减的，仅作演示 */
    /* 更推荐：return HAL_GetTick(); */
}

/* 进入某个状态时记录时间戳 */
static void enter_state(WifiState st)
{
    status = st;
    stateTick = get_tick();
}

/* ============================================================
 *                       串口空闲中断（DMA 接收）
 * ============================================================ */
void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
    {
        volatile uint32_t tmp;
        tmp = USART2->SR;      /* 先读 SR */
        tmp = USART2->DR;      /* 再读 DR，才能清 IDLE */
        (void)tmp;

        /* 停止 DMA */
        DMA_Cmd(DMA1_Channel6, DISABLE);

        uint16_t len = MAX_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel6);
        currentLen = len;

        /* ⚠️ 关键：补 '\0'，否则 strlen 会越界读！
         *    前提是 RecMessage 大小 >= MAX_BUFFER + 1
         */
        RecMessage[len] = '\0';

        /* 重置 DMA */
        DMA_SetCurrDataCounter(DMA1_Channel6, MAX_BUFFER);
        DMA_Cmd(DMA1_Channel6, ENABLE);

        RECFlage = 1;
    }
}

/* ============================================================
 *                       状态机处理函数
 * ============================================================ */
static void process_frame(void)
{
    char clean[CLEAN_BUF_SIZE];

    get_message((char*)RecMessage, clean, sizeof(clean));

    /* 串口数据都打印到 OLED 第二行（可选，调试用） */
    /* OLED_ShowString(2, 1, clean); */

    switch (status)
    {
    /* ---------- 1. AT 测试 ---------- */
    case WIFI_ST_AT_SEND:
        SendString("ATE0\r\n");     /* 关回显 */
        SendString("AT\r\n");
        enter_state(WIFI_ST_AT_WAIT);
        break;

    case WIFI_ST_AT_WAIT:
        if (str_contains(clean, "OK")) {
            OLED_ShowString(1, 1, "AT OK           ");
            enter_state(WIFI_ST_CWJAP_SEND);
        } else if (get_tick() - stateTick > TIMEOUT_AT_MS) {
            enter_state(WIFI_ST_AT_SEND);   /* 超时重试 */
        }
        break;

    /* ---------- 2. 连接 WiFi ---------- */
    case WIFI_ST_CWJAP_SEND:
        OLED_ShowString(1, 1, "WIFI CONNECTING ");
        SendString("AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PWD "\"\r\n");
        enter_state(WIFI_ST_CWJAP_WAIT);
        break;

    case WIFI_ST_CWJAP_WAIT:
        if (str_contains(clean, "GOT IP")) {
            OLED_ShowString(1, 1, "WIFI CONNECTED  ");
            enter_state(WIFI_ST_CIPSTART_SEND);
        } else if (str_contains(clean, "FAIL")
                || get_tick() - stateTick > TIMEOUT_CWJAP_MS) {
            enter_state(WIFI_ST_CWJAP_SEND);   /* 失败或超时重试 */
        }
        break;

    /* ---------- 3. 连接 TCP ---------- */
    case WIFI_ST_CIPSTART_SEND:
        OLED_ShowString(1, 1, "TCP CONNECTING  ");
        SendString("AT+CIPSTART=\"TCP\",\"" SERVER_IP "\"," SERVER_PORT "\r\n");
        enter_state(WIFI_ST_CIPSTART_WAIT);
        break;

    case WIFI_ST_CIPSTART_WAIT:
        if (str_contains(clean, "CONNECT")) {
            OLED_ShowString(1, 1, "TCP CONNECTED   ");
            enter_state(WIFI_ST_TCP_RECV);
        } else if (str_contains(clean, "ERROR")
                || get_tick() - stateTick > TIMEOUT_CIPSTART_MS) {
            enter_state(WIFI_ST_CIPSTART_SEND);
        }
        break;

    /* ---------- 4. TCP 数据接收 ---------- */
    case WIFI_ST_TCP_RECV:
        if (str_contains(clean, "CLOSED")) {
            /* 连接断开，重连 */
            OLED_ShowString(1, 1, "TCP CLOSED      ");
            OLED_ShowString(3, 1, "RECONNECTING... ");
            enter_state(WIFI_ST_CIPSTART_SEND);
        }
        else if (str_contains(clean, "+IPD")) {
            /* 收到数据，显示在第二行（非阻塞） */
            strncpy(oledLine2, clean, sizeof(oledLine2) - 1);
            oledLine2[sizeof(oledLine2) - 1] = '\0';
            OLED_ShowString(2, 1, oledLine2);

            /* 记录清除时间，1 秒后清屏（不用 Delay 卡死主循环） */
            oledClearTick = get_tick();
            oledNeedClear = true;
        }
        else {
            /* 其他数据也可以显示，调试用 */
            strncpy(oledLine2, clean, sizeof(oledLine2) - 1);
            oledLine2[sizeof(oledLine2) - 1] = '\0';
            OLED_ShowString(2, 1, oledLine2);
        }
        break;

    /* ---------- 错误状态 ---------- */
    case WIFI_ST_ERROR:
    default:
        OLED_ShowString(1, 1, "ERROR           ");
        enter_state(WIFI_ST_AT_SEND);   /* 简单粗暴：从头重来 */
        break;
    }
}

/* ============================================================
 *                       主函数
 * ============================================================ */
int main(void)
{
    /* 初始化 */
    OLED_Init();
    Serial_Init();

    OLED_ShowString(1, 1, "SYSTEM START    ");

    /* 启动状态机 */
    enter_state(WIFI_ST_AT_SEND);

    while (1)
    {
        /* ---------- 1. 处理串口收到的数据 ---------- */
        if (RECFlage)
        {
            RECFlage = 0;
            process_frame();
            memset((char*)RecMessage, 0, MAX_BUFFER);
        }

        /* ---------- 2. 非阻塞清除 OLED 第二行 ---------- */
        if (oledNeedClear && (get_tick() - oledClearTick > 1000))
        {
            oledNeedClear = false;
            OLED_ShowString(2, 1, "                ");
        }

        /* ---------- 3. 这里可以放其他任务 ---------- */
    }
}