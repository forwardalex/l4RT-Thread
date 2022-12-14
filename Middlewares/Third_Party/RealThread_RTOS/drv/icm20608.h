#ifndef __MPU6050_H
#define __MPU6050_H
#include "myiic.h"


//MPU6050 AD0?????
#define MPU_AD0_CTRL			PAout(15)	//????AD0???,???????MPU???

//#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs?????,?????汾??,????????δ??
//#define MPU_PROD_ID_REG			0X0C	//prod id?????,?????????δ??
#define MPU_SELF_TESTX_REG		0X0D	//???????X
#define MPU_SELF_TESTY_REG		0X0E	//???????Y
#define MPU_SELF_TESTZ_REG		0X0F	//???????Z
#define MPU_SELF_TESTA_REG		0X10	//???????A
#define MPU_SAMPLE_RATE_REG		0X19	//???????????
#define MPU_CFG_REG				0X1A	//???ü????
#define MPU_GYRO_CFG_REG		0X1B	//?????????ü????
#define MPU_ACCEL_CFG_REG		0X1C	//?????????ü????
#define MPU_MOTION_DET_REG		0X1F	//??????????ü????
#define MPU_FIFO_EN_REG			0X23	//FIFO???????
#define MPU_I2CMST_CTRL_REG		0X24	//IIC????????????
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC???0????????????
#define MPU_I2CSLV0_REG			0X26	//IIC???0???????????
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC???0????????
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC???1????????????
#define MPU_I2CSLV1_REG			0X29	//IIC???1???????????
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC???1????????
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC???2????????????
#define MPU_I2CSLV2_REG			0X2C	//IIC???2???????????
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC???2????????
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC???3????????????
#define MPU_I2CSLV3_REG			0X2F	//IIC???3???????????
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC???3????????
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC???4????????????
#define MPU_I2CSLV4_REG			0X32	//IIC???4???????????
#define MPU_I2CSLV4_DO_REG		0X33	//IIC???4д????????
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC???4????????
#define MPU_I2CSLV4_DI_REG		0X35	//IIC???4??????????

#define MPU_I2CMST_STA_REG		0X36	//IIC???????????
#define MPU_INTBP_CFG_REG		0X37	//?ж?/??????ü????
#define MPU_INT_EN_REG			0X38	//?ж????????
#define MPU_INT_STA_REG			0X3A	//?ж????????

#define MPU_ACCEL_XOUTH_REG		0X3B	//??????,X???8λ?????
#define MPU_ACCEL_XOUTL_REG		0X3C	//??????,X???8λ?????
#define MPU_ACCEL_YOUTH_REG		0X3D	//??????,Y???8λ?????
#define MPU_ACCEL_YOUTL_REG		0X3E	//??????,Y???8λ?????
#define MPU_ACCEL_ZOUTH_REG		0X3F	//??????,Z???8λ?????
#define MPU_ACCEL_ZOUTL_REG		0X40	//??????,Z???8λ?????

#define MPU_TEMP_OUTH_REG		0X41	//???????λ?????
#define MPU_TEMP_OUTL_REG		0X42	//??????8λ?????

#define MPU_GYRO_XOUTH_REG		0X43	//???????,X???8λ?????
#define MPU_GYRO_XOUTL_REG		0X44	//???????,X???8λ?????
#define MPU_GYRO_YOUTH_REG		0X45	//???????,Y???8λ?????
#define MPU_GYRO_YOUTL_REG		0X46	//???????,Y???8λ?????
#define MPU_GYRO_ZOUTH_REG		0X47	//???????,Z???8λ?????
#define MPU_GYRO_ZOUTL_REG		0X48	//???????,Z???8λ?????

#define MPU_I2CSLV0_DO_REG		0X63	//IIC???0????????
#define MPU_I2CSLV1_DO_REG		0X64	//IIC???1????????
#define MPU_I2CSLV2_DO_REG		0X65	//IIC???2????????
#define MPU_I2CSLV3_DO_REG		0X66	//IIC???3????????

#define MPU_I2CMST_DELAY_REG	0X67	//IIC???????????????
#define MPU_SIGPATH_RST_REG		0X68	//????????λ?????
#define MPU_MDETECT_CTRL_REG	0X69	//?????????????
#define MPU_USER_CTRL_REG		0X6A	//???????????
#define MPU_PWR_MGMT1_REG		0X6B	//???????????1
#define MPU_PWR_MGMT2_REG		0X6C	//???????????2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO????????????λ
#define MPU_FIFO_CNTL_REG		0X73	//FIFO????????????λ
#define MPU_FIFO_RW_REG			0X74	//FIFO??д?????
#define MPU_DEVICE_ID_REG		0X75	//????ID?????

//???AD0??(9??)???,IIC????0X68(?????????λ).
//?????V3.3,??IIC????0X69(?????????λ).
#define MPU_ADDR				0X68


////??????AD0????GND,????????д?????,?0XD1??0XD0(?????VCC,???0XD3??0XD2)
//#define MPU_READ    0XD1
//#define MPU_WRITE   0XD0

u8 ICM20608_Init(void); 								//?????MPU6050
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf); //IIC????д
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf); //IIC??????
u8 MPU_Write_Byte(u8 reg, u8 data);				//IICд??????
u8 MPU_Read_Byte(u8 reg);						//IIC????????

u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_LPF(u16 lpf);
u8 MPU_Set_Rate(u16 rate);
u8 MPU_Set_Fifo(u8 sens);


short MPU_Get_Temperature(void);
u8 MPU_Get_Gyroscope(short *gx, short *gy, short *gz);
u8 MPU_Get_Accelerometer(short *ax, short *ay, short *az);

#endif




































