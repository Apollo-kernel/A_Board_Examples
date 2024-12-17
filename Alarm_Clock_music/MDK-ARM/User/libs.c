/***************************************************************************
**   					             华东交通大学 RealSteel战队
**   					               钢铁洪流，风驰电掣!
**-------------------------------------------------------------------------
** 项    目：   STM32G0_Board
** 文 件 名：   libs.c
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

extern uint16_t t_tube;

void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness); // 刷新数码管
void Clear_Display(); // 数码管清屏
void Hundred_Display(uint32_t hundred, uint16_t brightness); // 数码管999
void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness);
void Refresh_LED(uint16_t state); // 刷新LED灯（0亮1灭）
void Refresh_LED_Neg(uint16_t state); // 刷新LED灯（0灭1亮）

/***********************************************************************
** 函 数 名： Get_Bit()
** 函数说明： 定时器中断回调函数函数，定时器溢出后进入
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
uint8_t Get_Bit(uint16_t num, uint8_t bit_id) // 取出num在二进制表示下的第id位,id表示二进制位的位置，从0开始，如二进制数0b111101的第0位是1，第1位是0，第2~5位为1
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
** 函 数 名： Clear_Display()
** 函数说明： 数码管清屏
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void Clear_Display()
{
  HAL_GPIO_WritePin( Display_A_GPIO_Port,  Display_A_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( Display_B_GPIO_Port,  Display_B_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( Display_C_GPIO_Port,  Display_C_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( Display_D_GPIO_Port,  Display_D_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( Display_E_GPIO_Port,  Display_E_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( Display_F_GPIO_Port,  Display_F_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( Display_G_GPIO_Port,  Display_G_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin( Display_DP_GPIO_Port,  Display_DP_Pin, GPIO_PIN_SET);
}

/***********************************************************************
** 函 数 名： Refresh_Display(uint8_t code, uint8_t id)
** 函数说明： 数码管显示
**---------------------------------------------------------------------
** 输入参数： 数码管字模，点亮不亮，第几位，亮度
** 返回参数： 无
***********************************************************************/
//void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness)
//{
//  code &= ~(dot << 5);
//  if(id == 2)
//  {
//    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 1000 - brightness);
//  }
//  else
//  {
//    htim1.Instance->CNT = 996;
//    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 1000);

//  }
//  if(id == 1)
//  {
//    __HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, brightness);
//  }
//  else
//  {
//    __HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, 0);

//  }

//  if(id == 0)
//  {
//    __HAL_TIM_SetCompare(&htim16, TIM_CHANNEL_1, brightness);

//  }
//  else
//  {
//    __HAL_TIM_SetCompare(&htim16, TIM_CHANNEL_1, 0);
//  }
//  HAL_GPIO_WritePin( Display_E_GPIO_Port,  Display_E_Pin, Get_Bit(code, 7));//B9		E  
//  HAL_GPIO_WritePin( Display_D_GPIO_Port,  Display_D_Pin, Get_Bit(code, 6));//B8		D
//  HAL_GPIO_WritePin( Display_DP_GPIO_Port,  Display_DP_Pin, Get_Bit(code, 5));//B7		DP
//  HAL_GPIO_WritePin( Display_C_GPIO_Port,  Display_C_Pin, Get_Bit(code, 4));//B6		C
//  HAL_GPIO_WritePin( Display_A_GPIO_Port,  Display_A_Pin, Get_Bit(code, 3));//B5		A
//  HAL_GPIO_WritePin( Display_F_GPIO_Port,  Display_F_Pin, Get_Bit(code, 2));//B4		F
//  HAL_GPIO_WritePin( Display_G_GPIO_Port,  Display_G_Pin, Get_Bit(code, 1));//B3		G
//	HAL_GPIO_WritePin( Display_B_GPIO_Port,  Display_B_Pin, Get_Bit(code, 0));//D3  B
//}

/***********************************************************************
** 函 数 名： Hundred_Display(uint8_t hundred)
** 函数说明： 数码管999
**---------------------------------------------------------------------
** 输入参数： number，亮度
** 返回参数： 无
***********************************************************************/
void Hundred_Display(uint32_t hundred, uint16_t brightness)
{
	uint8_t display_codes[10] = {0x22, 0xEE, 0x34, 0xA4, 0xE8, 0xA1, 0x21, 0xE6, 0x20, 0xA0};
	if(t_tube<5)
		Hundreds_Display(display_codes[hundred/100], 1, 2, brightness);
	else if(t_tube<10)
		Hundreds_Display(display_codes[(hundred/10)%10], 0, 1, brightness);
	else if(t_tube<15)
		Hundreds_Display(display_codes[hundred%10], 0, 0, brightness);
	else
		t_tube=0;
}

void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness)
{
  code &= ~(dot << 5);
  if(id == 2)
  {
		HAL_GPIO_WritePin(Display_DG1_GPIO_Port,Display_DG1_Pin,GPIO_PIN_SET);
  }
  else
  {
		HAL_GPIO_WritePin(Display_DG1_GPIO_Port,Display_DG1_Pin,GPIO_PIN_RESET);
  }
  if(id == 1)
  {
		HAL_GPIO_WritePin(Display_DG2_GPIO_Port,Display_DG2_Pin,GPIO_PIN_SET);
  }
  else
  {
		HAL_GPIO_WritePin(Display_DG2_GPIO_Port,Display_DG2_Pin,GPIO_PIN_RESET);
  }

  if(id == 0)
  {
		HAL_GPIO_WritePin(Display_DG3_GPIO_Port,Display_DG3_Pin,GPIO_PIN_SET);
  }
  else
  {
		HAL_GPIO_WritePin(Display_DG3_GPIO_Port,Display_DG3_Pin,GPIO_PIN_RESET);
  }
  HAL_GPIO_WritePin( Display_E_GPIO_Port,  Display_E_Pin, Get_Bit(code, 7));//B9		E  
  HAL_GPIO_WritePin( Display_D_GPIO_Port,  Display_D_Pin, Get_Bit(code, 6));//B8		D
  HAL_GPIO_WritePin( Display_DP_GPIO_Port,  Display_DP_Pin, Get_Bit(code, 5));//B7		DP
  HAL_GPIO_WritePin( Display_C_GPIO_Port,  Display_C_Pin, Get_Bit(code, 4));//B6		C
  HAL_GPIO_WritePin( Display_A_GPIO_Port,  Display_A_Pin, Get_Bit(code, 3));//B5		A
  HAL_GPIO_WritePin( Display_F_GPIO_Port,  Display_F_Pin, Get_Bit(code, 2));//B4		F
  HAL_GPIO_WritePin( Display_G_GPIO_Port,  Display_G_Pin, Get_Bit(code, 1));//B3		G
	HAL_GPIO_WritePin( Display_B_GPIO_Port,  Display_B_Pin, Get_Bit(code, 0));//D3  B
}

void buzzer_on(uint16_t psc, uint16_t pwm)
{
	//设置分频
	__HAL_TIM_PRESCALER(&htim1, psc);
	//设置 PWM 的占空比
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm);
}
void buzzer_off(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
}

