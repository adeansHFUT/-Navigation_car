#include "include.h"

unsigned char data1[16] = {0x02,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
u8 data_write[16] = {0};
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char data2[4]  = {0,0,0,0x01};
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
unsigned char g_ucTempbuf[20];
unsigned char status,i;
unsigned int temp;

/////////////////////////////////////////////////////////////////////
//功    能：总的RFID功能选择
//参数说明: 无
//返    回: 无
/////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////
//功    能：显示卡片id
//参数说明: 无
//返    回: 打印
/////////////////////////////////////////////////////////////////////
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
}
/////////////////////////////////////////////////////////////////////
//功    能：往卡里写内容
//参数说明: data_write[0] 是写入的内容
//返    回: 打印
/////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////
//功    能：读卡号和卡的内容
//参数说明: 
//返    回: 打印
/////////////////////////////////////////////////////////////////////
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