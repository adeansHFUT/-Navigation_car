#ifndef _time_H
#define _time_H

#include "system.h"
#define East 0x00
#define West 0x40
#define South 0x80
#define North 0xc0
void TIM2_Init(u16 per,u16 psc);
void TIM4_CH1_PWM_Init(u16 per,u16 psc);
void TIM3_CH1_PWM_Init(u16 per,u16 psc);
void Go(int sp1,int sp2,int sp3,int sp4);
void GoAcoddingDrec(int drec,int speed);
#endif
