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

#include "bsp_mpu6050.h"

/******************************************************************
 * �� �� �� �ƣ�IIC_Start
 * �� �� ˵ ����IIC��ʼʱ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void IIC_Start(void)
{
        SDA_OUT();
        SCL(1);  //no
        SDA(0);  
        
        SDA(1);
        Delay_us(5);
        SDA(0);
        Delay_us(5);
                       
        SCL(0);
}
/******************************************************************
 * �� �� �� �ƣ�IIC_Stop
 * �� �� ˵ ����IICֹͣ�ź�
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void IIC_Stop(void)
{
        SDA_OUT();
        SCL(0);
        SDA(0);
        
        SCL(1);
        Delay_us(5);
        SDA(1);
        Delay_us(5);
        
}

/******************************************************************
 * �� �� �� �ƣ�IIC_Send_Ack
 * �� �� ˵ ������������Ӧ����߷�Ӧ���ź�
 * �� �� �� �Σ�0����Ӧ��  1���ͷ�Ӧ��
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void IIC_Send_Ack(unsigned char ack)
{
        SDA_OUT();
        SCL(0);
        SDA(0);
        Delay_us(5);
        if(!ack) SDA(0);
        else         SDA(1);
        SCL(1);
        Delay_us(5);
        SCL(0);
        SDA(1);
}


/******************************************************************
 * �� �� �� �ƣ�I2C_WaitAck
 * �� �� ˵ �����ȴ��ӻ�Ӧ��
 * �� �� �� �Σ���
 * �� �� �� �أ�0��Ӧ��  1��ʱ��Ӧ��
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
unsigned char I2C_WaitAck(void)
{
        
        char ack = 0;
        unsigned char ack_flag = 10;
        SCL(0);
        SDA(1);
        SDA_IN();
        
        SCL(1);
        while( (SDA_GET()==1) && ( ack_flag ) )
        {
                ack_flag--;
                Delay_us(5);
        }
        
        if( ack_flag <= 0 )
        {
                IIC_Stop();
                return 1;
        }
        else
        {
                SCL(0);
                SDA_OUT();
        }
        return ack;
}

/******************************************************************
 * �� �� �� �ƣ�Send_Byte
 * �� �� ˵ ����д��һ���ֽ�
 * �� �� �� �Σ�datҪд�˵�����
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void Send_Byte(uint8_t dat)
{
        int i = 0;
        SDA_OUT();
        SCL(0);//����ʱ�ӿ�ʼ���ݴ���
        
        for( i = 0; i < 8; i++ )
        {
                SDA( (dat & 0x80) >> 7 );
                Delay_us(1);
                SCL(1);
                Delay_us(5);
                SCL(0);
                Delay_us(5);
                dat<<=1;
        }        
}

/******************************************************************
 * �� �� �� �ƣ�Read_Byte
 * �� �� ˵ ����IIC��ʱ��
 * �� �� �� �Σ���
 * �� �� �� �أ�����������
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
unsigned char Read_Byte(void)
{
        unsigned char i,receive=0;
        SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
        {
        SCL(0);
        Delay_us(5);
        SCL(1);
        Delay_us(5);
        receive<<=1;
        if( SDA_GET() )
        {        
            receive|=1;   
        }
        Delay_us(5); 
    }                                         
        SCL(0); 
  return receive;
}

/******************************************************************
 * �� �� �� �ƣ�MPU6050_WriteReg
 * �� �� ˵ ����IIC����д������
 * �� �� �� �Σ�addr������ַ regaddr�Ĵ�����ַ numҪд��ĳ��� regdataд������ݵ�ַ
 * �� �� �� �أ�0=��ȡ�ɹ�   ����=��ȡʧ��
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
char MPU6050_WriteReg(uint8_t addr,uint8_t regaddr,uint8_t num,uint8_t *regdata)
{
    uint16_t i = 0;
        IIC_Start();
        Send_Byte((addr<<1)|0);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return 1;}
        Send_Byte(regaddr);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return 2;}
    
        for(i=0;i<num;i++)
    {
        Send_Byte(regdata[i]);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return (3+i);}
    }        
        IIC_Stop();
    return 0;
}


/******************************************************************
 * �� �� �� �ƣ�MPU6050_ReadData
 * �� �� ˵ ����IIC������ȡ����
 * �� �� �� �Σ�addr������ַ regaddr�Ĵ�����ַ numҪ��ȡ�ĳ��� Read��ȡ��������Ҫ�洢�ĵ�ַ
 * �� �� �� �أ�0=��ȡ�ɹ�   ����=��ȡʧ�� 
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
char MPU6050_ReadData(uint8_t addr, uint8_t regaddr,uint8_t num,uint8_t* Read)
{
        uint8_t i;
        IIC_Start();
        Send_Byte((addr<<1)|0);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return 1;}
        Send_Byte(regaddr);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return 2;}
        
        IIC_Start();
        Send_Byte((addr<<1)|1);
        if( I2C_WaitAck() == 1 ) {IIC_Stop();return 3;}
        
        for(i=0;i<(num-1);i++){
                Read[i]=Read_Byte();
                IIC_Send_Ack(0);
        }
        Read[i]=Read_Byte();
        IIC_Send_Ack(1);         
        IIC_Stop();
        return 0;
}


/******************************************************************
 * �� �� �� �ƣ�MPU_Set_Gyro_Fsr
 * �� �� ˵ ��������MPU6050�����Ǵ����������̷�Χ
 * �� �� �� �Σ�fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
 * �� �� �� �أ�0,���óɹ�  ����,����ʧ��
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
        return MPU6050_WriteReg(0x68,MPU_GYRO_CFG_REG,1,(uint8_t*)(fsr<<3)); //���������������̷�Χ
}    

/******************************************************************
 * �� �� �� �ƣ�MPU_Set_Accel_Fsr
 * �� �� ˵ ��������MPU6050���ٶȴ����������̷�Χ
 * �� �� �� �Σ�fsr:0,��2g;1,��4g;2,��8g;3,��16g
 * �� �� �� �أ�0,���óɹ�  ����,����ʧ��
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
        return MPU6050_WriteReg(0x68,MPU_ACCEL_CFG_REG,1,(uint8_t*)(fsr<<3)); //���ü��ٶȴ����������̷�Χ  
}

/******************************************************************
 * �� �� �� �ƣ�MPU_Set_LPF
 * �� �� ˵ ��������MPU6050�����ֵ�ͨ�˲���
 * �� �� �� �Σ�lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
 * �� �� �� �أ�0,���óɹ�  ����,����ʧ��
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
uint8_t MPU_Set_LPF(uint16_t lpf)
{
        uint8_t data=0;
        
        if(lpf>=188)data=1;
        else if(lpf>=98)data=2;
        else if(lpf>=42)data=3;
        else if(lpf>=20)data=4;
        else if(lpf>=10)data=5;
        else data=6; 
    return data=MPU6050_WriteReg(0x68,MPU_CFG_REG,1,&data);//�������ֵ�ͨ�˲���  
}
/******************************************************************
 * �� �� �� �ƣ�MPU_Set_Rate
 * �� �� ˵ ��������MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
 * �� �� �� �Σ�rate:4~1000(Hz)  ��ʼ����rateȡ50
 * �� �� �� �أ�0,���óɹ�  ����,����ʧ��
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
uint8_t MPU_Set_Rate(uint16_t rate)
{
        uint8_t data;
        if(rate>1000)rate=1000;
        if(rate<4)rate=4;
        data=1000/rate-1;
        data=MPU6050_WriteReg(0x68,MPU_SAMPLE_RATE_REG,1,&data);        //�������ֵ�ͨ�˲���
         return MPU_Set_LPF(rate/2);            //�Զ�����LPFΪ�����ʵ�һ��
}


/******************************************************************
 * �� �� �� �ƣ�MPU6050ReadGyro
 * �� �� ˵ ������ȡ����������
 * �� �� �� �Σ����������ݴ洢��ַ 
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void MPU6050ReadGyro(short *gyroData)
{
        uint8_t buf[6];
        uint8_t reg = 0;
        //MPU6050_GYRO_OUT = MPU6050���������ݼĴ�����ַ
        //��������������Ĵ����ܹ���6���Ĵ�����ɣ�
        //���X/Y/Z������������Ǵ��������ݣ����ֽ���ǰ�����ֽ��ں�
        //ÿһ����16λ����˳��Ϊxyz
        reg = MPU6050_ReadData(0x68,MPU6050_GYRO_OUT,6,buf);
        if( reg == 0 )
        {
                gyroData[0] = (buf[0] << 8) | buf[1];
                gyroData[1] = (buf[2] << 8) | buf[3];
                gyroData[2] = (buf[4] << 8) | buf[5];
        }
}

/******************************************************************
 * �� �� �� �ƣ�MPU6050ReadAcc
 * �� �� ˵ ������ȡ���ٶ�����
 * �� �� �� �Σ����ٶ����ݴ洢��ַ 
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void MPU6050ReadAcc(short *accData)
{
        uint8_t buf[6];
        uint8_t reg = 0;
        //MPU6050_ACC_OUT = MPU6050���ٶ����ݼĴ�����ַ
        //���ٶȴ�������������Ĵ����ܹ���6���Ĵ�����ɣ�
        //���X/Y/Z������ļ��ٶȴ�����ֵ�����ֽ���ǰ�����ֽ��ں�
        reg = MPU6050_ReadData(0x68, MPU6050_ACC_OUT, 6, buf);
        if( reg == 0)
        {
                accData[0] = (buf[0] << 8) | buf[1];
                accData[1] = (buf[2] << 8) | buf[3];
                accData[2] = (buf[4] << 8) | buf[5];
        }
}

/******************************************************************
 * �� �� �� �ƣ�MPU6050_GetTemp
 * �� �� ˵ ������ȡMPU6050�ϵ��¶�
 * �� �� �� �Σ���
 * �� �� �� �أ��¶�ֵ��λΪ��
 * ��       �ߣ�LC
 * ��       ע���¶Ȼ��㹫ʽΪ��Temperature = 36.53 + regval/340
******************************************************************/
float MPU6050_GetTemp(void)
{
        short temp3;
        uint8_t buf[2];
        float Temperature = 0;
        MPU6050_ReadData(0x68,MPU6050_RA_TEMP_OUT_H,2,buf); 
    temp3= (buf[0] << 8) | buf[1];        
        Temperature=((double) temp3/340.0)+36.53;
    return Temperature;
}

/******************************************************************
 * �� �� �� �ƣ�MPU6050ReadID
 * �� �� ˵ ������ȡMPU6050��������ַ
 * �� �� �� �Σ���
 * �� �� �� �أ�0=��ⲻ��MPU6050   1=�ܼ�⵽MPU6050
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
uint8_t MPU6050ReadID(void)
{
        unsigned char Re[2] = {0};
        //����ID�Ĵ��� = 0x75
        printf("mpu=%d\r\n",MPU6050_ReadData(0x68,0X75,1,Re)); //��������ַ
        printf("Re[0]=%d\r\n",Re[0]);
        if (Re[0] != 0x68) 
        {
                        printf("disconnect MPU6050");
                        return 1;
         } 
        else
        {
                        printf("MPU6050 ID = %x\r\n",Re[0]);
                        return 0;
        }
        return 0;
}

/******************************************************************
 * �� �� �� �ƣ�MPU6050_Init
 * �� �� ˵ ����MPU6050��ʼ��
 * �� �� �� �Σ���
 * �� �� �� �أ�0�ɹ�  1û�м�⵽MPU6050
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
char MPU6050_Init(void)
{
    SDA_OUT();
    Delay_ms(10);
    //��λ6050
    MPU6050_WriteReg(0x68,MPU6050_RA_PWR_MGMT_1, 1,(uint8_t*)(0x80));
    Delay_ms(100);
    //��Դ�����Ĵ���
    //ѡ��X��������Ϊ�ο�PLL��ʱ��Դ������CLKSEL=001
    MPU6050_WriteReg(0x68,MPU6050_RA_PWR_MGMT_1,1, (uint8_t*)(0x00));
    
    MPU_Set_Gyro_Fsr(3);    //�����Ǵ�����,��2000dps
    MPU_Set_Accel_Fsr(0);   //���ٶȴ�����,��2g
    MPU_Set_Rate(50);                

    MPU6050_WriteReg(0x68,MPU_INT_EN_REG , 1,(uint8_t*)0x00);        //�ر������ж�
    MPU6050_WriteReg(0x68,MPU_USER_CTRL_REG,1,(uint8_t*)0x00);        //I2C��ģʽ�ر�
    MPU6050_WriteReg(0x68,MPU_FIFO_EN_REG,1,(uint8_t*)0x00);                //�ر�FIFO
    MPU6050_WriteReg(0x68,MPU_INTBP_CFG_REG,1,(uint8_t*)0X80);        //INT���ŵ͵�ƽ��Ч
      
    if( MPU6050ReadID() == 0 )//����Ƿ���6050
    {       
            MPU6050_WriteReg(0x68,MPU6050_RA_PWR_MGMT_1, 1,(uint8_t*)0x01);//����CLKSEL,PLL X��Ϊ�ο�
            MPU6050_WriteReg(0x68,MPU_PWR_MGMT2_REG, 1,(uint8_t*)0x00);//���ٶ��������Ƕ�����
            MPU_Set_Rate(50);        
            return 1;
    }
    return 0;
}

