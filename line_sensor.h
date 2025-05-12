/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * 巡线传感器驱动头文件
 */

#ifndef LINE_SENSOR_H_
#define LINE_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "ti_msp_dl_config.h"

// 传感器状态结构体
// 传感器值：0表示检测到黑线，1表示未检测到黑线
typedef struct {
    uint8_t LineL1;  // 左一传感器
    uint8_t LineL2;  // 左二传感器
    uint8_t LineR1;  // 右一传感器
    uint8_t LineR2;  // 右二传感器
} LineSensor_State_t;

// 初始化巡线传感器
void LineSensor_init(void);

// 读取所有传感器状态
// 返回值：0表示检测到黑线，1表示未检测到黑线
LineSensor_State_t LineSensor_readAll(void);

// 读取单个传感器状态
// 返回值：0表示检测到黑线，1表示未检测到黑线
uint8_t LineSensor_readL1(void);
uint8_t LineSensor_readL2(void);
uint8_t LineSensor_readR1(void);
uint8_t LineSensor_readR2(void);

#endif /* LINE_SENSOR_H_ */ 