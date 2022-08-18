#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/11/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
  
//LED IO��ʼ��
uint8_t i;

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();           	//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_0; 				//PB0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_1; 				//PB1
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);	//PA8��1��Ĭ�ϳ�ʼ�������
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);	//PD2��1��Ĭ�ϳ�ʼ�������
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
