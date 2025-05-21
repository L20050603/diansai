/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * 基于SysTick的精确毫秒延时函数实现
 */

#include "delay.h"
#include "ti_msp_dl_config.h"

// 全局变量用于计数毫秒
volatile uint32_t g_systickCount = 0;

// SysTick中断处理函数
void SysTick_Handler(void)
{
    // 毫秒计数器递增
    g_systickCount++;
}

// 初始化延时函数
void Delay_init(void)
{
    // SysTick已在syscfg中配置，无需额外初始化
    // 系统时钟频率为32MHz，SysTick周期设置为32000，每毫秒触发一次中断
    g_systickCount = 0;
}

// 毫秒延时函数
void Delay_ms(uint32_t ms)
{
    uint32_t startTick = g_systickCount;
    
    // 等待指定的毫秒数
    while ((g_systickCount - startTick) < ms)
    {
        // 等待SysTick中断更新g_systickCount
        __WFE(); // 等待事件（低功耗模式）
    }
}

// 微秒延时函数（使用循环实现，精度较低）
void Delay_us(uint32_t us)
{
    // 系统时钟频率为32MHz，每个循环大约需要几个时钟周期
    // 这个系数需要根据实际情况调整
    uint32_t cycles = us * (CPUCLK_FREQ / 1000000) / 4;
    
    // 简单的循环延时
    volatile uint32_t i;
    for (i = 0; i < cycles; i++)
    {
        // 空循环
        __NOP(); // 无操作指令，防止编译器优化
    }
}

// 获取当前系统时钟计数（毫秒）
uint32_t Delay_getTicks(void)
{
    return g_systickCount;
} 