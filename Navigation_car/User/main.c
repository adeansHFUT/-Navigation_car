/* ������ʹ�õ���RFID-RC522��Ƶģ����Ƶ�һ���Ž�ϵͳ������Ӧ���ŵ���Ƶģ�������ڻ��Ӧ��
	������������кźͳ������һ�¾ͻ���Ϊ����ȷ������D2ָʾ������LCD����ʾ������5�����Ժ�
	�Զ�������D2ָʾ���𡣵�������ʱ�򲻻���ʾ��D2Ҳ���������������к���Ψһ�ġ�
	
	�ܽŽ���ͼ��
	RST---PF4
	MISO---PF3
	MOSI---PF2
	SCK---PF1
	NSS(SDA)--PF0
	
	*/
#include "include.h"

extern u8 key_num;
int main()
{
	
	char g[MaxVerNum][MaxVerNum];//ͼ
	static char keyroad[MaxVerNum]={0};//·��
	SysTick_Init(72);        //ϵͳʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();             //led��ʼ��
	KEY_Init();             //������ʼ��
	TFTLCD_Init();			//LCD��ʼ��
	
	//RFID��ʼ��
	RC522_Init();
	PcdReset();
 	PcdAntennaOff(); 
	delay_ms(10);
 	PcdAntennaOn();
	delay_ms(10);
	
	//wifi��ʼ��
	USART1_Init(115200);
    USART3_Init(9600);
	TIM2_Init(9,7199);   //1ms�ж�һ��
	userInit();   
    gizwitsInit();

    //pwm��ʼ��
	TIM3_CH1_PWM_Init(500,72-1); //Ƶ����2Kh
	TIM4_CH1_PWM_Init(500,72-1);
	TIM_SetCompare1(TIM4,200);  // 1��x //ֵ������ȡ499����ΪARR���ֵ��499.,499�ǲ�ת
	TIM_SetCompare2(TIM4,0);    // 1��y
	TIM_SetCompare3(TIM4,0);    // 2��x
	TIM_SetCompare4(TIM4,499);  // 2��y 
	TIM_SetCompare1(TIM3,499);  // 3��x //ֵ������ȡ499����ΪARR���ֵ��499.,499�ǲ�ת
	TIM_SetCompare2(TIM3,0);    // 3��y
	TIM_SetCompare3(TIM3,0);    // 4��x
	TIM_SetCompare4(TIM3,200);  // 4��y
	//����Ϊͼ��ʼ����ͼ����Ϣ
	InitMatrix(g);
	g[1][2] = East;
	g[2][3] = East;
	g[8][9] = East;
	g[2][5] = North;
	g[1][4] = North;
	g[4][7] = North;
	g[3][6] = North;
	g[5][8] = North;
	g[8][10] = North;
	g[6][5] = West;
	FLOYD(g,10,1,10,keyroad);//�㷨�ҳ����·������keyroad�У�ͼg��10���ڵ㣬���1�յ�10,	
	while ( 1 )
    {   
		
		switch(key_num){
			case 0: 
				FRONT_COLOR=GREEN;
				LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,24, "######start######");
				LCD_ShowString(10,40,tftlcd_data.width,tftlcd_data.height,16, "L: rfid          R: wifi");
				LCD_ShowString(10,60,tftlcd_data.width,tftlcd_data.height,16, "U: total         D:     " );
				break;		
			case KEY_LEFT:
				key_num = 0;
				rfid_select();
				LCD_Clear(WHITE);
				break;
			case KEY_RIGHT:				
				key_num = 0;
				wifi();
		    	LCD_Clear(WHITE);
				break;
			case KEY_UP:
				key_num = 0;
				
				
		}
				 
    }
}


