/**
 * debug.c
 * 
 * 简单调试串口打印函数库的实现
 */

#include "debug.h"
#include "ti_msp_dl_config.h"  // 包含TI驱动库配置

// 定义用于调试的UART模块实例
// 注：根据您的系统配置，这里可能需要调整为实际使用的UART实例
#define DEBUG_UART_INSTANCE    UART0

// 定义缓冲区大小
#define DEBUG_BUFFER_SIZE      128

/**
 * @brief 通过串口发送格式化字符串(printf风格)
 * 
 * @param format 格式化字符串
 * @param ... 可变参数列表
 */
void DEBUG_printf(const char* format, ...)
{
    char buffer[DEBUG_BUFFER_SIZE];
    va_list args;
    
    // 格式化字符串
    va_start(args, format);
    vsnprintf(buffer, DEBUG_BUFFER_SIZE, format, args);
    va_end(args);
    
    // 通过UART发送格式化后的字符串
    for (uint16_t i = 0; buffer[i] != '\0'; i++)
    {
        DL_UART_transmitDataBlocking(DEBUG_UART_INSTANCE, buffer[i]);
    }
} 