/***************************************************************************
**   					             ������ͨ��ѧ RealSteelս��
**   					               ������������۵糸!
**-------------------------------------------------------------------------
** ��    Ŀ��   STM32G0_Board
** �� �� ����   libs.c
** �ļ�˵����   �̳̲��Գ���
**-------------------------------------------------------------------------
**						*�޶�*
**	*�汾*							*�޸�����*							*�޸���*      			 *����*
**	 1.0							   ��ʼ�汾						     ������     	     2024-09-16
***************************************************************************/

#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "stdint.h"
#include "test.h"
#include "libs.h"

extern uint16_t t_tube;

void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness); // ˢ�������
void Clear_Display(); // ���������
void Hundred_Display(uint32_t hundred, uint16_t brightness); // �����999
void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness);
void Refresh_LED(uint16_t state); // ˢ��LED�ƣ�0��1��
void Refresh_LED_Neg(uint16_t state); // ˢ��LED�ƣ�0��1����

/***********************************************************************
** �� �� ���� Get_Bit()
** ����˵���� ��ʱ���жϻص�������������ʱ����������
**---------------------------------------------------------------------
** ��������� ��
** ���ز����� ��
***********************************************************************/
uint8_t Get_Bit(uint16_t num, uint8_t bit_id) // ȡ��num�ڶ����Ʊ�ʾ�µĵ�idλ,id��ʾ������λ��λ�ã���0��ʼ�����������0b111101�ĵ�0λ��1����1λ��0����2~5λΪ1
{
  return (num >> bit_id) & 0b01;
}

/***********************************************************************
** �� �� ���� Refresh_LED(uint16_t state)
** ����˵���� ˢ��LED�Ƶ���״̬��0��1��
**---------------------------------------------------------------------
** ��������� һ��16λ������ÿ��λ��Ӧһ��LED�Ƶ�״̬
** ���ز����� ��
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
** �� �� ���� Refresh_LED_Neg(uint16_t state)
** ����˵���� ˢ��LED�Ƶ���״̬��0��1����
**---------------------------------------------------------------------
** ��������� һ��16λ������ÿ��λ��Ӧһ��LED�Ƶ�״̬
** ���ز����� ��
***********************************************************************/
void Refresh_LED_Neg(uint16_t state)
{
  Refresh_LED(~state);
}

/***********************************************************************
** �� �� ���� Clear_Display()
** ����˵���� ���������
**---------------------------------------------------------------------
** ��������� ��
** ���ز����� ��
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
** �� �� ���� Refresh_Display(uint8_t code, uint8_t id)
** ����˵���� �������ʾ
**---------------------------------------------------------------------
** ��������� �������ģ�������������ڼ�λ������
** ���ز����� ��
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
** �� �� ���� Hundred_Display(uint8_t hundred)
** ����˵���� �����999
**---------------------------------------------------------------------
** ��������� number������
** ���ز����� ��
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
	//���÷�Ƶ
	__HAL_TIM_PRESCALER(&htim1, psc);
	//���� PWM ��ռ�ձ�
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm);
}
void buzzer_off(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
}

