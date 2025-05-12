/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * 基于SysTick的精确毫秒延时函数
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>

// 声明全局变量用于计数毫秒
extern volatile uint32_t g_systickCount;

// SysTick中断处理函数，需要在外部实现
void SysTick_Handler(void);

// 初始化延时函数
void Delay_init(void);

// 毫秒延时函数
void Delay_ms(uint32_t ms);

// 微秒延时函数（使用循环实现，精度较低）
void Delay_us(uint32_t us);

#endif /* DELAY_H_ */ 