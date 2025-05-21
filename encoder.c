/**
 * encoder.c
 * 
 * 霍尔编码器驱动模块实现
 * 适用于带两个信号输出的霍尔编码器，每转11个脉冲
 * 
 * 基于GPIO中断实现脉冲计数
 */

#include "encoder.h"
#include "delay.h"
#include <stdlib.h>  // 添加标准库以使用abs函数

// 编码器特性常量
#define PULSES_PER_REVOLUTION 11  // 每转11个脉冲
#define MIN_SPEED_UPDATE_INTERVAL 50  // 速度更新的最小间隔，单位ms

// 编码器GPIO引脚定义
#define LEFT_ENCODER_PIN_B encoder_left_b_PIN
#define RIGHT_ENCODER_PIN_A encoder_right_a_PIN
#define ENCODER_PORT encoder_PORT

// 编码器状态数组，索引对应EncoderID_t
static EncoderState_t encoderStates[ENCODER_COUNT];

// 前一次中断时的引脚状态，用于检测方向
static uint32_t previousPinState = 0;

// 正向和反向计数的宏定义
#define COUNT_FORWARD(encoder) do { \
    encoderStates[(encoder)].pulseCount++; \
    encoderStates[(encoder)].direction = true; \
} while(0)

#define COUNT_REVERSE(encoder) do { \
    encoderStates[(encoder)].pulseCount--; \
    encoderStates[(encoder)].direction = false; \
} while(0)

/**
 * 初始化编码器模块
 */
void Encoder_init(void)
{
    // 重置所有编码器状态
    Encoder_resetAll();
}

/**
 * 重置编码器计数和状态
 * 
 * @param encoderID 要重置的编码器ID
 */
void Encoder_reset(EncoderID_t encoderID)
{
    if (encoderID < ENCODER_COUNT) {
        // 禁用中断以确保原子操作
        NVIC_DisableIRQ(encoder_INT_IRQN);
        
        // 重置状态
        encoderStates[encoderID].pulseCount = 0;
        encoderStates[encoderID].lastCount = 0;
        encoderStates[encoderID].lastTime = 0;
        encoderStates[encoderID].speed = 0;
        encoderStates[encoderID].direction = true;
        
        // 重新启用中断
        NVIC_EnableIRQ(encoder_INT_IRQN);
    }
}

/**
 * 重置所有编码器计数和状态
 */
void Encoder_resetAll(void)
{
    for (int i = 0; i < ENCODER_COUNT; i++) {
        Encoder_reset((EncoderID_t)i);
    }
}

/**
 * 获取编码器累计脉冲计数
 * 
 * @param encoderID 编码器ID
 * @return 累计脉冲计数
 */
int32_t Encoder_getPulseCount(EncoderID_t encoderID)
{
    if (encoderID < ENCODER_COUNT) {
        return encoderStates[encoderID].pulseCount;
    }
    return 0;
}

/**
 * 获取编码器当前速度（脉冲/秒）
 * 
 * @param encoderID 编码器ID
 * @return 当前速度
 */
int16_t Encoder_getSpeed(EncoderID_t encoderID)
{
    if (encoderID < ENCODER_COUNT) {
        return encoderStates[encoderID].speed;
    }
    return 0;
}

/**
 * 获取编码器当前方向
 * 
 * @param encoderID 编码器ID
 * @return 当前方向
 */
bool Encoder_getDirection(EncoderID_t encoderID)
{
    if (encoderID < ENCODER_COUNT) {
        return encoderStates[encoderID].direction;
    }
    return true;
}

/**
 * 更新编码器速度
 * 此函数应定期调用以更新速度计算
 */
void Encoder_updateSpeed(void)
{
    uint32_t currentTime = Delay_getTicks(); // 获取当前时间戳（ms）
    
    for (int i = 0; i < ENCODER_COUNT; i++) {
        uint32_t timeDiff = currentTime - encoderStates[i].lastTime;
        
        // 确保有足够的时间已经过去以计算速度
        if (timeDiff >= MIN_SPEED_UPDATE_INTERVAL) {
            int32_t countDiff = encoderStates[i].pulseCount - encoderStates[i].lastCount;
            
            // 计算速度：脉冲/秒
            encoderStates[i].speed = (int16_t)((countDiff * 1000) / timeDiff);
            
            // 更新上次计数和时间
            encoderStates[i].lastCount = encoderStates[i].pulseCount;
            encoderStates[i].lastTime = currentTime;
        }
    }
}

/**
 * 获取估算的转速（RPM）
 * 
 * @param encoderID 编码器ID
 * @return 估算的转速
 */
uint16_t Encoder_getRPM(EncoderID_t encoderID)
{
    if (encoderID < ENCODER_COUNT) {
        // 计算RPM = (脉冲/秒) * 60 / 每转脉冲数
        // 注意：这会将负速度转换为正值
        int16_t speed = encoderStates[encoderID].speed;
        return (uint16_t)((abs(speed) * 60) / PULSES_PER_REVOLUTION);
    }
    return 0;
}

/**
 * 获取估算的行驶距离（厘米）
 * 
 * @param encoderID 编码器ID
 * @param wheelDiameter 轮子直径（厘米）
 * @return 估算的行驶距离
 */
float Encoder_getDistance(EncoderID_t encoderID, float wheelDiameter)
{
    if (encoderID < ENCODER_COUNT) {
        // 计算距离 = 脉冲数 / 每转脉冲数 * 轮子周长
        float wheelCircumference = wheelDiameter * 3.14159f;
        return ((float)encoderStates[encoderID].pulseCount / PULSES_PER_REVOLUTION) * wheelCircumference;
    }
    return 0.0f;
}

/**
 * GPIO中断处理函数
 * 在ti_msp_dl_config.c中已配置中断处理
 */
void GPIOB_IRQHandler(void)
{
    // 获取中断状态（添加第二个参数 - 关心的引脚掩码）
    uint32_t interruptStatus = DL_GPIO_getRawInterruptStatus(ENCODER_PORT, 
                                   LEFT_ENCODER_PIN_B | RIGHT_ENCODER_PIN_A);
    
    // 读取当前引脚状态（增加第二个参数，读取所有引脚）
    uint32_t currentPinState = DL_GPIO_readPins(ENCODER_PORT, 0xFFFFFFFFU);
    
    // 处理左轮编码器中断
    if (interruptStatus & LEFT_ENCODER_PIN_B) {
        // 简单计数模式
        COUNT_FORWARD(ENCODER_LEFT);
        
        // 清除中断标志
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, LEFT_ENCODER_PIN_B);
    }
    
    // 处理右轮编码器中断
    if (interruptStatus & RIGHT_ENCODER_PIN_A) {
        // 简单计数模式
        COUNT_FORWARD(ENCODER_RIGHT);
        
        // 清除中断标志
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, RIGHT_ENCODER_PIN_A);
    }
    
    // 保存当前引脚状态用于下次中断
    previousPinState = currentPinState;
} 