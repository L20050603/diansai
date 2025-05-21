/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * 测试和日志记录功能头文件
 */

#ifndef TEST_LOGS_H_
#define TEST_LOGS_H_

#include <stdint.h>
#include "ti_msp_dl_config.h"
#include "car_motor_control.h"
#include "debug.h"
#include "delay.h"
#include "line_sensor.h"
#include "encoder.h"

/**
 * @brief 电机控制测试程序
 * 
 * 执行一系列电机控制测试，包括：
 * - 加速和减速
 * - 转向（左转和右转）
 * - 原地旋转
 * - 后退
 * - 制动
 */
void test_motor_control(void);

/**
 * @brief 打印系统日志信息
 * 
 * 显示智能车的各种状态信息，包括：
 * - 巡线传感器状态
 * - 电机速度
 * - 电池电压
 * 等
 * 
 * @param sensorState 传感器状态结构体
 */
void logs(LineSensor_State_t sensorState);

/**
 * @brief 编码器测试程序
 * 
 * 测试编码器功能，包括：
 * - 脉冲计数
 * - 速度测量
 * - 方向检测
 * - RPM计算
 * - 距离测量
 * 
 * @param testDuration 测试持续时间（毫秒）
 */
void test_encoder(uint32_t testDuration);

#endif /* TEST_LOGS_H_ */
