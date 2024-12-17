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
uint16_t i;

/***********************************************************************
** 函 数 名： Toggle_LED(void)
** 函数说明： 翻转LED灯点亮状态
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/


/***********************************************************************
** 函 数 名： Setup()
** 函数说明： 程序初始化函数，上电后执行一次
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void Setup()
{
	//start tim
	//开启定时器
	HAL_TIM_Base_Start(&htim3);
	//start pwm channel
	//开启PWM通道
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
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
	//从暗到亮
	for(i = 0; i < 999; i++)												//定时器重载值为999
	{
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, i);
	HAL_Delay(3);
	}
	//从亮到暗
		for(i = 999; i > 0; i--)
	{
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, i);
	HAL_Delay(3);
	}
}
