#ifndef __SPI3_H
#define __SPI3_H
#include "main.h"



void SPI3_Init(void);
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI3_ReadWriteByte(u8 TxData);
u8 SPI3_WriteByte(u8 *TxData,u16 size);











#endif


