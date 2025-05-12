/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for PWM_bin1 */
#define PWM_bin1_INST                                                      TIMA1
#define PWM_bin1_INST_IRQHandler                                TIMA1_IRQHandler
#define PWM_bin1_INST_INT_IRQN                                  (TIMA1_INT_IRQn)
#define PWM_bin1_INST_CLK_FREQ                                            100000
/* GPIO defines for channel 0 */
#define GPIO_PWM_bin1_C0_PORT                                              GPIOA
#define GPIO_PWM_bin1_C0_PIN                                      DL_GPIO_PIN_17
#define GPIO_PWM_bin1_C0_IOMUX                                   (IOMUX_PINCM39)
#define GPIO_PWM_bin1_C0_IOMUX_FUNC                  IOMUX_PINCM39_PF_TIMA1_CCP0
#define GPIO_PWM_bin1_C0_IDX                                 DL_TIMER_CC_0_INDEX

/* Defines for PWM_bin2 */
#define PWM_bin2_INST                                                      TIMA0
#define PWM_bin2_INST_IRQHandler                                TIMA0_IRQHandler
#define PWM_bin2_INST_INT_IRQN                                  (TIMA0_INT_IRQn)
#define PWM_bin2_INST_CLK_FREQ                                            100000
/* GPIO defines for channel 2 */
#define GPIO_PWM_bin2_C2_PORT                                              GPIOA
#define GPIO_PWM_bin2_C2_PIN                                      DL_GPIO_PIN_15
#define GPIO_PWM_bin2_C2_IOMUX                                   (IOMUX_PINCM37)
#define GPIO_PWM_bin2_C2_IOMUX_FUNC                  IOMUX_PINCM37_PF_TIMA0_CCP2
#define GPIO_PWM_bin2_C2_IDX                                 DL_TIMER_CC_2_INDEX

/* Defines for PWM_ain1 */
#define PWM_ain1_INST                                                      TIMG6
#define PWM_ain1_INST_IRQHandler                                TIMG6_IRQHandler
#define PWM_ain1_INST_INT_IRQN                                  (TIMG6_INT_IRQn)
#define PWM_ain1_INST_CLK_FREQ                                            100000
/* GPIO defines for channel 0 */
#define GPIO_PWM_ain1_C0_PORT                                              GPIOA
#define GPIO_PWM_ain1_C0_PIN                                      DL_GPIO_PIN_21
#define GPIO_PWM_ain1_C0_IOMUX                                   (IOMUX_PINCM46)
#define GPIO_PWM_ain1_C0_IOMUX_FUNC                  IOMUX_PINCM46_PF_TIMG6_CCP0
#define GPIO_PWM_ain1_C0_IDX                                 DL_TIMER_CC_0_INDEX

/* Defines for PWM_ain2 */
#define PWM_ain2_INST                                                      TIMG0
#define PWM_ain2_INST_IRQHandler                                TIMG0_IRQHandler
#define PWM_ain2_INST_INT_IRQN                                  (TIMG0_INT_IRQn)
#define PWM_ain2_INST_CLK_FREQ                                            100000
/* GPIO defines for channel 1 */
#define GPIO_PWM_ain2_C1_PORT                                              GPIOA
#define GPIO_PWM_ain2_C1_PIN                                      DL_GPIO_PIN_24
#define GPIO_PWM_ain2_C1_IOMUX                                   (IOMUX_PINCM54)
#define GPIO_PWM_ain2_C1_IOMUX_FUNC                  IOMUX_PINCM54_PF_TIMG0_CCP1
#define GPIO_PWM_ain2_C1_IDX                                 DL_TIMER_CC_1_INDEX



/* Defines for UART_1 */
#define UART_1_INST                                                        UART0
#define UART_1_INST_IRQHandler                                  UART0_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                         DL_GPIO_PIN_1
#define GPIO_UART_1_TX_PIN                                        DL_GPIO_PIN_28
#define GPIO_UART_1_IOMUX_RX                                      (IOMUX_PINCM2)
#define GPIO_UART_1_IOMUX_TX                                      (IOMUX_PINCM3)
#define GPIO_UART_1_IOMUX_RX_FUNC                       IOMUX_PINCM2_PF_UART0_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                       IOMUX_PINCM3_PF_UART0_TX
#define UART_1_BAUD_RATE                                                  (9600)
#define UART_1_IBRD_32_MHZ_9600_BAUD                                       (208)
#define UART_1_FBRD_32_MHZ_9600_BAUD                                        (21)





/* Port definition for Pin Group Line_sensor */
#define Line_sensor_PORT                                                 (GPIOB)

/* Defines for L_0: GPIOB.18 with pinCMx 44 on package pin 37 */
#define Line_sensor_L_0_PIN                                     (DL_GPIO_PIN_18)
#define Line_sensor_L_0_IOMUX                                    (IOMUX_PINCM44)
/* Defines for L_1: GPIOB.19 with pinCMx 45 on package pin 38 */
#define Line_sensor_L_1_PIN                                     (DL_GPIO_PIN_19)
#define Line_sensor_L_1_IOMUX                                    (IOMUX_PINCM45)
/* Defines for L_2: GPIOB.20 with pinCMx 48 on package pin 41 */
#define Line_sensor_L_2_PIN                                     (DL_GPIO_PIN_20)
#define Line_sensor_L_2_IOMUX                                    (IOMUX_PINCM48)
/* Defines for L_3: GPIOB.24 with pinCMx 52 on package pin 42 */
#define Line_sensor_L_3_PIN                                     (DL_GPIO_PIN_24)
#define Line_sensor_L_3_IOMUX                                    (IOMUX_PINCM52)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_bin1_init(void);
void SYSCFG_DL_PWM_bin2_init(void);
void SYSCFG_DL_PWM_ain1_init(void);
void SYSCFG_DL_PWM_ain2_init(void);
void SYSCFG_DL_UART_1_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
