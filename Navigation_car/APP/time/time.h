#ifndef _time_H
#define _time_H

#include "system.h"

void TIM2_Init(u16 per,u16 psc);
void TIM4_CH1_PWM_Init(u16 per,u16 psc);
void TIM3_CH1_PWM_Init(u16 per,u16 psc);
#endif
