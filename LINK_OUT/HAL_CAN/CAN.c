#include "CAN.h"
#include "PID.h"
//#define CAN_DEBUG
//#define PID_DEBUG
CAN_HandleTypeDef   CAN_ESC_Handler;   //CAN1���
CanTxMsgTypeDef     TxMessage;      //������Ϣ
CanRxMsgTypeDef     RxMessage;      //������Ϣ


u8 MOTOR_IN=0;

u8 CAN_Rmsg[8];

////CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+tbs2+1)*brp); ����tbs1��tbs2����ֻ�ù�ע��ʶ���ϱ�־����ţ�����CAN_BS2_1TQ�����Ǿ���Ϊtbs2=1�����㼴�ɡ�
//mode:CAN_MODE_NORMAL,��ͨģʽ;CAN_MODE_LOOPBACK,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ45M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_8tq,6,CAN_MODE_LOOPBACK);
//������Ϊ:45M/((6+8+1)*6)=500Kbps
u8 CAN1_Mode_Init(u32 mode)
{
    CAN_FilterConfTypeDef  CAN_ESC_FilerConf;

    CAN_ESC_Handler.Instance=CAN1; 
    CAN_ESC_Handler.pTxMsg=&TxMessage;     //������Ϣ
    CAN_ESC_Handler.pRxMsg=&RxMessage;     //������Ϣ
    CAN_ESC_Handler.Init.Prescaler=5;    //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN_ESC_Handler.Init.Mode=mode;        //ģʽ���� 
    CAN_ESC_Handler.Init.SJW=CAN_SJW_1TQ;         //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN_ESC_Handler.Init.BS1=CAN_BS1_3TQ;         //tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    CAN_ESC_Handler.Init.BS2=CAN_BS2_5TQ;         //tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    CAN_ESC_Handler.Init.TTCM=DISABLE;     //��ʱ�䴥��ͨ��ģʽ 
    CAN_ESC_Handler.Init.ABOM=ENABLE;     //����Զ����߹���
    CAN_ESC_Handler.Init.AWUM=DISABLE;     //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN_ESC_Handler.Init.NART=ENABLE;      //��ֹ�����Զ����� 
    CAN_ESC_Handler.Init.RFLM=DISABLE;     //���Ĳ�����,�µĸ��Ǿɵ� 
    CAN_ESC_Handler.Init.TXFP=DISABLE;     //���ȼ��ɱ��ı�ʶ������ 
	
    if(HAL_CAN_Init(&CAN_ESC_Handler)!=HAL_OK) return 1;   //��ʼ��
    
    u16 Filter_ID=0x200;//����������ĸ�ʽ 0x201-0x204
    u16 Filter_ID1 = Filter_ID<<5;
    u16 Filter_MASK_1 = 0x07F8<<5;	//�����3bits������ƥ�䣬����λ����ƥ��
    u16 Filter_ID2 = 0xFFFF;
    u16 Filter_MASK_2	=	0xFFFF;
				
    CAN_ESC_FilerConf.FilterIdHigh=Filter_ID2;     //32λID
    CAN_ESC_FilerConf.FilterIdLow=Filter_ID1;
    CAN_ESC_FilerConf.FilterMaskIdHigh=Filter_MASK_2; //32λMASK
    CAN_ESC_FilerConf.FilterMaskIdLow=Filter_MASK_1;  
    CAN_ESC_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//������0������FIFO0
    CAN_ESC_FilerConf.FilterNumber=0;          //������0
    CAN_ESC_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK; //����ģʽ
    CAN_ESC_FilerConf.FilterScale=CAN_FILTERSCALE_16BIT; //16λ�˲�
    CAN_ESC_FilerConf.FilterActivation=ENABLE; //�����˲���0
    CAN_ESC_FilerConf.BankNumber=14;
	
    if(HAL_CAN_ConfigFilter(&CAN_ESC_Handler,&CAN_ESC_FilerConf)!=HAL_OK) return 2;//�˲�����ʼ��
	   
//    CAN_ESC_Handler.State=HAL_CAN_STATE_READY;//�������CAN1״̬ ���𽫲���򿪽����ж�
//    if(HAL_CAN_Receive_IT(&CAN_ESC_Handler,CAN_FIFO0)!=HAL_OK)return 3;
    __HAL_CAN_ENABLE_IT(&CAN_ESC_Handler, CAN_IT_FMP0);
    return 0;
}


u8 time=0;

u8 MOTOR_Refesh(CAN_HandleTypeDef* _hcan)
{
  u16 i;
  i = _hcan->pRxMsg->StdId;
  if((i>=0x201)&&(i<=0x208))
  {
  i = i&0x0007;//ȡ�������bits. ֵ0-7
  MOTOR_Meas[i-1].pos_angle[NOW]=(uint16_t)((_hcan->pRxMsg->Data[0]<<8)|(_hcan->pRxMsg->Data[1]));
  MOTOR_Meas[i-1].speed_rpm[NOW]=(int16_t)((_hcan->pRxMsg->Data[2]<<8)|(_hcan->pRxMsg->Data[3]));
  MOTOR_Meas[i-1].current[NOW]=(int16_t)((_hcan->pRxMsg->Data[4]<<8)|(_hcan->pRxMsg->Data[5]));   
  MOTOR_Meas[i-1].tempeature=_hcan->pRxMsg->Data[6];
  return 0;
  }
  else 
  {
  return 1;
  }
}

u8 CAN_SEND_STA=0;//�������Ϊ0  ������Ϊ1
void MOTOR_SET_Current(int16_t M1, int16_t M2, int16_t M3, int16_t M4)//�����ŵ�����
{

	CAN_ESC_Handler.pTxMsg->StdId = 0x200;
	CAN_ESC_Handler.pTxMsg->IDE = CAN_ID_STD;
	CAN_ESC_Handler.pTxMsg->RTR = CAN_RTR_DATA;
	CAN_ESC_Handler.pTxMsg->DLC = 0x08;
  
	CAN_ESC_Handler.pTxMsg->Data[0] = M1 >> 8;
	CAN_ESC_Handler.pTxMsg->Data[1] = M1;
	CAN_ESC_Handler.pTxMsg->Data[2] = M2 >> 8;
	CAN_ESC_Handler.pTxMsg->Data[3] = M2;
	CAN_ESC_Handler.pTxMsg->Data[4] = M3 >> 8;
	CAN_ESC_Handler.pTxMsg->Data[5] = M3;
	CAN_ESC_Handler.pTxMsg->Data[6] = M4 >> 8;
	CAN_ESC_Handler.pTxMsg->Data[7] = M4;
  
  /*
  �˴���CAN�жϷ��ͺ�loop��������
  CAN�жϷ��ͳɹ������뷢�ͳɹ��ص�����
  */
  //HAL_CAN_Transmit_IT(&CAN_ESC_Handler);
  HAL_CAN_Transmit(&CAN_ESC_Handler,1000);
  
}	

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan)
{
	if(hcan->Instance==CAN1)
	{
     CAN_SEND_STA=0;
		 //printf("TX OK\r\n");
	}
}
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
		if(hcan->Instance==CAN1)
	{
//      /* Receive */
//    CAN_ESC_Handler.State=HAL_CAN_STATE_READY;//�������CAN1״̬ ���𽫲���򿪽����ж�
//    if(HAL_CAN_Receive_IT(&CAN_ESC_Handler, CAN_FIFO0) != HAL_OK)
//    {
//      /* Reception Error */
//      //Error_Handler();
//    }

    __HAL_CAN_ENABLE_IT(&CAN_ESC_Handler,CAN_IT_FMP0);//���¿���FIF00��Ϣ�Һ��ж�
    
    MOTOR_Refesh(hcan);
    
    MOTOR_IN=1;
    
    
    #ifdef CAN_DEBUG
      u8 length;
      length=hcan->pRxMsg->DLC;
      for(u8 i=0;i<length;i++)
      {
        printf("%X ",hcan->pRxMsg->Data[i]);
      } 
      printf(" ID:%X  DLC:%X\r\n",hcan->pRxMsg->StdId,hcan->pRxMsg->DLC);
    #endif
    
    #ifdef PID_DEBUG
      time++;
      if(time>=100)
      {
      u16 SPEED,I_SET,I_ACTUAL;
      if(hcan->pRxMsg->Data[2]>=0XBF)
      {
      SPEED=65535-((hcan->pRxMsg->Data[2]<<8)+hcan->pRxMsg->Data[3]);
      }
      else
      {
      SPEED=(hcan->pRxMsg->Data[2]<<8)+hcan->pRxMsg->Data[3];
      }
      if(hcan->pRxMsg->Data[4]>=0XBF)
      {
          I_ACTUAL=65535-((hcan->pRxMsg->Data[4]<<8)+hcan->pRxMsg->Data[5]);
      }
      else
      {
          I_ACTUAL=(hcan->pRxMsg->Data[4]<<8)+hcan->pRxMsg->Data[5];
      }
      I_SET=0;
      printf("SPEED=%d,I_SET=%d,I_ACTUAL=%d\r\n",SPEED,I_SET,I_ACTUAL);
      time=0;
      }
    #endif
    
	}
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
		printf("ERROR");
}

void Error_Handler(void)
{
 	  printf("RX ERROR");
}

