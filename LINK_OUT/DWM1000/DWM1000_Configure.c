#include "DWM1000_Configure.h"
#include "delay.h"
#include "deca_types.h"
#include "deca_param_types.h"
#include "deca_regs.h"
#include "deca_device_api.h"
#include "sleep.h"
#include "lcd.h"
///////////////////
SPI_HandleTypeDef SPI_DWM_Handler;  //SPI句柄



/* Default communication configuration. We use here EVK1000's default mode (mode 3). */
static dwt_config_t config = {
    2,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_1024,   /* Preamble length. Used in TX only. */
    DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_110K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

/* The frame sent in this example is an 802.15.4e standard blink. It is a 12-byte frame composed of the following fields:
 *     - byte 0: frame type (0xC5 for a blink).
 *     - byte 1: sequence number, incremented for each new frame.
 *     - byte 2 -> 9: device ID, see NOTE 1 below.
 *     - byte 10/11: frame check-sum, automatically set by DW1000.  */
u8 tx_msg[] = {0xC5, 0, 'D', 'E', 'C', 'A', 'W', 'A', 'V', 'E','T','E','S','T',0,0};
/* Index to access to sequence number of the blink frame in the tx_msg array. */
#define BLINK_FRAME_SN_IDX 1

u8 *RX=tx_msg+2;

/* Inter-frame delay period, in milliseconds. */
#define TX_DELAY_MS 50


void DWM1000_TX_TEST(void)
{
	        //dwt_readdevid();
	
	        /* Write frame data to DW1000 and prepare transmission. See NOTE 4 below.*/
        dwt_writetxdata(sizeof(tx_msg), tx_msg, 0); /* Zero offset in TX buffer. */
        dwt_writetxfctrl(sizeof(tx_msg), 0, 0); /* Zero offset in TX buffer, no ranging. */

        /* Start transmission. */
        dwt_starttx(DWT_START_TX_IMMEDIATE);

        /* Poll DW1000 until TX frame sent event set. See NOTE 5 below.
         * STATUS register is 5 bytes long but, as the event we are looking at is in the first byte of the register, we can use this simplest API
         * function to access it.*/
        while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS))
        { };
        /* Clear TX frame sent event. */
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);
								LCD_ShowString(500,200,200,32,32,"The send data"); 
								LCD_ShowString(500,240,300,32,32,RX); 
        /* Execute a delay between transmissions. */ 
        sleep_ms(TX_DELAY_MS);

        /* Increment the blink frame sequence number (modulo 256). */
        tx_msg[BLINK_FRAME_SN_IDX]++;
}
int DWM1000_STA(void)
{
	int i;
	DWM_SPI_Init();
	delay_ms(2);
	i=dwt_initialise(DWT_LOADUCODE);
	dwt_configure(&config);
	return i;
}


void DWM_SPI_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		 
    SPI_DWM_Handler.Instance=SPI2;                         //SP2
    SPI_DWM_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI_DWM_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI_DWM_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_DWM_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI_DWM_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_DWM_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_DWM_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_16;//定义波特率预分频的值:波特率预分频值为256
    SPI_DWM_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_DWM_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI_DWM_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI_DWM_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI_DWM_Handler);//初始化


				__HAL_RCC_GPIOH_CLK_ENABLE();//复位脚和中断引脚配置      
				GPIO_Initure.Pin = GPIO_PIN_2|GPIO_PIN_3; 
				GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_Initure.Pull = GPIO_PULLUP;        
				GPIO_Initure.Speed = GPIO_SPEED_HIGH;          
				HAL_GPIO_Init(GPIOH,&GPIO_Initure);
				
				DWM1000_NSS=1;
			 delay_ms(10);
				DWM1000_RST=0;//拉低
				delay_ms(100);	
				DWM1000_RST=1;			
				DWM1000_INT=1;
    //模块开始工作
		  __HAL_SPI_ENABLE(&SPI_DWM_Handler);                    //使能
				
}   
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	 if(hspi->Instance==SPI2)
	{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();       
    __HAL_RCC_SPI2_CLK_ENABLE();             
	
		GPIO_Initure.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; 
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;
		GPIO_Initure.Alternate=GPIO_AF5_SPI2;          
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
			
		GPIO_Initure.Pin = GPIO_PIN_12; 
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;         
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
	}
	
}



//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI_DWM_Handler,&TxData,&Rxdata,1, 1000);
				return Rxdata;          		    //返回收到的数据	
}
