/**
  ******************************************************************************
  * @file           : main.c
  * @author         : huangwankuan
  * @note
      @1.1--stm32f4xx_hal_conf.h（文件中改成对应MCU的外部晶振实际值）
  **/
#include "main.h"
#define Uart_size 18

int16_t WANT_SPD[4];
int16_t SET_Current[4];

//float MOTOR_Rate=0.6;//整体速度水平    2018/12/9 17:00
//float KP=1.8;
//float KI=0.2;
//float KD=0;

//float MOTOR_Rate=0.6;//整体速度水平    2018/12/9 17:52
//float KP=1.2;
//float KI=0.2;
//float KD=0;

float MOTOR_Rate=0.6;//整体速度水平    2018/12/9 17:52
float KP=2.0;  //0-2   P越大反应迟钝
float KI=0.4;  //0.1-0.8  I越大反应较大  容易产生震荡
float KD=0;

int16_t Current_MAX=15000;//16384-20A


u8 SEND_DATA[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

extern u8 RX_BUF[Uart_size];
u8 RX_Handle_Buf[Uart_size];
extern u8 UART_STA;

extern u8 CAN_Rmsg[8];

u8 CAN_Send_Buff[8]={0};



extern CanRxMsgTypeDef RxMessage;      //接收消息
extern CAN_HandleTypeDef   CAN1_Handler;   //CAN1句柄
extern DJI_REMOTE DJI_RE;

u8 ta[3];
u8 CAN_SEND_DATA[8];
void Romote_To_Currnt_Handle(DJI_REMOTE Remote,int16_t* M_Current,u8 gear,float Factor_Roll);
void DBUS_RST(DJI_REMOTE Remote);


void limit_abs(int16_t *a, int16_t ABS_MAX)//限幅
{
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}


int main(void)
{
	   u16 i;
    HAL_Init();                     //初始化HAL库
    Stm32_Clock_Init(360,12,2,8); 		//设置时钟,168Mhz  晶振12M
    delay_init(180);                //初始化延时函数


    for(int i=0; i<4; i++)
	{
		PID_Init(&PID_SPD[i],20000,10000,KP,KI,KD	);  //4 motos angular rate closeloop.
	}
    UART_Init();

//	  UART1_Init();
//
//	  DBUS_Init();
//	 	DJI_RE.Flag_S=1;


		  //UART2_Init();
			//	DMA_IT_Re_Init();
				//TIM1_Init(18000,50000);
		 // TIM1_PWM_Init(1800,2500,500);   //25MS一次更新中断
	   LED_Init();
     KEY_Init();
     ESC_POWER();
     DBUS_Init();


     CAN1_Mode_Init(CAN_MODE_NORMAL);
     MOTOR_Init();
     delay_ms(1000);
     DBUS_RST(DJI_RE);
     LED_G=LED_ON;
     LED_R=LED_ON;

	//   CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_6TQ,CAN_BS1_8TQ,6,CAN_MODE_NORMAL); //CAN初始化,波特率500Kbps

	//	CAN1_Send_Msg(0x022,SEND_DATA,8);

//  			for(int i=0; i<4; i++)
//			{
//				WANT_SPD[i]=1000;
//			}

	while(1)
	{
    if(DJI_RE.Flag_S==Connected)
    {
		//CAN1_Send_Msg(0x022,SEND_DATA,8);
	  LED_G=~LED_G;
    LED_R=LED_OFF;
//    printf("%d %d %d %d %d %d %d\r\n",
//    DJI_RE.Left_X,DJI_RE.Left_Y,DJI_RE.Right_X,DJI_RE.Right_Y,
//    DJI_RE.SW1,DJI_RE.SW2,DJI_RE.Flag_S);
     if((DJI_RE.SW1!=UP)&&(DJI_RE.SW1!=ERR))
     {
         if(DJI_RE.SW1==MID)
         {
            Romote_To_Currnt_Handle(DJI_RE,WANT_SPD,M_G_1,1);
         }
        else
        {
            Romote_To_Currnt_Handle(DJI_RE,WANT_SPD,M_G_2,2);
        }

        for(int i=0; i<4; i++)
        {
          pid_calc(&PID_SPD[i],MOTOR_Meas[i].speed_rpm[NOW],WANT_SPD[i]);
          //SET_Current[i]=WANT_SPD[i]*MOTOR_Rate+PID_SPD[i].delta_out;
          SET_Current[i]=PID_SPD[i].delta_out;
          limit_abs(&SET_Current[i],Current_MAX);
        }
//      printf("SPEED=%d,WANT_SPD=%d,I_ACTUAL=%d,delta_out=%.3f,SET_Current=%d\r\n",MOTOR_Meas[0].speed_rpm[NOW],WANT_SPD[0]
//      ,MOTOR_Meas[0].current[NOW],PID_SPD[0].delta_out,SET_Current[0]);

     // printf("%d %.3f %.3f %.3f\r\n",PID_SPD[0].MaxOutput,PID_SPD[0].delta_u,PID_SPD[0].delta_out,PID_SPD[0].pout);
     MOTOR_SET_Current(SET_Current[0],SET_Current[1],SET_Current[2],SET_Current[3]);
     }
     else
     {
       MOTOR_SET_Current(0,0,0,0);
     }
		delay_ms(50);
    }

  else
  {
    LED_G=LED_OFF;
    LED_R=~LED_R;
  	delay_ms(50);
  }
 }
}


u16 RST_Left_X;
u16 RST_Left_Y;
u16 RST_Right_X;
u16 RST_Right_Y;

void DBUS_RST(DJI_REMOTE Remote)
{
RST_Left_X=Remote.Left_X;
RST_Left_Y=Remote.Left_Y;
RST_Right_X=Remote.Right_X;
RST_Right_Y=Remote.Right_Y;
}


u8 RST_MID=10;

//gear 档位 默认M_G_1
//Factor_Roll 旋转系数  默认为1   该值越大  机器人旋转的角速度越大
void Romote_To_Currnt_Handle(
DJI_REMOTE Remote,int16_t* M_Current,u8 gear,float Factor_Roll
)
{
	 double KP;
	 int Vx=Remote.Left_X-RST_Left_X; //MAX=1684  MIN=354  差值660；
	 int Vy=Remote.Left_Y-RST_Left_Y;
	 int W=Remote.Right_X-RST_Right_X;
   int a=W;

   if((a>=0)&&(a<=RST_MID))W=0;
   if((a<=0)&&(a>=-RST_MID))W=0;

   switch(gear)
   {
   case M_G_1:KP=6;break;
      case M_G_2:KP=8;break;
         case M_G_3:KP=10;break;
            case M_G_4:KP=12;break;
              default:KP=1;break;
   }
		 if(DJI_RE.Flag_S==Connected)
			{
		M_Current[0]=(Vy+Vx+W*Factor_Roll)*KP;
		M_Current[1]=(Vy-Vx-W*Factor_Roll)*KP;
		M_Current[2]=(Vy-Vx+W*Factor_Roll)*KP;
		M_Current[3]=(Vy+Vx-W*Factor_Roll)*KP;

        for(int i=0;i<4;i++)
        {
           if(MOTOR_Meas[i].Dir==M_Back)
           {
            M_Current[i]=-M_Current[i];
           }
        }
			}
			else
			{
		M_Current[0]=0;
		M_Current[1]=0;
		M_Current[2]=0;
		M_Current[3]=0;
			}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==GPIO_PIN_2)
  {
     LED_R=~LED_R;
  }
}
