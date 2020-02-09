#include "OLED.h"
#include "FONT.h"

u8 OLED_GRAM[8][128];

SPI_HandleTypeDef SPI1_Handler;  //SPI句柄



void SPI1_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		 
    SPI1_Handler.Instance=SPI1;                         //SP1
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//定义波特率预分频的值:波特率预分频值为256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI1_Handler);//初始化
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //使能


    __HAL_RCC_GPIOA_CLK_ENABLE();       //
		GPIO_Initure.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6; 
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;          
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
//////复位OLED
RES_OLED=1;
delay_ms(10);
RES_OLED=0;
delay_ms(50);
RES_OLED=1;
 
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);

SPI1_ReadWriteByte(0Xff);                           //启动传输
}   

//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 1000);       
		return Rxdata;          		    //返回收到的数据		
}


void OLED_Write_Data(u8 Write_Data_OLED,u8 decide)
{
	
	 NSS_SPI=0;
	if(decide==1)//1写命令  0写数据
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
//开启OLED显示
void OLED_Display_On(void)
{
	OLED_Write_Data(0X8D,OLED_CMD);  //电荷泵
	OLED_Write_Data(0X14,OLED_CMD);  //ON
	OLED_Write_Data(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_Write_Data(0X8D,OLED_CMD);  //电荷泵
	OLED_Write_Data(0X10,OLED_CMD);  //DCDC OFF
	OLED_Write_Data(0XAE,OLED_CMD);  //DISPLAY OFF
}
void OLED_Reset_Initial(void)
{
	
	SPI1_Init();
	
	OLED_Display_Off();
	
	OLED_Write_Data(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_Write_Data(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	
	OLED_Write_Data(0xA8,OLED_CMD); //设置驱动路数
	OLED_Write_Data(0X3F,OLED_CMD); //默认0X3F(1/64) 
	
	OLED_Write_Data(0xD3,OLED_CMD); //设置显示偏移
	OLED_Write_Data(0X00,OLED_CMD); //默认为0

	OLED_Write_Data(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_Write_Data(0x8D,OLED_CMD); //电荷泵设置
	OLED_Write_Data(0x14,OLED_CMD); //bit2，开启/关闭
	
	OLED_Write_Data(0x20,OLED_CMD); //设置内存地址模式
	OLED_Write_Data(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;

	OLED_Write_Data(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127; //左右镜像        A0+C0将会倒立显示
	
	OLED_Write_Data(0xC8,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数   //上下镜像
	
	OLED_Write_Data(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_Write_Data(0x10,OLED_CMD); //[5:4]配置
		 
	OLED_Write_Data(0x81,OLED_CMD); //对比度设置
	OLED_Write_Data(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	
	OLED_Write_Data(0xD9,OLED_CMD); //设置预充电周期
	OLED_Write_Data(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	
	OLED_Write_Data(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_Write_Data(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_Write_Data(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	
	OLED_Write_Data(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示     A6正常显示  A7反相显示

	OLED_Display_On();
	OLED_Clean();
}

void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Data (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_Write_Data (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_Write_Data (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[i][n],OLED_DATA); 
	}   
}
void OLED_Clean(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Data (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_Write_Data (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_Write_Data (0x10,OLED_CMD);      //设置显示位置—列高地址   
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
		OLED_Write_Data (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_Write_Data (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_Write_Data (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_Write_Data(0xff,OLED_DATA); 
	} 
}
void OLED_Test(void)//测试ABCDEFGHIJKLMNOP   023456789
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
void OLED_XY(u8 X_OLED,u8 Y_OLED) //XY偏移量，|__对OLED_GRAM的内容进行移动
{  
	u8 i,n;		    
		OLED_Write_Data(0xD3,OLED_CMD);
		OLED_Write_Data(Y_OLED,OLED_CMD);
	for(i=0;i<8;i++)  
	{  
    OLED_Write_Data (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_Write_Data (0x00+X_OLED%16,OLED_CMD); //低地址   //128=8*16  所以分为8份  每份16列 
		OLED_Write_Data (0x10+X_OLED/16,OLED_CMD); //高地址    高地址负责选择份   低地址负责选择列
		for(n=0;n<128;n++)OLED_Write_Data(OLED_GRAM[i][n],OLED_DATA);
	}   
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 by,cy,temp=0;
	if(x>128||y>64)return;//超出范围了
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
	if(mode)//ASCII  模式
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




