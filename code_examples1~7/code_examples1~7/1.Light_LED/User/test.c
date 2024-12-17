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

void Init_LED()
{
  HAL_GPIO_WritePin( LED1_GPIO_Port,  LED1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( LED2_GPIO_Port,  LED2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( LED3_GPIO_Port,  LED3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( LED4_GPIO_Port,  LED4_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( LED5_GPIO_Port,  LED5_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( LED6_GPIO_Port,  LED6_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( LED7_GPIO_Port,  LED7_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( LED8_GPIO_Port,  LED8_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( LED9_GPIO_Port,  LED9_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED10_GPIO_Port, LED10_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED11_GPIO_Port, LED11_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED12_GPIO_Port, LED12_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED13_GPIO_Port, LED13_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED14_GPIO_Port, LED14_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED15_GPIO_Port, LED15_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED16_GPIO_Port, LED16_Pin, GPIO_PIN_SET);
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
  Init_LED();
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
  HAL_Delay(2000);
  HAL_GPIO_WritePin(LED1_GPIO_Port,  LED1_Pin, GPIO_PIN_RESET);
}
