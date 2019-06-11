/*
	管脚接线图：
	RST---PF4
	MISO---PF3
	MOSI---PF2
	SCK---PF1
	NSS(SDA)--PF0
	
	*/
#include <string.h>
#include "gizwits_product.h"
#include "common.h"
#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "RC522.h"
#include "time.h"
#include "key.h"
#include "Graph.h"

//

char DefalutRoad[]={1+East,2+West,3+South,4+North,0x00};
char Drec[4][18]={
"east",
"west",
"south",
"north"

};
u8 MidWay=1;//是否允许从中途 标号开始 1允许0不允许
//
unsigned char data1[16] = {0x02,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
u8 data_write[16] = {0};

unsigned char data2[4]  = {0,0,0,0x01};
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
//char DefalutRoad[]={0x04,0x09,0x0e,0x13,0x00};//后6位表示标号，前两位位方向，1东2西3南4北 0x00停止
extern u8 huoervalue[4];
unsigned char g_ucTempbuf[20];
extern u8 key_num;
unsigned char status,i;
unsigned int temp;
uint8_t wifi_sta;
u8 RMark=0;
bool Stop=0;
u8 Direction=0;
void task1_showid(void);
void task2_writecard(void);
void task3_readcard(void);
void rfid_select(void);
void wifi();
void AnalyzeRoad(char *road,char ID,char end);//分析传输的路径信息
void Go(int sp1,int sp2,int sp3,int sp4);
void GoAcoddingDrec(int drec,int speed);
void tonodrec(char (*G)[MaxVerNum],int n) ;
char g[21][21];//图
char keyroad[MaxVerNum]={0x00};//路径
void init_road(char *keyroad);
int main()
{
	int i,k=0;
	char startlabel = 0;
	int temp1,temp2;
	char start=1,end=20;
	char ID=0;//读取的卡号
	char g[MaxVerNum][MaxVerNum];//图
	static char keyroad[MaxVerNum]={0};//路径
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	KEY_Init();
	//wifi
	USART1_Init(115200);
  USART3_Init(9600);
	TIM2_Init(9,7199);
	userInit();   
  gizwitsInit();
	//wifi
	TFTLCD_Init();			//LCD初始化
	input_init();
	
	RC522_Init();
	PcdReset();
 	PcdAntennaOff(); 
	delay_ms(10);
 	PcdAntennaOn();
	delay_ms(10);
	
	//TIM2_Init(1000,7199);   //100ms一次中断
	//printf("Start \r\n");
	TIM3_CH1_PWM_Init(500,72-1); //频率是2Kh
	TIM4_CH1_PWM_Init(500,72-1);
	
	
		InitMatrix(g);//以下为图的信息
	g[1][2] = East;
	g[2][3] = East;
	g[3][4] = East;
	g[5][6]=East;
	g[6][7]=East;
	g[7][8]=East;
	g[9][10]=East;
	g[10][11]=East;
	g[11][12]=East;
	g[13][14]=East;
	g[14][15]=East;
	g[15][16]=East;
	g[17][18]=East;
	g[18][19]=East;
	g[19][20]=East;
	g[1][5]=North;
	g[2][6]=North;
	g[3][7]=North;
	g[4][8]=North;
	g[5][9]=North;
	g[6][10]=North;
	g[7][11]=North;
	g[8][12]=North;
	g[9][13]=North;
	g[10][14]=North;
	g[11][15]=North;
	g[16][20]=North;
	g[13][17]=North;
	g[14][18]=North;
	g[15][19]=North;
	g[16][20]=North;
	tonodrec(g,20);
	//图g有10个节点，起点1终点10,	
	Go(-0,0,0,0);
	/*
	GoAcoddingDrec(East,400);
	//Go(-400,400,-400,400);//4个轮子占空比 -499~499，正数正传负数倒转
	delay_ms(1000);	
	delay_ms(1000);	
	GoAcoddingDrec(West,400);
	//Go(0,0,0,0);
	delay_ms(1000);	
	//Go(400,-400,-400,400);//4个轮子占空比 -499~499，正数正传负数倒转
	delay_ms(1000);
delay_ms(1000);	
Go(0,0,0,0);
delay_ms(1000);	
GoAcoddingDrec(North,400);
	//Go(-400,400,400,-400);//4个轮子占空比 -499~499，正数正传负数倒转
	delay_ms(1000);
delay_ms(1000);
Go(0,0,0,0);
delay_ms(1000);	
GoAcoddingDrec(South,400);
	//Go(-400,-400,-400,-400);//4个轮子占空比 -499~499，正数正传负数倒转
	*/
	
	//while(currentDataPoint.valueEND == 1);
	//
 
 
	Stop = 1;
	LCD_ShowString(20,30,tftlcd_data.width,tftlcd_data.height,24,"NO:2018CXCYS262" );
	    LCD_ShowString(20,60,tftlcd_data.width,tftlcd_data.height,24,"Navigation  Car" );
			LCD_ShowString(10,120,tftlcd_data.width,tftlcd_data.height,16,"Direaction:");
			LCD_ShowString(10,100,tftlcd_data.width,tftlcd_data.height,16,"Mark:");
			LCD_ShowString(10,260,tftlcd_data.width,tftlcd_data.height,16,"road:");
			LCD_ShowString(0,200,tftlcd_data.width,tftlcd_data.height,16,"Start point :" );
			LCD_ShowString(0,220,tftlcd_data.width,tftlcd_data.height,16,"End point :" );
			//LCD_ShowxNum(10,260,currentDataPoint.valueEND+1,3,16,0x80);
		while(1){
			
			LCD_ShowxNum(50,100,ID,3,16,0);
			temp1=(int)currentDataPoint.valueSTART;temp2=(int)currentDataPoint.valueEND;
			wifi();
			
			
			LCD_ShowxNum(150,200,currentDataPoint.valueSTART+1,3,16,0x80);
			
			LCD_ShowxNum(150,220,currentDataPoint.valueEND+1,3,16,0x80);
		if(temp1!=currentDataPoint.valueSTART||temp2!=currentDataPoint.valueEND)
			{
				init_road(keyroad);		
			FLOYD(g,20,currentDataPoint.valueSTART,currentDataPoint.valueEND,keyroad);
				Stop=0;
				LCD_ShowString(0,240,tftlcd_data.width,tftlcd_data.height,16,"Start Navigation!" );
			LCD_ShowString(50,260,tftlcd_data.width,tftlcd_data.height,16,"                                      " );
			
			while(keyroad[k+1]!=0)
			{
			
			LCD_ShowxNum(50+k*20,260,keyroad[k+1]&0x3f,2,16,0x80);
			
			k++;
			
			}
			LCD_ShowxNum(50+k*20,260,currentDataPoint.valueEND+1,2,16,0x80);
			k=0;
			}
				 //gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
		
    	/*
		
		switch(key_num){
			case 0: 
				FRONT_COLOR=GREEN;
				LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,24,"######start######");
				LCD_ShowString(10,40,tftlcd_data.width,tftlcd_data.height,16,"L: rfid          R: wifi");
				LCD_ShowString(10,60,tftlcd_data.width,tftlcd_data.height,16,"U: cichang       D: " );
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
				
		}
		*/
		//AnalyzeRoad(keyroad);
		//AnalyzeRoad(DefalutRoad);
		/////////////////////读卡号////////////////
	      if(startlabel == 1 && Stop == 0)
				{
				if(huoervalue[0] == 1 && huoervalue[1] == 1 && huoervalue[2] == 1 && huoervalue[3] == 1)
				GoAcoddingDrec(Direction,300);
		  	else
				GoAcoddingDrec(Direction,50);	
				}
		   status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
         if (status != MI_OK)
         {    
		     PcdReset();
		     PcdAntennaOff(); 
		     PcdAntennaOn(); 
			 continue;
         }	
         status = PcdAnticoll(g_ucTempbuf);//防冲撞
         if(status != MI_OK)
         {    continue;    }
		////////以下为超级终端打印出的内容////////////////////////
		//LCD_ShowFontHZ(10, 230, "序列号");	
		//LCD_ShowString(10,230,tftlcd_data.width,tftlcd_data.height,16,"ID:");
		//LCD_ShowString(100,230,tftlcd_data.width,tftlcd_data.height,16,g_ucTempbuf);
		//LCD_ShowxNum(50,230,g_ucTempbuf[0]*1000000+g_ucTempbuf[1]*1000+g_ucTempbuf[2],9,16,0x80);
		
		status = PcdSelect(g_ucTempbuf);//选定卡片
	    if (status != MI_OK)
	    {   
				// LCD_ShowString(10,150,tftlcd_data.width,tftlcd_data.height,16,"mei you xuan ding ka pian ");
			continue;    }
	     
	    status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//验证卡片密码
	    if (status != MI_OK)
	    {    continue;    }
		
     ///////////////读卡内容////////////////////////
		 status = PcdRead(1, g_ucTempbuf);
		 if (status != MI_OK)
         {    continue;    }
		 //LCD_ShowString(10,260,tftlcd_data.width,tftlcd_data.height,16,"biao hao");
	   	startlabel = 1;
			ID=g_ucTempbuf[0];
				AnalyzeRoad(keyroad,ID,end);//碰到标号ID后该怎么走
			if(!Stop)
			{
			i++;
			}
   /* 
		*/
	}
		
}
void init_road(char *keyroad)
{
	int i=0;
	for(i=0;i<MaxVerNum;i++)
	{
		keyroad[i]=0x00;
	}
}
void tonodrec(char (*G)[MaxVerNum],int n)  //有向图变为无向图
{
	int i,j=0;
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(G[i][j]==East)
			{
				G[j][i]=West;
			}
			else if(G[i][j]==North)
			{
				G[j][i]=South;
			}
				
		}
	}
}



u8 ReadMark()
{
	
	while(1){
	
			 }

}
void AnalyzeRoad(char *road,char ID,char end)
{
	u8 i=0;u8 Mark=0; 
	RMark = ID; 
	
	currentDataPoint.valueCurrentPoint = RMark;
	if((MidWay==1))//如果允许从中途开始
	{
		while(RMark!=(road[i]&0x3f)&&road[i]!=0x00){//找寻中途标号在总路径的位置
      i++;
			}
		
	}
	//while()
		if(road[i]!=0x00&&(road[i]&0x3f)!=end)
		{
			Direction=road[i]&0xc0;//得到方向
			GoAcoddingDrec(Direction,200);
			Mark = road[i]&0x03f;	//得到标号
		 
			
			//LCD_ShowxNum(80,230,Mark,3,16,0);
			
				
				//LCD_ShowString(50,120,tftlcd_data.width,tftlcd_data.height,16,Drec[(Direction>>6)&0x03]);
		///////////////////////修正////////////////////////
//				if(huoervalue[0] == 0 && huoervalue[1] == 1 && Direction != East && Direction != West)  //往右偏
//				{
//					GoAcoddingDrec(West,150);
//					LCD_ShowString(50,160,tftlcd_data.width,tftlcd_data.height,16,"+");
//					while(huoervalue[0]^huoervalue[1] == 1);
//					LCD_ShowString(50,160,tftlcd_data.width,tftlcd_data.height,16," ");
//					GoAcoddingDrec(Direction,200);
//				}
//				else if(huoervalue[0] == 1 && huoervalue[1] == 0 && Direction != West && Direction != East)  //往left pian
//				{
//					GoAcoddingDrec(East,150);
//					LCD_ShowString(50,160,tftlcd_data.width,tftlcd_data.height,16,"+");
//					while(huoervalue[0]^huoervalue[1] == 1);
//					LCD_ShowString(50,160,tftlcd_data.width,tftlcd_data.height,16," ");
//					GoAcoddingDrec(Direction,200);
//				}
//				else if(huoervalue[2] == 0 && huoervalue[3] == 1 && Direction != North && Direction != South)  //往north pian
//				{
//					GoAcoddingDrec(South,150);
//					LCD_ShowString(50,160,tftlcd_data.width,tftlcd_data.height,16,"+");
//					while(huoervalue[2]^huoervalue[3] == 1);
//					LCD_ShowString(50,160,tftlcd_data.width,tftlcd_data.height,16," ");
//					GoAcoddingDrec(Direction,200);
//				}
//				else if(huoervalue[2] == 1 && huoervalue[3] == 0 && Direction != North && Direction != South)  //往south pian
//				{
//					GoAcoddingDrec(North,150);
//					LCD_ShowString(50,160,tftlcd_data.width,tftlcd_data.height,16,"+");
//					while(huoervalue[2]^huoervalue[3] == 1);
//					LCD_ShowString(50,160,tftlcd_data.width,tftlcd_data.height,16," ");
//					GoAcoddingDrec(Direction,200);
//				}
		}
		else
		{
			Go(0,0,0,0);
			LCD_ShowString(0,240,tftlcd_data.width,tftlcd_data.height,16,"stop Navigation!   " );
			Stop=1;
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
		 if(key_num == KEY_RIGHT)  //返回键
		 {
		     key_num = 0;
			 return;
		 }
			 
		/////////////////////读卡号////////////////
		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
         if (status != MI_OK)
         {    
		     PcdReset();
		     PcdAntennaOff(); 
		     PcdAntennaOn(); 
			 continue;
         }
		 
         status = PcdAnticoll(g_ucTempbuf);//防冲撞
         if(status != MI_OK)
         {    continue;    }
		   
		////////以下为超级终端打印出的内容////////////////////////
		//LCD_ShowFontHZ(10, 230, "序列号");	
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
				return;			  //返回
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
		/////////////////////////写卡//////////////////////////////////
		
		status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
        if (status != MI_OK)
        {    
		     PcdReset();
		     PcdAntennaOff(); 
		     PcdAntennaOn(); 
			 continue;
        }
     
        status = PcdAnticoll(g_ucTempbuf);//防冲撞
        if(status != MI_OK)
        {    continue;    }
	    status = PcdSelect(g_ucTempbuf);//选定卡片
	    if (status != MI_OK)
	    {   
				// LCD_ShowString(10,150,tftlcd_data.width,tftlcd_data.height,16,"mei you xuan ding ka pian ");
			continue;    }
	     
	    status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//验证卡片密码
	    if (status != MI_OK)
	    {    continue;    }
	    
	    status = PcdWrite(1, data_write);//写块
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
				LCD_ShowString(50,260,tftlcd_data.width,tftlcd_data.height,16,"  ");  //清除显示的数字
				key_num = 0;
				break;
			
			case KEY_RIGHT :
				key_num = 0;
				return;
		}
			/////////////////////读卡号////////////////
		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
         if (status != MI_OK)
         {    
		     PcdReset();
		     PcdAntennaOff(); 
		     PcdAntennaOn(); 
			 continue;
         }	
         status = PcdAnticoll(g_ucTempbuf);//防冲撞
         if(status != MI_OK)
         {    continue;    }
		////////以下为超级终端打印出的内容////////////////////////
		//LCD_ShowFontHZ(10, 230, "序列号");	
		LCD_ShowString(10,230,tftlcd_data.width,tftlcd_data.height,16,"ID:");
		//LCD_ShowString(100,230,tftlcd_data.width,tftlcd_data.height,16,g_ucTempbuf);
		LCD_ShowxNum(50,230,g_ucTempbuf[0]*1000000+g_ucTempbuf[1]*1000+g_ucTempbuf[2],9,16,0x80);
		 
		status = PcdSelect(g_ucTempbuf);//选定卡片
	    if (status != MI_OK)
	    {   
				// LCD_ShowString(10,150,tftlcd_data.width,tftlcd_data.height,16,"mei you xuan ding ka pian ");
			continue;    }
	     
	    status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//验证卡片密码
	    if (status != MI_OK)
	    {    continue;    }
		
     ///////////////读卡内容////////////////////////
		 status = PcdRead(1, g_ucTempbuf);
		 if (status != MI_OK)
         {    continue;    }
		 LCD_ShowString(10,260,tftlcd_data.width,tftlcd_data.height,16,"biao hao");
	     LCD_ShowxNum(50,260,g_ucTempbuf[0],3,16,0x80);		
	}
}
	

void wifi()
{
	//wifi
		{
			 userHandle();//数据采集
			 gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
			  if(key_num==KEY_UP)
			  {
					 GPIO_ResetBits(GPIOC,GPIO_Pin_5);
			     printf("wifi enter airlink\r\n");
					 gizwitsSetMode(WIFI_AIRLINK_MODE);//airlink模式
			  }
			  if(key_num==KEY_DOWN)
				{
					printf("wifi reast!\r\n");
					gizwitsSetMode(WIFI_RESET_MODE);//复位
					
				}
				if(key_num==KEY_LEFT)
				{
				
				    return;
				
				}
				if(key_num==KEY_RIGHT)
				{
				    currentDataPoint.valueCurrentPoint++;
					  if(currentDataPoint.valueCurrentPoint==21)
						{
						      currentDataPoint.valueCurrentPoint=1;
						}
				   
				
				}
				delay_ms(200);
				led3=!led3;
			}
			//wifi
}