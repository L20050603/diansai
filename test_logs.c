/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * 测试和日志记录功能实现
 */

#include "test_logs.h"


#define printf DEBUG_printf

// 电机控制测试程序
void test_motor_control(void)
{
    // 前进测试 - 逐渐加速
    for (uint8_t speed = 20; speed <= 80; speed += 5) {
        Motor_forward(speed);
        Delay_ms(500);
    }

    // 保持最高速前进一段时间
    Motor_forward(80);
    Delay_ms(5000);

    // 减速并停止
    Motor_forward(50);
    Delay_ms(1000);
    Motor_forward(20);
    Delay_ms(1000);
    Motor_stop();
    Delay_ms(1000);

    // 左转测试
    Motor_forward(60);
    Delay_ms(1000);
    Motor_turnLeft(60, 50); // 中等转弯
    Delay_ms(1500);
    Motor_forward(60);
    Delay_ms(1000);

    // 右转测试
    Motor_turnRight(60, 50); // 中等转弯
    Delay_ms(1500);
    Motor_forward(60);
    Delay_ms(1000);

    // 原地旋转测试
    Motor_stop();
    Delay_ms(500);
    Motor_rotateLeft(40);
    Delay_ms(1000);
    Motor_rotateRight(40);
    Delay_ms(1000);

    // 后退测试
    Motor_stop();
    Delay_ms(500);
    Motor_backward(50);
    Delay_ms(2000);

    // 电气制动测试
    Motor_brake();
    Delay_ms(1000);

    // 重新开始测试序列前延停
    Delay_ms(2000);
}

// 打印系统日志信息
void logs(LineSensor_State_t sensorState)
{
    printf("智能车调试信息\r\n");
    printf("巡线传感器: L1=%d, L2=%d, R1=%d, R2=%d (0=检测到黑线)\r\n", 
           sensorState.LineL1, sensorState.LineL2, 
           sensorState.LineR1, sensorState.LineR2);
}