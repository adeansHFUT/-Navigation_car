#include "time.h"
#include "led.h"
#include "tftlcd.h"
#include "key.h"
#include "gizwits_product.h"
#include "adc.h"
#include "tftlcd.h"


extern u8 Direction;
extern uint32_t timerMsCount;
u8 huoervalue[4] = {0};

u8 key_num = 0;  
/*******************************************************************************
* 函 数 名         : TIM4_Init
* 函数功能		   : TIM4初始化函数
* 输    入         : per:重装载值
					 psc:分频系数
* 输    出         : 无
*******************************************************************************/
void TIM2_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM2,ENABLE); //使能定时器	
}

/*******************************************************************************
* 函 数 名         : TIM4_IRQHandler
* 函数功能		   : TIM4中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM3_CH1_PWM_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure2;//通道2
	TIM_OCInitTypeDef TIM_OCInitStructure3;//通道3
	TIM_OCInitTypeDef TIM_OCInitStructure4;//通道3
	GPIO_InitTypeDef GPIO_InitStructure;//
	GPIO_InitTypeDef GPIO_InitStructure7;//c7管脚
	GPIO_InitTypeDef GPIO_InitStructure8;//c8
	GPIO_InitTypeDef GPIO_InitStructure9;//c9
	
	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ioC enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//复用
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//6
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOC,&GPIO_InitStructure);//c
	
	GPIO_InitStructure7.GPIO_Pin=GPIO_Pin_7;//7
	GPIO_InitStructure7.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure7.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOC,&GPIO_InitStructure7);//c
	
	GPIO_InitStructure8.GPIO_Pin=GPIO_Pin_8;//7
	GPIO_InitStructure8.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure8.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOC,&GPIO_InitStructure8);//c
	
	GPIO_InitStructure9.GPIO_Pin=GPIO_Pin_9;//7
	GPIO_InitStructure9.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure9.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOC,&GPIO_InitStructure9);//c
	////////////////////////////////////////////以上完成4个管脚配置
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//改变指定管脚的映射	
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure); //输出比较通道1初始化
	
	TIM_OCInitStructure2.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure2.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStructure2.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure2); //输出比较通道2初始化
	
	TIM_OCInitStructure3.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure3.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStructure3.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC3Init(TIM3,&TIM_OCInitStructure3); //输出比较通道3初始化
	
	TIM_OCInitStructure4.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure4.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStructure4.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC4Init(TIM3,&TIM_OCInitStructure4); //输出比较通道4初始化
	//////////////////////以上完成4个pwm输出配置
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIMx在 CCR1 上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIMx在 CCR2 上的预装载寄存器
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIMx在 CCR2 上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIMx在 CCR2 上的预装载寄存器
	TIM_ARRPreloadConfig(TIM3,ENABLE);//使能预装载寄存器
	
	TIM_Cmd(TIM3,ENABLE); //使能定时器
		
}
void TIM4_CH1_PWM_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;//
	
	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ioD enable
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_DeInit(TIM4);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,DISABLE);//复用
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;//
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);//D
	
	////////////////////////////////////////////以上完成4个管脚配置
	
	//GPIO_PinRemapConfig(GPIO_Remap_TIM4,DISABLE);//改变指定管脚的映射	
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);	
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM4,&TIM_OCInitStructure); //输出比较通道1初始化
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable); //使能TIMx在 CCR1 上的预装载寄存器
	TIM_OC2Init(TIM4,&TIM_OCInitStructure); //输出比较通道1初始化
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable); //使能TIMx在 CCR2 上的预装载寄存器
	TIM_OC3Init(TIM4,&TIM_OCInitStructure); //输出比较通道1初始化
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable); //使能TIMx在 CCR3 上的预装载寄存器
	TIM_OC4Init(TIM4,&TIM_OCInitStructure); //输出比较通道1初始化
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); //使能TIMx在 CCR4 上的预装载寄存器
	
	//////////////////////以上完成4个pwm输出配置
	
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);//使能预装载寄存器
	
	TIM_Cmd(TIM4,ENABLE); //使能定时器
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //??????
GPIO_Init(GPIOA,&GPIO_InitStructure);


TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);	


TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM????

TIM_OCInitStructure.TIM_Pulse=10;

TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
//TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
TIM_OC3Init(TIM1,&TIM_OCInitStructure);


TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_Pulse=10;
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;
TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
TIM_OC4Init(TIM1,&TIM_OCInitStructure);

TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

TIM_Cmd(TIM1,ENABLE);//使能或者失能TIMx外设

//????????????,??PWM????

TIM_CtrlPWMOutputs(TIM1, ENABLE); 


}
/*

spx:x号轮子占空比 -499~499，正数正传负数倒转

*/
void Go(int sp1,int sp2,int sp3,int sp4)
{
		if(sp1<0)
		{
			TIM_SetCompare4(TIM1,0);
			TIM_SetCompare1(TIM4,0-sp1);  //值最大可以取499，因为ARR最大值是499.,499是不转
		}
		else
		{
			TIM_SetCompare4(TIM1,sp1);
			TIM_SetCompare1(TIM4,0);  //值最大可以取499，因为ARR最大值是499.,499是不转
		}
		if(sp2<0)
		{
			TIM_SetCompare1(TIM3,0);
			TIM_SetCompare2(TIM3,0-sp2);
		}
		else
		{
			TIM_SetCompare1(TIM3,sp2);
			TIM_SetCompare2(TIM3,0);  //值最大可以取499，因为ARR最大值是499.,499是不转
		}
		if(sp3<0)
		{
			TIM_SetCompare4(TIM3,0);
			TIM_SetCompare3(TIM3,0-sp3);
		}
		else
		{
			TIM_SetCompare4(TIM3,sp3);
			TIM_SetCompare3(TIM3,0);  //值最大可以取499，因为ARR最大值是499.,499是不转
		}
		
		
		
		if(sp4<0)
		{
			TIM_SetCompare3(TIM4,0);
			TIM_SetCompare4(TIM4,0-sp4);
		}
		else
		{
			TIM_SetCompare3(TIM4,sp4);
			TIM_SetCompare4(TIM4,0);  //值最大可以取499，因为ARR最大值是499.,499是不转
		}
		
		//TIM_SetCompare1(TIM3,499);  //值最大可以取499，因为ARR最大值是499.,499是不转
		//TIM_SetCompare2(TIM3,499);
		//TIM_SetCompare3(TIM3,499);
		//TIM_SetCompare4(TIM3,499);
		
}

void GoAcoddingDrec(int drec,int speed)
{
//#define East 0x00
//#define West 0x40
//#define South 0x80
//#define North 0xc0
	if(drec==East)
	{
		Go(-speed-40,speed+30,-speed-20,speed);
	}
	else if(drec==West) 
	{
		Go(speed+40,-speed-40,speed,-speed);
	}
	else if(drec==North)
	{
		Go(speed+40,speed+40,speed,speed);
	}
	else if(drec==South)
	{
		Go(-speed-40,-speed-40,-speed,-speed);
	}
	else
	{
		Go(0,0,0,0);
	}
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update))
	{
		if(0 == timerMsCount%100)
			key_num = KEY_Scan(0);  //100ms
		gizTimerMs();//系统毫秒定时
//		if(0 == timerMsCount%50)
//		{
			huoervalue[0] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5); //左亮
			huoervalue[1] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2); //右
			huoervalue[2] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);  //south
			huoervalue[3] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);//north
////			LCD_ShowxNum(20,100,huoervalue[0],1,16,0x80);
////			LCD_ShowxNum(20,120,huoervalue[1],1,16,0x80);
////			LCD_ShowxNum(20,140,huoervalue[2],1,16,0x80);
////			LCD_ShowxNum(20,160,huoervalue[3],1,16,0x80);
			
			
		//}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	
}

