#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED_R      	PEout(11) 
#define LED_G      	PFout(14) 

#define LED_ON 0
#define LED_OFF 1

void LED_Init(void);

#endif

