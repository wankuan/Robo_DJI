#include "PID.h"
#include <math.h>

pid_t PID_SPD[4];//四路电机PID
MOTOR_Measure  MOTOR_Meas[4];//四路电机

#define ABS(x)		((x>0)? (x): (-x)) //绝对值

void abs_limit(float *a, float ABS_MAX)//限幅
{
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}

void MOTOR_Init(void)
{
MOTOR_Meas[0].Dir=M_Forward;
MOTOR_Meas[1].Dir=M_Back;
MOTOR_Meas[2].Dir=M_Forward;
MOTOR_Meas[3].Dir=M_Back;
}


/*参数初始化*/
void PID_Init(
    pid_t *pid, 
    uint32_t maxout,
    uint32_t intergral_limit,
    float 	kp, 
    float 	ki, 
    float 	kd)
{
    pid->IntegralLimit = intergral_limit;
    pid->MaxOutput = maxout;  
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
    pid->last_delta_out=0;
    
    

}

/**
    *@bref. calculate delta PID and position PID
    *@param[in] set: target
    *@param[in] real	measure
    */
float pid_calc(pid_t* pid, float get, float set)
{
    pid->get[NOW] = get;
    pid->set[NOW] = set;
    pid->err[NOW] = set - get;	//set - measure
    
//    if(pid->max_err != 0 && ABS(pid->err[NOW]) >  pid->max_err)
//		return 0;
//    if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband)
//		return 0;
//    
    pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]);
    pid->iout = pid->i * pid->err[NOW];
    pid->dout = pid->d * (pid->err[NOW] - 2*pid->err[LAST] + pid->err[LLAST]);
    
    abs_limit(&(pid->iout), pid->IntegralLimit);
    pid->delta_u = pid->pout + pid->iout + pid->dout;
    
    pid->delta_out = pid->last_delta_out + pid->delta_u;
    abs_limit(&(pid->delta_out), pid->MaxOutput);
    
    pid->last_delta_out = pid->delta_out;	//update last time
        
    pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
    
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
    
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];
    
    return pid->delta_out;
}









