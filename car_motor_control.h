/**
 * car_motor_control.h
 * 
 * 基于DRV8833驱动芯片的小车PWM控制模块
 * 使用TI MSP430 driverlib库控制电机
 * 
 * 功能：提供小车运动控制函数，包括前进、后退、转向和停止
 * 
 * 说明：
 * - 使用DRV8833驱动芯片控制两个电机
 * - A口控制右轮，B口控制左轮
 * - 采用差速控制实现转向
 * - 针对竞速要求，电机默认以高速运行
 */

#ifndef CAR_MOTOR_CONTROL_H_
#define CAR_MOTOR_CONTROL_H_

#include <stdbool.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"

/**
 * 初始化电机控制模块
 * 
 * 注意：在调用任何电机控制函数前必须先调用此函数
 */
void Motor_init(void);

/**
 * 设置电机速度
 * 
 * @param leftSpeed  左轮速度 (0-100)
 * @param rightSpeed 右轮速度 (0-100)
 */
void Motor_setSpeed(int16_t leftSpeed, int16_t rightSpeed);

/**
 * 小车前进
 * 
 * @param speed 速度百分比 (0-100)
 */
void Motor_forward(uint8_t speed);

/**
 * 小车后退
 * 
 * @param speed 速度百分比 (0-100)
 */
void Motor_backward(uint8_t speed);

/**
 * 小车左转
 * 
 * @param speed 基础速度百分比 (0-100)
 * @param turnRatio 转弯比例 (0-100)，数值越大转弯越急
 */
void Motor_turnLeft(uint8_t speed, uint8_t turnRatio);

/**
 * 小车右转
 * 
 * @param speed 基础速度百分比 (0-100)
 * @param turnRatio 转弯比例 (0-100)，数值越大转弯越急
 */
void Motor_turnRight(uint8_t speed, uint8_t turnRatio);

/**
 * 小车原地左转
 * 
 * @param speed 速度百分比 (0-100)
 */
void Motor_rotateLeft(uint8_t speed);

/**
 * 小车原地右转
 * 
 * @param speed 速度百分比 (0-100)
 */
void Motor_rotateRight(uint8_t speed);

/**
 * 小车停止
 * 
 * 立即停止所有电机
 */
void Motor_stop(void);

/**
 * 小车刹车
 * 
 * 使用电气制动快速停止
 */
void Motor_brake(void);

/**
 * 小车直线行走闭环控制
 * 
 * 使用IMU数据进行闭环控制，使小车保持直线行走
 * 
 * @param speed 基础速度百分比 (0-100)
 * @param kp 比例系数，控制修正的强度
 */
void Motor_straightLine(uint8_t speed, float kp);

#endif /* CAR_MOTOR_CONTROL_H_ */ 