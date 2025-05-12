/**
 * car_motor_control.c
 * 
 * 基于DRV8833驱动芯片的小车PWM控制模块
 * 使用TI MSP430 driverlib库控制电机
 * 
 * 实现car_motor_control.h中定义的功能
 */

#include "car_motor_control.h"

// 根据syscfg配置的PWM信号定义
// PWM_bin1 (PWM1) -> 右轮 IN1 (TIMA1 CCP0 - PA17)*
// PWM_bin2 (PWM2) -> 右轮 IN2 (TIMA0 CCP2 - PA15)
// PWM_ain1 (PWM3) -> 左轮 IN1 (TIMG6 CCP0 - PA21)*
// PWM_ain2 (PWM4) -> 左轮 IN2 (TIMG0 CCP1 - PA24)

// 定义PWM周期值 (最大值)
#define PWM_PERIOD 1000

// 定义PWM模块和通道
// 右轮控制信号 (TimerA)
#define RIGHT_IN1_TIMER           PWM_bin1_INST
#define RIGHT_IN1_CHANNEL         GPIO_PWM_bin1_C0_IDX
#define RIGHT_IN2_TIMER           PWM_bin2_INST
#define RIGHT_IN2_CHANNEL         GPIO_PWM_bin2_C2_IDX

// 左轮控制信号 (TimerG)
#define LEFT_IN1_TIMER            PWM_ain1_INST
#define LEFT_IN1_CHANNEL          GPIO_PWM_ain1_C0_IDX
#define LEFT_IN2_TIMER            PWM_ain2_INST
#define LEFT_IN2_CHANNEL          GPIO_PWM_ain2_C1_IDX

// 用于标识定时器类型的枚举
typedef enum {
    TIMER_TYPE_A,
    TIMER_TYPE_G
} TimerType;

// 内部函数声明
static void setPWMDuty(void *timer, uint32_t channel, uint16_t dutyValue, TimerType type);
static uint16_t getPWMDuty(void *timer, uint32_t channel, TimerType type);

/**
 * 初始化电机控制模块
 */
void Motor_init(void)
{
    // 假设PWM外设已经由syscfg配置和初始化，这里只需确保初始状态
    
    // 初始状态：停止/惯性滑行 (Coast)
    // DRV8833真值表: xIN1=0, xIN2=0 -> Coast
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, 0, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, 0, TIMER_TYPE_A);
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, 0, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, 0, TIMER_TYPE_G);
}

/**
 * 设置电机速度
 * 
 * @param leftSpeed  左轮速度 (0-100)
 * @param rightSpeed 右轮速度 (0-100)
 */
void Motor_setSpeed(uint8_t leftSpeed, uint8_t rightSpeed)
{
    // 确保速度在有效范围内
    if (leftSpeed > 100)
        leftSpeed = 100;
    
    if (rightSpeed > 100)
        rightSpeed = 100;
    
    // 计算PWM占空比值
    uint16_t leftDuty = (uint16_t)((uint32_t)leftSpeed * PWM_PERIOD / 100);
    uint16_t rightDuty = (uint16_t)((uint32_t)rightSpeed * PWM_PERIOD / 100);
    
    // 获取当前电机方向状态
    uint16_t rightIN1 = getPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, TIMER_TYPE_A);
    uint16_t rightIN2 = getPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, TIMER_TYPE_A);
    uint16_t leftIN1 = getPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, TIMER_TYPE_G);
    uint16_t leftIN2 = getPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, TIMER_TYPE_G);
    
    // 更新PWM占空比，保持电机方向不变
    if (rightIN1 > 0 && rightIN2 == 0) {
        // 右轮向前
        setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, rightDuty, TIMER_TYPE_A);
    } else if (rightIN1 == 0 && rightIN2 > 0) {
        // 右轮向后
        setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, rightDuty, TIMER_TYPE_A);
    }
    
    if (leftIN1 > 0 && leftIN2 == 0) {
        // 左轮向前
        setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, leftDuty, TIMER_TYPE_G);
    } else if (leftIN1 == 0 && leftIN2 > 0) {
        // 左轮向后
        setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, leftDuty, TIMER_TYPE_G);
    }
}

/**
 * 小车前进
 * 
 * @param speed 速度百分比 (0-100)
 */
void Motor_forward(uint8_t speed)
{
    // 确保速度在有效范围内
    if (speed > 100)
        speed = 100;
    
    // 计算PWM占空比值
    uint16_t duty = (uint16_t)((uint32_t)speed * PWM_PERIOD / 100);
    
    // DRV8833真值表: xIN1=1, xIN2=0 -> Forward
    // 右轮前进
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, duty, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, 0, TIMER_TYPE_A);
    
    // 左轮前进
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, duty, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, 0, TIMER_TYPE_G);
}

/**
 * 小车后退
 * 
 * @param speed 速度百分比 (0-100)
 */
void Motor_backward(uint8_t speed)
{
    // 确保速度在有效范围内
    if (speed > 100)
        speed = 100;
    
    // 计算PWM占空比值
    uint16_t duty = (uint16_t)((uint32_t)speed * PWM_PERIOD / 100);
    
    // DRV8833真值表: xIN1=0, xIN2=1 -> Reverse
    // 右轮后退
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, 0, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, duty, TIMER_TYPE_A);
    
    // 左轮后退
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, 0, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, duty, TIMER_TYPE_G);
}

/**
 * 小车左转 (差速转向)
 * 
 * @param speed 基础速度百分比 (0-100)
 * @param turnRatio 转弯比例 (0-100)，数值越大转弯越急
 */
void Motor_turnLeft(uint8_t speed, uint8_t turnRatio)
{
    // 确保参数在有效范围内
    if (speed > 100)
        speed = 100;
    
    if (turnRatio > 100)
        turnRatio = 100;
    
    // 计算差速：左轮速度降低，右轮保持原速
    uint8_t rightSpeed = speed;
    uint8_t leftSpeed = speed - (speed * turnRatio / 100);
    
    // 计算PWM占空比值
    uint16_t rightDuty = (uint16_t)((uint32_t)rightSpeed * PWM_PERIOD / 100);
    uint16_t leftDuty = (uint16_t)((uint32_t)leftSpeed * PWM_PERIOD / 100);
    
    // DRV8833真值表: xIN1=1, xIN2=0 -> Forward (两轮均前进，左轮较慢)
    // 右轮前进（较快）
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, rightDuty, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, 0, TIMER_TYPE_A);
    
    // 左轮前进（较慢）
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, leftDuty, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, 0, TIMER_TYPE_G);
}

/**
 * 小车右转 (差速转向)
 * 
 * @param speed 基础速度百分比 (0-100)
 * @param turnRatio 转弯比例 (0-100)，数值越大转弯越急
 */
void Motor_turnRight(uint8_t speed, uint8_t turnRatio)
{
    // 确保参数在有效范围内
    if (speed > 100)
        speed = 100;
    
    if (turnRatio > 100)
        turnRatio = 100;
    
    // 计算差速：右轮速度降低，左轮保持原速
    uint8_t leftSpeed = speed;
    uint8_t rightSpeed = speed - (speed * turnRatio / 100);
    
    // 计算PWM占空比值
    uint16_t leftDuty = (uint16_t)((uint32_t)leftSpeed * PWM_PERIOD / 100);
    uint16_t rightDuty = (uint16_t)((uint32_t)rightSpeed * PWM_PERIOD / 100);
    
    // DRV8833真值表: xIN1=1, xIN2=0 -> Forward (两轮均前进，右轮较慢)
    // 右轮前进（较慢）
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, rightDuty, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, 0, TIMER_TYPE_A);
    
    // 左轮前进（较快）
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, leftDuty, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, 0, TIMER_TYPE_G);
}

/**
 * 小车原地左转
 * 
 * @param speed 速度百分比 (0-100)
 */
void Motor_rotateLeft(uint8_t speed)
{
    // 确保速度在有效范围内
    if (speed > 100)
        speed = 100;
    
    // 计算PWM占空比值
    uint16_t duty = (uint16_t)((uint32_t)speed * PWM_PERIOD / 100);
    
    // 右轮前进，左轮后退
    // 右轮 - DRV8833真值表: xIN1=1, xIN2=0 -> Forward
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, duty, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, 0, TIMER_TYPE_A);
    
    // 左轮 - DRV8833真值表: xIN1=0, xIN2=1 -> Reverse
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, 0, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, duty, TIMER_TYPE_G);
}

/**
 * 小车原地右转
 * 
 * @param speed 速度百分比 (0-100)
 */
void Motor_rotateRight(uint8_t speed)
{
    // 确保速度在有效范围内
    if (speed > 100)
        speed = 100;
    
    // 计算PWM占空比值
    uint16_t duty = (uint16_t)((uint32_t)speed * PWM_PERIOD / 100);
    
    // 左轮前进，右轮后退
    // 右轮 - DRV8833真值表: xIN1=0, xIN2=1 -> Reverse
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, 0, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, duty, TIMER_TYPE_A);
    
    // 左轮 - DRV8833真值表: xIN1=1, xIN2=0 -> Forward
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, duty, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, 0, TIMER_TYPE_G);
}

/**
 * 小车停止 (惯性滑行)
 */
void Motor_stop(void)
{
    // DRV8833真值表: xIN1=0, xIN2=0 -> Coast / fast decay
    // 右轮停止
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, 0, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, 0, TIMER_TYPE_A);
    
    // 左轮停止
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, 0, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, 0, TIMER_TYPE_G);
}

/**
 * 小车刹车 (电气制动)
 */
void Motor_brake(void)
{
    // DRV8833真值表: xIN1=1, xIN2=1 -> Brake / slow decay
    // 右轮制动
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, PWM_PERIOD, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, PWM_PERIOD, TIMER_TYPE_A);
    
    // 左轮制动
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, PWM_PERIOD, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, PWM_PERIOD, TIMER_TYPE_G);
}

/**
 * 设置PWM占空比
 * 
 * @param timer PWM定时器实例
 * @param channel PWM通道
 * @param dutyValue 占空比值 (0 - PWM_PERIOD)
 * @param type 定时器类型(TIMER_TYPE_A或TIMER_TYPE_G)
 */
static void setPWMDuty(void *timer, uint32_t channel, uint16_t dutyValue, TimerType type)
{
    // 确保占空比不超过最大值
    if (dutyValue > PWM_PERIOD)
        dutyValue = PWM_PERIOD;
    
    // 根据定时器类型设置PWM占空比
    if (type == TIMER_TYPE_A) {
        DL_TimerA_setCaptureCompareValue(timer, dutyValue, channel);
    } else {
        DL_TimerG_setCaptureCompareValue(timer, dutyValue, channel);
    }
}

/**
 * 获取PWM占空比
 * 
 * @param timer PWM定时器实例
 * @param channel PWM通道
 * @param type 定时器类型(TIMER_TYPE_A或TIMER_TYPE_G)
 * @return 当前占空比值
 */
static uint16_t getPWMDuty(void *timer, uint32_t channel, TimerType type)
{
    // 根据定时器类型获取PWM占空比
    if (type == TIMER_TYPE_A) {
        return DL_TimerA_getCaptureCompareValue(timer, channel);
    } else {
        return DL_TimerG_getCaptureCompareValue(timer, channel);
    }
} 