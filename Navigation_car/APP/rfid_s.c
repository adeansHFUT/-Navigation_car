#include "include.h"

unsigned char data1[16] = {0x02,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
u8 data_write[16] = {0};
//M1����ĳһ��дΪ���¸�ʽ����ÿ�ΪǮ�����ɽ��տۿ�ͳ�ֵ����
//4�ֽڽ����ֽ���ǰ����4�ֽڽ��ȡ����4�ֽڽ�1�ֽڿ��ַ��1�ֽڿ��ַȡ����1�ֽڿ��ַ��1�ֽڿ��ַȡ�� 
unsigned char data2[4]  = {0,0,0,0x01};
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
unsigned char g_ucTempbuf[20];
unsigned char status,i;
unsigned int temp;

/////////////////////////////////////////////////////////////////////
//��    �ܣ��ܵ�RFID����ѡ��
//����˵��: ��
//��    ��: ��
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
//��    �ܣ���ʾ��Ƭid
//����˵��: ��
//��    ��: ��ӡ
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
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ�������д����
//����˵��: data_write[0] ��д�������
//��    ��: ��ӡ
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
/////////////////////////////////////////////////////////////////////
//��    �ܣ������źͿ�������
//����˵��: 
//��    ��: ��ӡ
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