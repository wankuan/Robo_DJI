#ifndef __HAL_SDRAM_H
#define __HAL_SDRAM_H

#include "sys.h"

extern SDRAM_HandleTypeDef SDRAM_Handler;//SDRAM句柄

#define Bank5_SDRAM_ADDR    ((u32)(0XC0000000)) //SDRAM开始地址

//用于配置SDRAM初始化序列
#define SDRAM_MODEREG_BURST_LENGTH_1             ((u16)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((u16)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((u16)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((u16)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((u16)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((u16)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((u16)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((u16)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((u16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((u16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((u16)0x0200)
///////////////

void SDRAM_Init(void);
u8 SDRAM_Send_Cmd(u32 BANK_Num,u8 cmd,u8 refresh_time,u16 value);
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram); 
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n);
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);
u8 FMC_SDRAM_Write_ALL_BANK_32Bit(void);
u8 FMC_SDRAM_Write_ALL_BANK_16Bit(void);
u8 FMC_SDRAM_Write_ALL_BANK_8Bit(void);

#endif
