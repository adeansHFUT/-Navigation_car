/* 本程序使用的是RFID-RC522射频模块设计的一个门禁系统，当感应卡放到射频模块区域内会感应到
	卡，如果卡序列号和程序设计一致就会认为是正确开锁，D2指示灯亮，LCD上显示开锁，5秒钟以后
	自动关锁，D2指示灯灭。当卡错误时候不会显示，D2也不会亮。卡的序列号是唯一的。
	
	管脚接线图：
	RST---PF4
	MISO---PF3
	MOSI---PF2
	SCK---PF1
	NSS(SDA)--PF0
	
	uart: pa 9 10
	key:  pa 0 , pe 2 3 4
	RFID: PF
	TFT: PD PE PG
	pwm2: PA15，PB3,PB10 PB11
	pwM3: PA 67 PB 01
	*/
#include "include.h"

extern u8 key_num;

int main()
{
	
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	//LED_Init();
	KEY_Init();
	
	USART1_Init(9600);
	TFTLCD_Init();			//LCD初始化
	RC522_Init();
	PcdReset();
 	PcdAntennaOff(); 
	delay_ms(10);
 	PcdAntennaOn();
	delay_ms(10);
	TIM4_Init(1000,7199);   //100ms一次中断
	//PA 0123
	pwm2_GPIO_Cfg();
	TIM2_Cfg();
	PWM2_Cfg(100, 200, 300, 400);
	//PC 6789
	pwm3_GPIO_Cfg();
	TIM3_Cfg();
	PWM3_Cfg(100, 200, 300, 400);
	
	while ( 1 )
    {   
		switch(key_num){
			case 0: 
				FRONT_COLOR=GREEN;
				LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,24,"######start######");
				LCD_ShowString(10,40,tftlcd_data.width,tftlcd_data.height,16,"L: rfid          R:clear");
				LCD_ShowString(10,60,tftlcd_data.width,tftlcd_data.height,16,"U: cichang       D: " );
				break;		
			case KEY_LEFT:
				key_num = 0;
				rfid_select();
				LCD_Clear(WHITE);
				break;
			case KEY_RIGHT:
				LCD_Clear(WHITE);
				key_num = 0;
				break;
				
		}
		   
    }
}
