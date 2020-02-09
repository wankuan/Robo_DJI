#include "HAL_SDRAM.h"
#include "delay.h"
SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM���

//SDRAM��ʼ��
void SDRAM_Init(void)
{

    FMC_SDRAM_TimingTypeDef SDRAM_Timing;
                                                     
    SDRAM_Handler.Instance=FMC_SDRAM_DEVICE;                             //SDRAM��BANK5,6  
    SDRAM_Handler.Init.SDBank=FMC_SDRAM_BANK1;                           //��һ��SDRAM BANK
    SDRAM_Handler.Init.ColumnBitsNumber=FMC_SDRAM_COLUMN_BITS_NUM_9;     //������
    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_13;          //������
    SDRAM_Handler.Init.MemoryDataWidth=FMC_SDRAM_MEM_BUS_WIDTH_16;       //���ݿ��Ϊ16λ
    SDRAM_Handler.Init.InternalBankNumber=FMC_SDRAM_INTERN_BANKS_NUM_4;  //һ��4��BANK
    SDRAM_Handler.Init.CASLatency=FMC_SDRAM_CAS_LATENCY_3;               //CASΪ3
    SDRAM_Handler.Init.WriteProtection=FMC_SDRAM_WRITE_PROTECTION_DISABLE;//ʧ��д����
    SDRAM_Handler.Init.SDClockPeriod=FMC_SDRAM_CLOCK_PERIOD_2;           //SDRAMʱ��ΪHCLK/2=180M/2=90M=11.1ns
    SDRAM_Handler.Init.ReadBurst=FMC_SDRAM_RBURST_DISABLE;                //ʹ��ͻ��
    SDRAM_Handler.Init.ReadPipeDelay=FMC_SDRAM_RPIPE_DELAY_1;            //��ͨ����ʱ
    
    SDRAM_Timing.LoadToActiveDelay=2;                                   //����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������
    SDRAM_Timing.ExitSelfRefreshDelay=8;                                //�˳���ˢ���ӳ�Ϊ8��ʱ������
    SDRAM_Timing.SelfRefreshTime=6;                                     //��ˢ��ʱ��Ϊ6��ʱ������                                 
    SDRAM_Timing.RowCycleDelay=6;                                       //��ѭ���ӳ�Ϊ6��ʱ������
    SDRAM_Timing.WriteRecoveryTime=2;                                   //�ָ��ӳ�Ϊ2��ʱ������
    SDRAM_Timing.RPDelay=2;                                             //��Ԥ����ӳ�Ϊ2��ʱ������
    SDRAM_Timing.RCDDelay=2;                                            //�е����ӳ�Ϊ2��ʱ������
    HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);
    SDRAM_Initialization_Sequence(&SDRAM_Handler);//����SDRAM��ʼ������
		
	//ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
	//COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
    //����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=180/2=90Mhz,����Ϊ8192(2^13).
	//����,COUNT=64*1000*90/8192-20=683
		HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler,683);//����ˢ��Ƶ��

}
//����SDRAM��ʼ������
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    u32 temp=0;
    //SDRAM��������ʼ������Ժ���Ҫ��������˳���ʼ��SDRAM
	   delay_us(100); 
    SDRAM_Send_Cmd(FMC_SDRAM_CMD_TARGET_BANK1,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //ʱ������ʹ��
    delay_us(500); //������ʱ200us
    SDRAM_Send_Cmd(FMC_SDRAM_CMD_TARGET_BANK1,FMC_SDRAM_CMD_PALL,1,0);       //�����д洢��Ԥ���
    SDRAM_Send_Cmd(FMC_SDRAM_CMD_TARGET_BANK1,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);//������ˢ�´��� 
    //����ģʽ�Ĵ���,SDRAM��bit0~bit2Ϊָ��ͻ�����ʵĳ��ȣ�
			 //bit3Ϊָ��ͻ�����ʵ����ͣ�bit4~bit6ΪCASֵ��bit7��bit8Ϊ����ģʽ
			 //bit9Ϊָ����дͻ��ģʽ��bit10��bit11λ����λ
				temp=(u32)SDRAM_MODEREG_BURST_LENGTH_1          |	//����ͻ������:1(������1/2/4/8)
              SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	//����ͻ������:����(����������/����)
              SDRAM_MODEREG_CAS_LATENCY_3           |	//����CASֵ:3(������2/3)
              SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //���ò���ģʽ:0,��׼ģʽ
              SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //����ͻ��дģʽ:1,�������
    SDRAM_Send_Cmd(FMC_SDRAM_CMD_TARGET_BANK1,FMC_SDRAM_CMD_LOAD_MODE,1,temp);   //����SDRAM��ģʽ�Ĵ���
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_FMC_CLK_ENABLE();                 //ʹ��FMCʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();               //ʹ��GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();               //ʹ��GPIODʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();               //ʹ��GPIOEʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();               //ʹ��GPIOFʱ��
    __HAL_RCC_GPIOG_CLK_ENABLE();               //ʹ��GPIOGʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3;  
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_PULLUP;              //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //����
    GPIO_Initure.Alternate=GPIO_AF12_FMC;       //����ΪFMC    
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);          //��ʼ��PC0,2,3
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //��ʼ��PD0,1,8,9,10,14,15
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10| GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);     //��ʼ��PE0,1,7,8,9,10,11,12,13,14,15
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);     //��ʼ��PF0,1,2,3,4,5,11,12,13,14,15
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);      //��ʼ��PG0,1,2,4,5,8,15 
}
//��SDRAM��������
//bankx:0,��BANK5�����SDRAM����ָ��
//      1,��BANK6�����SDRAM����ָ��
//cmd:ָ��(0,����ģʽ/1,ʱ������ʹ��/2,Ԥ������д洢��/3,�Զ�ˢ��/4,����ģʽ�Ĵ���/5,��ˢ��/6,����)
//refresh:��ˢ�´���
//regval:ģʽ�Ĵ����Ķ���
//����ֵ:0,����;1,ʧ��.
u8 SDRAM_Send_Cmd(u32 BANK_Num,u8 cmd,u8 refresh_time,u16 value)
{
    FMC_SDRAM_CommandTypeDef Command;
    Command.CommandMode=cmd;                //����
    Command.CommandTarget=BANK_Num;      //Ŀ��SDRAM�洢����
    Command.AutoRefreshNumber=refresh_time;      //��ˢ�´���
    Command.ModeRegisterDefinition=value;  //Ҫд��ģʽ�Ĵ�����ֵ
    if(HAL_SDRAM_SendCommand(&SDRAM_Handler,&Command,0X1000)==HAL_OK) //��SDRAM��������
						return 0;  
    else 
						return 1;    
}



//u8 FMC_SDRAM_Write_ALL_BANK_32Bit()
//{
//	u32 i=0;  	  
//	vu32 temp=0;  
// u32 test[3];
//	for(i=0;i<8*1024*1024;i+=1)
//	{
//		*(vu32*)(Bank5_SDRAM_ADDR+i)=0xf0f0fefe;
//	}
// 	for(i=0;i<8*1024*1024;i+=1) 
//	{	
//  temp=*(vu32*)(Bank5_SDRAM_ADDR+i);
//	}
//	delay_ms(1);
//	test[0]=*(vu32*)(Bank5_SDRAM_ADDR+(unsigned long)8*1024*1024-1);
//	test[1]=*(vu32*)(Bank5_SDRAM_ADDR+(unsigned long)6*1024*1024-1);
//	test[2]=*(vu32*)(Bank5_SDRAM_ADDR+(unsigned long)4*1024*1024-1);
//	if((test[0]==0xf0f0fefe)&&(test[1]==0xf0f0fefe)&&(test[2]==0xf0f0fefe))
// return 0;
//	else
//	return 1;
//}

//u8 FMC_SDRAM_Write_ALL_BANK_16Bit()
//{
//	u32 i=0;  	  
//	vu16 temp=0;	   
// u16 test[3];
//	for(i=0;i<16*1024*1024;i+=1)
//	{
//		*(vu16*)(Bank5_SDRAM_ADDR+i)=0xf0fe;
//	}
// 	for(i=0;i<16*1024*1024;i+=1) 
//	{	
//  temp=*(vu16*)(Bank5_SDRAM_ADDR+i);
//	}
//	test[0]=*(vu32*)(Bank5_SDRAM_ADDR+(unsigned long)8*1024*1024-1);
//	test[1]=*(vu32*)(Bank5_SDRAM_ADDR+(unsigned long)6*1024*1024-1);
//	test[2]=*(vu32*)(Bank5_SDRAM_ADDR+(unsigned long)4*1024*1024-1);
//	if((test[0]==0xf0fe)&&(test[1]==0xf0fe)&&(test[2]==0xf0fe))
// return 0;
//	else
//	return 1;
//}
//u8 FMC_SDRAM_Write_ALL_BANK_8Bit()
//{
//	u32 i=0;  	  
//	vu8 temp=0;	   
// u8 test[3];
//	for(i=0;i<32*1024*1024;i+=1)
//	{
//		*(vu8*)(Bank5_SDRAM_ADDR+i)=0xf0;
//	}
// 	for(i=0;i<32*1024*1024;i+=1) 
//	{	
//  temp=*(vu8*)(Bank5_SDRAM_ADDR+i);
//	}
//	test[0]=*(vu8*)(Bank5_SDRAM_ADDR+(unsigned long)8*1024*1024-1);
//	test[1]=*(vu8*)(Bank5_SDRAM_ADDR+(unsigned long)6*1024*1024-1);
//	test[2]=*(vu8*)(Bank5_SDRAM_ADDR+(unsigned long)4*1024*1024-1);
//	if((test[0]==0xf0)&&(test[1]==0xf0)&&(test[2]==0xf0))
// return 0;
//	else
//	return 1;
//}


//��ָ����ַ(WriteAddr+Bank5_SDRAM_ADDR)��ʼ,����д��n���ֽ�.
//pBuffer:�ֽ�ָ��
//WriteAddr:Ҫд��ĵ�ַ
//n:Ҫд����ֽ���
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*(vu8*)(Bank5_SDRAM_ADDR+WriteAddr)=*pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//��ָ����ַ((WriteAddr+Bank5_SDRAM_ADDR))��ʼ,��������n���ֽ�.
//pBuffer:�ֽ�ָ��
//ReadAddr:Ҫ��������ʼ��ַ
//n:Ҫд����ֽ���
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*pBuffer++=*(vu8*)(Bank5_SDRAM_ADDR+ReadAddr);
		ReadAddr++;
	}
}
/*
HAL_StatusTypeDef HAL_SDRAM_Read_8b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint8_t *pDstBuffer, uint32_t BufferSize);
HAL_StatusTypeDef HAL_SDRAM_Write_8b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint8_t *pSrcBuffer, uint32_t BufferSize);
HAL_StatusTypeDef HAL_SDRAM_Read_16b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint16_t *pDstBuffer, uint32_t BufferSize);
HAL_StatusTypeDef HAL_SDRAM_Write_16b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint16_t *pSrcBuffer, uint32_t BufferSize);
HAL_StatusTypeDef HAL_SDRAM_Read_32b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pDstBuffer, uint32_t BufferSize);
HAL_StatusTypeDef HAL_SDRAM_Write_32b(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pSrcBuffer, uint32_t BufferSize);

HAL_StatusTypeDef HAL_SDRAM_Read_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t * pAddress, uint32_t *pDstBuffer, uint32_t BufferSize);
HAL_StatusTypeDef HAL_SDRAM_Write_DMA(SDRAM_HandleTypeDef *hsdram, uint32_t *pAddress, uint32_t *pSrcBuffer, uint32_t BufferSize);
*/




