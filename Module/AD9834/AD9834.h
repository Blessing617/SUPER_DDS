#ifndef _AD9833_H
#define _AD9833_H

#include "main.h"

#define Sine_Wave     0X2008	//正弦波
#define Triangle_Wave 0x2002	//三角波
#define Square_Wave   0x2028	//方波

#define AD9834_MCLK 75000000UL	//AD9834晶振频率

//控制脚A3-A6，FS，PS即B0,B1直接置低，由控制位控制

#define AD9834_FSYNC_SET			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET)

#define AD9834_FSYNC_CLR   			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET)

#define AD9834_SCLK_SET				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET) 

#define AD9834_SCLK_CLR				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET) 

#define AD9834_SDATA_SET   			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET) 

#define AD9834_SDATA_CLR   			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)

#define AD9834_RESET_SET   			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET) 

#define AD9834_RESET_CLR   			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)

#define AD9834_FS_SET       		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET)

#define AD9834_FS_CLR      	 		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET)

#define AD9833_PS_SET				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET)

#define AD9833_PS_CLR				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET)



//频率寄存器
#define FREQ_0      0 

#define FREQ_1      1 

/* AD9834函数声明 */ 
	
void AD9834_Write_Data(unsigned int data);			 //写16位数据

void AD9834_Select_Wave(unsigned int initdata) ;   //选择输出波形 

void AD9834_Init(void);  													 //初始化配置 

void AD9834_Set_Freq(unsigned char freq_number, unsigned long freq) ;		//选择输出寄存器和输出频率值 
	
#endif
