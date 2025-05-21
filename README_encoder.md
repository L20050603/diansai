# 编码器驱动模块使用说明

## 概述

本模块提供了对霍尔编码器的驱动支持，用于测量电机旋转速度、方向和距离。适用于带两个信号输出的霍尔编码器，电机每旋转一圈产生11个脉冲信号。

## 功能特性

- 基于GPIO中断的脉冲计数
- 速度计算（脉冲/秒）
- RPM计算（转/分）
- 方向检测
- 距离计算

## 硬件连接

编码器的信号线已连接到以下GPIO引脚：

- 左轮编码器：GPIOB.8 (B信号线)
- 右轮编码器：GPIOB.9 (A信号线)

中断配置已在系统配置中完成，设置为上升沿触发。

## API函数说明

### 初始化

```c
void Encoder_init(void);
```

在使用编码器功能前必须先调用此函数进行初始化。

### 获取脉冲计数

```c
int32_t Encoder_getPulseCount(EncoderID_t encoderID);
```

获取指定编码器的累计脉冲计数，正值表示正向旋转，负值表示反向旋转。

参数：
- `encoderID`: 编码器ID，可以是`ENCODER_LEFT`或`ENCODER_RIGHT`

### 获取速度

```c
int16_t Encoder_getSpeed(EncoderID_t encoderID);
```

获取指定编码器的当前速度，单位为脉冲/秒。正值表示正向旋转，负值表示反向旋转。

参数：
- `encoderID`: 编码器ID

### 获取旋转方向

```c
bool Encoder_getDirection(EncoderID_t encoderID);
```

获取指定编码器的当前旋转方向。`true`表示正向，`false`表示反向。

参数：
- `encoderID`: 编码器ID

### 更新速度计算

```c
void Encoder_updateSpeed(void);
```

更新所有编码器的速度计算。此函数应当定期调用（比如在主循环中），建议调用间隔不少于50ms。

### 获取转速

```c
uint16_t Encoder_getRPM(EncoderID_t encoderID);
```

获取指定编码器的估算转速（每分钟转数）。

参数：
- `encoderID`: 编码器ID

### 估算行驶距离

```c
float Encoder_getDistance(EncoderID_t encoderID, float wheelDiameter);
```

根据脉冲计数和车轮直径估算行驶距离（厘米）。

参数：
- `encoderID`: 编码器ID
- `wheelDiameter`: 车轮直径（厘米）

### 重置函数

```c
void Encoder_reset(EncoderID_t encoderID);
void Encoder_resetAll(void);
```

重置指定编码器或所有编码器的计数和状态。

## 使用示例

```c
#include "encoder.h"
#include "delay.h"

void main(void)
{
    // 系统初始化
    SYSCFG_DL_init();
    
    // 初始化延时模块和编码器
    Delay_init();
    Encoder_init();
    
    while(1)
    {
        // 更新速度计算
        Encoder_updateSpeed();
        
        // 读取电机信息
        int32_t leftCount = Encoder_getPulseCount(ENCODER_LEFT);
        int32_t rightCount = Encoder_getPulseCount(ENCODER_RIGHT);
        
        int16_t leftSpeed = Encoder_getSpeed(ENCODER_LEFT);
        int16_t rightSpeed = Encoder_getSpeed(ENCODER_RIGHT);
        
        uint16_t leftRPM = Encoder_getRPM(ENCODER_LEFT);
        uint16_t rightRPM = Encoder_getRPM(ENCODER_RIGHT);
        
        float leftDistance = Encoder_getDistance(ENCODER_LEFT, 6.5); // 假设车轮直径为6.5厘米
        float rightDistance = Encoder_getDistance(ENCODER_RIGHT, 6.5);
        
        // 等待一段时间
        Delay_ms(100);
    }
}
```

## 注意事项

1. 当前实现中主要使用上升沿计数，如果需要更高精度的方向检测，可以考虑同时使用A和B两个信号。

2. 速度计算需要定期调用`Encoder_updateSpeed()`函数，建议不少于每50ms调用一次。

3. 距离计算依赖于车轮直径参数，请确保提供准确的直径值。

4. 如果发现中断丢失或计数不准确，可能需要调整中断优先级或减少中断中的处理时间。 