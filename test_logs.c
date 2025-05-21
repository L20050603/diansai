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
    //printf("智能车调试信息\r\n");
    printf("Line Sensors: L1=%d, L2=%d, R1=%d, R2=%d (0=Black Line Detected)\r\n", 
           sensorState.LineL1, sensorState.LineL2, 
           sensorState.LineR1, sensorState.LineR2);
}

// 全局变量用于测试
static const float wheelDiameter = 6.5f;
static uint32_t startTime = 0;
static uint32_t lastSampleTime = 0;
static uint32_t sampleInterval = 500; // 每500ms打印一次数据

/**
 * 初始化编码器测试
 * 重置计数器并设置起始时间
 */
static void encoder_test_init(void)
{
    
    // 确保编码器已经正确初始化
    printf("0\r\n");
    NVIC_DisableIRQ(encoder_INT_IRQN);
    Delay_ms(10);
    NVIC_EnableIRQ(encoder_INT_IRQN);
    printf("1\r\n");
    
    // 重置编码器计数
    Encoder_resetAll();
    printf("2\r\n");
    
    // 设置测试开始时间
    startTime = Delay_getTicks();
    lastSampleTime = startTime;
}

/**
 * 启动电机并运行一小段时间
 * @return 电机是否成功启动
 */
static bool encoder_test_start_motor(void)
{
    
    // 先确保电机停止
    Motor_stop();
    Delay_ms(500);
    
    // 尝试启动电机
    Motor_forward(50);
    printf("Motor start\r\n");
    
    // 等待一小段时间确认电机启动
    Delay_ms(1000);
    
    // 更新编码器速度
    Encoder_updateSpeed();
    
    // 检查是否有编码器脉冲生成（电机是否确实运转）
    int32_t leftPulses = Encoder_getPulseCount(ENCODER_LEFT);
    int32_t rightPulses = Encoder_getPulseCount(ENCODER_RIGHT);
    
    printf("Left=%ld, Right=%ld\r\n", leftPulses, rightPulses);
    
    // 如果两个编码器都没有检测到脉冲，可能电机未启动
    return (leftPulses > 0 || rightPulses > 0);
}

/**
 * 显示编码器数据
 */
static void encoder_test_display_data(void)
{
    // 获取左轮编码器数据
    int32_t leftPulses = Encoder_getPulseCount(ENCODER_LEFT);
    int16_t leftSpeed = Encoder_getSpeed(ENCODER_LEFT);
    uint16_t leftRPM = Encoder_getRPM(ENCODER_LEFT);
    float leftDistance = Encoder_getDistance(ENCODER_LEFT, wheelDiameter);
    bool leftDirection = Encoder_getDirection(ENCODER_LEFT);
    
    // 获取右轮编码器数据
    int32_t rightPulses = Encoder_getPulseCount(ENCODER_RIGHT);
    int16_t rightSpeed = Encoder_getSpeed(ENCODER_RIGHT);
    uint16_t rightRPM = Encoder_getRPM(ENCODER_RIGHT);
    float rightDistance = Encoder_getDistance(ENCODER_RIGHT, wheelDiameter);
    bool rightDirection = Encoder_getDirection(ENCODER_RIGHT);
    
    // 打印测试数据
    printf("Time: %lu ms\r\n", Delay_getTicks() - startTime);
    printf("Left wheel: Pulses=%ld, Speed=%d p/s, RPM=%u, Direction=%s, Distance=%.2f cm\r\n",
           leftPulses, leftSpeed, leftRPM, 
           leftDirection ? "Forward" : "Reverse", leftDistance);
    printf("Right wheel: Pulses=%ld, Speed=%d p/s, RPM=%u, Direction=%s, Distance=%.2f cm\r\n",
           rightPulses, rightSpeed, rightRPM,
           rightDirection ? "Forward" : "Reverse", rightDistance);
}

/**
 * 运行编码器监控测试
 * @param duration 测试持续时间（毫秒）
 */
static void encoder_test_monitor(uint32_t duration)
{
    uint32_t endTime = startTime + duration;
    
    while (Delay_getTicks() < endTime) {
        // 更新编码器速度
        Encoder_updateSpeed();
        
        // 每隔sampleInterval打印一次数据
        if (Delay_getTicks() - lastSampleTime >= sampleInterval) {
            encoder_test_display_data();
            
            // 更新上次采样时间
            lastSampleTime = Delay_getTicks();
        }
        
        // 短暂延时，避免过度占用CPU
        Delay_ms(10);
    }
}

/**
 * 结束编码器测试
 */
static void encoder_test_cleanup(void)
{
    // 停止电机
    Motor_stop();
    
    // 最后再采集一次数据
    encoder_test_display_data();
    
    // 测试结束
    printf("===== ENCODER TEST END =====\r\n\r\n");
}

/**
 * 编码器测试程序
 * @param testDuration 测试持续时间（毫秒）
 */
void test_encoder(uint32_t testDuration)
{
    // 1. 初始化测试
    encoder_test_init();
    
    // 2. 启动电机
    bool motorStarted = encoder_test_start_motor();
    
    // 检查电机是否启动成功
    if (!motorStarted) {
        printf("WARNING: Motor may not have started or encoder didn't detect pulses!\r\n");
        printf("Trying to increase motor speed...\r\n");
        
        // 尝试增加速度
        Motor_forward(70);
        Delay_ms(1000);
        
        // 再次检查
        Encoder_updateSpeed();
        int32_t leftPulses = Encoder_getPulseCount(ENCODER_LEFT);
        int32_t rightPulses = Encoder_getPulseCount(ENCODER_RIGHT);
        
        if (leftPulses == 0 && rightPulses == 0) {
            printf("Motor still not running or encoder not responding, please check hardware connections\r\n");
            // 尽管电机可能未启动，我们还是继续测试，以便检测可能的问题
        } else {
            printf("Motor is running, continuing test...\r\n");
        }
    }
    
    // 3. 运行测试监控
    encoder_test_monitor(testDuration);
    
    // 4. 清理和结束测试
    encoder_test_cleanup();
}