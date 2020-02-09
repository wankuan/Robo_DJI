
/*
extern u8 ov_sta;	//֡���� ��λ��־λ


//����LCD��ʾ
void camera_refresh(void)
{
	 u32 j;
 	u16 color;
		 
	if(ov_sta)				//�жϻ������Ƿ�����������
	{
	  	 
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK=0;
		OV7670_RCK=1;
		OV7670_RCK=0;		
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK=1;  
		//���÷� �ǽ� ����д���ݵĲ�����ȫ��  ��Ҫ��Ϊ�������ʾ�ٶ�  ������ע�Ͳ���Ϊ�ο����֣�

//////д����
////void Lcd_Write_Data(unsigned int data)
////{
//////TFT_RS(1);       //ѡ������
//// GPIOC->BSRR|=GPIO_Pin_7;
//// TFT_DATA(data);  //��ֵ
//// GPIOC->BRR|=GPIO_Pin_8;
//// GPIOC->BSRR|=GPIO_Pin_8;
//////TFT_WR(0);
//////TFT_WR(1);       //д�����ߵ�ƽ
////}
		u16 x,y;                        
		for(j=0;j<76800;j++)			   //�ֱ���Ϊ240x320   ÿ����ɫ��Ҫ�����ֽ� ���� 240x320x2=76800  ��
		{
			OV7670_RCK=0;				 //ÿһ��ʱ������ ��һ������
			color=((GPIOA->IDR)&0x3FC)>>2;	//������   ��ȡ��ɫ���ֽ�����
//			LCD_ShowNum(20,360,color,20,24);	
			OV7670_RCK=1; 
			color<<=8;  				 //����8λ  �����ֽ��Ƶ���8λ  Ϊ���յ�8λ�ֽ���׼��
			OV7670_RCK=0;
			color|=((GPIOA->IDR)&0x3FC)>>2;	//������   ��ȡ��ɫ���ֽ����� ��ת��Ϊ16λ 565 ��ɫ����
			OV7670_RCK=1; 
 //  LCD_ShowNum(20,400,color,20,24);	
//  		 // TFT_DATA(color);  //��ֵ
//		     GPIOB->BRR=GPIO_Pin_All;	   //��Ĵ�������
//             GPIOB->BSRR=color;			   //���ɼ�����ɫֵ��PB�ڼĴ��� 

//             GPIOC->BRR|=GPIO_Pin_8;		//��ʾ��TFT
//             GPIOC->BSRR|=GPIO_Pin_8;
//		 // Lcd_Write_Data(color);
			y=j/240;
			x=j%240;
			
			*(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
		}   							 
		ov_sta=0;					//��ʼ��һ�βɼ�
	} 
}
*/

#include "OV7670.h"
#include "ov7670config.h"
#include "LED.h"
u8 ov_sta;

//�ⲿ�ж�8��ʼ��
void EXTI2_Init(void)
{   
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_GPIOC_CLK_ENABLE();           
		
		GPIO_Initure.Pin=GPIO_PIN_7; 
		GPIO_Initure.Mode=GPIO_MODE_IT_RISING ;
		GPIO_Initure.Pull=GPIO_PULLDOWN;        
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		HAL_NVIC_SetPriority(EXTI9_5_IRQn,0,1);   //��ռ���ȼ�Ϊ2�������ȼ�Ϊ3
		HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);         //ʹ���ж���10  
}

void EXTI9_5_IRQHandler(void)
{
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);//�����жϴ����ú���
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 if(GPIO_Pin==GPIO_PIN_7)
	{
					if(ov_sta<2)
			{
							OV7670_WRST=0;	 	//��λдָ��	  		 
							OV7670_WRST=1;	
							OV7670_WREN=1;		//����д��FIFO
							ov_sta++;
			}
	}
}
		    			    
//��ʼ��OV7670
//����0:�ɹ�
//��������ֵ:�������
u8 OV7670_Init(void)
{
	u8 temp;
	u16 i=0;
	 
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_GPIOA_CLK_ENABLE(); 
		__HAL_RCC_GPIOB_CLK_ENABLE();           
		__HAL_RCC_GPIOC_CLK_ENABLE();  
		__HAL_RCC_GPIOD_CLK_ENABLE();  
	
	
		GPIO_Initure.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9; 
		GPIO_Initure.Mode = GPIO_MODE_INPUT;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	
		GPIO_Initure.Pin = GPIO_PIN_5; 
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
		GPIO_Initure.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6; 
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
		GPIO_Initure.Pin = GPIO_PIN_3; 
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);

 	SCCB_Init();        		//��ʼ��SCCB ��IO��	   	  
 	if(SCCB_WR_Reg(0x12,0x80))return 1;	//��λSCCB	  
		delay_ms(50); 
	//��ȡ��Ʒ�ͺ�
 	temp=SCCB_RD_Reg(0x0b);   
	if(temp!=0x73)return 2;  
 	temp=SCCB_RD_Reg(0x0a);   
	if(temp!=0x76)return 2;
	//��ʼ������	  
	for(i=0;i<sizeof(ov7670_init_reg_tbl)/sizeof(ov7670_init_reg_tbl[0])/2;i++)
	{
	   	SCCB_WR_Reg(ov7670_init_reg_tbl[i][0],ov7670_init_reg_tbl[i][1]);
		delay_ms(2);
 	}
   	return 0x00; 	//ok
} 
////////////////////////////////////////////////////////////////////////////
//OV7670��������
//��ƽ������
//0:�Զ�
//1:̫��sunny
//2,����cloudy
//3,�칫��office
//4,����home
void OV7670_Light_Mode(u8 mode)
{
	u8 reg13val=0XE7;//Ĭ�Ͼ�������Ϊ�Զ���ƽ��
	u8 reg01val=0;
	u8 reg02val=0;
	switch(mode)
	{
		case 1://sunny
			reg13val=0XE5;
			reg01val=0X5A;
			reg02val=0X5C;
			break;	
		case 2://cloudy
			reg13val=0XE5;
			reg01val=0X58;
			reg02val=0X60;
			break;	
		case 3://office
			reg13val=0XE5;
			reg01val=0X84;
			reg02val=0X4c;
			break;	
		case 4://home
			reg13val=0XE5;
			reg01val=0X96;
			reg02val=0X40;
			break;	
	}
	SCCB_WR_Reg(0X13,reg13val);//COM8���� 
	SCCB_WR_Reg(0X01,reg01val);//AWB��ɫͨ������ 
	SCCB_WR_Reg(0X02,reg02val);//AWB��ɫͨ������ 
}				  
//ɫ������
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(u8 sat)
{
	u8 reg4f5054val=0X80;//Ĭ�Ͼ���sat=2,��������ɫ�ȵ�����
 	u8 reg52val=0X22;
	u8 reg53val=0X5E;
 	switch(sat)
	{
		case 0://-2
			reg4f5054val=0X40;  	 
			reg52val=0X11;
			reg53val=0X2F;	 	 
			break;	
		case 1://-1
			reg4f5054val=0X66;	    
			reg52val=0X1B;
			reg53val=0X4B;	  
			break;	
		case 3://1
			reg4f5054val=0X99;	   
			reg52val=0X28;
			reg53val=0X71;	   
			break;	
		case 4://2
			reg4f5054val=0XC0;	   
			reg52val=0X33;
			reg53val=0X8D;	   
			break;	
	}
	SCCB_WR_Reg(0X4F,reg4f5054val);	//ɫ�ʾ���ϵ��1
	SCCB_WR_Reg(0X50,reg4f5054val);	//ɫ�ʾ���ϵ��2 
	SCCB_WR_Reg(0X51,0X00);			//ɫ�ʾ���ϵ��3  
	SCCB_WR_Reg(0X52,reg52val);		//ɫ�ʾ���ϵ��4 
	SCCB_WR_Reg(0X53,reg53val);		//ɫ�ʾ���ϵ��5 
	SCCB_WR_Reg(0X54,reg4f5054val);	//ɫ�ʾ���ϵ��6  
	SCCB_WR_Reg(0X58,0X9E);			//MTXS 
}
//��������
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//Ĭ�Ͼ���bright=2
  	switch(bright)
	{
		case 0://-2
			reg55val=0XB0;	 	 
			break;	
		case 1://-1
			reg55val=0X98;	 	 
			break;	
		case 3://1
			reg55val=0X18;	 	 
			break;	
		case 4://2
			reg55val=0X30;	 	 
			break;	
	}
	SCCB_WR_Reg(0X55,reg55val);	//���ȵ��� 
}
//�Աȶ�����
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(u8 contrast)
{
	u8 reg56val=0X40;//Ĭ�Ͼ���contrast=2
  	switch(contrast)
	{
		case 0://-2
			reg56val=0X30;	 	 
			break;	
		case 1://-1
			reg56val=0X38;	 	 
			break;	
		case 3://1
			reg56val=0X50;	 	 
			break;	
		case 4://2
			reg56val=0X60;	 	 
			break;	
	}
	SCCB_WR_Reg(0X56,reg56val);	//�Աȶȵ��� 
}
//��Ч����
//0:��ͨģʽ    
//1,��Ƭ
//2,�ڰ�   
//3,ƫ��ɫ
//4,ƫ��ɫ
//5,ƫ��ɫ
//6,����	    
void OV7670_Special_Effects(u8 eft)
{
	u8 reg3aval=0X04;//Ĭ��Ϊ��ͨģʽ
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1://��Ƭ
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://�ڰ�
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://����
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	SCCB_WR_Reg(0X3A,reg3aval);//TSLB���� 
	SCCB_WR_Reg(0X68,reg67val);//MANU,�ֶ�Uֵ 
	SCCB_WR_Reg(0X67,reg68val);//MANV,�ֶ�Vֵ 
}	
//����ͼ���������
//��QVGA���á�
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp; 
	endx=sx+width*2;	//V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	temp=SCCB_RD_Reg(0X03);				//��ȡVref֮ǰ��ֵ
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(0X03,temp);				//����Vref��start��end�����2λ
	SCCB_WR_Reg(0X19,sx>>2);			//����Vref��start��8λ
	SCCB_WR_Reg(0X1A,endx>>2);			//����Vref��end�ĸ�8λ

	temp=SCCB_RD_Reg(0X32);				//��ȡHref֮ǰ��ֵ
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(0X17,sy>>3);			//����Href��start��8λ
	SCCB_WR_Reg(0X18,endy>>3);			//����Href��end�ĸ�8λ
}





//��ʼ��SCCB�ӿ�
//CHECK OK
void SCCB_Init(void)
{											   
	GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_GPIOB_CLK_ENABLE();           
		
		GPIO_Initure.Pin = GPIO_PIN_10|GPIO_PIN_11; 
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}			 

//SCCB��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void SCCB_Start(void)
{
    SCCB_SDA=1;     //�����߸ߵ�ƽ	   
    SCCB_SCL=1;	    //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(50);  
    SCCB_SDA=0;
    delay_us(50);	 
    SCCB_SCL=0;	    //�����߻ָ��͵�ƽ��������������Ҫ	  
}

//SCCBֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void SCCB_Stop(void)
{
    SCCB_SDA=0;
    delay_us(50);	 
    SCCB_SCL=1;	
    delay_us(50); 
    SCCB_SDA=1;	
    delay_us(50);
}  
//����NA�ź�
void SCCB_No_Ack(void)
{
	delay_us(50);
	SCCB_SDA=1;	
	SCCB_SCL=1;	
	delay_us(50);
	SCCB_SCL=0;	
	delay_us(50);
	SCCB_SDA=0;	
	delay_us(50);
}
//SCCB,д��һ���ֽ�
//����ֵ:0,�ɹ�;1,ʧ��. 
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if(dat&0x80)SCCB_SDA=1;	
		else SCCB_SDA=0;
		dat<<=1;
		delay_us(50);
		SCCB_SCL=1;	
		delay_us(50);
		SCCB_SCL=0;		   
	}			 
	SCCB_SDA_IN();		//����SDAΪ���� 
	delay_us(50);
	SCCB_SCL=1;			//���յھ�λ,���ж��Ƿ��ͳɹ�
	delay_us(50);
	if(SCCB_READ_SDA)res=1;  //SDA=1����ʧ�ܣ�����1
	else res=0;         //SDA=0���ͳɹ�������0
	SCCB_SCL=0;		 
	SCCB_SDA_OUT();		//����SDAΪ���    
	return res;  
}	 
//SCCB ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN();		//����SDAΪ����  
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{		     	  
		delay_us(50);
		SCCB_SCL=1;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		delay_us(50);
		SCCB_SCL=0;
	}	
	SCCB_SDA_OUT();		//����SDAΪ���    
	return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID	  
	delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ	  
	delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//д����	 
  	SCCB_Stop();	  
  	return	res;
}		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//����SCCB����
	SCCB_WR_Byte(SCCB_ID);		//д����ID	  
	delay_us(100);	 
  	SCCB_WR_Byte(reg);			//д�Ĵ�����ַ	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//���Ͷ�����	  
	delay_us(100);
  	val=SCCB_RD_Byte();		 	//��ȡ����
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}






