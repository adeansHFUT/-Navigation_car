#include "include.h"

extern u8 key_num;   //按键扫描返回值
extern unsigned char status;
extern unsigned char g_ucTempbuf[20];
extern unsigned char DefaultKey[6];  
u8 MidWay=1;//是否允许从中途 标号开始 1允许0不允许
char Drec[4][18]={   //显示用
    "east",
	"west",
	"south",
	"north"
};

char DefalutRoad[]={1+East,2+West,3+South,4+North,0x00};     //调试时默认的路径

u8 ReadMark(void);

void AnalyzeRoad(char *road)
{
	u8 i=0;u8 Mark=0,RMark=0; u8 Direction=0;
	RMark = ReadMark();
	if(MidWay==1)//如果允许从中途开始
	{
		while(RMark!=(road[i]&0x3f)&&road[i]!=0x00){//找寻中途标号在总路径的位置
      i++;
			}
		
	}
	//while()
	while(1){
		if(road[i]!=0x00)
		{
			Direction=(road[i]>>6)&0x03;//得到标号
			Mark = road[i]&0x03f;	//	
		  RMark = ReadMark();//
			LCD_ShowString(10,230,tftlcd_data.width,tftlcd_data.height,16,"Mark:");
			LCD_ShowxNum(50,230,RMark,3,16,0);
			//LCD_ShowxNum(80,230,Mark,3,16,0);
			if(RMark==Mark)//走到了标号为Mark处
			{

				LCD_ShowString(10,260,tftlcd_data.width,tftlcd_data.height,16,"Direaction:");
				LCD_ShowString(150,260,tftlcd_data.width,tftlcd_data.height,16,Drec[Direction]);
				
				i++;//获取下一标号
				//DirectionMove(Direction,50);添加方向运动函数
			}
			
		}
		else
		{
			LCD_ShowString(10,130,tftlcd_data.width,tftlcd_data.height,16,"mark not in road or finish");
			break;
		}
	}
	
}

u8 ReadMark(void)
{
	while(1){
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
	  // LCD_ShowxNum(50,260,g_ucTempbuf[0],3,16,0x80);		
			return 	 g_ucTempbuf[0];
			 }

}
/////////////////////////////////////////////////////////////////////
//功    能：初始化邻接矩阵
//参数说明: 二维数组指针
//返    回: 无
/////////////////////////////////////////////////////////////////////
void InitMatrix(char (*G)[MaxVerNum])
{

    int  i, j;
    for(i = 0; i < MaxVerNum; i++)
       for(j = 0; j < MaxVerNum; j++)
           G[i][j] = MaxValue;

}
/////////////////////////////////////////////////////////////////////
//功    能：打印邻接矩阵
//参数说明: 二维数组指针，n：边长 ，x、y：显示的位置
//返    回: 无
/////////////////////////////////////////////////////////////////////
void PrintMatrix(char (*G)[MaxVerNum], int n,int x,int y)

{

    int i, j;
    for(i = 0; i <n; i++)
    {

       for(j = 0; j <n; j++)
       {
           if(G[i][j] == MaxValue)
					 {
							LCD_ShowString(x+j*16,y+i*12,tftlcd_data.width,tftlcd_data.height,12," 0");
					 }  
           else
					 {
						  LCD_ShowxNum(x+j*16,y+i*12,G[i][j],2,12,0);
              //LCD_ShowString(x+j*8,y+i*12,tftlcd_data.width,tftlcd_data.height,12,Drec[G[i][j]>>6]);
					 }
       }
    }

}
/////////////////////////////////////////////////////////////////////
//功    能：FLOYD算法找出图的最短路径
//参数说明: 
//返    回: road:最短路径
/////////////////////////////////////////////////////////////////////
void FLOYD(char (*IG)[MaxVerNum],int n,int start,int end,char *road)
{
    int i = 0,j=0;
	int u = 0,v,w;
	int k=1;
	int temp = 0;
    static char DFloyd[20][20];
    static char Path[20][20];   
	static char G[MaxVerNum][MaxVerNum]; 	//路径初始化
	//static char road[MaxVerNum]={0};
	//f[i][j] = 0;  
	//InitMatrix(f);
	//InitMatrix(Path);
	//InitMatrix(G);
	for(i=0;i<n;i++)//将IG图变为值仅表示 距离的图
	{
		for(j=0;j<n;j++)
		{
			if(IG[i+1][j+1]!=MaxValue)
			{
				G[i][j]=1;
			}
			else
			{
				G[i][j]=MaxValue;
			}
			if(i==j)
			{
				G[i][j]=0;
			}
		}
	}
		//PrintMatrix(G,n,10,20);
	
  for ( i = 0;i < n; i++){
		//Path[i][0] =i;  
        for (j= 0; j < n; j++) {
            if (i == j)                                                         //
                G[i][j] = 0;
            DFloyd[i][j] = G[i][j];  
            Path[i][j] =j;                                             //
        }
			}
//
		//
     //PrintMatrix(Path,n,10,20);
 for (u = 0; u < n; u++)
        for (v = 0; v < n; v++)
            for (w = 0; w < n; w++)
                if (DFloyd[v][u] + DFloyd[u][w] < DFloyd[v][w]) {               //??????,????
                    DFloyd[v][w] = DFloyd[v][u] + DFloyd[u][w];
                    Path[v][w] = Path[v][u];                                    //??Path??
                }
			//PrintMatrix(Path,n,10,200);					
						if(DFloyd[start-1][end-1] < MaxValue)
            {
							LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,12,"length:");                                                   //???????????????
             LCD_ShowxNum(50,10,DFloyd[start-1][end-1],3,12,0);
            }else if (DFloyd[i][j] >= MaxValue)
            {
                LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,12,"no ");
            }
						
						//LCD_ShowString(60,10,tftlcd_data.width,tftlcd_data.height,12,"path:");
						//LCD_ShowxNum(90,10,start,2,12,0);
            temp = Path[start-1][end-1]+1;
						road[0]=start;
            while (temp != end) {                                                     //??????(Path[temp][j] == j)??!!!
                //LCD_ShowxNum(90+20*k,10,temp,3,12,0); 
                road[k]=temp;                     //??????
                temp = Path[temp-1][end-1]+1;
							k++;
            }
						road[k]=end;
						//LCD_ShowxNum(90+20*k,10,end,3,12,0); 
		k=0;				
    while(road[k+1]!=0)
		{
			temp=road[k];
			road[k]=road[k]+(IG[temp][road[k+1]]);
			//LCD_ShowString(10+k*20,30,tftlcd_data.width,tftlcd_data.height,12,Drec[road[k]/64]);
			k++;
			
		}
		
		
   //*/
                                                          //??Path[][]??
}

