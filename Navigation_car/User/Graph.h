#include "tftlcd.h"
#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"

#define MaxVerNum 22

#define MaxValue 55

     //?????????

 


void InitMatrix(char (*G)[MaxVerNum]);
void PrintMatrix(char (*G)[MaxVerNum], int n,int x,int y);
void FLOYD(char (*IG)[MaxVerNum],int n,int start,int end,char *road);