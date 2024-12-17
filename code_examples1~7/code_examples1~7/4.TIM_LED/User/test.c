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
#include "tim.h"
#include "stdint.h"
#include "test.h"

uint16_t  tim3clk = 1;


/***********************************************************************
** 函 数 名： Toggle_LED(void)
** 函数说明： 翻转LED灯点亮状态
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void Toggle_LED(void)
{
	HAL_GPIO_TogglePin( LED1_GPIO_Port,  LED1_Pin);			//翻转LED1电平
	HAL_GPIO_TogglePin( LED2_GPIO_Port,  LED2_Pin);
	HAL_GPIO_TogglePin( LED3_GPIO_Port,  LED3_Pin);
	HAL_GPIO_TogglePin( LED4_GPIO_Port,  LED4_Pin);
	HAL_GPIO_TogglePin( LED5_GPIO_Port,  LED5_Pin);
	HAL_GPIO_TogglePin( LED6_GPIO_Port,  LED6_Pin);
	HAL_GPIO_TogglePin( LED7_GPIO_Port,  LED7_Pin);
	HAL_GPIO_TogglePin( LED8_GPIO_Port,  LED8_Pin);
	HAL_GPIO_TogglePin( LED9_GPIO_Port,  LED9_Pin);
	HAL_GPIO_TogglePin(LED10_GPIO_Port, LED10_Pin);
	HAL_GPIO_TogglePin(LED11_GPIO_Port, LED11_Pin);
	HAL_GPIO_TogglePin(LED12_GPIO_Port, LED12_Pin);
	HAL_GPIO_TogglePin(LED13_GPIO_Port, LED13_Pin);
	HAL_GPIO_TogglePin(LED14_GPIO_Port, LED14_Pin);
	HAL_GPIO_TogglePin(LED15_GPIO_Port, LED15_Pin);
	HAL_GPIO_TogglePin(LED16_GPIO_Port, LED16_Pin);
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
	//开启定时器中断
  HAL_TIM_Base_Start_IT(&htim3);
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

}
/***********************************************************************
** 函 数 名： HAL_TIM_PeriodElapsedCallback()
** 函数说明： 定时器中断回调函数函数，定时器溢出后进入
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim3)
    {
			// 500m 翻转
      Toggle_LED();
		}
}