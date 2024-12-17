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

void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness); // 刷新数码管
void Clear_Display(); // 数码管清屏
void Hundred_Display(uint32_t hundred, uint16_t brightness); // 数码管999
void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness);

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
void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness)
{
  code &= ~(dot << 5);
  if(id == 2)
  {
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 1000 - brightness);
  }
  else
  {
    htim1.Instance->CNT = 996;
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 1000);

  }
  if(id == 1)
  {
    __HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, brightness);
  }
  else
  {
    __HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, 0);

  }

  if(id == 0)
  {
    __HAL_TIM_SetCompare(&htim16, TIM_CHANNEL_1, brightness);

  }
  else
  {
    __HAL_TIM_SetCompare(&htim16, TIM_CHANNEL_1, 0);
  }
  HAL_GPIO_WritePin( Display_A_GPIO_Port,  Display_A_Pin, Get_Bit(code, 7));
  HAL_GPIO_WritePin( Display_B_GPIO_Port,  Display_B_Pin, Get_Bit(code, 6));
  HAL_GPIO_WritePin( Display_C_GPIO_Port,  Display_C_Pin, Get_Bit(code, 5));
  HAL_GPIO_WritePin( Display_D_GPIO_Port,  Display_D_Pin, Get_Bit(code, 4));
  HAL_GPIO_WritePin( Display_E_GPIO_Port,  Display_E_Pin, Get_Bit(code, 3));
  HAL_GPIO_WritePin( Display_F_GPIO_Port,  Display_F_Pin, Get_Bit(code, 2));
  HAL_GPIO_WritePin( Display_G_GPIO_Port,  Display_G_Pin, Get_Bit(code, 1));
	HAL_GPIO_WritePin( Display_DP_GPIO_Port,  Display_DP_Pin, Get_Bit(code, 0));
}

/***********************************************************************
** 函 数 名： Hundred_Display(uint8_t hundred)
** 函数说明： 数码管999
**---------------------------------------------------------------------
** 输入参数： number，亮度
** 返回参数： 无
***********************************************************************/
void Hundred_Display(uint32_t hundred, uint16_t brightness)
{
	while(hundred<601)
	{
		uint8_t display_codes[10] = {0x22, 0xEE, 0x34, 0xA4, 0xE8, 0xA1, 0x21, 0xE6, 0x20, 0xA0};
		for(int i=0;i<11;i++)
		{
			Refresh_Display(display_codes[hundred/100], 0, 2, brightness);//hundred/100
			HAL_Delay(2);
			Refresh_Display(display_codes[(hundred/10)%10], 1, 1, brightness);//(hundred/10)%10
			HAL_Delay(2);
			Refresh_Display(display_codes[hundred%10], 0, 0, brightness);//hundred%10
			HAL_Delay(2);
		}
		hundred++;
	}
	hundred=0;
}

void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness)
{
  code &= ~(dot << 5);
  if(id == 2)
  {
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 1000 - brightness);
  }
//  else
//  {
//    htim1.Instance->CNT = 996;
//    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 1000);

//  }
  if(id == 1)
  {
    __HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, brightness);
  }
//  else
//  {
//    __HAL_TIM_SetCompare(&htim17, TIM_CHANNEL_1, 0);

//  }

  if(id == 0)
  {
    __HAL_TIM_SetCompare(&htim16, TIM_CHANNEL_1, brightness);

  }
//  else
//  {
//    __HAL_TIM_SetCompare(&htim16, TIM_CHANNEL_1, 0);
//  }
  HAL_GPIO_WritePin( Display_A_GPIO_Port,  Display_A_Pin, Get_Bit(code, 7));//B9		E  
  HAL_GPIO_WritePin( Display_B_GPIO_Port,  Display_B_Pin, Get_Bit(code, 6));//B8		D
  HAL_GPIO_WritePin( Display_C_GPIO_Port,  Display_C_Pin, Get_Bit(code, 5));//B7		DP
  HAL_GPIO_WritePin( Display_D_GPIO_Port,  Display_D_Pin, Get_Bit(code, 4));//B6		C
  HAL_GPIO_WritePin( Display_E_GPIO_Port,  Display_E_Pin, Get_Bit(code, 3));//B5		A
  HAL_GPIO_WritePin( Display_F_GPIO_Port,  Display_F_Pin, Get_Bit(code, 2));//B4		F
  HAL_GPIO_WritePin( Display_G_GPIO_Port,  Display_G_Pin, Get_Bit(code, 1));//B3		G
	HAL_GPIO_WritePin( Display_DP_GPIO_Port,  Display_DP_Pin, Get_Bit(code, 0));//D3  B
}

