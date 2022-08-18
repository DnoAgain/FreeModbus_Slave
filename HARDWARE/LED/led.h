#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/11/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define LED0 PBout(0)   	//LED1
#define LED1 PBout(1)   	//LED2


void LED_Init(void);//初始化
void LED_ON(void);//亮
void LED_OFF(void);//灭
void LED_Glitter1(void);//闪烁
void LED_Glitter2(void);//交替闪烁
#endif
