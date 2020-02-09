#ifndef __24C02_H
#define __24C02_H
#include "sys.h"

u8 IIC_Erase_ALL(void);

u8 AT24CXX_ReadOneByte(u8 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u8 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u8 WriteAddr,u8 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u8 AT24CXX_ReadLenByte(u8 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(u8 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(u8 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 AT24CXX_Check(void);  //�������

void AT24CXX_Init(void); //��ʼ��IIC

#define WP_24C02 PHout(6) 

//IO��������
#define SDA_IN()  {GPIOH->MODER&=0XFFFCFFFF;GPIOH->MODER|=0X00000000;}	//����ģʽ
#define SDA_OUT() {GPIOH->MODER&=0XFFFCFFFF;GPIOH->MODER|=0X00010000;} //���ģʽ

//IO����
#define IIC_SCL   PHout(7) //SCL
#define IIC_SDA   PHout(8) //SDA
#define READ_SDA  PHin(8)  //����SDA

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 


#endif
