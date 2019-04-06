/* 本程序使用的是RFID-RC522射频模块设计的一个门禁系统，当感应卡放到射频模块区域内会感应到
	卡，如果卡序列号和程序设计一致就会认为是正确开锁，D2指示灯亮，LCD上显示开锁，5秒钟以后
	自动关锁，D2指示灯灭。当卡错误时候不会显示，D2也不会亮。卡的序列号是唯一的。
	
	管脚接线图：
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
	
	char g[MaxVerNum][MaxVerNum];//图
	static char keyroad[MaxVerNum]={0};//路径
	SysTick_Init(72);        //系统时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();             //led初始化
	KEY_Init();             //按键初始化
	TFTLCD_Init();			//LCD初始化
	
	//RFID初始化
	RC522_Init();
	PcdReset();
 	PcdAntennaOff(); 
	delay_ms(10);
 	PcdAntennaOn();
	delay_ms(10);
	
	//wifi初始化
	USART1_Init(115200);
    USART3_Init(9600);
	TIM2_Init(9,7199);   //1ms中断一次
	userInit();   
    gizwitsInit();

    //pwm初始化
	TIM3_CH1_PWM_Init(500,72-1); //频率是2Kh
	TIM4_CH1_PWM_Init(500,72-1);
	TIM_SetCompare1(TIM4,200);  // 1轮x //值最大可以取499，因为ARR最大值是499.,499是不转
	TIM_SetCompare2(TIM4,0);    // 1轮y
	TIM_SetCompare3(TIM4,0);    // 2轮x
	TIM_SetCompare4(TIM4,499);  // 2轮y 
	TIM_SetCompare1(TIM3,499);  // 3轮x //值最大可以取499，因为ARR最大值是499.,499是不转
	TIM_SetCompare2(TIM3,0);    // 3轮y
	TIM_SetCompare3(TIM3,0);    // 4轮x
	TIM_SetCompare4(TIM3,200);  // 4轮y
	//以下为图初始化和图的信息
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
	FLOYD(g,10,1,10,keyroad);//算法找出最短路径存与keyroad中，图g有10个节点，起点1终点10,	
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


