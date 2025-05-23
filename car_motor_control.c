/**
 * car_motor_control.c
 * 
 * 基于DRV8833驱动芯片的小车PWM控制模块
 * 使用TI MSP430 driverlib库控制电机
 * 
 * 实现car_motor_control.h中定义的功能
 */

#include "car_motor_control.h"
#include "inv_mpu.h"

// 电机死区定义，低于此值电机不会转动
#define left_deadzone 3
#define right_deadzone 3

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
 * 设置左右电机速度和方向
 * 
 * @param leftSpeed  左轮速度 (-100 到 100)，正值前进，负值后退
 * @param rightSpeed 右轮速度 (-100 到 100)，正值前进，负值后退
 */
void Motor_setSpeed(int16_t leftSpeed, int16_t rightSpeed)
{
    // 记录方向
    bool leftForward = (leftSpeed >= 0);
    bool rightForward = (rightSpeed >= 0);
    
    // 取速度绝对值
    uint8_t leftSpeedAbs = (leftSpeed < 0) ? -leftSpeed : leftSpeed;
    uint8_t rightSpeedAbs = (rightSpeed < 0) ? -rightSpeed : rightSpeed;
    
    // 限制速度在有效范围内
    if (leftSpeedAbs > 100)
        leftSpeedAbs = 100;
    
    if (rightSpeedAbs > 100)
        rightSpeedAbs = 100;
    
    // 计算PWM占空比值
    uint16_t leftDuty = (uint16_t)((uint32_t)leftSpeedAbs * PWM_PERIOD / 100);
    uint16_t rightDuty = (uint16_t)((uint32_t)rightSpeedAbs * PWM_PERIOD / 100);
    
    // 对于右轮
    if (rightForward) {
        // 右轮前进: IN1=PWM, IN2=0
        setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, rightDuty, TIMER_TYPE_A);
        setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, 0, TIMER_TYPE_A);
    } else {
        // 右轮后退: IN1=0, IN2=PWM
        setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, 0, TIMER_TYPE_A);
        setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, rightDuty, TIMER_TYPE_A);
    }
    
    // 对于左轮
    if (leftForward) {
        // 左轮前进: IN1=PWM, IN2=0
        setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, leftDuty, TIMER_TYPE_G);
        setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, 0, TIMER_TYPE_G);
    } else {
        // 左轮后退: IN1=0, IN2=PWM
        setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, 0, TIMER_TYPE_G);
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
 * 小车刹车
 * 
 * 使用电气制动快速停止
 */
void Motor_brake(void)
{
    // DRV8833真值表: xIN1=1, xIN2=1 -> Brake (short circuit)
    // 右轮刹车
    setPWMDuty(RIGHT_IN1_TIMER, RIGHT_IN1_CHANNEL, PWM_PERIOD, TIMER_TYPE_A);
    setPWMDuty(RIGHT_IN2_TIMER, RIGHT_IN2_CHANNEL, PWM_PERIOD, TIMER_TYPE_A);
    
    // 左轮刹车
    setPWMDuty(LEFT_IN1_TIMER, LEFT_IN1_CHANNEL, PWM_PERIOD, TIMER_TYPE_G);
    setPWMDuty(LEFT_IN2_TIMER, LEFT_IN2_CHANNEL, PWM_PERIOD, TIMER_TYPE_G);
}

/**
 * 小车直线行走闭环控制
 * 
 * 使用IMU数据进行闭环控制，使小车保持直线行走
 * 
 * @param speed 基础速度百分比 (0-100)
 * @param kp 比例系数，控制修正的强度
 */
void Motor_straightLine(uint8_t speed, float kp)
{
    // 声明变量
    float pitch, roll, yaw;         // 欧拉角
    static float initial_yaw = 0;   // 初始偏航角
    static bool is_initialized = false;  // 初始化标志
    float yaw_error;                // 偏航角误差
    int16_t left_speed, right_speed;  // 左右轮速度
    int16_t speed_correction;       // 速度修正值
    
    // 获取当前姿态角数据
    if (mpu_dmp_get_data(&pitch, &roll, &yaw) != 0) {
        // 如果读取失败，使用最近的有效数据
        return;
    }
    
    // 首次运行时保存初始偏航角作为参考
    if (!is_initialized) {
        initial_yaw = yaw;
        is_initialized = true;
    }
    
    // 计算偏航角误差（当前偏航角与初始偏航角之差）
    yaw_error = yaw - initial_yaw;
    
    // 角度归一化到 -180 到 180 范围内
    while (yaw_error > 180) yaw_error -= 360;
    while (yaw_error < -180) yaw_error += 360;
    
    // 使用比例控制计算速度修正值
    // 当偏航角为正（向右偏）时，左轮速度减小，右轮速度增加
    // 当偏航角为负（向左偏）时，左轮速度增加，右轮速度减小
    speed_correction = (int16_t)(kp * yaw_error);
    
    // 计算左右轮的速度
    left_speed = speed - speed_correction;
    right_speed = speed + speed_correction;
    
    // 限制速度在有效范围内
    if (left_speed < left_deadzone) left_speed = left_deadzone;
    if (right_speed < right_deadzone) right_speed = right_deadzone;
    if (left_speed > 100) left_speed = 100;
    if (right_speed > 100) right_speed = 100;
    
    // 应用电机速度
    Motor_setSpeed(left_speed, right_speed);
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