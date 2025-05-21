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
#define DEBUG_BUFFER_SIZE      256

/**
 * @brief 发送单个字符到串口
 * 
 * @param c 要发送的字符
 */
static void DEBUG_putchar(char c)
{
    DL_UART_transmitDataBlocking(DEBUG_UART_INSTANCE, c);
}

/**
 * @brief 发送字符串到串口
 * 
 * @param str 要发送的字符串
 */
static void DEBUG_puts(const char* str)
{
    while (*str) {
        DEBUG_putchar(*str++);
    }
}

/**
 * @brief 发送十进制整数到串口
 * 
 * @param num 要发送的整数
 * @param is_signed 是否为有符号数
 */
static void DEBUG_print_int(long num, bool is_signed)
{
    char buffer[12]; // 足够存储32位整数及符号
    int i = 0;
    bool negative = false;
    
    // 处理0特殊情况
    if (num == 0) {
        DEBUG_putchar('0');
        return;
    }
    
    // 处理负数
    if (is_signed && num < 0) {
        negative = true;
        num = -num;
    }
    
    // 转换为字符串（倒序）
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // 添加负号
    if (negative) {
        DEBUG_putchar('-');
    }
    
    // 倒序输出
    while (--i >= 0) {
        DEBUG_putchar(buffer[i]);
    }
}

/**
 * @brief 发送无符号整数到串口
 * 
 * @param num 要发送的无符号整数
 */
static void DEBUG_print_uint(unsigned long num)
{
    DEBUG_print_int((long)num, false);
}

/**
 * @brief 发送十六进制数到串口
 * 
 * @param num 要发送的十六进制数
 */
static void DEBUG_print_hex(unsigned long num)
{
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[8]; // 32位数需要8个十六进制字符
    int i = 0;
    
    // 处理0特殊情况
    if (num == 0) {
        DEBUG_puts("0x0");
        return;
    }
    
    // 转换为十六进制字符串（倒序）
    while (num > 0 && i < 8) {
        buffer[i++] = hex_chars[num & 0xF];
        num >>= 4;
    }
    
    // 添加0x前缀
    DEBUG_puts("0x");
    
    // 倒序输出
    while (--i >= 0) {
        DEBUG_putchar(buffer[i]);
    }
}

/**
 * @brief 发送浮点数到串口（简化版，仅支持2位小数）
 * 
 * @param num 要发送的浮点数
 */
static void DEBUG_print_float(float num)
{
    // 处理负数
    if (num < 0) {
        DEBUG_putchar('-');
        num = -num;
    }
    
    // 整数部分
    long int_part = (long)num;
    DEBUG_print_int(int_part, false);
    
    // 小数部分（仅保留2位）
    DEBUG_putchar('.');
    long decimal_part = (long)((num - int_part) * 100 + 0.5); // 四舍五入到2位小数
    
    // 确保显示两位小数
    if (decimal_part < 10) {
        DEBUG_putchar('0');
    }
    DEBUG_print_int(decimal_part, false);
}

/**
 * @brief 通过串口发送格式化字符串(简化版printf)
 * 
 * @param format 格式化字符串
 * @param ... 可变参数列表
 *
 * 支持的格式说明符:
 * %c - 字符
 * %s - 字符串
 * %d - 有符号整数
 * %u - 无符号整数
 * %x - 十六进制数
 * %f - 浮点数（简化版，仅支持2位小数）
 * %% - 百分号
 */
void DEBUG_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    while (*format) {
        if (*format == '%') {
            format++;
            
            // 检查格式说明符
            switch (*format) {
                case 'c':
                    DEBUG_putchar((char)va_arg(args, int));
                    break;
                case 's':
                    DEBUG_puts(va_arg(args, const char*));
                    break;
                case 'd':
                    DEBUG_print_int(va_arg(args, int), true);
                    break;
                case 'u':
                    DEBUG_print_uint(va_arg(args, unsigned int));
                    break;
                case 'x':
                    DEBUG_print_hex(va_arg(args, unsigned int));
                    break;
                case 'l':
                    format++;
                    if (*format == 'd') {
                        DEBUG_print_int(va_arg(args, long), true);
                    } else if (*format == 'u') {
                        DEBUG_print_uint(va_arg(args, unsigned long));
                    } else {
                        DEBUG_putchar('%');
                        DEBUG_putchar('l');
                        DEBUG_putchar(*format);
                    }
                    break;
                case 'f':
                    DEBUG_print_float((float)va_arg(args, double));
                    break;
                case '%':
                    DEBUG_putchar('%');
                    break;
                default:
                    DEBUG_putchar('%');
                    DEBUG_putchar(*format);
                    break;
            }
        } else {
            DEBUG_putchar(*format);
    }
        
        format++;
    }
    
    va_end(args);
} 