/***************************************************************************
**   					             华东交通大学 RealSteel战队
**   					               铁甲钢拳，风驰电掣!
**-------------------------------------------------------------------------
** 项    目：   STM32G0_Board
** 文 件 名：   test.c
** 文件说明：   教程测试程序
**-------------------------------------------------------------------------
**						*修订*
**	*版本*							*修改内容*							*修改人*      			 *日期*
**	 1.0							   初始版本						     黄宇阳     	     2024-11-23
***************************************************************************/

#include "main.h"
#include "gpio.h"
#include "stdint.h"
#include "test.h"

// 使用一个16位2进制数存放16个led灯的状态，高位给标号大的LED
uint16_t led_state = 0b0000000000000001;
int k = 0;

/***********************************************************************
** 函 数 名： Get_Bit()
** 函数说明： 获取此时的LED灯设置状态 
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
uint8_t Get_Bit(uint16_t num, uint8_t bit_id) 
// id表示二进制位的位置，从0开始，如二进制数0b111101的第0位是1，第1位是0，第2~5位为1
{
  return (num >> bit_id) & 0b01;
}

/***********************************************************************
** 函 数 名： Refresh_LED(uint16_t state)
** 函数说明： 刷新LED灯点亮状态（0亮1灭）
**---------------------------------------------------------------------
** 输入参数： 一个16位整数，每个位对应一个LED灯的状态
** 返回参数： 无
***********************************************************************/
void Refresh_LED(uint16_t state)
{
  HAL_GPIO_WritePin( LED1_GPIO_Port,  LED1_Pin, Get_Bit(state,  0));
  HAL_GPIO_WritePin( LED2_GPIO_Port,  LED2_Pin, Get_Bit(state,  1));
  HAL_GPIO_WritePin( LED3_GPIO_Port,  LED3_Pin, Get_Bit(state,  2));
  HAL_GPIO_WritePin( LED4_GPIO_Port,  LED4_Pin, Get_Bit(state,  3));
  HAL_GPIO_WritePin( LED5_GPIO_Port,  LED5_Pin, Get_Bit(state,  4));
  HAL_GPIO_WritePin( LED6_GPIO_Port,  LED6_Pin, Get_Bit(state,  5));
  HAL_GPIO_WritePin( LED7_GPIO_Port,  LED7_Pin, Get_Bit(state,  6));
  HAL_GPIO_WritePin( LED8_GPIO_Port,  LED8_Pin, Get_Bit(state,  7));
  HAL_GPIO_WritePin( LED9_GPIO_Port,  LED9_Pin, Get_Bit(state,  8));
  HAL_GPIO_WritePin(LED10_GPIO_Port, LED10_Pin, Get_Bit(state,  9));
  HAL_GPIO_WritePin(LED11_GPIO_Port, LED11_Pin, Get_Bit(state, 10));
  HAL_GPIO_WritePin(LED12_GPIO_Port, LED12_Pin, Get_Bit(state, 11));
  HAL_GPIO_WritePin(LED13_GPIO_Port, LED13_Pin, Get_Bit(state, 12));
  HAL_GPIO_WritePin(LED14_GPIO_Port, LED14_Pin, Get_Bit(state, 13));
  HAL_GPIO_WritePin(LED15_GPIO_Port, LED15_Pin, Get_Bit(state, 14));
  HAL_GPIO_WritePin(LED16_GPIO_Port, LED16_Pin, Get_Bit(state, 15));
}

/***********************************************************************
** 函 数 名： Refresh_LED_Neg(uint16_t state)
** 函数说明： 刷新LED灯点亮状态（0灭1亮）
**---------------------------------------------------------------------
** 输入参数： 一个16位整数，每个位对应一个LED灯的状态
** 返回参数： 无
***********************************************************************/
void Refresh_LED_Neg(uint16_t state)
{
  Refresh_LED(~state);
}
/***********************************************************************
** 函 数 名： Setup()
** 函数说明： 程序初始化函数，上电后执行一次
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void Setup()
{
	Refresh_LED(~0);
}

/***********************************************************************
** 函 数 名： Loop()
** 函数说明： 程序主循环函数，上电后循环执行
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void Loop()
{

	for(k = 0; k < 16; k++)
	{
		led_state<<=1;
		led_state++;
//		Refresh_LED(led_state);
		Refresh_LED(~led_state);
		HAL_Delay(500);
	}
	led_state = 0;
}

