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

#define East 0x00
#define West 0x40
#define South 0x80
#define North 0xc0

extern u8 key_num;
extern unsigned char status;
extern unsigned char g_ucTempbuf[20];
extern unsigned char DefaultKey[6];


char Drec[4][18]={
"east",
	"west",
	"south",
	"north"

};
u8 MidWay=1;//�Ƿ��������; ��ſ�ʼ 1����0������

char DefalutRoad[]={1+East,2+West,3+South,4+North,0x00};

void AnalyzeRoad(char *road);//���������·����Ϣ

int main()
{
	
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	KEY_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	
	RC522_Init();
	PcdReset();
 	PcdAntennaOff(); 
	delay_ms(10);
 	PcdAntennaOn();
	delay_ms(10);
	
	TIM2_Init(1000,7199);   //100msһ���ж�
	//printf("Start \r\n");
	TIM3_CH1_PWM_Init(500,72-1); //Ƶ����2Kh
	TIM4_CH1_PWM_Init(500,72-1);
	
	TIM_SetCompare1(TIM4,200);  //ֵ������ȡ499����ΪARR���ֵ��499.,499�ǲ�ת
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare3(TIM4,0);
		TIM_SetCompare4(TIM4,499);
		TIM_SetCompare1(TIM3,499);  //ֵ������ȡ499����ΪARR���ֵ��499.,499�ǲ�ת
		TIM_SetCompare2(TIM3,0);
		TIM_SetCompare3(TIM3,0);
		TIM_SetCompare4(TIM3,200);
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
		//AnalyzeRoad(DefalutRoad);
				 
    }
}
u8 ReadMark()
{
	while(1){
	/////////////////////������////////////////
		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
         if (status != MI_OK)
         {    
		     PcdReset();
		     PcdAntennaOff(); 
		     PcdAntennaOn(); 
			 continue;
         }	
         status = PcdAnticoll(g_ucTempbuf);//����ײ
         if(status != MI_OK)
         {    continue;    }
		////////����Ϊ�����ն˴�ӡ��������////////////////////////
		//LCD_ShowFontHZ(10, 230, "���к�");	
		//LCD_ShowString(10,230,tftlcd_data.width,tftlcd_data.height,16,"ID:");
		//LCD_ShowString(100,230,tftlcd_data.width,tftlcd_data.height,16,g_ucTempbuf);
		//LCD_ShowxNum(50,230,g_ucTempbuf[0]*1000000+g_ucTempbuf[1]*1000+g_ucTempbuf[2],9,16,0x80);
		 
		status = PcdSelect(g_ucTempbuf);//ѡ����Ƭ
	    if (status != MI_OK)
	    {   
				// LCD_ShowString(10,150,tftlcd_data.width,tftlcd_data.height,16,"mei you xuan ding ka pian ");
			continue;    }
	     
	    status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//��֤��Ƭ����
	    if (status != MI_OK)
	    {    continue;    }
		
     ///////////////��������////////////////////////
		 status = PcdRead(1, g_ucTempbuf);
		 if (status != MI_OK)
         {    continue;    }
		 //LCD_ShowString(10,260,tftlcd_data.width,tftlcd_data.height,16,"biao hao");
	  // LCD_ShowxNum(50,260,g_ucTempbuf[0],3,16,0x80);		
			return 	 g_ucTempbuf[0];
			 }

}
void AnalyzeRoad(char *road)
{
	u8 i=0;u8 Mark=0,RMark=0; u8 Direction=0;
	RMark = ReadMark();
	if((MidWay==1))//����������;��ʼ
	{
		while(RMark!=(road[i]&0x3f)&&road[i]!=0x00){//��Ѱ��;�������·����λ��
      i++;
			}
		
	}
	//while()
	while(1){
		if(road[i]!=0x00)
		{
			Direction=(road[i]>>6)&0x03;//�õ����
			Mark = road[i]&0x03f;	//	
		  RMark = ReadMark();//
			LCD_ShowString(10,230,tftlcd_data.width,tftlcd_data.height,16,"Mark:");
			LCD_ShowxNum(50,230,RMark,3,16,0);
			//LCD_ShowxNum(80,230,Mark,3,16,0);
			if(RMark==Mark)//�ߵ��˱��ΪMark��
			{
				LCD_ShowString(10,260,tftlcd_data.width,tftlcd_data.height,16,"Direaction:");
				LCD_ShowString(150,260,tftlcd_data.width,tftlcd_data.height,16,Drec[Direction]);
				
				i++;//��ȡ��һ���
				//DirectionMove(Direction,50);��ӷ����˶�����
			}
			
		}
		else
		{
			LCD_ShowString(10,130,tftlcd_data.width,tftlcd_data.height,16,"mark not in road or finish");
			break;
		}
	}
	
}
