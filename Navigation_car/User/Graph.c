#include "Graph.h"

void InitMatrix(char (*G)[MaxVerNum])

{

    int  i, j;

    for(i = 0; i < MaxVerNum; i++)

       for(j = 0; j < MaxVerNum; j++)

           G[i][j] = MaxValue;

}
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
void FLOYD(char (*IG)[MaxVerNum],int n,int start,int end,char *road)
{
   int i = 0,j=0;int u = 0,v,w;int k=1;int temp = 0;
  static char DFloyd[MaxVerNum][MaxVerNum];
  static char Path[MaxVerNum][MaxVerNum];   
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
		road[end]=0x00;
		
   //*/
                                                          //??Path[][]??
}

