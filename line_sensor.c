/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * 巡线传感器驱动实现
 */

#include "line_sensor.h"

// 传感器GPIO定义
// 根据syscfg文件中的配置，这些是PORTB上的引脚
#define LINE_SENSOR_PORT         GPIOB
#define LINE_SENSOR_L1_PIN       DL_GPIO_PIN_18  // 左一传感器，PB18
#define LINE_SENSOR_L2_PIN       DL_GPIO_PIN_19  // 左二传感器，PB19
#define LINE_SENSOR_R1_PIN       DL_GPIO_PIN_20  // 右一传感器，PB20
#define LINE_SENSOR_R2_PIN       DL_GPIO_PIN_24  // 右二传感器，PB24

// 初始化巡线传感器
void LineSensor_init(void)
{
    // GPIO已在syscfg中配置为输入，无需额外初始化
}

// 读取所有传感器状态
LineSensor_State_t LineSensor_readAll(void)
{
    LineSensor_State_t state;
    
    // 读取四个传感器状态 - 反转逻辑，0表示检测到黑线，1表示未检测到
    state.LineL1 = DL_GPIO_readPins(LINE_SENSOR_PORT, LINE_SENSOR_L1_PIN) > 0 ? 0 : 1; // 读取左一
    state.LineL2 = DL_GPIO_readPins(LINE_SENSOR_PORT, LINE_SENSOR_L2_PIN) > 0 ? 0 : 1; // 读取左二
    state.LineR1 = DL_GPIO_readPins(LINE_SENSOR_PORT, LINE_SENSOR_R1_PIN) > 0 ? 0 : 1; // 读取右一
    state.LineR2 = DL_GPIO_readPins(LINE_SENSOR_PORT, LINE_SENSOR_R2_PIN) > 0 ? 0 : 1; // 读取右二
    
    return state;
}

// 读取左一传感器状态
uint8_t LineSensor_readL1(void)
{
    return DL_GPIO_readPins(LINE_SENSOR_PORT, LINE_SENSOR_L1_PIN) > 0 ? 0 : 1;
}

// 读取左二传感器状态
uint8_t LineSensor_readL2(void)
{
    return DL_GPIO_readPins(LINE_SENSOR_PORT, LINE_SENSOR_L2_PIN) > 0 ? 0 : 1;
}

// 读取右一传感器状态
uint8_t LineSensor_readR1(void)
{
    return DL_GPIO_readPins(LINE_SENSOR_PORT, LINE_SENSOR_R1_PIN) > 0 ? 0 : 1;
}

// 读取右二传感器状态
uint8_t LineSensor_readR2(void)
{
    return DL_GPIO_readPins(LINE_SENSOR_PORT, LINE_SENSOR_R2_PIN) > 0 ? 0 : 1;
} 