#ifndef __DWM1000_Configure_H
#define __DWM1000_Configure_H
#include "sys.h"


#define DWM1000_RST PHout(2) 
#define DWM1000_INT PHout(3) 
#define DWM1000_NSS PBout(12) 

void DWM_SPI_Init(void);
int DWM1000_STA(void);
u8 SPI_ReadWriteByte(u8 TxData);
void DWM1000_TX_TEST(void);
#endif


