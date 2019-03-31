#include "include.h"

u8 key_num = 0;  
/*******************************************************************************
* �� �� ��         : TIM4_Init
* ��������		   : TIM4��ʼ������
* ��    ��         : per:��װ��ֵ
					 psc:��Ƶϵ��
* ��    ��         : ��
*******************************************************************************/
void TIM4_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��	
}

/*******************************************************************************
* �� �� ��         : TIM4_IRQHandler
* ��������		   : TIM4�жϺ���
* ��    ��         : ��
* ��    ��         : ��
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
//			LCD_ShowString(10,110,tftlcd_data.width,tftlcd_data.height,16,"Close Door...");	//����
//		}	
		key_num = KEY_Scan(0);
							
	}

	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}



void pwm2_GPIO_Cfg(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
      
       //ȫ��ӳ�䣬��TIM3_CH2ӳ�䵽PB5
       //����STM32���Ĳο��ֲ�2010�еڵ�119ҳ��֪��
       //TIM2���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPA15��PB3,PB10 PB11
    
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
     //����ṹ��
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

//       //���½�Timer����Ϊȱʡֵ
//       TIM_DeInit(TIM2);
//       //�����ڲ�ʱ�Ӹ�TIM3�ṩʱ��Դ
//       TIM_InternalClockConfig(TIM2);
     
     //Ԥ��Ƶϵ��Ϊ0����������Ԥ��Ƶ����ʱTIMER��Ƶ��Ϊ72MHzre.TIM_Prescaler =0;
       TIM_TimeBaseStructure.TIM_Prescaler = 0;
     //���ü��������С��ÿ��1000�����Ͳ���һ�������¼�
       TIM_TimeBaseStructure.TIM_Period = 7200 - 1;
       //����ʱ�ӷָ�
       TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
       //���ü�����ģʽΪ���ϼ���ģʽ
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       
       //������Ӧ�õ�TIM2��
       TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
       //�������жϱ�־
       //TIM_ClearFlag(TIM2, TIM_FLAG_Update);
       //��ֹARRԤװ�ػ�����
       //TIM_ARRPreloadConfig(TIM2, DISABLE);
       //����TIM2���ж�
       //TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    
}
/*******************************************************************************
* �� �� ��         : PWM���������ú���
* ��������         : PWM_Cfg
* ��    ��         : dutyfactor ռ�ձ���ֵ����С0-1000
* ��    ��         : ��
*******************************************************************************/
void PWM2_Cfg(u16 dutyfactor1,u16 dutyfactor2,u16 dutyfactor3,u16 dutyfactor4)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
      //����ȱʡֵ
      TIM_OCStructInit(&TIM_OCInitStructure);
      
      //TIM2��CH1���
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2; //������PWMģʽ���ǱȽ�ģʽ 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //�Ƚ����ʹ�ܣ�ʹ��PWM������˿�
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //���ü����Ǹ߻��ǵ�
      //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
    TIM_OCInitStructure.TIM_Pulse = dutyfactor1*72;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
      
      
      //TIM2��CH2���
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2; //������PWMģʽ���ǱȽ�ģʽ 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //�Ƚ����ʹ�ܣ�ʹ��PWM������˿�
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //���ü����Ǹ߻��ǵ�
      //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor2*72;
      TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
      
   
      //TIM3��CH3���
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2; //������PWMģʽ���ǱȽ�ģʽ 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //�Ƚ����ʹ�ܣ�ʹ��PWM������˿�
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //���ü����Ǹ߻��ǵ�
      //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor3*72;
      TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
      
      
      //TIM3��CH4���
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2; //������PWMģʽ���ǱȽ�ģʽ 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //�Ƚ����ʹ�ܣ�ʹ��PWM������˿�
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //���ü����Ǹ߻��ǵ�
      //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor4*72;
      TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
		
	 TIM_ARRPreloadConfig(TIM2, ENABLE);
	 
	 //ʹ�����״̬
      TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
      TIM_OCStructInit(&TIM_OCInitStructure);
    //����TIM3��PWM���Ϊʹ��
      TIM_CtrlPWMOutputs(TIM2,ENABLE);
	  TIM_Cmd(TIM2, ENABLE);   //ʹ��
}

void pwm3_GPIO_Cfg(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
      
       //ȫ��ӳ�䣬��TIM3_CH2ӳ�䵽PB5
       //����STM32���Ĳο��ֲ�2010�еڵ�119ҳ��֪��
       //��û����ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPA6��PA7,PB0,PB1
       //��������ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPB4��PB5,PB0,PB1
       //����ȫ��ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPC6��PC7,PC8,PC9
       //Ҳ����˵����ȫ��ӳ��֮���ĸ�ͨ����PWM������ŷֱ�ΪPC6��PC7,PC8,PC9�������õ���ͨ��1��ͨ��2�����Զ�Ӧ����ΪPC6��PC7,PC8,PC9�������õ���ͨ��1��ͨ��2�����Զ�Ӧ����Ϊ
      // GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
      
       //������ӳ��Ĳ���
      // GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //����PC6��PC7��PC8��PC9Ϊ������������4·PWM
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
     //����ṹ��
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//       //���½�Timer����Ϊȱʡֵ
//       TIM_DeInit(TIM3);
//       //�����ڲ�ʱ�Ӹ�TIM3�ṩʱ��Դ
//       TIM_InternalClockConfig(TIM3);
     
     //Ԥ��Ƶϵ��Ϊ0����������Ԥ��Ƶ����ʱTIMER��Ƶ��Ϊ72MHzre.TIM_Prescaler =0;
       TIM_TimeBaseStructure.TIM_Prescaler = 0;
     //���ü��������С��ÿ��1000�����Ͳ���һ�������¼�
       TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
       //����ʱ�ӷָ�
       TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
       //���ü�����ģʽΪ���ϼ���ģʽ
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       
       //������Ӧ�õ�TIM2��
       TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
       //�������жϱ�־
       //TIM_ClearFlag(TIM2, TIM_FLAG_Update);
       //��ֹARRԤװ�ػ�����
       //TIM_ARRPreloadConfig(TIM2, DISABLE);
       //����TIM2���ж�
       //TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    
}
/*******************************************************************************
* �� �� ��         : PWM���������ú���
* ��������         : PWM_Cfg
* ��    ��         : dutyfactor ռ�ձ���ֵ����С0-1000
* ��    ��         : ��
*******************************************************************************/
void PWM3_Cfg(u16 dutyfactor1,u16 dutyfactor2,u16 dutyfactor3,u16 dutyfactor4)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
      //����ȱʡֵ
      //TIM_OCStructInit(&TIM_OCInitStructure);
      
      //TIM3��CH1���
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1; //������PWMģʽ���ǱȽ�ģʽ 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //�Ƚ����ʹ�ܣ�ʹ��PWM������˿�
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //���ü����Ǹ߻��ǵ�
      //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
    TIM_OCInitStructure.TIM_Pulse = dutyfactor1;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
      
      
      //TIM3��CH2���
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1; //������PWMģʽ���ǱȽ�ģʽ 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //�Ƚ����ʹ�ܣ�ʹ��PWM������˿�
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //���ü����Ǹ߻��ǵ�
      //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor2;
      TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
      
   
      //TIM3��CH3���
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1; //������PWMģʽ���ǱȽ�ģʽ 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //�Ƚ����ʹ�ܣ�ʹ��PWM������˿�
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //���ü����Ǹ߻��ǵ�
      //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor3;
      TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
      
      
      //TIM3��CH4���
      TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1; //������PWMģʽ���ǱȽ�ģʽ 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; //�Ƚ����ʹ�ܣ�ʹ��PWM������˿�
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; //���ü����Ǹ߻��ǵ�
      //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
      TIM_OCInitStructure.TIM_Pulse = dutyfactor4;
      TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
	 TIM_ARRPreloadConfig(TIM3, ENABLE);
	 //ʹ�����״̬
   //   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
     // TIM_OCStructInit(&TIM_OCInitStructure);
    //����TIM3��PWM���Ϊʹ��
    //  TIM_CtrlPWMOutputs(TIM3,ENABLE);
	 TIM_Cmd(TIM3, ENABLE);   //ʹ��
}


