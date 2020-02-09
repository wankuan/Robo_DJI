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
	
    float pout;							//p输出
    float iout;							//i输出
    float dout;							//d输出
    
    float delta_u;						//本次增量值
    float delta_out;					//增量式输出 = last_delta_out + delta_u
    float last_delta_out;     //上次增量值
    
	  float max_err;    
	  float deadband;				//err < deadband return  最小差值 
    uint32_t MaxOutput;				//输出限幅
    uint32_t IntegralLimit;		//积分限幅
    
}pid_t;

enum MOTOR_Status //电机的四种状态
{
    M_CCW	= 0,
    M_CW 	= 1,
    M_STOP 	= 2,
    M_STANDBY = 3,
};
enum MOTOR_Gear //电机的四个档位  默认使用低速挡M_G_1
{
    M_G_1	= 0,
    M_G_2 = 1,
    M_G_3 = 2,
    M_G_4 = 3 ,
};

enum MOTOR_Dir //电机的四个档位  默认使用低速挡M_G_1
{
    M_Forward	= 0,//默认向前
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
