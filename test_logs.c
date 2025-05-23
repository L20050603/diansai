/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * 测试和日志记录功能实现
 */

#include "test_logs.h"


#ifndef printf
#define printf DEBUG_printf
#endif

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
/*mpu6050测试函数，返回三个角
*函数内置死循环，此函数之后的内容都不会被运行
*/
void mpu_6050_test(){
	    uint8_t ret = 1;
      float pitch=0,roll=0,yaw=0;   //欧拉角

      printf("start\r\n");
      //DMP初始化
      while( mpu_dmp_init() )
      {
            printf("dmp error\r\n");
            Delay_ms(200);
      }

      printf("Initialization Data Succeed \r\n");
		  while(1){                                        //死循环
      //获取欧拉角
      if( mpu_dmp_get_data(&pitch,&roll,&yaw) == 0 )
            {
                  printf("\r\npitch =%d\r\n", (int)pitch);
                  printf("\r\nroll =%d\r\n", (int)roll);
                  printf("\r\nyaw =%d\r\n", (int)yaw);
            }

            Delay_ms(20);//根据设置的采样率，不可设置延时过大
      }
		}

/**
 * @brief MPU6050 SDA信号测试函数
 * 此函数用于测试MPU6050的SDA信号线功能是否正常
 * 适合与示波器一起使用观察SDA信号
 */
void test_mpu6050_sda(void)
{
    uint8_t id = 0;
    short accData[3] = {0};
    short gyroData[3] = {0};
    float temp = 0;
    uint8_t testResult = 0;
    
    printf("===== MPU6050 SDA信号测试开始 =====\r\n");
    
    // 1. 初始化MPU6050
    printf("初始化MPU6050...\r\n");
    if (MPU6050_Init() == 0)
    {
        printf("初始化失败！\r\n");
        testResult |= 0x01;
    }
    else
    {
        printf("初始化成功！\r\n");
    }
    
    Delay_ms(100);
    
    // 2. 读取设备ID (设备地址为0x68，ID寄存器为0x75)
    printf("读取MPU6050 ID...\r\n");
    if (MPU6050ReadID() != 0)
    {
        printf("读取ID失败！\r\n");
        testResult |= 0x02;
    }
    else
    {
        printf("ID读取成功！\r\n");
    }
    
    Delay_ms(100);
    
    // 3. 读取加速度传感器数据
    printf("读取加速度数据...\r\n");
    MPU6050ReadAcc(accData);
    printf("加速度数据: X=%d, Y=%d, Z=%d\r\n", accData[0], accData[1], accData[2]);
    
    Delay_ms(100);
    
    // 4. 读取陀螺仪数据
    printf("读取陀螺仪数据...\r\n");
    MPU6050ReadGyro(gyroData);
    printf("陀螺仪数据: X=%d, Y=%d, Z=%d\r\n", gyroData[0], gyroData[1], gyroData[2]);
    
    Delay_ms(100);
    
    // 5. 读取温度数据
    printf("读取温度数据...\r\n");
    temp = MPU6050_GetTemp();
    printf("温度: %.2f°C\r\n", temp);
    
    // 测试IIC发送和接收
    printf("\r\n测试IIC通信 - 读写寄存器...\r\n");
    
    // 尝试写入采样率寄存器 (SMPLRT_DIV，地址0x19)
    uint8_t rate = 0x07;  // 设置采样率为1000/(1+7) = 125Hz
    if (MPU6050_WriteReg(0x68, MPU6050_RA_SMPLRT_DIV, 1, &rate) != 0)
    {
        printf("写入采样率寄存器失败！\r\n");
        testResult |= 0x04;
    }
    else
    {
        printf("写入采样率寄存器成功！\r\n");
    }
    
    Delay_ms(100);
    
    // 读取刚设置的采样率寄存器
    uint8_t readRate = 0;
    if (MPU6050_ReadData(0x68, MPU6050_RA_SMPLRT_DIV, 1, &readRate) != 0)
    {
        printf("读取采样率寄存器失败！\r\n");
        testResult |= 0x08;
    }
    else
    {
        printf("读取采样率寄存器成功，值为: 0x%02X\r\n", readRate);
        if (readRate == rate)
        {
            printf("读写一致，SDA通信正常！\r\n");
        }
        else
        {
            printf("读写不一致，SDA通信异常！\r\n");
            testResult |= 0x10;
        }
    }
    
    // 总结测试结果
    printf("\r\n===== MPU6050 SDA信号测试结果 =====\r\n");
    if (testResult == 0)
    {
        printf("所有测试通过，SDA功能正常！\r\n");
    }
    else
    {
        printf("测试失败，错误代码: 0x%02X\r\n", testResult);
        printf("错误分析:\r\n");
        if (testResult & 0x01) printf("- 初始化失败\r\n");
        if (testResult & 0x02) printf("- 读取ID失败\r\n");
        if (testResult & 0x04) printf("- 写入寄存器失败\r\n");
        if (testResult & 0x08) printf("- 读取寄存器失败\r\n");
        if (testResult & 0x10) printf("- 读写值不一致\r\n");
    }
    
    printf("===== 测试结束 =====\r\n\r\n");
}

/**
 * @brief MPU6050 SDA信号波形生成函数(示波器专用)
 * 此函数用于生成清晰的I2C通信波形，方便用示波器观察SDA信号特性
 */
void test_mpu6050_signal_wave(void)
{
    uint8_t i;
    uint8_t dummy_data = 0xA5;  // 测试数据 10100101，有多次高低电平变化
    
    printf("生成SDA测试波形...\r\n");
    
    for (i = 0; i < 5; i++)
    {
        // 生成起始信号
        IIC_Start();
        Delay_ms(10);  // 延时，使示波器容易观察
        
        // 发送一个地址字节 (MPU6050地址 0x68 + 写位 0)
        Send_Byte(0x68 << 1);
        I2C_WaitAck();
        Delay_ms(10);
        
        // 发送寄存器地址 (WHO_AM_I寄存器地址 0x75)
        Send_Byte(0x75);
        I2C_WaitAck();
        Delay_ms(10);
        
        // 再次启动，准备读操作
        IIC_Start();
        Delay_ms(10);
        
        // 发送读地址
        Send_Byte((0x68 << 1) | 0x01);
        I2C_WaitAck();
        Delay_ms(10);
        
        // 读取数据
        dummy_data = Read_Byte();  // 读一个字节但不使用
        IIC_Send_Ack(1);  // 发送NACK
        Delay_ms(10);
        
        // 发送停止信号
        IIC_Stop();
        
        printf("完成波形生成循环 %d/5\r\n", i+1);
        Delay_ms(200);  // 每次循环间隔较大，方便观察
    }
    
    // 额外测试：单纯的SDA信号切换测试
    printf("\r\n开始SDA信号切换测试...\r\n");
    
    // 确保SDA为输出模式
    SDA_OUT();
    
    // 生成多个明显的SDA高低电平切换
    for (i = 0; i < 10; i++)
    {
        SDA(1);  // SDA高电平
        Delay_ms(50);
        SDA(0);  // SDA低电平
        Delay_ms(50);
    }
    
    printf("SDA信号测试完成!\r\n");
}

/**
 * @brief SDA引脚状态检测函数
 * 此函数用于检测SDA引脚的当前状态、输入/输出模式和切换能力
 */
void test_sda_pin_state(void)
{
    uint8_t i;

    
    printf("start\r\n");
    
    // 1. 测试SDA输入模式
    SDA_IN();  // 配置为输入模式
    Delay_ms(10);
    

    // 2. 测试SDA输出模式和控制能力
    SDA_OUT();  // 配置为输出模式
    
    printf("SDA 0\r\n");
    SDA(0);  // 输出低电平
    Delay_ms(50);
    
    printf("SDA output 1\r\n");
    SDA(1);  // 输出高电平
    Delay_ms(50);
    
    // 3. 测试SDA快速切换 (观察信号质量)
    printf("\r\n1 and 0\r\n");
    for (i = 0; i < 20; i++)
    {
        SDA(i % 2);  // 交替输出高低电平
        Delay_us(100);  // 使用较短延时
    }
    
    // 4. 模拟I2C总线上常见的START和STOP条件
    printf("\r\nstart shut\r\n");
    
    // START: SCL高电平时SDA从高到低
    printf("1100\r\n");
    SCL(1);  // SCL高电平
    SDA(1);  // SDA高电平
    Delay_ms(10);
    SDA(0);  // SDA拉低 (产生START条件)
    Delay_ms(10);
    SCL(0);  // SCL拉低
    
    Delay_ms(50);
    
    // STOP: SCL高电平时SDA从低到高
    printf("0011\r\n");
    SCL(0);  // SCL低电平
    SDA(0);  // SDA低电平
    Delay_ms(10);
    SCL(1);  // SCL拉高
    Delay_ms(10);
    SDA(1);  // SDA拉高 (产生STOP条件)
    
    Delay_ms(50);
    
    printf("\r\n SDA end \r\n");
}

/**
 * @brief 测试SCL引脚功能
 * 此函数专门测试SCL引脚的输出能力
 */
void test_scl_pin(void)
{
    uint8_t i;
    
    
    SDA_OUT();
    // 测试SCL引脚高低电平切换
    for (i = 0; i < 10; i++)
    {
        SCL(1);  // 设置高电平
        Delay_ms(500);  // 较长延时，便于观察示波器
        
        SCL(0);  // 设置低电平
        Delay_ms(500);
    }
    
    // 测试直接用GPIO函数控制
    for (i = 0; i < 5; i++)
    {
        DL_GPIO_setPins(GPIO_PORT, GPIO_SCL_PIN);
        Delay_ms(500);
        
        DL_GPIO_clearPins(GPIO_PORT, GPIO_SCL_PIN);
        Delay_ms(500);
    }
    
}

