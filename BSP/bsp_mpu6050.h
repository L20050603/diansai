/*
 * ������������Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ
 * �����������www.lckfb.com
 * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ
 * ������̳��https://oshwhub.com/forum
 * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
 * ��������׬Ǯ���������й�����ʦΪ����
 * Change Logs:
 * Date           Author       Notes
 * 2024-07-01     LCKFB-LP    first version
 */
#ifndef _BSP_MPU6050_H_
#define _BSP_MPU6050_H_

#include "board.h"

#ifndef printf
#define printf DEBUG_printf
#endif

/* Defines for SCL: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_SCL_PIN                                             (DL_GPIO_PIN_0)
#define GPIO_SCL_IOMUX                                            (IOMUX_PINCM1)
/* Defines for SDA: GPIOA.1 with pinCMx 2 on package pin 34 */
#define GPIO_SDA_PIN                                             (DL_GPIO_PIN_1)
#define GPIO_SDA_IOMUX                                            (IOMUX_PINCM2)
#define GPIO_PORT                                                (GPIOA)

//����SDA���ģʽ
#define SDA_OUT()   {                                                  \
                        DL_GPIO_initDigitalOutput(GPIO_SDA_IOMUX);     \
                        DL_GPIO_setPins(GPIO_PORT, GPIO_SDA_PIN);      \
                        DL_GPIO_enableOutput(GPIO_PORT, GPIO_SDA_PIN); \
                    }
//����SDA����ģʽ
#define SDA_IN()    { DL_GPIO_initDigitalInput(GPIO_SDA_IOMUX); }

//��ȡSDA���ŵĵ�ƽ�仯
#define SDA_GET()   ( ( ( DL_GPIO_readPins(GPIO_PORT,GPIO_SDA_PIN) & GPIO_SDA_PIN ) > 0 ) ? 1 : 0 )
//SDA��SCL���
#define SDA(x)      ( (x) ? (DL_GPIO_setPins(GPIO_PORT,GPIO_SDA_PIN)) : (DL_GPIO_clearPins(GPIO_PORT,GPIO_SDA_PIN)) )
#define SCL(x)      ( (x) ? (DL_GPIO_setPins(GPIO_PORT,GPIO_SCL_PIN)) : (DL_GPIO_clearPins(GPIO_PORT,GPIO_SCL_PIN)) )


//MPU6050��AD0��IIC��ַ���ţ��ӵ���IIC��ַΪ0x68,��VCC��IIC��ַΪ0x69


#define MPU6050_RA_SMPLRT_DIV       0x19        //�����ǲ����� ��ַ
#define MPU6050_RA_CONFIG           0x1A        //�������ֵ�ͨ�˲��� ��ַ
#define MPU6050_RA_GYRO_CONFIG      0x1B        //���������üĴ���
#define MPU6050_RA_ACCEL_CONFIG     0x1C        //���ٶȴ��������üĴ���
#define MPU_INT_EN_REG              0X38        //�ж�ʹ�ܼĴ���
#define MPU_USER_CTRL_REG           0X6A        //�û����ƼĴ���
#define MPU_FIFO_EN_REG             0X23        //FIFOʹ�ܼĴ���
#define MPU_PWR_MGMT2_REG           0X6C        //��Դ�����Ĵ���2
#define MPU_GYRO_CFG_REG            0X1B        //���������üĴ���
#define MPU_ACCEL_CFG_REG           0X1C        //���ٶȼ����üĴ���
#define MPU_CFG_REG                 0X1A        //���üĴ���
#define MPU_SAMPLE_RATE_REG         0X19        //����Ƶ�ʷ�Ƶ��
#define MPU_INTBP_CFG_REG           0X37        //�ж�/��·���üĴ���

#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_PWR_MGMT_2       0x6C

#define MPU6050_WHO_AM_I            0x75
#define MPU6050_SMPLRT_DIV          0            //8000Hz  
#define MPU6050_DLPF_CFG            0        
#define MPU6050_GYRO_OUT            0x43         //MPU6050���������ݼĴ�����ַ
#define MPU6050_ACC_OUT             0x3B         //MPU6050���ٶ����ݼĴ�����ַ
        
#define MPU6050_RA_TEMP_OUT_H       0x41        //�¶ȸ�λ
#define MPU6050_RA_TEMP_OUT_L       0x42        //�¶ȵ�λ

#define MPU_ACCEL_XOUTH_REG         0X3B        //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG         0X3C        //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG         0X3D        //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG         0X3E        //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG         0X3F        //���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG         0X40        //���ٶ�ֵ,Z���8λ�Ĵ���

#define MPU_TEMP_OUTH_REG           0X41        //�¶�ֵ�߰�λ�Ĵ���
#define MPU_TEMP_OUTL_REG           0X42        //�¶�ֵ��8λ�Ĵ���

#define MPU_GYRO_XOUTH_REG          0X43        //������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG          0X44        //������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG          0X45        //������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG          0X46        //������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG          0X47        //������ֵ,Z���8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG          0X48        //������ֵ,Z���8λ�Ĵ���


char MPU6050_WriteReg(uint8_t addr,uint8_t regaddr,uint8_t num,uint8_t *regdata);
char MPU6050_ReadData(uint8_t addr, uint8_t regaddr,uint8_t num,uint8_t* Read);

char MPU6050_Init(void);
void MPU6050ReadGyro(short *gyroData);
void MPU6050ReadAcc(short *accData);
float MPU6050_GetTemp(void);
uint8_t MPU6050ReadID(void);

/* IIC通信基础函数声明 */
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Ack(unsigned char ack);
unsigned char I2C_WaitAck(void);
void Send_Byte(uint8_t dat);
unsigned char Read_Byte(void);

#endif
