/***************************************************************************
**   					             华东交通大学 RealSteel战队
**   					               钢铁洪流，风驰电掣!
**-------------------------------------------------------------------------
** 项    目：   STM32G0_Board
** 文 件 名：   test.c
** 文件说明：   教程测试程序
**-------------------------------------------------------------------------
**						*修订*
**	*版本*							*修改内容*							*修改人*      			 *日期*
**	 1.0							   初始版本						     黄宇阳     	     2024-09-16
***************************************************************************/

#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "stdint.h"
#include "test.h"
#include "libs.h"


// 数码管字模 0 1 2 3 4 5 6 7 8 9
uint8_t display_codes[10] = {0x22, 0xEE, 0x34, 0xA4, 0xE8, 0xA1, 0x21, 0xE6, 0x20, 0xA0};

// 数码管亮度，0~1000（htim3.Instance->ARR）
uint16_t display_birghness = 999;

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
//  // 数码管显示
//  Refresh_Display(display_codes[6], 0, 2, display_birghness);
//	// 视觉暂留
//  HAL_Delay(3);
//	Refresh_Display(display_codes[6], 0, 1, display_birghness);
//	// 视觉暂留
//  HAL_Delay(3);
//  Refresh_Display(display_codes[6], 0, 0, display_birghness);  
//	// 视觉暂留
//  HAL_Delay(3);

//	 // 数码管显示
//  Hundred_Display(800, display_birghness);
//	// 视觉暂留
//  HAL_Delay(200);
//	// 数码管显示
  Hundred_Display(000, display_birghness);
//	// 视觉暂留
//  HAL_Delay(200);
//	// 数码管显示
//  Hundred_Display(666, display_birghness);
//	// 视觉暂留
//  HAL_Delay(200);


	
}
int k = 0;
