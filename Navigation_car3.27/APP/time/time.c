#include "include.h"

u8 key_num = 0;  
/*******************************************************************************
* 函 数 名         : TIM4_Init
* 函数功能		   : TIM4初始化函数
* 输    入         : per:重装载值
					 psc:分频系数
* 输    出         : 无
*******************************************************************************/
void TIM4_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM4,ENABLE); //使能定时器	
}

/*******************************************************************************
* 函 数 名         : TIM4_IRQHandler
* 函数功能		   : TIM4中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
//		led1=!led1;
//		i++;
//		if(i==50)
//		{
//			i=0;
//			led2=1;
//			FRONT_COLOR=RED;
//			LCD_ShowString(10,110,tftlcd_data.width,tftlcd_data.height,16,"Close Door...");	//关门
//		}	
		key_num = KEY_Scan(0);
							
	}

	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}



void pwm2_GPIO_Cfg(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
      
       //全部映射，将TIM3_CH2映射到PB5
       //根据STM32中文参考手册2010中第第119页可知：
       //TIM2的四个通道CH1，CH2，CH3，CH4分别对应PA15，PB3,PB10 PB11
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	
	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_10|GPIO_Pin_11;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}


void TIM2_Cfg(void)
{
     //定义结构体
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

//       //重新将Timer设置为缺省值
//       TIM_DeInit(TIM2);
//       //采用内部时钟给TIM3提供时钟源
//       TIM_InternalClockConfig(TIM2);
     
     //预分频系数为0，即不进行预分频，此时TIMER的频率为72MHzre.TIM_Prescaler =0;
       TIM_TimeBaseStructure.TIM_Prescaler = 0;
     //设置计数溢出大小，每计1000个数就产生一个更新事件
       TIM_TimeBaseStructure.TIM_Period = 7200 - 1;
       //设置时钟分割
       TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
       //设置计数器模式为向上计数模式
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       
       //将配置应用到TIM2中
       TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
       //清除溢出中断标志
       //TIM_ClearFlag(TIM2, TIM_FLAG_Update);
       //禁止ARR预装载缓冲器
       //TIM_ARRPreloadConfig(TIM2, DISABLE);
       //开启TIM2的中断
       //TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    
}
/*******************************************************************************
* 函 数 名         : PWM波产生配置函数
* 函数功能         : PWM_Cfg
* 输    入         : dutyfactor 占空比数值，大小0-1000
* 输    出         : 无
*******************************************************************************/
void PWM2_Cfg(u16 dutyfactor1,u16 dutyfactor2,u16 dutyfactor3,u16 dutyfactor4)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
      //设置缺省值
      TIM_OCStructInit(&TIM_OCInitStructure);
      
      //TIM2的CH1输出
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2; //设置是PWM模式还是比较模式 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //比较输出使能，使能PWM输出到端口
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //设置极性是高还是低
      //设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
    TIM_OCInitStructure.TIM_Pulse = dutyfactor1*72;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
      
      
      //TIM2的CH2输出
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2; //设置是PWM模式还是比较模式 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //比较输出使能，使能PWM输出到端口
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //设置极性是高还是低
      //设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor2*72;
      TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
      
   
      //TIM3的CH3输出
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2; //设置是PWM模式还是比较模式 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //比较输出使能，使能PWM输出到端口
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //设置极性是高还是低
      //设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor3*72;
      TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
      
      
      //TIM3的CH4输出
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2; //设置是PWM模式还是比较模式 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //比较输出使能，使能PWM输出到端口
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //设置极性是高还是低
      //设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor4*72;
      TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
		
	 TIM_ARRPreloadConfig(TIM2, ENABLE);
	 
	 //使能输出状态
      TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
      TIM_OCStructInit(&TIM_OCInitStructure);
    //设置TIM3的PWM输出为使能
      TIM_CtrlPWMOutputs(TIM2,ENABLE);
	  TIM_Cmd(TIM2, ENABLE);   //使能
}

void pwm3_GPIO_Cfg(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
      
       //全部映射，将TIM3_CH2映射到PB5
       //根据STM32中文参考手册2010中第第119页可知：
       //当没有重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PA6，PA7,PB0,PB1
       //当部分重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PB4，PB5,PB0,PB1
       //当完全重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PC6，PC7,PC8,PC9
       //也即是说，完全重映射之后，四个通道的PWM输出引脚分别为PC6，PC7,PC8,PC9，我们用到了通道1和通道2，所以对应引脚为PC6，PC7,PC8,PC9，我们用到了通道1和通道2，所以对应引脚为
      // GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
      
       //部分重映射的参数
      // GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //设置PC6、PC7、PC8、PC9为复用输出，输出4路PWM
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void TIM3_Cfg(void)
{
     //定义结构体
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//       //重新将Timer设置为缺省值
//       TIM_DeInit(TIM3);
//       //采用内部时钟给TIM3提供时钟源
//       TIM_InternalClockConfig(TIM3);
     
     //预分频系数为0，即不进行预分频，此时TIMER的频率为72MHzre.TIM_Prescaler =0;
       TIM_TimeBaseStructure.TIM_Prescaler = 0;
     //设置计数溢出大小，每计1000个数就产生一个更新事件
       TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
       //设置时钟分割
       TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
       //设置计数器模式为向上计数模式
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       
       //将配置应用到TIM2中
       TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
       //清除溢出中断标志
       //TIM_ClearFlag(TIM2, TIM_FLAG_Update);
       //禁止ARR预装载缓冲器
       //TIM_ARRPreloadConfig(TIM2, DISABLE);
       //开启TIM2的中断
       //TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    
}
/*******************************************************************************
* 函 数 名         : PWM波产生配置函数
* 函数功能         : PWM_Cfg
* 输    入         : dutyfactor 占空比数值，大小0-1000
* 输    出         : 无
*******************************************************************************/
void PWM3_Cfg(u16 dutyfactor1,u16 dutyfactor2,u16 dutyfactor3,u16 dutyfactor4)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
      //设置缺省值
      //TIM_OCStructInit(&TIM_OCInitStructure);
      
      //TIM3的CH1输出
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1; //设置是PWM模式还是比较模式 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //比较输出使能，使能PWM输出到端口
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //设置极性是高还是低
      //设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
    TIM_OCInitStructure.TIM_Pulse = dutyfactor1;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
      
      
      //TIM3的CH2输出
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1; //设置是PWM模式还是比较模式 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //比较输出使能，使能PWM输出到端口
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //设置极性是高还是低
      //设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor2;
      TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
      
   
      //TIM3的CH3输出
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1; //设置是PWM模式还是比较模式 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //比较输出使能，使能PWM输出到端口
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //设置极性是高还是低
      //设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor3;
      TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
      
      
      //TIM3的CH4输出
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1; //设置是PWM模式还是比较模式 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //比较输出使能，使能PWM输出到端口
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //设置极性是高还是低
      //设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor4;
      TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
	 TIM_ARRPreloadConfig(TIM3, ENABLE);
	 //使能输出状态
   //   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
     // TIM_OCStructInit(&TIM_OCInitStructure);
    //设置TIM3的PWM输出为使能
    //  TIM_CtrlPWMOutputs(TIM3,ENABLE);
	 TIM_Cmd(TIM3, ENABLE);   //使能
}


