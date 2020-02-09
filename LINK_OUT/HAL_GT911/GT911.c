#include "GT911.h"
#include "HAL_LTDC.h"	


TouchStructure touchInfo; //������Ϣ�ṹ��

/*---------------------- IIC��غ��� ------------------------*/

// ����: ����ʱ����
//	˵����Ϊ����ֲ�ļ�����Ҷ���ʱ���Ȳ��ߣ����Բ���Ҫ���ж�ʱ����ʱ
//
void IIC_Touch_Delay(u16 a)
{
	int i;
	while (a --)
	{
		for (i = 0; i < 10; i++);
	}
}

//	����������IIC�����ݽ�Ϊ���ģʽ
//
void IIC_Touch_SDA_Out(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		
		__HAL_RCC_GPIOH_CLK_ENABLE();           
		
		GPIO_Initure.Pin = GPIO_PIN_8; 
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOH,&GPIO_Initure);
}

//	����������IIC�����ݽ�Ϊ����ģʽ
//
void IIC_Touch_SDA_In(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		
		__HAL_RCC_GPIOH_CLK_ENABLE();           
		
		GPIO_Initure.Pin = GPIO_PIN_8; 
		GPIO_Initure.Mode = GPIO_MODE_INPUT;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOH,&GPIO_Initure);
}

//	������IIC��ʼ�ź�
//
void IIC_Touch_Start(void)
{
	IIC_Touch_SDA_Out();
	
	SDA(1);
	SCL(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	
	SDA(0);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(0);
	IIC_Touch_Delay( Touch_DelayVaule );
}

//	������IICֹͣ�ź�
//
void IIC_Touch_Stop(void)
{
    SCL(0);
    IIC_Touch_Delay( Touch_DelayVaule );
    SDA(0);
    IIC_Touch_Delay( Touch_DelayVaule );
    SCL(1);
    IIC_Touch_Delay( Touch_DelayVaule );
    SDA(1);
    IIC_Touch_Delay( Touch_DelayVaule );
}

//	������IICӦ���ź�
//
void IIC_Touch_Response(void)
{
	IIC_Touch_SDA_Out();

	SDA(0);
	IIC_Touch_Delay( Touch_DelayVaule );	
	SCL(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(0);
	IIC_Touch_Delay( Touch_DelayVaule );
}

//	������IIC��Ӧ���ź�
//
void IIC_Touch_NoResponse(void)
{
	IIC_Touch_SDA_Out();
	
	SCL(0);	
	IIC_Touch_Delay( Touch_DelayVaule );
	SDA(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(0);
	IIC_Touch_Delay( Touch_DelayVaule );
}

//	�������ȴ��豸������Ӧ�ͺ�
//
u8 IIC_Touch_WaitResponse(void)
{

	SCL(0);
	IIC_Touch_Delay( Touch_DelayVaule );
	SDA(1);
	IIC_Touch_Delay( Touch_DelayVaule );
	SCL(1);

	IIC_Touch_SDA_In();	//����Ϊ����ģʽ
	IIC_Touch_Delay( Touch_DelayVaule );
	
	SCL(0);	
	if( HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_8) != 0) //�ж��豸�Ƿ���������Ӧ
	{		
		return (Response_ERR);
	}
	else
	{
		return (Response_OK);
	}

}

// ������IICд�ֽ�
//	������IIC_Data - Ҫд���8λ����
//	���أ��豸����Ӧ�򷵻� 1������Ϊ0
//
u8 IIC_Touch_WriteByte(u8 IIC_Data)
{
	u8 i;

	IIC_Touch_SDA_Out(); //���ݽ�Ϊ���ģʽ
	
	for (i = 0; i < 8; i++)
	{
		SDA(IIC_Data & 0x80);
		
		IIC_Touch_Delay( Touch_DelayVaule );
		SCL(1);
		IIC_Touch_Delay( Touch_DelayVaule );
		SCL(0);		
		
		IIC_Data <<= 1;
	}

	return (IIC_Touch_WaitResponse()); //�ȴ��豸��Ӧ
}

// ������IIC���ֽ�
//	������ResponseMode - Ӧ��ģʽѡ��
//       ResponseMode = 1 ʱ��CPU������Ӧ�źţ�Ϊ 0 ʱ��CPU������Ӧ���ź�
//	���أ�����������
//
u8 IIC_Touch_ReadByte(u8 ResponseMode)
{
	u8 IIC_Data;
	u8 i;
	
	SDA(1);
	SCL(0);

	IIC_Touch_SDA_In(); //����ģʽ
	
	//��һ�ֽ�����
	for (i = 0; i < 8; i++)
	{
		IIC_Data <<= 1;
		
		SCL(1);
		IIC_Touch_Delay( Touch_DelayVaule );

		IIC_Data |= ((HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_8) ) & 0x01);
		
		SCL(0);
		IIC_Touch_Delay( Touch_DelayVaule );
	}

	//	������Ӧ�ź�
	if (ResponseMode)
	{
		IIC_Touch_Response();
	}
	else
	{
		IIC_Touch_NoResponse();
	}
	
	return (IIC_Data); 
}

//	��������ʼ��IIC��GPIO��
//
void IIC_Touch_GPIO_Config (void)
{
		GPIO_InitTypeDef GPIO_Initure;
		
		__HAL_RCC_GPIOH_CLK_ENABLE();           
		
		GPIO_Initure.Pin = GPIO_PIN_7|GPIO_PIN_8; 
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;        
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
		HAL_GPIO_WritePin(GPIOH,GPIO_PIN_7,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOH,GPIO_PIN_8,GPIO_PIN_RESET);
}

/*---------------------- GT9XX��غ��� ------------------------*/

// ������GT9XX д����
//	������addr - Ҫ�����ļĴ���
//			
u8 GT9XX_WriteHandle (u16 addr)
{
	u8 status;

	IIC_Touch_Start();
	if( IIC_Touch_WriteByte(GT9XX_IIC_WADDR) == Response_OK ) //д����ָ��
	{
		if( IIC_Touch_WriteByte((u8)(addr >> 8)) == Response_OK ) //д��16λ��ַ
		{
			if( IIC_Touch_WriteByte((u8)(addr)) != Response_OK )
			{
				status = ERROR;
			}			
		}
	}
	status = SUCCESS;
	return status;	
}

// ������GT9XX д����
//	������addr - Ҫд���ݵĵ�ַ
//			value - д�������
//
u8 GT9XX_WriteData (u16 addr,u8 value)
{
	u8 status;
	
	IIC_Touch_Start(); //����IICͨѶ

	if( GT9XX_WriteHandle(addr) == SUCCESS)	//д��Ҫ�����ļĴ���
	{
		if (IIC_Touch_WriteByte(value) != Response_OK) //д����
		{
			status = ERROR;						
		}
	}	
	IIC_Touch_Stop(); //ֹͣͨѶ
	
	status = SUCCESS;
	return status;
}

// ������GT9XX ������
//	������addr - Ҫ�����ݵĵ�ַ
//			num - �������ֽ���
//			*value - ���ڻ�ȡ�洢���ݵ��׵�ַ
//
u8 GT9XX_ReadData (u16 addr, u8 cnt, u8 *value)
{
	u8 status;
	u8 i;

	status = ERROR;
	IIC_Touch_Start();

	if( GT9XX_WriteHandle(addr) == SUCCESS) //д��Ҫ�����ļĴ���
	{
		IIC_Touch_Start(); //��������IICͨѶ

		if (IIC_Touch_WriteByte(GT9XX_IIC_RADDR) == Response_OK)
		{	
			for(i = 0 ; i < cnt; i++)
			{
				if (i == (cnt - 1))
				{
					value[i] = IIC_Touch_ReadByte(0);//�������һ������ʱ���� ��Ӧ���ź�
				}
				else
				{
					value[i] = IIC_Touch_ReadByte(1);
				}
			}					
			IIC_Touch_Stop();
			status = SUCCESS;
		}
	}
	IIC_Touch_Stop();
	return (status);	
}

// ����: ��������ʼ��
//	���أ�1 - ��ʼ���ɹ��� 0 - ����δ��⵽������	
//
u8 Touch_Init(void)
{
	u8 touchIC_ID[4];	

	IIC_Touch_GPIO_Config(); //��ʼ��IIC����
	
	GT9XX_ReadData (GT9XX_ID_ADDR,4,touchIC_ID);	//��ID��Ϣ
	
	if( touchIC_ID[0] == '9' )	//�жϵ�һ���ַ��Ƿ�Ϊ 9
	{
	//	printf("Touch ID: %s \r\n",touchIC_ID);	//��ӡ����оƬ��ID
		return 0;
	}
	else
	{
	//	printf("Touch Error\r\n");	//����δ��⵽������
		return 1;
	}
}

// ����������ɨ��
//	˵�����ڳ����������Եĵ��øú��������Լ�ⴥ������
//
void Touch_Scan(void)
{
 	u8  touchData[2 + 8 * TOUCH_MAX ]; //���ڴ洢��������

	GT9XX_ReadData (GT9XX_READ_ADDR,2 + 8 * TOUCH_MAX ,touchData);	//������
	GT9XX_WriteData (GT9XX_READ_ADDR,0);	//	�������оƬ�ļĴ�����־λ
	touchInfo.num = touchData[0] & 0x0f;	//ȡ��ǰ�Ĵ�������
	
	if ( (touchInfo.num >= 1) && (touchInfo.num <=5) ) //���������� 1-5 ֮��ʱ
	{
		// ȡ��Ӧ�Ĵ�������
		switch(touchInfo.num)
		{
			case 5:
			{
				touchInfo.y[4] = ((touchData[5+32]<<8) | touchData[4+32])*0.80;
				touchInfo.x[4] = ((touchData[3+32]<<8) | touchData[2+32])*0.78;	
     		}
			case 4:
			{
				touchInfo.y[3] = ((touchData[5+24]<<8) | touchData[4+24])*0.80;
				touchInfo.x[3] = ((touchData[3+24]<<8) | touchData[2+24])*0.78;		
			}
			case 3:
			{
				touchInfo.y[2] = ((touchData[5+16]<<8) | touchData[4+16])*0.80;
				touchInfo.x[2] = ((touchData[3+16]<<8) | touchData[2+16])*0.78;				
			}
			case 2:
			{
				touchInfo.y[1] = ((touchData[5+8]<<8) | touchData[4+8])*0.80;
				touchInfo.x[1] = ((touchData[3+8]<<8) | touchData[2+8])*0.78;						
			}
			case 1:
			{
				touchInfo.y[0] = ((touchData[5]<<8) | touchData[4])*0.80;
				touchInfo.x[0] = ((touchData[3]<<8) | touchData[2])*0.78;					
			}
			default:break;
		}	
		touchInfo.flag = 1;		
	}
	else                       
	{
		touchInfo.flag = 0;
	}
}

//���崥����������
u16 Touch_Num[20][4];

void Touch_Set_Rec(u16 sx,u16 sy, u16 ex,u16 ey,u32 color,u8 num)
{
		LTDC_Draw_Rectangle(sx,sy,ex,ey,color);
		Touch_Num[num][0]=sx;
		Touch_Num[num][1]=sy;
		Touch_Num[num][2]=ex;
		Touch_Num[num][3]=ey;
}
void Touch_Set_Cir(u16 x,u16 y, u16 D,u32 color,u8 num)
{
	 u16 width=(double)D/2.828;
	 u16 sx,sy,ex,ey;
	 sx=x-width;
		ex=x+width;
	 sy=y-width;
		ey=y+width;	
		LTDC_Draw_Cir(x,y,D,color);
	
		Touch_Num[num][0]=sx;
		Touch_Num[num][1]=sy;
		Touch_Num[num][2]=ex;
		Touch_Num[num][3]=ey;
}
u8 Touch_Single_Judge(u8 num)
{
	if((touchInfo.x[0]<=Touch_Num[num][2])
		&&(touchInfo.x[0]>=Touch_Num[num][0])
		&&(touchInfo.y[0]<=Touch_Num[num][3])
		&&(touchInfo.y[0]>=Touch_Num[num][1])
		)
	{
		return 1;
	}
	else
	{
		return 0;		
	}

}
u8 Touch_Get(u8 num)//0-ɨ��ؼ�0 1-ɨ��ؼ�1  2-ɨ��ؼ�2
{  
	if(touchInfo.flag == 1)
	{
				if(Touch_Single_Judge(num))
									return num;
				else
								 return 0xFF;
	}
	else
	{
			 return 0xFF;
	}
}



