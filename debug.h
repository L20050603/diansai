/**
 * debug.h
 * 
 * 简单调试串口打印函数库
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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
 * %ld, %lu - 长整型
 * %% - 百分号
 */
void DEBUG_printf(const char* format, ...);

#endif /* DEBUG_H_ */ 