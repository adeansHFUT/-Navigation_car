#ifndef _GRAPH_H
#define _GRAPH_H

#define MaxVerNum 20
#define MaxValue 55

#define East 0x00
#define West 0x40
#define South 0x80
#define North 0xc0
     //?????????

 


void InitMatrix(char (*G)[MaxVerNum]);
void PrintMatrix(char (*G)[MaxVerNum], int n,int x,int y);
void FLOYD(char (*IG)[MaxVerNum],int n,int start,int end,char *road);
void AnalyzeRoad(char *road);//分析传输的路径信息

#endif
