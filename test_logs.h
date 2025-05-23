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
#include "bsp_mpu6050.h"
#include "inv_mpu.h"

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

void mpu_6050_test(void);
//测试mpu并读取三个值


void test_mpu6050_sda(void);

/**
 * @brief MPU6050 SDA信号波形生成函数(示波器专用)
 * 
 * 生成清晰的I2C通信波形，专为示波器观测优化
 * 特点：
 * - 放慢I2C通信速度
 * - 重复执行标准I2C序列
 * - 包含简单的SDA信号高低电平切换测试
 * 使用方法：将示波器探头连接到SDA信号线
 */
void test_mpu6050_signal_wave(void);

/**
 * @brief SDA引脚状态检测函数
 * 
 * 详细诊断SDA引脚的各种状态和功能
 * 测试内容：
 * - 输入模式下的电平检测
 * - 输出模式下的控制能力
 * - 高低电平快速切换
 * - 模拟I2C起始和停止条件
 * 适合硬件调试与故障排除
 */
void test_sda_pin_state(void);

/**
 * @brief SCL引脚功能测试函数
 * 
 * 专门用于测试SCL引脚的输出功能
 * 测试内容：
 * - SCL引脚高低电平控制
 * - 直接GPIO功能测试
 * 适合排查SCL信号问题
 */
void test_scl_pin(void);

#endif /* TEST_LOGS_H_ */
