#include "OLED.h"
#include "FONT.h"

u8 OLED_GRAM[8][128];

SPI_HandleTypeDef SPI1_Handler;  //SPI���



void SPI1_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		 
    SPI1_Handler.Instance=SPI1;                         //SP1
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI1_Handler);//��ʼ��
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //ʹ��


    __HAL_RCC_GPIOA_CLK_ENABLE();       //
		GPIO_Initure.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6; 
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;          
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
//////��λOLED
RES_OLED=1;
delay_ms(10);
RES_OLED=0;
delay_ms(50);
RES_OLED=1;
 
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);

SPI1_ReadWriteByte(0Xff);                           //��������
}   

//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 1000);       
		return Rxdata;          		    //�����յ�������		
}


void OLED_Write_Data(u8 Write_Data_OLED,u8 decide)
{
	
	 NSS_SPI=0;
	if(decide==1)//1д����  0д����
	{           
		CMD_OLED=0;
		SPI1_ReadWriteByte(Write_Data_OLED);

	}
	else
	{
		CMD_OLED=1;
		SPI1_ReadWriteByte(Write_Data_OLED);
	}
	NSS_SPI=1;
}
//����OLED��ʾ
void OLED_Display_On(void)
{
	OLED_Write_Data(0X8D,OLED_CMD);  //��ɱ�
	OLED_Write_Data(0X14,OLED_CMD);  //ON
	OLED_Write_Data(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_Write_Data(0X8D,OLED_CMD);  //��ɱ�
	OLED_Write_Data(0X10,OLED_CMD);  //DCDC OFF
	OLED_Write_Data(0XAE,OLED_CMD);  //DISPLAY OFF
}
void OLED_Reset_Initial(void)
{
	
	SPI1_Init();
	
	OLED_Display_Off();
	
	OLED_Write_Data(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_Write_Data(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	
	OLED_Write_Data(0xA8,OLED_CMD); //��������·��
	OLED_Write_Data(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	
	OLED_Write_Data(0xD3,OLED_CMD); //������ʾƫ��
	OLED_Write_Data(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_Write_Data(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_Write_Data(0x8D,OLED_CMD); //��ɱ�����
	OLED_Write_Data(0x14,OLED_CMD); //bit2������/�ر�
	
	OLED_Write_Data(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_Write_Data(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;

	OLED_Write_Data(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127; //���Ҿ���        A0+C0���ᵹ����ʾ
	
	OLED_Write_Data(0xC8,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��   //���¾���
	
	OLED_Write_Data(0xDA,OLED_CMD); //����COMӲ����������
	OLED_Write_Data(0x10,OLED_CMD); //[5:4]����
		 
	OLED_Write_Data(0x81,OLED_CMD); //�Աȶ�����
	OLED_Write_Data(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	
	OLED_Write_Data(0xD9,OLED_CMD); //����Ԥ�������
	OLED_Write_Data(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	
	OLED_Write_Data(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_Write_Data(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_Write_Data(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	
	OLED_Write_Data(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ     A6������ʾ  A7������ʾ

	OLED_Display_On();
	OLED_Clean();
}

void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Data (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Data (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_Write_Data (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[i][n],OLED_DATA); 
	}   
}
void OLED_Clean(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Data (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Data (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_Write_Data (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_Data(0,OLED_DATA); 
	}  
}
void OLED_Clean_GRAM(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{    
		for(n=0;n<128;n++)OLED_GRAM[i][n]=0;
	}  
}


void OLED_ALL_ON(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Data (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Data (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_Write_Data (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_Data(0xff,OLED_DATA); 
	} 
}
void OLED_Test(void)//����ABCDEFGHIJKLMNOP   023456789
{
			u8 i,n,j;

		for(j=0;j<16;j++)
		{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[0][j*8+n]=zimu[j][n];
				}
		}
				for(j=0;j<16;j++)
		{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[1][j*8+n]=zimu[j][n+8];
				}
		}
						for(j=0;j<10;j++)
		{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[2][j*8+n]=DZ[j][n];
				}
		}
				   for(j=0;j<10;j++)
		{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[3][j*8+n]=DZ[j][n+8];
				}
		}
		for(i=4;i<8;i++)
	{
						for(j=0;j<16;j++)
		{
        for(n=0;n<8;n++)
		{
	      OLED_GRAM[i][j*8+n]=0;
		}		
	}
	}
}
void OLED_BMP(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{     
		for(n=0;n<128;n++)
		{
			OLED_GRAM[i][n]= BMP[8*i+n/16][n%16];
			//OLED_GRAM[i][n]= BMP_1[i][n];

		}
	}  
}
void OLED_BMP_3(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{     
		for(n=0;n<128;n++)
		{
			OLED_GRAM[i][n]= BMP_3[8*i+n/16][n%16];
		}
	}  
}
void OLED_XY(u8 X_OLED,u8 Y_OLED) //XYƫ������|__��OLED_GRAM�����ݽ����ƶ�
{  
	u8 i,n;		    
		OLED_Write_Data(0xD3,OLED_CMD);
		OLED_Write_Data(Y_OLED,OLED_CMD);
	for(i=0;i<8;i++)  
	{  
    OLED_Write_Data (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Data (0x00+X_OLED%16,OLED_CMD); //�͵�ַ   //128=8*16  ���Է�Ϊ8��  ÿ��16�� 
		OLED_Write_Data (0x10+X_OLED/16,OLED_CMD); //�ߵ�ַ    �ߵ�ַ����ѡ���   �͵�ַ����ѡ����
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[i][n],OLED_DATA);
	}   
}
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 by,cy,temp=0;
	if(x>128||y>64)return;//������Χ��
	by=y%8;
	cy=y-1;
	switch(by)
	{
		case 1: temp=0x80;break;
	  case 2: temp=0x40;break;
		case 3: temp=0x20;break;
	  case 4: temp=0x10;break;
		case 5: temp=0x08;break;
	  case 6: temp=0x04;break;
		case 7: temp=0x02;break;
	  case 0: temp=0x01;break;
		default: break;
	}
	if(t)OLED_GRAM[7-cy/8][x-1]|=temp;
	else OLED_GRAM[7-cy/8][x-1]&=~temp;	    
}

void OLED_OUT_String(u8 x,u8 y,u8 value,u8 mode)
{
	u8 n;
	y=4-y;
	if(mode)//ASCII  ģʽ
	{
        for(n=0;n<8;n++)
				{
	      OLED_GRAM[2*y][8*(x-1)+n]=ASCII[value][n];
				}	
				for(n=0;n<8;n++)
				{
	      OLED_GRAM[2*y+1][8*(x-1)+n]=ASCII[value][n+8];
				}	
	}
	else
	{
		    for(n=0;n<16;n++)
				{
	      OLED_GRAM[2*y][8*(x-1)+n]=String_ST[value][n];
				}	
				for(n=0;n<16;n++)
				{
	      OLED_GRAM[2*y+1][8*(x-1)+n]=String_ST[value][n+16];
				}	
	}
		
}




