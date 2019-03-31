#ifndef _time_H
#define _time_H

#include "system.h"

void TIM4_Init(u16 per,u16 psc);
//PA 0123
void pwm2_GPIO_Cfg(void);
void TIM2_Cfg(void);
void PWM2_Cfg(u16 dutyfactor1,u16 dutyfactor2,u16 dutyfactor3,u16 dutyfactor4);
//PC 6789
void pwm3_GPIO_Cfg(void);
void TIM3_Cfg(void);
void PWM3_Cfg(u16 dutyfactor1,u16 dutyfactor2,u16 dutyfactor3,u16 dutyfactor4);


#endif
