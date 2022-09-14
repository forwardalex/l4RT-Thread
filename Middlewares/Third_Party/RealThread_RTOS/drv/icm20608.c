#include "icm20608.h"
#include "main.h"
#include "delay.h"
//#include "usart.h"



/**
 * @brief	ICM20608初始化函数
 *
 * @param   void
 *
 * @return  u8		0,成功，其他,失败
 */
u8 ICM20608_Init(void)
{
    u8 res;

    IIC_Init();//初始化IIC总线
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80);	//复位ICM20608
    delay_ms(100);
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00);	//唤醒ICM20608
    MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
    MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
    MPU_Set_Rate(50);						//设置采样率50Hz
    MPU_Write_Byte(MPU_INT_EN_REG, 0X00);	//关闭所有中断
    MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00);	//I2C主模式关闭
    MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);	//关闭FIFO
    MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80);	//INT引脚低电平有效

    MPU_Write_Byte(0x1D, 1);	//INT引脚低电平有效

    res = MPU_Read_Byte(MPU_DEVICE_ID_REG);

    if(res == 0XAF) //器件ID正确
    {
        MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01);	//设置CLKSEL,PLL X轴为参考
        MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00);	//加速度与陀螺仪都工作
        MPU_Set_Rate(50);						//设置采样率为50Hz
    }
    else return 1;

    return 0;

}
/**
 * @brief	设置ICM20608加速度传感器满量程范围
 *
 * @param   fsr		0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
 *
 * @return  u8		0,成功，其他,失败
 */
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3); //设置陀螺仪满量程范围
}
/**
 * @brief	设置ICM20608陀螺仪传感器满量程范围
 *
 * @param   fsr		0,±2g;1,±4g;2,±8g;3,±16g
 *
 * @return  u8		0,成功，其他,失败
 */
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3); //设置加速度传感器满量程范围
}
/**
 * @brief	设置ICM20608的数字低通滤波器
 *
 * @param   lpf		数字低通滤波频率(Hz)
 *
 * @return  u8		0,成功，其他,失败
 */
u8 MPU_Set_LPF(u16 lpf)
{
    u8 data = 0;

    if(lpf >= 188)data = 1;

    else if(lpf >= 98)data = 2;

    else if(lpf >= 42)data = 3;

    else if(lpf >= 20)data = 4;

    else if(lpf >= 10)data = 5;

    else data = 6;

    return MPU_Write_Byte(MPU_CFG_REG, data); //设置数字低通滤波器
}

/**
 * @brief	设置ICM20608的采样率(假定Fs=1KHz)
 *
 * @param   rate	4~1000(Hz)
 *
 * @return  u8		0,成功，其他,失败
 */
u8 MPU_Set_Rate(u16 rate)
{
    u8 data;

    if(rate > 1000)rate = 1000;

    if(rate < 4)rate = 4;

    data = 1000 / rate - 1;
    data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data);	//设置数字低通滤波器
    return MPU_Set_LPF(rate / 2);	//自动设置LPF为采样率的一半
}
/**
 * @brief	得到温度值
 *
 * @param   rate	4~1000(Hz)
 *
 * @return  short	温度值(扩大了100倍)
 */
short MPU_Get_Temperature(void)
{
    u8 buf[2];
    short raw;
    float temp;
    MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
    raw = ((u16)buf[0] << 8) | buf[1];
    temp = 25 + ((double)raw) / 326.8;
    return temp * 100;;
}

/**
 * @brief	得到陀螺仪值(原始值)
 *
 * @param   gx,gy,gz	陀螺仪x,y,z轴的原始读数(带符号)
 *
 * @return  u8			0,成功，其他,失败
 */
u8 MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    u8 buf[6], res;
    res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);

    if(res == 0)
    {
        *gx = ((u16)buf[0] << 8) | buf[1];
        *gy = ((u16)buf[2] << 8) | buf[3];
        *gz = ((u16)buf[4] << 8) | buf[5];
    }

    return res;;
}
/**
 * @brief	得到加速度值(原始值)
 *
 * @param   ax,ay,az	加速度值x,y,z轴的原始读数(带符号)
 *
 * @return  u8			0,成功，其他,失败
 */
u8 MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
    u8 buf[6], res;
    res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);

    if(res == 0)
    {
        *ax = ((u16)buf[0] << 8) | buf[1];
        *ay = ((u16)buf[2] << 8) | buf[3];
        *az = ((u16)buf[4] << 8) | buf[5];
    }

    return res;;
}
/**
 * @brief	IIC连续写
 *
 * @param   addr	器件地址
 * @param   reg		寄存器地址
 * @param   len		写入长度
 * @param   buf		数据区
 *
 * @return  u8			0,成功，其他,失败
 */
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    u8 i;
    IIC_Start();
    IIC_Send_Byte((addr << 1) | 0); //发送器件地址+写命令

    if(IIC_Wait_Ack())	//等待应答
    {
        IIC_Stop();
        return 1;
    }

    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答

    for(i = 0; i < len; i++)
    {
        IIC_Send_Byte(buf[i]);	//发送数据

        if(IIC_Wait_Ack())		//等待ACK
        {
            IIC_Stop();
            return 1;
        }
    }

    IIC_Stop();
    return 0;
}

/**
 * @brief	IIC连续读
 *
 * @param   addr	器件地址
 * @param   reg		要读取的寄存器地址
 * @param   len		要读取的长度
 * @param   buf		读取到的数据存储区
 *
 * @return  u8			0,成功，其他,失败
 */
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    IIC_Start();
    IIC_Send_Byte((addr << 1) | 0); //发送器件地址+写命令

    if(IIC_Wait_Ack())	//等待应答
    {
        IIC_Stop();
        return 1;
    }

    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
    IIC_Start();
    IIC_Send_Byte((addr << 1) | 1); //发送器件地址+读命令
    IIC_Wait_Ack();		//等待应答

    while(len)
    {
        if(len == 1)*buf = IIC_Read_Byte(0); //读数据,发送nACK

        else *buf = IIC_Read_Byte(1);		//读数据,发送ACK

        len--;
        buf++;
    }

    IIC_Stop();	//产生一个停止条件
    return 0;
}
/**
 * @brief	IIC写一个字节
 *
 * @param   reg		寄存器地址
 * @param   data		数据
 *
 * @return  u8			0,成功，其他,失败
 */
u8 MPU_Write_Byte(u8 reg, u8 data)
{
    IIC_Start();
    IIC_Send_Byte((MPU_ADDR << 1) | 0); //发送器件地址+写命令

    if(IIC_Wait_Ack())	//等待应答
    {
        IIC_Stop();
        return 1;
    }

    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
    IIC_Send_Byte(data);//发送数据

    if(IIC_Wait_Ack())	//等待ACK
    {
        IIC_Stop();
        return 1;
    }

    IIC_Stop();
    return 0;
}
/**
 * @brief	IIC读一个字节
 *
 * @param   reg		寄存器地址
 *
 * @return  u8		读到的数据
 */
u8 MPU_Read_Byte(u8 reg)
{
    u8 res;
    IIC_Start();
    IIC_Send_Byte((MPU_ADDR << 1) | 0); //发送器件地址+写命令
    IIC_Wait_Ack();		//等待应答
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
    IIC_Start();
    IIC_Send_Byte((MPU_ADDR << 1) | 1); //发送器件地址+读命令
    IIC_Wait_Ack();		//等待应答
    res = IIC_Read_Byte(0); //读取数据,发送nACK
    IIC_Stop();			//产生一个停止条件
    return res;
}

