#include "DWM1000_Configure.h"
#include "delay.h"
#include "deca_types.h"
#include "deca_param_types.h"
#include "deca_regs.h"
#include "deca_device_api.h"
#include "sleep.h"
#include "lcd.h"
///////////////////
SPI_HandleTypeDef SPI_DWM_Handler;  //SPI���



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
    SPI_DWM_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI_DWM_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI_DWM_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_DWM_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_DWM_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_DWM_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_DWM_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_16;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_DWM_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_DWM_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI_DWM_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI_DWM_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI_DWM_Handler);//��ʼ��


				__HAL_RCC_GPIOH_CLK_ENABLE();//��λ�ź��ж���������      
				GPIO_Initure.Pin = GPIO_PIN_2|GPIO_PIN_3; 
				GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_Initure.Pull = GPIO_PULLUP;        
				GPIO_Initure.Speed = GPIO_SPEED_HIGH;          
				HAL_GPIO_Init(GPIOH,&GPIO_Initure);
				
				DWM1000_NSS=1;
			 delay_ms(10);
				DWM1000_RST=0;//����
				delay_ms(100);	
				DWM1000_RST=1;			
				DWM1000_INT=1;
    //ģ�鿪ʼ����
		  __HAL_SPI_ENABLE(&SPI_DWM_Handler);                    //ʹ��
				
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



//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI_DWM_Handler,&TxData,&Rxdata,1, 1000);
				return Rxdata;          		    //�����յ�������	
}
