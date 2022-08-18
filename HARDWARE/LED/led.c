#include "led.h"
#include "delay.h"
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
  
//LED IO初始化
uint8_t i;

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0; 				//PB0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_1; 				//PB1
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);	//PA8置1，默认初始化后灯灭
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);	//PD2置1，默认初始化后灯灭
}

void LED_ON(void)
{
	LED0 = 1;
	LED1 = 1;
	delay_ms(500);
}

void LED_OFF(void)
{
	LED0 = 0;
	LED1 = 0;
}

void LED_Glitter1(void)
{
	for(i=0;i<8;i++)
	{
		LED0 = !LED0;
		LED1 = !LED1;
		delay_ms(500);
	}
}

void LED_Glitter2(void)
{
	for(i=0;i<8;i++)
	{
		LED0 = !LED0;
		delay_ms(300);
		LED1 = !LED1;	
		delay_ms(300);
	}
}
