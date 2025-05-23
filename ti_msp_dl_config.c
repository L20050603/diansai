/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

DL_TimerA_backupConfig gPWM_bin1Backup;
DL_TimerA_backupConfig gPWM_bin2Backup;
DL_TimerG_backupConfig gPWM_ain1Backup;

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_PWM_bin1_init();
    SYSCFG_DL_PWM_bin2_init();
    SYSCFG_DL_PWM_ain1_init();
    SYSCFG_DL_PWM_ain2_init();
    SYSCFG_DL_UART_1_init();
    SYSCFG_DL_SYSTICK_init();
    /* Ensure backup structures have no valid state */
	gPWM_bin1Backup.backupRdy 	= false;
	gPWM_bin2Backup.backupRdy 	= false;
	gPWM_ain1Backup.backupRdy 	= false;


}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(PWM_bin1_INST, &gPWM_bin1Backup);
	retStatus &= DL_TimerA_saveConfiguration(PWM_bin2_INST, &gPWM_bin2Backup);
	retStatus &= DL_TimerG_saveConfiguration(PWM_ain1_INST, &gPWM_ain1Backup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(PWM_bin1_INST, &gPWM_bin1Backup, false);
	retStatus &= DL_TimerA_restoreConfiguration(PWM_bin2_INST, &gPWM_bin2Backup, false);
	retStatus &= DL_TimerG_restoreConfiguration(PWM_ain1_INST, &gPWM_ain1Backup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(PWM_bin1_INST);
    DL_TimerA_reset(PWM_bin2_INST);
    DL_TimerG_reset(PWM_ain1_INST);
    DL_TimerG_reset(PWM_ain2_INST);
    DL_UART_Main_reset(UART_1_INST);


    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(PWM_bin1_INST);
    DL_TimerA_enablePower(PWM_bin2_INST);
    DL_TimerG_enablePower(PWM_ain1_INST);
    DL_TimerG_enablePower(PWM_ain2_INST);
    DL_UART_Main_enablePower(UART_1_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_bin1_C0_IOMUX,GPIO_PWM_bin1_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_bin1_C0_PORT, GPIO_PWM_bin1_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_bin2_C2_IOMUX,GPIO_PWM_bin2_C2_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_bin2_C2_PORT, GPIO_PWM_bin2_C2_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_ain1_C0_IOMUX,GPIO_PWM_ain1_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_ain1_C0_PORT, GPIO_PWM_ain1_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_ain2_C1_IOMUX,GPIO_PWM_ain2_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_ain2_C1_PORT, GPIO_PWM_ain2_C1_PIN);

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_1_IOMUX_TX, GPIO_UART_1_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_1_IOMUX_RX, GPIO_UART_1_IOMUX_RX_FUNC);

    DL_GPIO_initDigitalOutput(sys_A14_IOMUX);

    DL_GPIO_initDigitalInputFeatures(Line_sensor_L_0_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(Line_sensor_L_1_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(Line_sensor_L_2_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(Line_sensor_L_3_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInput(encoder_left_b_IOMUX);

    DL_GPIO_initDigitalInput(encoder_right_a_IOMUX);

    DL_GPIO_initDigitalOutput(I2C_SDA_IOMUX);

    DL_GPIO_initDigitalOutput(I2C_SCL_IOMUX);

    DL_GPIO_setPins(GPIOA, sys_A14_PIN |
		I2C_SDA_PIN |
		I2C_SCL_PIN);
    DL_GPIO_enableOutput(GPIOA, sys_A14_PIN |
		I2C_SDA_PIN |
		I2C_SCL_PIN);
    DL_GPIO_setLowerPinsPolarity(GPIOB, DL_GPIO_PIN_8_EDGE_RISE |
		DL_GPIO_PIN_9_EDGE_RISE);
    DL_GPIO_clearInterruptStatus(GPIOB, encoder_left_b_PIN |
		encoder_right_a_PIN);
    DL_GPIO_enableInterrupt(GPIOB, encoder_left_b_PIN |
		encoder_right_a_PIN);

}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
    /* INT_GROUP1 Priority */
    NVIC_SetPriority(GPIOB_INT_IRQn, 1);

}


/*
 * Timer clock configuration to be sourced by  / 8 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   100000 Hz = 4000000 Hz / (8 * (39 + 1))
 */
static const DL_TimerA_ClockConfig gPWM_bin1ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 39U
};

static const DL_TimerA_PWMConfig gPWM_bin1Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_bin1_init(void) {

    DL_TimerA_setClockConfig(
        PWM_bin1_INST, (DL_TimerA_ClockConfig *) &gPWM_bin1ClockConfig);

    DL_TimerA_initPWMMode(
        PWM_bin1_INST, (DL_TimerA_PWMConfig *) &gPWM_bin1Config);

    DL_TimerA_setCaptureCompareValue(PWM_bin1_INST, 1000, DL_TIMER_CC_0_INDEX);
    DL_TimerA_setCaptureCompareOutCtl(PWM_bin1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_0_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(PWM_bin1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_0_INDEX);

    DL_TimerA_enableClock(PWM_bin1_INST);


    
    DL_TimerA_setCCPDirection(PWM_bin1_INST , DL_TIMER_CC0_OUTPUT );

}
/*
 * Timer clock configuration to be sourced by  / 8 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   100000 Hz = 4000000 Hz / (8 * (39 + 1))
 */
static const DL_TimerA_ClockConfig gPWM_bin2ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 39U
};

static const DL_TimerA_PWMConfig gPWM_bin2Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .isTimerWithFourCC = true,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_bin2_init(void) {

    DL_TimerA_setClockConfig(
        PWM_bin2_INST, (DL_TimerA_ClockConfig *) &gPWM_bin2ClockConfig);

    DL_TimerA_initPWMMode(
        PWM_bin2_INST, (DL_TimerA_PWMConfig *) &gPWM_bin2Config);

    DL_TimerA_setCaptureCompareValue(PWM_bin2_INST, 1000, DL_TIMER_CC_2_INDEX);
    DL_TimerA_setCaptureCompareOutCtl(PWM_bin2_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_2_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(PWM_bin2_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_2_INDEX);

    DL_TimerA_enableClock(PWM_bin2_INST);


    
    DL_TimerA_setCCPDirection(PWM_bin2_INST , DL_TIMER_CC2_OUTPUT );

}
/*
 * Timer clock configuration to be sourced by  / 8 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   100000 Hz = 4000000 Hz / (8 * (39 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_ain1ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 39U
};

static const DL_TimerG_PWMConfig gPWM_ain1Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_ain1_init(void) {

    DL_TimerG_setClockConfig(
        PWM_ain1_INST, (DL_TimerG_ClockConfig *) &gPWM_ain1ClockConfig);

    DL_TimerG_initPWMMode(
        PWM_ain1_INST, (DL_TimerG_PWMConfig *) &gPWM_ain1Config);

    DL_TimerG_setCaptureCompareValue(PWM_ain1_INST, 1000, DL_TIMER_CC_0_INDEX);
    DL_TimerG_setCaptureCompareOutCtl(PWM_ain1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_0_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(PWM_ain1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);

    DL_TimerG_enableClock(PWM_ain1_INST);


    
    DL_TimerG_setCCPDirection(PWM_ain1_INST , DL_TIMER_CC0_OUTPUT );

}
/*
 * Timer clock configuration to be sourced by  / 8 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   100000 Hz = 4000000 Hz / (8 * (39 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_ain2ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale = 39U
};

static const DL_TimerG_PWMConfig gPWM_ain2Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_ain2_init(void) {

    DL_TimerG_setClockConfig(
        PWM_ain2_INST, (DL_TimerG_ClockConfig *) &gPWM_ain2ClockConfig);

    DL_TimerG_initPWMMode(
        PWM_ain2_INST, (DL_TimerG_PWMConfig *) &gPWM_ain2Config);

    DL_TimerG_setCaptureCompareValue(PWM_ain2_INST, 1000, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setCaptureCompareOutCtl(PWM_ain2_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(PWM_ain2_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

    DL_TimerG_enableClock(PWM_ain2_INST);


    
    DL_TimerG_setCCPDirection(PWM_ain2_INST , DL_TIMER_CC1_OUTPUT );

}



static const DL_UART_Main_ClockConfig gUART_1ClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Main_Config gUART_1Config = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity      = DL_UART_MAIN_PARITY_NONE,
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE
};

SYSCONFIG_WEAK void SYSCFG_DL_UART_1_init(void)
{
    DL_UART_Main_setClockConfig(UART_1_INST, (DL_UART_Main_ClockConfig *) &gUART_1ClockConfig);

    DL_UART_Main_init(UART_1_INST, (DL_UART_Main_Config *) &gUART_1Config);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 9600
     *  Actual baud rate: 9600.24
     */
    DL_UART_Main_setOversampling(UART_1_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(UART_1_INST, UART_1_IBRD_32_MHZ_9600_BAUD, UART_1_FBRD_32_MHZ_9600_BAUD);



    DL_UART_Main_enable(UART_1_INST);
}

SYSCONFIG_WEAK void SYSCFG_DL_SYSTICK_init(void)
{
    /*
     * Initializes the SysTick period to 1.00 ms,
     * enables the interrupt, and starts the SysTick Timer
     */
    DL_SYSTICK_config(32000);
}

