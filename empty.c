/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "                                                                                                                                                                                                                                                                                                       
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "ti_msp_dl_config.h"
#include "car_motor_control.h"
#include "debug.h"
#include "delay.h"
#include "line_sensor.h"
#include "test_logs.h"

#define printf DEBUG_printf


int main(void)
{

    // 初始化系统配置
    SYSCFG_DL_init();
    
    // 初始化延时函数
    Delay_init();
    
    // 初始化电机控制模块
    Motor_init();
    
    // 初始化巡线传感器
    LineSensor_init();
    
    
    while (1) {
        int leftSpeed = 1000;
        int rightSpeed = 1000;
        float batteryVoltage = 12.0;
        
        // 读取所有传感器状态 (0表示检测到黑线，1表示未检测到黑线)
        LineSensor_State_t sensorState = LineSensor_readAll();
        // 使用延时函数
        //Delay_ms(100); // 延时0.1秒
        test_motor_control();
        Delay_ms(5000); 
        }
    
    }


