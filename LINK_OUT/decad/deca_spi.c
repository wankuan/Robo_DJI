/*! ----------------------------------------------------------------------------
 * @file	deca_spi.c
 * @brief	SPI access functions
 *
 * @attention
 *
 * Copyright 2013 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */
#include <string.h>

#include "deca_spi.h"
#include "deca_device_api.h"
#include "DWM1000_Configure.h"
#include "sys.h"
#include "delay.h"


extern SPI_HandleTypeDef SPI_DWM_Handler;  //SPI句柄
/*! ------------------------------------------------------------------------------------------------------------------
 * Function: openspi()
 *
 * Low level abstract function to open and initialise access to the SPI device.
 * returns 0 for success, or -1 for error
 */
int openspi(SPI_HandleTypeDef* SPIx)
{
	// done by port.c, default SPI used is SPI1
 __HAL_SPI_ENABLE(SPIx);                    //使能
	return 0;
} // end openspi()

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: closespi()
 *
 * Low level abstract function to close the the SPI device.
 * returns 0 for success, or -1 for error
 */
int closespi(SPI_HandleTypeDef* SPIx)
{
 __HAL_SPI_DISABLE(SPIx);                    //使能
	return 0;
} // end closespi()

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: writetospi()
 *
 * Low level abstract function to write to the SPI
 * Takes two separate byte buffers for write header and write data
 * returns 0 for success, or -1 for error
 */
//#pragma GCC optimize ("O3")
int writetospi(uint16 headerLength, uint8 *headerBuffer, uint32 bodylength, uint8 *bodyBuffer)
{


	  DWM1000_NSS=0;
		 HAL_SPI_Transmit(&SPI_DWM_Handler,headerBuffer, headerLength, 1000);
		 HAL_SPI_Transmit(&SPI_DWM_Handler,bodyBuffer, bodylength, 1000);
			DWM1000_NSS=1;
  //  decamutexoff(stat) ;
    return 0;
} // end writetospi()


/*! ------------------------------------------------------------------------------------------------------------------
 * Function: readfromspi()
 *
 * Low level abstract function to read from the SPI
 * Takes two separate byte buffers for write header and read data
 * returns the offset into read buffer where first byte of read data may be found,
 * or returns -1 if there was an error
 */
//#pragma GCC optimize ("O3")
int readfromspi(uint16 headerLength,uint8 *headerBuffer, uint32 readlength, uint8 *readBuffer)
{

	  DWM1000_NSS=0;
	  delay_us(1);
	  u8 R_G;
    for(int i=0; i<headerLength; i++)
    {
						 HAL_SPI_TransmitReceive(&SPI_DWM_Handler, &headerBuffer[i], &R_G,1,2000);
							readBuffer[0]=R_G;
    }
					HAL_SPI_Receive(&SPI_DWM_Handler,readBuffer, readlength,2000);
   // decamutexoff(stat) ;
			delay_us(1);
	  DWM1000_NSS=1;
    return 0;
} // end readfromspi()
