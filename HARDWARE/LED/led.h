#ifndef _LED_H
#define _LED_H
#include "sys.h"
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

#define LED0 PBout(0)   	//LED1
#define LED1 PBout(1)   	//LED2


void LED_Init(void);//��ʼ��
void LED_ON(void);//��
void LED_OFF(void);//��
void LED_Glitter1(void);//��˸
void LED_Glitter2(void);//������˸
#endif
