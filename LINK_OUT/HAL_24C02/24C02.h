#ifndef __24C02_H
#define __24C02_H
#include "sys.h"

u8 IIC_Erase_ALL(void);

u8 AT24CXX_ReadOneByte(u8 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u8 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u8 WriteAddr,u8 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u8 AT24CXX_ReadLenByte(u8 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(u8 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u8 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件

void AT24CXX_Init(void); //初始化IIC

#define WP_24C02 PHout(6) 

//IO方向设置
#define SDA_IN()  {GPIOH->MODER&=0XFFFCFFFF;GPIOH->MODER|=0X00000000;}	//输入模式
#define SDA_OUT() {GPIOH->MODER&=0XFFFCFFFF;GPIOH->MODER|=0X00010000;} //输出模式

//IO操作
#define IIC_SCL   PHout(7) //SCL
#define IIC_SDA   PHout(8) //SDA
#define READ_SDA  PHin(8)  //输入SDA

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 


#endif
