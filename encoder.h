/**
 * encoder.h
 * 
 * 霍尔编码器驱动模块
 * 适用于带两个信号输出的霍尔编码器，每转11个脉冲
 * 
 * 功能：
 * - 脉冲计数，用于测量速度和距离
 * - 方向检测（基于信号相位）
 * - 速度计算（基于单位时间内的脉冲数）
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdbool.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"

/**
 * 编码器ID枚举
 */
typedef enum {
    ENCODER_LEFT,  // 左轮编码器
    ENCODER_RIGHT, // 右轮编码器
    ENCODER_COUNT  // 编码器总数
} EncoderID_t;

/**
 * 编码器状态结构体
 */
typedef struct {
    int32_t  pulseCount;  // 总脉冲计数（正值表示正向，负值表示反向）
    int32_t  lastCount;   // 上次速度计算时的脉冲计数
    uint32_t lastTime;    // 上次速度计算的时间戳（ms）
    int16_t  speed;       // 当前速度（脉冲/秒）
    bool     direction;   // 当前方向（true为正向，false为反向）
} EncoderState_t;

/**
 * 初始化编码器模块
 * 
 * 注意：在使用编码器前必须先调用此函数
 */
void Encoder_init(void);

/**
 * 重置编码器计数和状态
 * 
 * @param encoderID 要重置的编码器ID (ENCODER_LEFT 或 ENCODER_RIGHT)
 */
void Encoder_reset(EncoderID_t encoderID);

/**
 * 重置所有编码器计数和状态
 */
void Encoder_resetAll(void);

/**
 * 获取编码器累计脉冲计数
 * 
 * @param encoderID 编码器ID (ENCODER_LEFT 或 ENCODER_RIGHT)
 * @return 累计脉冲计数（正值表示正向，负值表示反向）
 */
int32_t Encoder_getPulseCount(EncoderID_t encoderID);

/**
 * 获取编码器当前速度（脉冲/秒）
 * 
 * @param encoderID 编码器ID (ENCODER_LEFT 或 ENCODER_RIGHT)
 * @return 当前速度（脉冲/秒）
 */
int16_t Encoder_getSpeed(EncoderID_t encoderID);

/**
 * 获取编码器当前方向
 * 
 * @param encoderID 编码器ID (ENCODER_LEFT 或 ENCODER_RIGHT)
 * @return 当前方向（true为正向，false为反向）
 */
bool Encoder_getDirection(EncoderID_t encoderID);

/**
 * 更新编码器速度
 * 此函数应定期调用以更新速度计算
 */
void Encoder_updateSpeed(void);

/**
 * 获取估算的转速（RPM - 每分钟转数）
 * 
 * @param encoderID 编码器ID (ENCODER_LEFT 或 ENCODER_RIGHT)
 * @return 估算的转速（RPM）
 */
uint16_t Encoder_getRPM(EncoderID_t encoderID);

/**
 * 获取估算的行驶距离（厘米）
 * 
 * @param encoderID 编码器ID (ENCODER_LEFT 或 ENCODER_RIGHT)
 * @param wheelDiameter 轮子直径（厘米）
 * @return 估算的行驶距离（厘米）
 */
float Encoder_getDistance(EncoderID_t encoderID, float wheelDiameter);

#endif /* ENCODER_H_ */ 