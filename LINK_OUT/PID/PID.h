#ifndef __PID_H__
#define __PID_H__
#include "sys.h"

enum{
    LLAST	= 0,
    LAST 	= 1,
    NOW 	= 2,
    POSITION_PID,
    DELTA_PID,
};

typedef struct
{
    float p;
    float i;
    float d;
    
    float set[3];				//NOW, LAST, LLAST
    float get[3];				//
    float err[3];				//
	
    float pout;							//p���
    float iout;							//i���
    float dout;							//d���
    
    float delta_u;						//��������ֵ
    float delta_out;					//����ʽ��� = last_delta_out + delta_u
    float last_delta_out;     //�ϴ�����ֵ
    
	  float max_err;    
	  float deadband;				//err < deadband return  ��С��ֵ 
    uint32_t MaxOutput;				//����޷�
    uint32_t IntegralLimit;		//�����޷�
    
}pid_t;

enum MOTOR_Status //���������״̬
{
    M_CCW	= 0,
    M_CW 	= 1,
    M_STOP 	= 2,
    M_STANDBY = 3,
};
enum MOTOR_Gear //������ĸ���λ  Ĭ��ʹ�õ��ٵ�M_G_1
{
    M_G_1	= 0,
    M_G_2 = 1,
    M_G_3 = 2,
    M_G_4 = 3 ,
};

enum MOTOR_Dir //������ĸ���λ  Ĭ��ʹ�õ��ٵ�M_G_1
{
    M_Forward	= 0,//Ĭ����ǰ
    M_Back = 1,
};

typedef struct
{
	int16_t	 	speed_rpm[3];
	uint16_t 	pos_angle[3];				//abs angle range:[0,8191]
  int16_t   current[3];
	uint8_t	  gear;
	uint8_t		status;
  uint8_t   tempeature;
  uint8_t   Dir;
}MOTOR_Measure;


extern MOTOR_Measure  MOTOR_Meas[4];
extern pid_t PID_SPD[4];
void PID_Init(
    pid_t *pid, 
    uint32_t maxout,
    uint32_t intergral_limit,
    float 	kp, 
    float 	ki, 
    float 	kd);
float pid_calc(pid_t* pid, float get, float set);
void MOTOR_Init(void);
#endif
