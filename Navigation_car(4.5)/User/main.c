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
#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "RC522.h"
#include "time.h"
#include "key.h"

//
#define East 0x00
#define West 0x40
#define South 0x80
#define North 0xc0
char Drec[4][18]={
"east",
	"west",
	"south",
	"north"

};
u8 MidWay=1;//�Ƿ��������; ��ſ�ʼ 1����0������
//
unsigned char data1[16] = {0x02,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
u8 data_write[16] = {0};
//M1����ĳһ��дΪ���¸�ʽ����ÿ�ΪǮ�����ɽ��տۿ�ͳ�ֵ����
//4�ֽڽ����ֽ���ǰ����4�ֽڽ��ȡ����4�ֽڽ�1�ֽڿ��ַ��1�ֽڿ��ַȡ����1�ֽڿ��ַ��1�ֽڿ��ַȡ�� 
unsigned char data2[4]  = {0,0,0,0x01};
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
//char DefalutRoad[]={0x04,0x09,0x0e,0x13,0x00};//ǰ6λ��ʾ��ţ�����λ����1��2��3��4�� 0x00ֹͣ
char DefalutRoad[]={1+East,2+West,3+South,4+North,0x00};
unsigned char g_ucTempbuf[20];
extern u8 key_num;
unsigned char status,i;
unsigned int temp;

void task1_showid(void);
void task2_writecard(void);
void task3_readcard(void);
void rfid_select(void);

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
void rfid_select(void)
{
	LCD_Clear(WHITE);
	while(1)
	{
		switch(key_num){
			case 0:
				FRONT_COLOR=GREEN;
				LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"select:");
				LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"L: write card R:  exit");
				LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"U: read card  D: clear" );
				break;			
			case KEY_LEFT:			
				key_num = 0;
				task2_writecard();
				LCD_Clear(WHITE);
				break;
			case KEY_RIGHT:
				key_num = 0;
				LCD_Clear(WHITE);
				return ;
			case KEY_UP:
				key_num = 0;
				task3_readcard();
				LCD_Clear(WHITE);
				break;
			case KEY_DOWN:
				key_num = 0;
				LCD_Clear(WHITE);
				break;
		}
	}
	

}
void task1_showid(void)
{
	LCD_Clear(WHITE);
	FRONT_COLOR=GREEN;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"showing id now");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"R: exit");
	FRONT_COLOR=RED;
	while(1)
	{
		 if(key_num == KEY_RIGHT)  //���ؼ�
		 {
		     key_num = 0;
			 return;
		 }
			 
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
		LCD_ShowString(10,230,tftlcd_data.width,tftlcd_data.height,16,"ID:");
		//LCD_ShowString(100,230,tftlcd_data.width,tftlcd_data.height,16,g_ucTempbuf);
		LCD_ShowxNum(50,230,g_ucTempbuf[0]*1000000+g_ucTempbuf[1]*1000+g_ucTempbuf[2],9,16,0x80);
	}
	LCD_Clear(WHITE);
}

void task2_writecard(void)
{
	LCD_Clear(WHITE);
	FRONT_COLOR=GREEN;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"writing now");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"U: add  D: minus");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"R: exit L: clear");
	FRONT_COLOR=RED;
	while(1)
	{
		switch(key_num){
			case 0: 
				break;			
			case KEY_LEFT: 
				data_write[0] = 0;
				key_num = 0;
				break;
			case KEY_RIGHT:   
				key_num = 0;
				return;			  //����
			case KEY_UP: 
				data_write[0]++;
				key_num = 0;
				break;			
			case KEY_DOWN: 
				data_write[0]--;
				key_num = 0;
				break;
		}
		LCD_ShowxNum(10,150,data_write[0],3,16,0x80);
		/////////////////////////д��//////////////////////////////////
		
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
	    status = PcdSelect(g_ucTempbuf);//ѡ����Ƭ
	    if (status != MI_OK)
	    {   
				// LCD_ShowString(10,150,tftlcd_data.width,tftlcd_data.height,16,"mei you xuan ding ka pian ");
			continue;    }
	     
	    status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//��֤��Ƭ����
	    if (status != MI_OK)
	    {    continue;    }
	    
	    status = PcdWrite(1, data_write);//д��
        if (status != MI_OK)
        {    
				 continue;    }
		LCD_ShowString(10,180,tftlcd_data.width,tftlcd_data.height,16, "ok");
		LCD_ShowxNum(30,180,data_write[0],3,16,0x80);
		delay_ms(500);
		LCD_ShowString(10,180,tftlcd_data.width,tftlcd_data.height,16, "       ");
		data_write[0]++;
	}
}

void task3_readcard(void)
{
	LCD_Clear(WHITE);
	FRONT_COLOR=GREEN;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"reading now");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"L: clear R: exit" );
	FRONT_COLOR=RED;
	while(1)
	{
		switch(key_num){
			case 0: break;
			
			case KEY_LEFT: 
				LCD_ShowString(50,260,tftlcd_data.width,tftlcd_data.height,16,"  ");  //�����ʾ������
				key_num = 0;
				break;
			
			case KEY_RIGHT :
				key_num = 0;
				return;
		}
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
		LCD_ShowString(10,230,tftlcd_data.width,tftlcd_data.height,16,"ID:");
		//LCD_ShowString(100,230,tftlcd_data.width,tftlcd_data.height,16,g_ucTempbuf);
		LCD_ShowxNum(50,230,g_ucTempbuf[0]*1000000+g_ucTempbuf[1]*1000+g_ucTempbuf[2],9,16,0x80);
		 
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
		 LCD_ShowString(10,260,tftlcd_data.width,tftlcd_data.height,16,"biao hao");
	     LCD_ShowxNum(50,260,g_ucTempbuf[0],3,16,0x80);		
	}
}
	