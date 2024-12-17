/***************************************************************************
**   					             ������ͨ��ѧ RealSteelս��
**   					               ������������۵糸!
**-------------------------------------------------------------------------
** ��    Ŀ��   STM32G0_Board
** �� �� ����   test.c
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
#include "music_note.h"


#define MAX_PSC             500			//��Ƶϵ��
#define MAX_BUZZER_PWM      2500		//����ֵ����
#define MIN_BUZZER_PWM      500			//����ֵ����

//����PWM��������������Ķ�ʱ��
#define NOTE_PWM_TIM         htim1
//PWM�����ͨ��
#define NOTE_PWM_CHANNEL     TIM_CHANNEL_1
//��������ʱ�䶨ʱ��
#define NOTE_CONTINUE_TIM    htim16

//�������ֲ��ŵ�ȫ�ֱ���
uint8_t Music_Volume;     //����
uint8_t Music_Flag;       //�л����ֵı�־λ
uint8_t Music_Off=0;     	//����ֹͣ�ı�־λ
uint8_t Cycle_Flag=0;     //����ѭ���ı�־λ
uint8_t Music_List=0;     //�����б����
uint16_t BGM_Length;      //BGM�������ֵĳ���
Note_TypeDef *BGM_Current;      //ָ��ǰBGM��ָ��
unsigned char MoleMusicVolum;   //����,���鷶Χ1~10,1��10С

uint16_t psc = 0;
uint16_t pwm = MIN_BUZZER_PWM;
uint16_t  t_ms = 0;
uint16_t	t_led = 0;
uint16_t	t_tube = 0;
uint8_t	  alarm =0;
uint8_t	  minute =5;	//����ʱ����
uint8_t	  second =0;	//����ʱ����
uint16_t  number =0;
// ʹ��һ��16λ2���������16��led�Ƶ�״̬����λ����Ŵ��LED
uint16_t led_state = 0b0000000000000001;
int k = 0;
int iButtonCount_SW1,iButtonCount_SW2,iButtonCount_SW3;//i����int�ͱ�����ButtonCount��ʾ������������
int iButtonFlag_SW1,iButtonFlag_SW2,iButtonFlag_SW3;//i����int�ͱ�����ButtonFlag��ʾ�ذ�����־��1�������°�����0Ϊû�����°���
int g_iButtonState_SW1=0,g_iButtonState_SW2=0,g_iButtonState_SW3=0;//g��globle����ȫ�ֱ��������������ط����ã�i����int�ͱ�����ButtonState��ʾ������־��1�����£�0�����ɿ�
uint8_t Count_Down_Flag=0;


/***********************************************************************
** �� �� ���� Toggle_LED(void)
** ����˵���� ��תLED�Ƶ���״̬
**---------------------------------------------------------------------
** ��������� ��
** ���ز����� ��
***********************************************************************/
void Toggle_LED(void)
{
	HAL_GPIO_TogglePin( LED1_GPIO_Port,  LED1_Pin);			//��תLED1��ƽ
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

// �������ģ 0 1 2 3 4 5 6 7 8 9
uint8_t display_codes[10] = {0x22, 0xEE, 0x34, 0xA4, 0xE8, 0xA1, 0x21, 0xE6, 0x20, 0xA0};

// ��������ȣ�0~1000��htim3.Instance->ARR��
uint16_t display_birghness = 999;

// ���������ʾ������
uint16_t num;

/**
  * @brief ʹ�����ֲ�����
  * @param
  * @note  ʹ�ܲ�����������������
  * @retval None
  */
void enable_music_player(void)
{
  HAL_TIM_Base_Start_IT(&NOTE_CONTINUE_TIM);
  HAL_TIM_PWM_Start(&NOTE_PWM_TIM,NOTE_PWM_CHANNEL);
}

/**
  * @brief ����1�����������������÷�������������
  * @param ������Ƶ�ʣ�������
  * @note  �������鷶Χ1~10��1�����10�����������ˡ�
  *        NOTE_PWM_TIM���������PWM�Ķ�ʱ��
  *        NOTE_PWM_CHANNEL�Ǹö�ʱ�����������PWM��ͨ��
  * @retval None
  */
void play_a_note(uint16_t tone,uint8_t volumeLevel)
{
	uint16_t autoReload;
	if((tone<20)||(tone>20000))//̫����̫�ߵ�Ƶ�ʶ���������
	{
    //�Ƚ�ֵ����Ϊ0������
		__HAL_TIM_SET_COMPARE(&NOTE_PWM_TIM,NOTE_PWM_CHANNEL,0);
		__HAL_TIM_SET_COUNTER(&NOTE_PWM_TIM,0);
	}
	else
	{
    //����Ƶ�ʼ����Զ���װֵ
		autoReload=(BEEP_TIM_CLOCK/tone)-1;
    //�����Զ���װֵ
		__HAL_TIM_SET_AUTORELOAD(&NOTE_PWM_TIM,autoReload);
    //���Զ���װֵ���ƣ��ɱ���С����Ϊ�Ƚ�ֵ
		__HAL_TIM_SET_COMPARE(&NOTE_PWM_TIM,NOTE_PWM_CHANNEL,autoReload>>volumeLevel);
    //�ڲ�ʹ�û��������£�����Ѽ���ֵ����
		__HAL_TIM_SET_COUNTER(&NOTE_PWM_TIM,0);
	}
}

/**
  * @brief �������ķ�ʽ������ֻ�ϻ�
  * @param ����
  * @note  �������鷶Χ1~10��1�����10������������
  * @retval None
  */
void play_music_block(uint8_t volume_level)
{
  //��ֻ�ϻ�����
  const Note_TypeDef TwoTigersMusic[]=
  {
    {0,37},//��һ��Ԫ�ص�ʱ��ֵΪ���鳤�� �������������鳤��С1
    {CM1,40},{CM2,40},{CM3,40},{CM1,40},
    {CM1,40},{CM2,40},{CM3,40},{CM1,40},
    {CM3,40},{CM4,40},{CM5,40},{0,40},
    {CM3,40},{CM4,40},{CM5,40},{0,40},
    {CM5,20},{CM6,20},{CM5,20},{CM4,20},{CM3,40},{CM1,40},
    {CM5,20},{CM6,20},{CM5,20},{CM4,20},{CM3,40},{CM1,40},
    {CM1,40},{CL5,40},{CM1,40},{0,40},
    {CM1,40},{CL5,40},{CM1,40},{0,40},
  };

	int i = 1 ;
  int length = TwoTigersMusic[0].time;
	while(i < length) //i<length��ʾ��ǰ���ֻ�û�в�����
	{
	  //�������������ó�һ������������
    play_a_note(TwoTigersMusic[i].tone,volume_level);
    //���������е�ĳ�������ĳ���ʱ���������ʽ��ʱ
    HAL_Delay(TwoTigersMusic[i].time * 10);
    i++;
	}
}

/**
  * @brief ���ű�������
  * @param ��������������
  * @note  �������鷶Χ1~10��1�����10������������
  *        �˺��������ڱ��Դ�ļ��е���
  * @retval None
  */
static void play_BGM(uint8_t volume_level,Note_TypeDef* BGM)
{
  Music_Flag = MUSIC_BEGIN;    //���ø���BGM��־
  BGM_Length = BGM[0].time;
  BGM_Current = BGM;
  Music_Volume = volume_level;
	//printf("���ű������֡�\n");
}

/**
  * @brief �������ֵĴ�����
  * @param None
  * @note  ����ȫ�ֱ�־λMusic_Flag��״̬���ж��Ƿ���Ҫֹͣ��������
  * @retval None
  */
static void BGM_handler(void)
{
	//��̬����������ִ����Ҳ�����ͷ�
  static uint16_t i = 0;//��������ʾ���׵ĵڼ�������
	static uint16_t passed_time = 0;//��ǰ�����Ѿ������˶��ٱ�10�����ʱ��
	static uint16_t curretnt_note_time = 0; //��ǰ������Ҫ�������ٱ�10�����ʱ��
	passed_time++;//ÿ�ε��ô˺�������˱���+1����˵���Ѿ�����10����
	//�����ǰ������û��������ϣ�����Ҫ���κβ������������༴��
	if(passed_time >= curretnt_note_time) //�����ǰ�����������
	{
		if(Music_Flag == MUSIC_BEGIN)//�����һ�β���BGM�����BGM��������ͷ��ʼ
		{
			i = 1;
			Music_Flag = MUSIC_CONTINUE;
		}

		if(i < BGM_Length) //��ǰBGMδ�����꣬��������һ������
		{
			play_a_note(BGM_Current[i].tone,Music_Volume);
			curretnt_note_time = BGM_Current[i].time; //���õ�ǰ��������ʱ��
			i++;
		}
		else//�������
		{
			i = 1;
			if(!Cycle_Flag)
				Music_Flag = MUSIC_FINISH;
		}
		passed_time = 0;//ʱ������
	}
}


/**
  * @brief �������ֲ��Żص��������ڶ�ʱ�����жϺ����У�ÿ��10ms����һ��
  * @param
  * @note  ����ȫ�ֱ�־λMusic_Flag��״̬���ж��Ƿ���Ҫֹͣ��������
  * @retval None
  */
void music_callback(void)
{
	if((Music_Flag == MUSIC_BEGIN)||(Music_Flag == MUSIC_CONTINUE))
	 BGM_handler();
 else//���ž�ֹ����
	 play_a_note(0,Music_Volume);
}

/**
  * @brief ���ֲ�����
  * @param ���������ֵ����
  * @note  �������鷶Χ1~10��1�����10������������
  * @retval None
  */
void music_player(uint8_t volume_level , enum MusicList num)
{
	switch (num)
	{
		case CREATE_MUSIC:play_BGM(volume_level,(Note_TypeDef *)CreatMusic);break;
		case MISS_MUSIC:play_BGM(volume_level,(Note_TypeDef *)MissMusic);break;
		case TWO_TIGERS_MUSIC:play_BGM(volume_level,(Note_TypeDef *)TwoTigersMusic);break;
		case PEPPA_PIG_MUSIC:play_BGM(volume_level,(Note_TypeDef *)PeppaPigMusic);break;
		case SUPER_MARIO_MUSIC:play_BGM(volume_level,(Note_TypeDef *)SuperMarioMusic);break;
		case DOU_DI_ZHU_MUSIC:play_BGM(volume_level,(Note_TypeDef *)DouDiZhuMusic);break;
		case POLICE_MUSIC:play_BGM(volume_level,(Note_TypeDef *)PoliceMusic);break;
		case AMBULANCE_MUSIC:play_BGM(volume_level,(Note_TypeDef *)AmbulanceMusic);break;
	}
}

/***********************************************************************
** �� �� ���� setup()
** ����˵���� �����ʼ���������ϵ��ִ��һ��
**---------------------------------------------------------------------
** ��������� ��
** ���ز����� ��
***********************************************************************/
void Setup()
{
	//������ʱ���ж�
  HAL_TIM_Base_Start_IT(&htim3);

  // �������������ֹ�������
  Clear_Display();
	
	MoleMusicVolum = 6;						//����,���鷶Χ1~10,1��10С
	enable_music_player();

	//start tim
	//������ʱ��
//	HAL_TIM_Base_Start(&htim1);
	//start pwm channel
	//����PWMͨ��
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	//�رշ�����
//	buzzer_off();

	Refresh_LED(~0);

}

/***********************************************************************
** �� �� ���� Loop()
** ����˵���� ������ѭ���������ϵ��ѭ��ִ��
**---------------------------------------------------------------------
** ��������� ��
** ���ز����� ��
***********************************************************************/
void Loop()
{
	//�� �� pwm ��ֵ��MIN_BUZZER_PWM�� MAX_BUZZER_PWM֮��䶯
//	if(pwm > MAX_BUZZER_PWM)
//	{
//		pwm = MIN_BUZZER_PWM;
//	}
	//psc ��ֵ��0 �� MAX_PSC֮��䶯
//	if(psc > MAX_PSC)
//	{
//		psc = 0;
//	}

	if(t_led>61)
	{
		t_led=0;
		Refresh_LED(~led_state<<k);
		k++;
	}
	number=minute*100+second;
	if(!alarm)
		Hundred_Display(number,display_birghness);
	else
	{
		HAL_GPIO_WritePin(Display_DG1_GPIO_Port,Display_DG1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(Display_DG2_GPIO_Port,Display_DG2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(Display_DG3_GPIO_Port,Display_DG3_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin( Display_G_GPIO_Port,  Display_G_Pin, GPIO_PIN_RESET);//B3		G
		HAL_GPIO_WritePin( Display_E_GPIO_Port,  Display_E_Pin, GPIO_PIN_SET);//B9		E  
		HAL_GPIO_WritePin( Display_D_GPIO_Port,  Display_D_Pin, GPIO_PIN_SET);//B8		D
		HAL_GPIO_WritePin( Display_DP_GPIO_Port,  Display_DP_Pin, GPIO_PIN_SET);//B7		DP
		HAL_GPIO_WritePin( Display_C_GPIO_Port,  Display_C_Pin, GPIO_PIN_SET);//B6		C
		HAL_GPIO_WritePin( Display_A_GPIO_Port,  Display_A_Pin, GPIO_PIN_SET);//B5		A
		HAL_GPIO_WritePin( Display_F_GPIO_Port,  Display_F_Pin, GPIO_PIN_SET);//B4		F
		HAL_GPIO_WritePin( Display_B_GPIO_Port,  Display_B_Pin, GPIO_PIN_SET);//D3  B
		//����������
//		buzzer_on(psc, pwm);		
	}
	if(g_iButtonState_SW3)
	{
		Music_Off=!Music_Off;
		if(Music_Off)
			Music_Flag=MUSIC_FINISH;
		else
		{
			Cycle_Flag=!Cycle_Flag;
			Music_Flag=MUSIC_BEGIN;
		}

//		if(alarm)
//		{
//			buzzer_off();
//			alarm=0;
//			Count_Down_Flag=!Count_Down_Flag;
//		}
//		else
//			Count_Down_Flag=!Count_Down_Flag;
		g_iButtonState_SW3=0;
	}
	if(g_iButtonState_SW1)
	{
		if(Music_List==5)
			Music_List=0;
		else
			Music_List++;
		if(Music_List==0)
			music_player(MoleMusicVolum,POLICE_MUSIC);
		else if(Music_List==1)
			music_player(MoleMusicVolum,SUPER_MARIO_MUSIC);
		else if(Music_List==2)
			music_player(MoleMusicVolum,TWO_TIGERS_MUSIC);
		else if(Music_List==3)
			music_player(MoleMusicVolum,DOU_DI_ZHU_MUSIC);
		else if(Music_List==4)
			music_player(MoleMusicVolum,PEPPA_PIG_MUSIC);
		else
			music_player(MoleMusicVolum,AMBULANCE_MUSIC);
//		minute++;
		g_iButtonState_SW1=0;
	}
	if(g_iButtonState_SW2)
	{
		if(Music_Volume==10)
			Music_Volume=1;
		else
			Music_Volume++;
//		minute--;
		g_iButtonState_SW2=0;
	}
	
	
	
//	if(t_tube<3)
//		Refresh_Display(display_codes[1], 0, 2, display_birghness);
//	else if(t_tube<6)
//		Refresh_Display(display_codes[2], 1, 1, display_birghness);
//	else if(t_tube<9)
//		Refresh_Display(display_codes[3], 0, 0, display_birghness);
//	else
//		t_tube=0;
		
//  // �������ʾ
//  Refresh_Display(display_codes[6], 0, 2, display_birghness);
//	// �Ӿ�����
//  HAL_Delay(3);
//	Refresh_Display(display_codes[6], 0, 1, display_birghness);
//	// �Ӿ�����
//  HAL_Delay(3);
//  Refresh_Display(display_codes[6], 0, 0, display_birghness);  
//	// �Ӿ�����
//  HAL_Delay(3);

//	 // �������ʾ
//  Hundred_Display(800, display_birghness);
//	// �Ӿ�����
//  HAL_Delay(200);
//	// �������ʾ
//  Hundred_Display(000, display_birghness);
//	// �Ӿ�����
//  HAL_Delay(200);
//	// �������ʾ
//  Hundred_Display(666, display_birghness);
//	// �Ӿ�����
//  HAL_Delay(200);


	
}

/***********************************************************************
** �� �� ���� HAL_TIM_PeriodElapsedCallback()
** ����˵���� ��ʱ���жϻص�������������ʱ����������
**---------------------------------------------------------------------
** ��������� ��
** ���ز����� ��
***********************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim3)
	{
		t_ms++;
		t_led++;
		t_tube++;
		if(t_ms>1000)
		{
			k=0;
			t_ms=0;
			if(Count_Down_Flag)
			{
				if(second)
					second--;
				else
				{
					if(minute)
					{
						minute--;
						second=59;
					}
					else
						alarm=1;
				}
			}
		}
		if(alarm)
			psc++;
/*******************************************************************************************************************/
		if( HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == GPIO_PIN_RESET )//������ż�⵽          �͵�ƽ
		{
			iButtonCount_SW1++; //�������£�����iButtonCount��1
			if(iButtonCount_SW1>=7) //1ms�жϷ�����������һ�Σ�iButtonCount���ڵ���16�����������ȶ�����16ms
			{
				if(iButtonFlag_SW1==0) //�ж���û���ذ�����1Ϊ�У�0Ϊû��
				{
					g_iButtonState_SW1=1; //���ð�����־
					iButtonCount_SW1=0;
					iButtonFlag_SW1=1; //�����ذ�����־
				}
				else //����ذ����������¼���
					iButtonCount_SW1=0;
			}
			else //���û���ȶ�����7ms�������û�а��°���
				g_iButtonState_SW1=0;
		}
		else //���һֱ�޼�⵽�͵�ƽ����һֱ�ް�������
		{
			iButtonCount_SW1=0; //����iButtonCount
			g_iButtonState_SW1=0; //���������־
			iButtonFlag_SW1=0; //����ذ�����־
		}
		if( HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == GPIO_PIN_RESET )//������ż�⵽          �͵�ƽ
		{
			iButtonCount_SW2++; //�������£�����iButtonCount��1
			if(iButtonCount_SW2>=7) //1ms�жϷ�����������һ�Σ�iButtonCount���ڵ���16�����������ȶ�����16ms
			{
				if(iButtonFlag_SW2==0) //�ж���û���ذ�����1Ϊ�У�0Ϊû��
				{
					g_iButtonState_SW2=1; //���ð�����־
					iButtonCount_SW2=0;
					iButtonFlag_SW2=1; //�����ذ�����־
				}
				else //����ذ����������¼���
					iButtonCount_SW2=0;
			}
			else //���û���ȶ�����7ms�������û�а��°���
				g_iButtonState_SW2=0;
		}
		else //���һֱ�޼�⵽�͵�ƽ����һֱ�ް�������
		{
			iButtonCount_SW2=0; //����iButtonCount
			g_iButtonState_SW2=0; //���������־
			iButtonFlag_SW2=0; //����ذ�����־
		}
		if( HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == GPIO_PIN_RESET )//������ż�⵽          �͵�ƽ
		{
			iButtonCount_SW3++; //�������£�����iButtonCount��1
			if(iButtonCount_SW3>=7) //1ms�жϷ�����������һ�Σ�iButtonCount���ڵ���16�����������ȶ�����16ms
			{
				if(iButtonFlag_SW3==0) //�ж���û���ذ�����1Ϊ�У�0Ϊû��
				{
					g_iButtonState_SW3=1; //���ð�����־
					iButtonCount_SW3=0;
					iButtonFlag_SW3=1; //�����ذ�����־
				}
				else //����ذ����������¼���
					iButtonCount_SW3=0;
			}
			else //���û���ȶ�����7ms�������û�а��°���
				g_iButtonState_SW3=0;
		}
		else //���һֱ�޼�⵽�͵�ƽ����һֱ�ް�������
		{
			iButtonCount_SW3=0; //����iButtonCount
			g_iButtonState_SW3=0; //���������־
			iButtonFlag_SW3=0; //����ذ�����־
		}
	}
	if(htim == &NOTE_CONTINUE_TIM)
  {
    music_callback();
  }

	
}
