/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "time.h"
#include <stdio.h>
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    TIM3_Handler.Instance = TIM3;
    TIM3_Handler.Init.Prescaler = 3599;								    // 50us记一次数
    TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM3_Handler.Init.Period = usTim1Timerout50us - 1;					// usTim1Timerout50us * 50即为定时器溢出时间
    TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&TIM3_Handler) != HAL_OK)
    {
        return FALSE;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&TIM3_Handler, &sClockSourceConfig) != HAL_OK)
    {
        return FALSE;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&TIM3_Handler, &sMasterConfig) != HAL_OK)
    {
        return FALSE;
    }

	__HAL_TIM_CLEAR_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE);               // 先清除一下定时器的中断标记,防止使能中断后直接触发中断
    __HAL_TIM_ENABLE_IT(&TIM3_Handler, TIM_IT_UPDATE);					// 使能定时器更新中断
    HAL_TIM_Base_Start_IT(&TIM3_Handler);	
    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */

  __HAL_TIM_SET_COUNTER(&TIM3_Handler, 0);		//清空定时器
  __HAL_TIM_ENABLE(&TIM3_Handler);						//使能定时器
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
   __HAL_TIM_DISABLE(&TIM3_Handler);				//关闭定时器
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
 void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

/// 定时器3中断服务程序
void TIM3_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE))			// 更新中断标记被置位
    {
        __HAL_TIM_CLEAR_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE);		// 清除中断标记
        prvvTIMERExpiredISR();								        // 通知modbus3.5个字符等待时间到
    }
}
