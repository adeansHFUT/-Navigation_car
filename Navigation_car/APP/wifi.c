#include "include.h"

uint8_t wifi_sta;

void wifi(void)
{
	//wifi
		while(1){
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
