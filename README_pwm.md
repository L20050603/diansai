# 循迹小车PWM电机控制模块

本项目实现了基于DRV8833电机驱动芯片的循迹小车PWM控制模块。使用TI MSP430的driverlib库来控制PWM输出，实现对小车电机的控制。

## 功能特点

- 支持小车前进、后退、左转、右转、原地旋转等基本动作
- 通过PWM信号控制电机速度，支持0-100%的速度控制
- 使用差速控制实现平滑转向
- 支持电气制动和惯性滑行两种停止方式
- 针对竞速要求优化，电机可高速运行

## 硬件连接

- 控制器: TI MSPM0G3507
- 电机驱动: DRV8833双H桥驱动
- 电机: TT减速电机
- 连接方式:

## DRV8833控制真值表

| xIN1 | xIN2 | 功能       |
| :--- | :--- | :---------- |
| 0    | 0    | 惯性滑行(Coast) |
| 0    | 1    | 后退(Reverse) |
| 1    | 0    | 前进(Forward) |
| 1    | 1    | 电气制动(Brake) |

## API接口

提供以下函数接口用于控制小车运动:

```c
// 初始化电机控制模块
void Motor_init(void);

// 设置电机速度
void Motor_setSpeed(uint8_t leftSpeed, uint8_t rightSpeed);

// 小车前进
void Motor_forward(uint8_t speed);

// 小车后退
void Motor_backward(uint8_t speed);

// 小车左转
void Motor_turnLeft(uint8_t speed, uint8_t turnRatio);

// 小车右转
void Motor_turnRight(uint8_t speed, uint8_t turnRatio);

// 小车原地左转
void Motor_rotateLeft(uint8_t speed);

// 小车原地右转
void Motor_rotateRight(uint8_t speed);

// 小车停止(惯性滑行)
void Motor_stop(void);

// 小车刹车(电气制动)
void Motor_brake(void);
```

## 使用示例

```c
// 初始化
Motor_init();

// 前进
Motor_forward(80);  // 80%速度前进

// 左转(60%基础速度，50%转弯比例)
Motor_turnLeft(60, 50);

// 原地右转
Motor_rotateRight(40);

// 停止(惯性滑行)
Motor_stop();

// 快速停止(电气制动)
Motor_brake();
```

## 实现说明

- 电机控制使用PWM信号控制DRV8833驱动芯片
- PWM频率和分辨率由系统配置决定
- 通过改变PWM占空比来控制电机速度
- 转向通过差速方式实现

## 性能优化

- 为竞速要求优化电机速度控制
- 快速响应的刹车制动功能
- 平滑的转向控制算法

## Example Summary

Empty project using DriverLib.
This example shows a basic empty project using DriverLib with just main file
and SysConfig initialization.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage

Compile, load and run the example.
