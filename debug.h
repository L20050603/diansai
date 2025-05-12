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

/**
 * @brief 通过串口发送格式化字符串(printf风格)
 * 
 * @param format 格式化字符串
 * @param ... 可变参数列表
 */
void DEBUG_printf(const char* format, ...);

#endif /* DEBUG_H_ */ 