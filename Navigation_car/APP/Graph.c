#include "include.h"

extern u8 key_num;   //����ɨ�践��ֵ
extern unsigned char status;
extern unsigned char g_ucTempbuf[20];
extern unsigned char DefaultKey[6];  
u8 MidWay=1;//�Ƿ��������; ��ſ�ʼ 1����0������
char Drec[4][18]={   //��ʾ��
    "east",
	"west",
	"south",
	"north"
};

char DefalutRoad[]={1+East,2+West,3+South,4+North,0x00};     //����ʱĬ�ϵ�·��

u8 ReadMark(void);

void AnalyzeRoad(char *road)
{
	u8 i=0;u8 Mark=0,RMark=0; u8 Direction=0;
	RMark = ReadMark();
	if(MidWay==1)//����������;��ʼ
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

u8 ReadMark(void)
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
/////////////////////////////////////////////////////////////////////
//��    �ܣ���ʼ���ڽӾ���
//����˵��: ��ά����ָ��
//��    ��: ��
/////////////////////////////////////////////////////////////////////
void InitMatrix(char (*G)[MaxVerNum])
{

    int  i, j;
    for(i = 0; i < MaxVerNum; i++)
       for(j = 0; j < MaxVerNum; j++)
           G[i][j] = MaxValue;

}
/////////////////////////////////////////////////////////////////////
//��    �ܣ���ӡ�ڽӾ���
//����˵��: ��ά����ָ�룬n���߳� ��x��y����ʾ��λ��
//��    ��: ��
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
//��    �ܣ�FLOYD�㷨�ҳ�ͼ�����·��
//����˵��: 
//��    ��: road:���·��
/////////////////////////////////////////////////////////////////////
void FLOYD(char (*IG)[MaxVerNum],int n,int start,int end,char *road)
{
    int i = 0,j=0;
	int u = 0,v,w;
	int k=1;
	int temp = 0;
    static char DFloyd[20][20];
    static char Path[20][20];   
	static char G[MaxVerNum][MaxVerNum]; 	//·����ʼ��
	//static char road[MaxVerNum]={0};
	//f[i][j] = 0;  
	//InitMatrix(f);
	//InitMatrix(Path);
	//InitMatrix(G);
	for(i=0;i<n;i++)//��IGͼ��Ϊֵ����ʾ �����ͼ
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

