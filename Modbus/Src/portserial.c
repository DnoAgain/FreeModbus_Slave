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

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "usart.h"
#include "gpio.h"
/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    // int i=0;
    if(xRxEnable)
    {
        // do
        // {
        //     i++;
        // } while (i<1500);
        __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE); // 使能接收非空中断
        // HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET); //MAX485操作 低电平为接收模式//RE端控制接收/发送状态，RE为1时发送，为0时接收。
    }
    else
    {
        __HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE); // 禁能接收非空中断	
        // HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET);
    }
    
    if(xTxEnable)
    {
        // i=0;
        // do
        // {
        //     i++;
        // } while (i<1500);
        // __HAL_UART_ENABLE_IT(&huart3, UART_IT_TXE);			// 使能发送为空中断
        __HAL_UART_ENABLE_IT(&huart3, UART_IT_TXE);			// 使能发送为空中断		
        HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET); //MAX485操作 高电平为发送模式
    }
    else
    {
        __HAL_UART_DISABLE_IT(&huart3, UART_IT_TXE);		// 禁能发送为空中断
        __HAL_UART_ENABLE_IT(&huart3, UART_IT_TC);
        // HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET); //MAX485操作 高电平为发送模式
    }
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    (void)ucPORT;  //不修改串口号
    (void)ucDataBits;  //不修改数据位长度
    (void)eParity;  //不修改检验格式
    huart3.Instance = USART3;
    huart3.Init.BaudRate = ulBaudRate;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    switch(eParity)
    {
        // 奇校验
    case MB_PAR_ODD:
        huart3.Init.Parity = UART_PARITY_ODD;
        huart3.Init.WordLength = UART_WORDLENGTH_9B;                        // 带奇偶校验数据位为9bits
        break;
        
        // 偶校验
    case MB_PAR_EVEN:
        huart3.Init.Parity = UART_PARITY_EVEN;
        huart3.Init.WordLength = UART_WORDLENGTH_9B;                        // 带奇偶校验数据位为9bits
        break;
        
        // 无校验
    default:
        huart3.Init.Parity = UART_PARITY_NONE;
        huart3.Init.WordLength = UART_WORDLENGTH_8B;                        // 无奇偶校验数据位为8bits
        break;
    }
    return HAL_UART_Init(&huart3) == HAL_OK ? TRUE : FALSE;
    
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    USART3->DR = ucByte;
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    *pucByte = (USART3->DR & (uint16_t)0x00FF);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}

void USART3_IRQHandler(void)
{
    if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE))			// 接收非空中断标记被置位
    {
        __HAL_UART_CLEAR_FLAG(&huart3, UART_FLAG_RXNE);			// 清除中断标记
        prvvUARTRxISR();										// 通知modbus有数据到达
    }

    if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE))				// 发送为空中断标记被置位
    {
        __HAL_UART_CLEAR_FLAG(&huart3, UART_FLAG_TXE);			// 清除中断标记
        prvvUARTTxReadyISR();									// 通知modbus数据可以发松
    }

    if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC))			//发送完成中断
		{
				__HAL_UART_CLEAR_FLAG(&huart3, UART_FLAG_TC);		//清除中断标记
				HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);	//485芯片设置为接收模式
		}
}
