//////////////////////////////////////////////////////////////////////////////////	 
//////////////////////////////////////////////////////////////////////////////////	 
/* NRF24L01使用笔记
关于SPI：
MISO 浮空输入
MOSI 复用推挽输出
SCK 复用推挽输出
NSS 推挽输出  如果用硬件 即复用推挽输出（硬件SPI控制）  软件即推挽输出
CE  NRF24L01发送/接收使能   推挽输出
IRQ 中断输出脚  接入外部中断  上拉输入   低电平使能  外部中断  使用  下降沿产生中断


关于CE：
接收状态   选择接收模式后   拉高  1 高电平
发送状态   选择发送模式后   拉低  进入待机模式  数据写入TX_FIFO 拉高保持>10US   拉低 送出一包数据


关于IRQ：（有三种情况可以出发低电平中断）
		发送端
		1.发送一包数据 出去  收到ACK   拉低 表示发送成功
		2.重发数据包达到最大次数   

		接收端
		1.成功接收到数据
		

各电平跳变沿的间距
	发送成功    发送端的CE脚和IRQ脚之间  <1MS  的间距
	发送失败 达到重发次数（即接收端没有ACK）  >3MS  的坚决
*/
//////////////////////////////////////////////////////////////////////////////////	 
//Editor：Huangwankuan
//VERSION:1.0
//2017.2.26


/*
函数使用方法:
发送端：
   SPI初始化  调用SPI.C     （需要调整参数）
	 24L01初始化  调用void NRF24L01_Initial(void)   （需要调整参数）
	 TX模式配置  void NRF24L01_TX_Mode(void);					//配置为发送模式  （不用调整参数）
	 发送一包数据  u8 NRF24L01_TxPacket(u8 *txbuf);				//发送一个包的数据   （不用调整参数）
	 
接收端：
      SPI初始化  调用SPI.C     （需要调整参数）
			24L01初始化  调用void NRF24L01_Initial(void)   （需要调整参数）
			中断模式————外部中断初始化  调用EXTI.c 
			查询模式————不需要进行中断初始化
			RX模式配置  void NRF24L01_RX_Mode(void);					//配置为发送模式  （不用调整参数）
			中断模式————
		void EXTI0_IRQHandler(void)
{
  	u8 sta;	
	
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值	
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志	  
	
	if(sta&RX_OK)//接收到数据
	{	
		NRF24L01_Read_Buf(RD_RX_PLOAD,Data_buf,RX_PLOAD_WIDTH);//读取数据 
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
	}	
	
	EXTI_ClearITPendingBit(EXTI_Line0);//清除EXTI线路挂起位
}

			查询模式———接收一个包数据  u8 NRF24L01_RxPacket(u8 *txbuf);				//接收一个包的数据   （不用调整参数）
*/
//////////////////////////////////////////////////////////////////////////////////	 


#include "NRF24L01.h"
#include "delay.h"
//const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x33,0x33,0x33,0x33,0x33}; //发送地址
//const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x33,0x33,0x33,0x33,0x33};//接收地址

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x02}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x02}; //接收地址


u8 NRF_RXD[8];
//初始化24L01的IO口
//

SPI_HandleTypeDef SPI2_Handler;  //SPI句柄

void NRF24L01_Initial(void)
{ 	
	  GPIO_InitTypeDef GPIO_Initure;
	
	  __HAL_RCC_GPIOB_CLK_ENABLE();       
    __HAL_RCC_SPI2_CLK_ENABLE();       
		__HAL_RCC_GPIOD_CLK_ENABLE(); 
  
    SPI2_Handler.Instance=SPI2;                         //SP1
    SPI2_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI2_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_LOW;    //串行同步时钟的空闲状态为高电平
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_1EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI2_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_8;//定义波特率预分频的值:波特率预分频值为256
    SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI2_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI2_Handler);//初始化

		GPIO_Initure.Pin = GPIO_PIN_11;    //CE
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;          
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);

		
		GPIO_Initure.Pin = GPIO_PIN_12;    //CSN
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;          
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	  #ifdef  NRF24L01_IT  //配置并打开中断
		
		GPIO_Initure.Pin = GPIO_PIN_12;    //IRQ
		GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;          
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);
				
	  HAL_NVIC_SetPriority(EXTI15_10_IRQn,1,0);     
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);      
		
		#else
		
		GPIO_Initure.Pin = GPIO_PIN_12;    //IRQ
		GPIO_Initure.Mode = GPIO_MODE_INPUT;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;          
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);
		
		#endif 

    __HAL_SPI_ENABLE(&SPI2_Handler);                    //使能

	  SPI2_Read_Write_Byte(0xff);
	  
		NRF24L01_CE=0; 			//使能24L01
		NRF24L01_CSN=1;			//SPI片选取消  
	 		 	 
}




//检测24L01是否存在
//返回值:0，成功;1，失败	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XCC,0XCC,0XCC,0XCC,0XCC};
	u8 i;	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)
	{
		if(buf[i]!=0XCC)
		{
			return 1;//检测24L01错误	
		}
	}		
	return 0;		 //检测到24L01
}	 	 




//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_Read_Write_Byte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI2_Handler,&TxData,&Rxdata,1, 1000);       
		return Rxdata;          		    //返回收到的数据		
}

//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN=0;                 //使能SPI传输
  	status =SPI2_Read_Write_Byte(reg);//发送寄存器号 
  	SPI2_Read_Write_Byte(value);      //写入寄存器的值
  	NRF24L01_CSN=1;                 //禁止SPI传输	   
  	return(status);       			//返回状态值
}


//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN = 0;          //使能SPI传输		
  	SPI2_Read_Write_Byte(reg);   //发送寄存器号
  	reg_val=SPI2_Read_Write_Byte(0XFF);//读取寄存器内容
  	NRF24L01_CSN = 1;          //禁止SPI传输		    
  	return(reg_val);           //返回状态值
}	


//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN = 0;           //使能SPI传输
  	status=SPI2_Read_Write_Byte(reg);//发送寄存器值(位置),并读取状态值   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI2_Read_Write_Byte(0XFF);//读出数据
  	NRF24L01_CSN=1;       //关闭SPI传输
  	return status;        //返回读到的状态值
}




//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          //使能SPI传输
  	status = SPI2_Read_Write_Byte(reg);//发送寄存器值(位置),并读取状态值
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI2_Read_Write_Byte(*pBuf++); //写入数据	 *pbuf++ 先取BUF的值 再将BUF的地址++
  	NRF24L01_CSN = 1;       //关闭SPI传输
  	return status;          //返回读到的状态值
}		








//启动NRF24L01接收一次数据
//txbuf:待发送数据首地址  可定义指针进行移动
//返回值:0，接收完成；返回1没有接收
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;	
	
	while(NRF24L01_IRQ!=0);//等待接收完成	
	
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除接收/发送/多次重发标志位
	//务必清除，根据DS清除标志位才可进行通讯
	if(sta&RX_OK)//接收到数据      
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//从FIFO寄存器中读取数据  接收字节长度32字节
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器    读取完毕后清除  以进行下次FIFO寄存器接收
		return 0; 
	}	   
	return 1;//没收到任何数据
}				




//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void NRF24L01_RX_Mode(void)
{
	  NRF24L01_CE=0;	  
		NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//指针指向RX_ADDRESS 写RX节点地址  五个字节 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //设置RF通信频率		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	NRF24L01_CE = 1; //CE为高,进入接收模式 
}						 







//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
		u8 sta;	
	  u8 feedback=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节  准备进入发送模式
		NRF24L01_CE=1;//启动发送数据	
		delay_us(20); //短暂延时 >10us
		NRF24L01_CE=0;//启动发送
	
	while((NRF24L01_IRQ!=0)&&(feedback<100))//发送过程一直为1  低电平即表示收到ACK或达到最大重发次数
		{
			feedback++;
			delay_us(100);
		}
	//while((NRF24L01_IRQ!=0));//发送过程一直为1  低电平即表示收到ACK或达到最大重发次数
	
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); ////清除接收/发送/多次重发标志位
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器    读取完毕后清除  以进行下次FIFO寄存器接收
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		 return TX_OK;
	}
		return sta;//其他原因发送失败
}




//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void NRF24L01_TX_Mode(void)
{														 
		NRF24L01_CE=0;	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//指针指向TX_ADDRESS 写RX节点地址  五个字节 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x05);//设置自动重发间隔时间:250us + 86us;最大自动重发次数:5次
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通信频率+40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
}



void EXTI15_10_IRQHandler(void)
{
      HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);//调用中断处理公用函数
	    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);//调用中断处理公用函数
	    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);//调用中断处理公用函数
	    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);//调用中断处理公用函数
	    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);//调用中断处理公用函数
		  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);//调用中断处理公用函数
}
//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   if(GPIO_Pin==GPIO_PIN_12)
	 {
	u8 sta;	
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除接收/发送/多次重发标志位
	//务必清除，根据DS清除标志位才可进行通讯
	if(sta&RX_OK)//接收到数据      
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,NRF_RXD,RX_PLOAD_WIDTH);//从FIFO寄存器中读取数据  接收字节长度32字节
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器    读取完毕后清除  以进行下次FIFO寄存器接收
	}	   
	 }
}



