/* ������ʹ�õ���RFID-RC522��Ƶģ����Ƶ�һ���Ž�ϵͳ������Ӧ���ŵ���Ƶģ�������ڻ��Ӧ��
	������������кźͳ������һ�¾ͻ���Ϊ����ȷ������D2ָʾ������LCD����ʾ������5�����Ժ�
	�Զ�������D2ָʾ���𡣵�������ʱ�򲻻���ʾ��D2Ҳ���������������к���Ψһ�ġ�
	
	�ܽŽ���ͼ��
	RST---PF4
	MISO---PF3
	MOSI---PF2
	SCK---PF1
	NSS(SDA)--PF0
	
	uart: pa 9 10
	key:  pa 0 , pe 2 3 4
	RFID: PF
	TFT: PD PE PG
	pwm2: PA15��PB3,PB10 PB11
	pwM3: PA 67 PB 01
	*/
#include "include.h"

extern u8 key_num;

int main()
{
	
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	//LED_Init();
	KEY_Init();
	
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	RC522_Init();
	PcdReset();
 	PcdAntennaOff(); 
	delay_ms(10);
 	PcdAntennaOn();
	delay_ms(10);
	TIM4_Init(1000,7199);   //100msһ���ж�
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
