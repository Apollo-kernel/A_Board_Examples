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
#include "tim.h"
#include "gpio.h"
#include "stdint.h"
#include "test.h"
#include "libs.h"


// 数码管字模 0 1 2 3 4 5 6 7 8 9
uint8_t display_codes[10] = {0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09};

// 数码管亮度，0~1000（htim3.Instance->ARR）
uint16_t display_birghness = 400;

// 数码管上显示的数字
uint16_t num;


/***********************************************************************
** 函 数 名： setup()
** 函数说明： 程序初始化函数，上电后执行一次
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void Setup()
{
  // 数码管清屏，防止意外点亮
  Clear_Display();

  // 设置数码管亮度PWM
  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 1000);
  __HAL_TIM_SetCompare(&htim16, TIM_CHANNEL_1, 0);
  __HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, 0);

  // 玄学延时
  HAL_Delay(10);

  // 开启数码管公共端PWM信号
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

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
  // 数码管显示
  Refresh_Display(display_codes[7], 0, 2, display_birghness);
	// 视觉暂留
  HAL_Delay(3);
	Refresh_Display(display_codes[8], 0, 1, display_birghness);
	// 视觉暂留
  HAL_Delay(3);
  Refresh_Display(display_codes[9], 0, 0, display_birghness);  
	// 视觉暂留
  HAL_Delay(3);

}
int k = 0;
