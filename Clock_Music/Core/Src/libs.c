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
#include "libs.h"


extern uint16_t t_tube;

void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness); // ˢ�������
void Clear_Display(); // ���������
void Hundred_Display(uint32_t hundred, uint16_t brightness); // �����999
void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness);
void Refresh_LED(uint16_t state); // ˢ��LED�ƣ�0��1��
void Refresh_LED_Neg(uint16_t state); // ˢ��LED�ƣ�0��1����

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
int g_iButtonStates_SW1=0,g_iButtonStates_SW2=0,g_iButtonStates_SW3=0;//g��globle����ȫ�ֱ��������������ط����ã�i����int�ͱ�����ButtonState��ʾ������־��1�����£�0�����ɿ�
int g_iButtonStatel_SW1=0,g_iButtonStatel_SW2=0,g_iButtonStatel_SW3=0;//g��globle����ȫ�ֱ��������������ط����ã�i����int�ͱ�����ButtonState��ʾ������־��1�����£�0�����ɿ�
int setting_mode=0;

uint8_t Count_Down_Flag=0;

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
//  HAL_TIM_Base_Start_IT(&NOTE_CONTINUE_TIM);
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
void play_BGM(uint8_t volume_level,Note_TypeDef* BGM)
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
void BGM_handler(void)
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
		
		case TEST_MUSIC:play_BGM(volume_level,(Note_TypeDef *)testMusic);break;
		case TEST1_MUSIC:play_BGM(volume_level,(Note_TypeDef *)test1Music);break;
		case GALAYOUMUSIC:play_BGM(volume_level,(Note_TypeDef *)GalaYouMusic);break;
		
		
	}
}




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

//void buzzer_on(uint16_t psc, uint16_t pwm)
//{
//	//���÷�Ƶ
//	__HAL_TIM_PRESCALER(&htim1, psc);
//	//���� PWM ��ռ�ձ�
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm);
//}
//void buzzer_off(void)
//{
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
//}


//test����
const Note_TypeDef testMusic[]=
{
	{0,63},//��һ��Ԫ�ص�ʱ��2ֵΪ���鳤�� �������������鳤��С1
	{CL1,100},{CL2,100},{CL3,100},{CL4,100},{CL5,100},{CL6,100},{CL7,100},
	{CM1,100},{CM2,100},{CM3,100},{CM4,100},{CM5,100},{CM6,100},{CM7,100},
	{CH1,100},{CH2,100},{CH3,100},{CH4,100},{CH5,100},{CH6,100},{CH7,100},
	{DL1,100},{DL2,100},{DL3,100},{DL4,100},{DL5,100},{DL6,100},{DL7,100},
	{DM1,100},{DM2,100},{DM3,100},{DM4,100},{DM5,100},{DM6,100},{DM7,100},
	{DH1,100},{DH2,100},{DH3,100},{DH4,100},{DH5,100},{DH6,100},{DH7,100},
	{EL1,100},{EL2,100},{EL3,100},{EL4,100},{EL5,100},{EL6,100},{EL65,100},{EL7,100},
	{EM1,100},{EM2,100},{EM3,100},{EM4,100},{EM5,100},{EM6,100},{EM7,100},
	{EH1,100},{EH2,100},{EH3,100},{EH4,100},{EH5,100},
	{FL1,100},{FL2,100},{FL3,100},{FL4,100},{FL5,100},{FL6,100},{FL7,100},
	{FM1,100},{FM2,100},{FM3,100},{FM4,100},{FM5,100},{FM6,100},{FM7,100},
	{FH1,100},{FH2,100},{FH3,100},{FH4,100}

};

//test1����
const Note_TypeDef test1Music[]=
{
	{0,33},//��һ��Ԫ�ص�ʱ��ֵΪ���鳤�� �������������鳤��С1
	{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,50},
	{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,50},
	{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,50},
	{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,50}
};

////���㡷-GALA ���ף������棩
//const Note_TypeDef GalaYouMusic[] =
//{
//    {0, 567,},// �������� = 566�����鳤�� = 566 + 1 = 567
//    // ǰ��
//    {CL7, 20}, {CL5, 20}, {CL7, 40}, {CL5, 20}, {CL7, 40}, {CL5, 20}, {DL5, 40}, {CL5, 20}, // 1-8
//    {CM1, 20}, {CM1, 20}, {CL5, 20}, {CL7, 20}, {CM1, 20}, {CL7, 20}, {CL5, 20}, {CL7, 40}, // 9-16
//    {CL5, 20}, {CL7, 40}, {CL5, 20}, {DL5, 40}, {CL5, 20}, {CM1, 20}, {CM1, 20}, {CL5, 20}, // 17-24
//    {CL7, 20}, {CM1, 20}, {CL7, 20}, {CL5, 20}, {CL7, 40}, {CL5, 20}, {CL7, 40}, {CL5, 20}, // 25-32
//    {DL5, 40}, {CL5, 20}, {CM1, 20}, {CM1, 20}, {CL5, 20}, {DM1, 40}, {EM1, 120}, {DM1, 40}, // 33-40
//    {EM1, 40}, {DM1, 40}, {EM1, 40}, {CM1, 160}, {CM1, 80}, {CL7, 80}, {EM1, 80}, {DM1, 40}, // 41-48
//    {EM1, 120}, {DM1, 40}, {EM1, 40}, {DM1, 40}, {EM1, 40}, {CM1, 160}, {CL7, 40}, {DL5, 80}, // 49-56
//    {EM1, 200}, {DM1, 40}, {EM1, 40}, {DM1, 40}, {EM1, 40}, {CL7, 80}, {CM1, 80}, {CL7, 80}, // 57-64
//    {EM1, 80}, {DM1, 40}, {EM1, 120}, {DM1, 40}, {EM1, 40}, {DM1, 40}, {EM1, 40}, {CL7, 80}, // 65-72
//    {CM1, 80}, {DL5, 200}, {0, 80}, {EM1, 40}, {0, 4}, {EM1, 40}, {DM1, 20}, {EM1, 60}, // 73-80
//    {DM1, 40}, {EM1, 40}, {CL5, 40}, {0, 40}, {EM1, 40}, {0, 4}, {EM1, 40}, {DM1, 20}, // 81-88
//    {EM1, 60}, {DM1, 40}, {EM1, 40}, {CL5, 80}, {CL1, 120}, {0, 40}, {EM1, 40}, {0, 4}, // 89-96
//    {EM1, 40}, {DM1, 20}, {EM1, 60}, {DM1, 40}, {CL1, 40}, {DM1, 40}, {0, 40}, {EM1, 40}, // 97-104
//    {0, 4}, {EM1, 40}, {DM1, 20}, {EM1, 60}, {DM1, 40}, {EM1, 40}, {CM1, 80}, {CL1, 120}, // 105-112
//    {0, 40}, {EM1, 40}, {0, 4}, {EM1, 40}, {DM1, 20}, {EM1, 60}, {DM1, 40}, {CL7, 40}, // 113-120
//    {CL6, 20}, {CL3, 100}, {0, 20}, {CL3, 20}, {0, 4}, {CL3, 20}, {CL5, 80}, {EM1, 60}, // 121-128
//    {DM1, 20}, {CL1, 40}, {0, 2}, {CL1, 80}, {0, 40}, {CL6, 40}, {EM1, 40}, {FL1, 40}, // 129-136
//    {CL5, 120}, {CL1, 40}, {DM1, 40}, {EM1, 40}, {CL5, 80}, {CL6, 100}, {CL5, 20}, {CL6, 60}, // 137-144
//    {CL5, 20}, {0, 2}, {CL5, 60}, {0, 2}, {CL5, 80}, {DM1, 160}, {EM1, 80}, {FL1, 80}, // 145-152
//    {CL5, 120}, {CL1, 40}, {DM1, 40}, {EM1, 40}, {CM1, 80}, {CL6, 120}, {0, 4}, {CL6, 40}, // 153-160
//    {CL5, 20}, {CL6, 60}, {CL5, 20}, {0, 2}, {CL5, 60}, {0, 2}, {CL5, 80}, {DM1, 160}, // 161-168
//    {EM1, 80}, {FL1, 80}, {CL5, 120}, {CL1, 40}, {DM1, 40}, {EM1, 40}, {CM1, 80}, {DL5, 120}, // 169-176
//    {0, 40}, {CL5, 40}, {CM1, 40}, {CL7, 20}, {CM1, 240}, {0, 80}, {EM1, 80}, {DM1, 80}, // 177-184
//    {CL1, 80}, {CL5, 80}, {CL1, 80}, {DM1, 80}, {EM1, 80}, {FL1, 80}, {CL5, 80}, {FL1, 80}, // 185-192
//    {EM1, 80}, {DM1, 80}, {CL1, 80}, {DM1, 80}, {EM1, 80}, {FL1, 80}, {EM1, 80}, {DM1, 80}, // 193-200
//    {CL1, 80}, {CL5, 80}, {CL1, 80}, {DM1, 80}, {EM1, 80}, {FL1, 80}, {EM1, 80}, {DM1, 80}, // 201-208
//};
// ���㡷-GALA ���ף�����ռ������о�ȷ��䣩
const Note_TypeDef GalaYouMusic[] = {
//    {0, 390}, // ����ͳ����������767��ʵ���辫׼ͳ�ƺ��޸�
//    // ǰ�ಿ��
//			
//			
////		{note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20},
////		{note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_5B, 20}, {note_5C, 20},
////		{note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20},
////		{note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_5B, 20}, {note_5C, 20},
////		{note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20},
////		{note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_5B, 20}, {note_5C, 20},
//		
////		{AM7, 80}, {AM5, 80}, {AH2, 80}, {AH1, 80},
////		{AM7, 80}, {AM5, 80}, {AH2, 80}, {AH1, 80},
////		{AM7, 80}, {AM5, 80}, {AH2, 80}, {AH1, 80},
////		{AM7, 80}, {AM5, 80}, {AH2, 80}, {AH1, 80},
//		
//    
//		{note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_5B, 20}, {note_5C, 20},
//		{note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_5B, 20}, {note_5C, 20},
//		{note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_5B, 20}, {note_5C, 20},
//		{note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_D, 40},
//		{note_E, 120}, {note_D, 40}, {note_E, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80},
//		{note_5C, 80}, {note_5B, 80}, {note_E, 80}, {note_D, 40},
//		{note_E, 120}, {note_D, 40}, {note_E, 40}, {note_D, 40}, {note_E, 40}, {note_5C, 160},
//		{note_5B, 40}, {note_5D, 80}, {note_E, 200},
//		{note_D, 40}, {note_E, 40}, {note_D, 40}, {note_E, 40}, {note_5B, 80},
//		{note_5C, 80}, {note_5B, 80}, {note_E, 80}, {note_D, 40},
//		{note_E, 120}, {note_D, 40}, {note_E, 40}, {note_D, 40}, {note_E, 40}, {note_5B, 80},
//		{note_5C, 80}, {note_5D, 200},
//		{0, 80}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 40},
//		{0, 40}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_C, 120},
//		{0, 40}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_C, 40}, {note_D, 40},
//		{0, 40}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 40},
//		{0, 40}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_C, 120},
//		{0, 40}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_B, 40}, {note_A, 20}, {note_3G, 100},
//		{0, 20}, {note_3G, 20}, {0, 1}, {note_3G, 20}, {0, 1}, {note_3G, 20}, {note_G, 80}, {note_E, 60}, {note_D, 20}, {note_C, 40}, {0, 1}, {note_C, 80},
//		{0, 20}, {note_C, 40}, {0, 1}, {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_A, 120},
//		{0, 40}, {note_A, 40}, {note_E, 40}, {note_D, 40}, {note_C, 40}, {note_D, 120},
//		{note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
//		{note_C, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_5A, 80}, {note_G, 20}, {note_5A, 60}, {note_G, 20}, {0, 2}, {note_G, 60}, {0, 2}, {note_G, 80}, {note_D, 160},
//		{note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
//		{note_C, 40}, {note_D, 40}, {note_E, 40}, {note_5B, 80}, {note_5A, 120}, {0, 1}, {note_5A, 40}, {note_G, 20}, {note_5A, 60}, {note_5C, 80}, {note_5D, 120},
//		{0, 40}, {note_G, 40}, {note_5C, 40}, {note_5B, 20}, {note_5C, 240},
//		{0, 80},
//		{0, 40}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 40},
//		{0, 40}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_C, 120},
//		{0, 40}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_C, 40}, {note_D, 40},
//		{0, 40}, {note_E, 40}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 40},
//		{0, 40}, {note_E, 40}, {0, 1}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_C, 120},
//		{0, 40}, {note_E, 40}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_B, 40}, {note_A, 20}, {note_3G, 100},
//		{0, 20}, {note_3G, 20}, {0, 1}, {note_3G, 20}, {0, 1}, {note_3G, 20}, {note_G, 80}, {note_E, 60}, {note_D, 20}, {note_C, 40}, {0, 1}, {note_C, 80},
//		{0, 20}, {note_C, 40}, {0, 1}, {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_A, 120},
//		{0, 40}, {note_A, 40}, {note_E, 40}, {note_D, 40}, {note_C, 40}, {note_D, 200},
//		{0, 80}, {note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
//		{note_C, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_5A, 100}, {note_G, 20}, {note_5A, 40}, {note_G, 20}, {0, 1}, {note_G, 60}, {0, 2}, {note_D, 40}, {0, 2}, {note_D, 160},
//		{note_E, 80}, {note_F, 80}, {note_G, 80}, {0, 10}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5D, 80}, {note_5C, 120},
//		{note_C, 40}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_5A, 100},
//		{note_G, 20}, {note_5A, 40}, {note_G, 20}, {note_5A, 60}, {note_5C, 80}, {note_5D, 120},
//		{0, 40}, {note_D, 40}, {note_E, 40}, {note_D, 20}, {note_C, 240},
//		{0, 80}, {note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
//		{note_C, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_5A, 100}, {note_G, 20}, {note_5A, 40}, {note_G, 20}, {0, 2}, {note_G, 60}, {0, 2}, {note_G, 80}, {note_D, 160},
//		{note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
//		{note_C, 40}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_5A, 120}, {0, 1}, {note_5A, 40}, {note_G, 20}, {note_5A, 60}, {note_5C, 80}, {note_5D, 120},
//		{note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
//		{note_C, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_5A, 100}, {note_G, 20}, {note_5A, 40}, {note_G, 20}, {0, 2}, {note_G, 60}, {0, 2}, {note_G, 80}, {note_D, 160},
//		{note_E, 80}, {note_F, 80}, {note_G, 80}, {0, 20}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5D, 80}, {note_5C, 120},
//		{note_C, 40}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_5A, 100},
//		{note_G, 20}, {note_5A, 40}, {note_G, 20}, {note_5A, 60}, {note_5C, 80}, {note_5D, 120},
//		{0, 40}, {note_G, 40}, {note_5C, 40}, {note_5B, 20}, {note_5C, 240},
//		{0, 80}, {note_E, 80}, {note_D, 80}, {note_C, 80}, {note_G, 80}, {note_C, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80}, {note_G, 80}, {note_F, 80},
//		{note_E, 80}, {note_D, 80}, {note_C, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80}, {note_E, 80}, {note_D, 80}, {note_C, 80}, {note_G, 80},
//		{note_E, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80}, {note_G, 80}, {note_F, 80}, {note_E, 80}, {note_D, 80},
//		{note_E, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80},
//		{note_E, 80}, {note_D, 80}, {note_C, 80}, {note_G, 80}, {note_C, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80}, {note_G, 80}, {note_F, 80}, {note_E, 80}, {note_D, 80},



		{0, 481},
    {note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_5B, 20}, {note_5C, 20},
    {note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_5B, 20}, {note_5C, 20},
    {note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_5B, 20}, {note_5C, 20},
    {note_5B, 20}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5B, 40}, {note_G, 20}, {note_5D, 40}, {note_G, 20}, {note_5C, 20}, {note_5C, 20}, {note_G, 20}, {note_D, 40},
    {note_E, 120}, {note_D, 40}, {note_E, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80},
    {note_5C, 80}, {note_5B, 80}, {note_E, 80}, {note_D, 40},
    {note_E, 120}, {note_D, 40}, {note_E, 40}, {note_D, 40}, {note_E, 40}, {note_5C, 160},
    {note_5B, 40}, {note_5D, 80}, {note_E, 200},
    {note_D, 40}, {note_E, 40}, {note_D, 40}, {note_E, 40}, {note_5B, 80},
    {note_5C, 80}, {note_5B, 80}, {note_E, 80}, {note_D, 40},
    {note_E, 120}, {note_D, 40}, {note_E, 40}, {note_D, 40}, {note_E, 40}, {note_5B, 80},
    {note_5C, 80}, {note_5D, 200},
    {0, 80}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 40},
    {0, 40}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_C, 120},
    {0, 40}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_C, 40}, {note_D, 40},
    {0, 40}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 40},
    {0, 40}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_C, 120},
    {0, 40}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_B, 40}, {note_A, 20}, {note_3G, 100},
    {0, 20}, {note_3G, 20}, {0, 4}, {note_3G, 20}, {0, 4}, {note_3G, 20}, {note_G, 80}, {note_E, 60}, {note_D, 20}, {note_C, 40}, {0, 4}, {note_C, 80},
    {0, 20}, {note_C, 40}, {0, 4}, {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_A, 120},
    {0, 40}, {note_A, 40}, {note_E, 40}, {note_D, 40}, {note_C, 40}, {note_D, 120},
    {note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
    {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_5A, 80}, {note_G, 20}, {note_5A, 60}, {note_G, 20}, {0, 2}, {note_G, 60}, {0, 2}, {note_G, 80}, {note_D, 160},
    {note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
    {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_5B, 80}, {note_5A, 120}, {0, 4}, {note_5A, 40}, {note_G, 20}, {note_5A, 60}, {note_5C, 80}, {note_5D, 120},
    {0, 40}, {note_G, 40}, {note_5C, 40}, {note_5B, 20}, {note_5C, 240},
    {0, 80},
    {0, 40}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 40},
    {0, 40}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_C, 120},
    {0, 40}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_C, 40}, {note_D, 40},
    {0, 40}, {note_E, 40}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_G, 40},
    {0, 40}, {note_E, 40}, {0, 4}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_C, 120},
    {0, 40}, {note_E, 40}, {note_E, 40}, {note_D, 20}, {note_E, 60}, {note_D, 40}, {note_B, 40}, {note_A, 20}, {note_3G, 100},
    {0, 20}, {note_3G, 20}, {0, 4}, {note_3G, 20}, {0, 4}, {note_3G, 20}, {note_G, 80}, {note_E, 60}, {note_D, 20}, {note_C, 40}, {0, 4}, {note_C, 80},
    {0, 20}, {note_C, 40}, {0, 4}, {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_A, 120},
    {0, 40}, {note_A, 40}, {note_E, 40}, {note_D, 40}, {note_C, 40}, {note_D, 200},
    {0, 80}, {note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
    {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_5A, 100}, {note_G, 20}, {note_5A, 40}, {note_G, 20}, {0, 1}, {note_G, 60}, {0, 2}, {note_D, 40}, {0, 2}, {note_D, 160},
    {note_E, 80}, {note_F, 80}, {note_G, 80}, {0, 10}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5D, 80}, {note_5C, 120},
    {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_5A, 100},
    {note_G, 20}, {note_5A, 40}, {note_G, 20}, {note_5A, 60}, {note_5C, 80}, {note_5D, 120},
    {0, 40}, {note_D, 40}, {note_E, 40}, {note_D, 20}, {note_C, 240},
    {0, 80}, {note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
    {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_5A, 100}, {note_G, 20}, {note_5A, 40}, {note_G, 20}, {0, 2}, {note_G, 60}, {0, 2}, {note_G, 80}, {note_D, 160},
    {note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
    {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_5A, 120}, {0, 4}, {note_5A, 40}, {note_G, 20}, {note_5A, 60}, {note_5C, 80}, {note_5D, 120},
    {note_E, 80}, {note_F, 80}, {note_G, 120}, {note_E, 40}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5B, 80}, {note_5C, 120},
    {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_G, 80}, {note_5A, 100}, {note_G, 20}, {note_5A, 40}, {note_G, 20}, {0, 2}, {note_G, 60}, {0, 2}, {note_G, 80}, {note_D, 160},
    {note_E, 80}, {note_F, 80}, {note_G, 80}, {0, 20}, {note_G, 40}, {note_E, 20}, {note_G, 60}, {note_5D, 80}, {note_5C, 120},
    {note_C, 40}, {note_D, 40}, {note_E, 40}, {note_5C, 80}, {note_5A, 100},
    {note_G, 20}, {note_5A, 40}, {note_G, 20}, {note_5A, 60}, {note_5C, 80}, {note_5D, 120},
    {0, 40}, {note_G, 40}, {note_5C, 40}, {note_5B, 20}, {note_5C, 240},
    {0, 80}, {note_E, 80}, {note_D, 80}, {note_C, 80}, {note_G, 80}, {note_C, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80}, {note_G, 80}, {note_F, 80},
    {note_E, 80}, {note_D, 80}, {note_C, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80}, {note_E, 80}, {note_D, 80}, {note_C, 80}, {note_G, 80},
    {note_E, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80}, {note_G, 80}, {note_F, 80}, {note_E, 80}, {note_D, 80},
    {note_E, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80},
    {note_E, 80}, {note_D, 80}, {note_C, 80}, {note_G, 80}, {note_C, 80}, {note_D, 80}, {note_E, 80}, {note_F, 80}, {note_G, 80}, {note_F, 80}, {note_E, 80}, {note_D, 80}



};
//δ��������
const Note_TypeDef MissMusic[]=
{
	{0,4},//��һ��Ԫ�ص�ʱ��ֵΪ���鳤�� �������������鳤��С1
	{CM3,15},{CM2,15},{CM1,15},{0,30},
};

//�����µ�������
const Note_TypeDef CreatMusic[]=
{
	{0,4},//��һ��Ԫ�ص�ʱ��ֵΪ���鳤�� �������������鳤��С1
	{CH1,15},{CH2,15},{CH3,15},{0,30},
};

//˫��ת����������
const Note_TypeDef PoliceMusic[]=
{
	{0,40},//��һ��Ԫ�ص�ʱ��ֵΪ���鳤�� �������������鳤��С1
	{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},
	{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},
	{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},
	{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},
};

//˫��ת���Ȼ�������
const Note_TypeDef AmbulanceMusic[]=
{
	{0,40},//��һ��Ԫ�ص�ʱ��ֵΪ���鳤�� �������������鳤��С1
	{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},
	{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},
	{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},
	{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},
};

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

//С����������
const Note_TypeDef PeppaPigMusic[]=
{
  {0,13},//��һ��Ԫ�ص�ʱ��ֵΪ���鳤�� �������������鳤��С1
	{CH5,25},{CH3,13},{CH1,13},{CH2,25},{CM5,25},
	{CM5,13},{CM7,13},{CH2,13},{CH4,13},{CH3,25},{CH1,25},{0,25},
};

//������������
const Note_TypeDef SuperMarioMusic[]=
{
	{0,396},//��һ��Ԫ�ص�ʱ��ֵΪ���鳤�� �������������鳤��С1
  {CH3,15}, {CH3,30}, {CH3,15}, {0,15}, {CH1,15}, {CH3,30}, {CH5,30}, {0,30}, {CM5,30}, {0,30},  //10
  {CH1,30}, {0,15}, {CM5,15}, {0,30}, {CM3,30}, {0,15}, {CM6,30}, {CM7,15},{0,15},{CM7-27,15},{CM6,30},  //11
  {CM5,30}, {CH3,30}, {CH5,30}, {CH6,30}, {CH4,15}, {CH5,15}, {0,15}, {CH3,30}, {CH1,15}, {CH2,15}, {CM7,30}, {0,15}, //12

  {CH1,30}, {0,15}, {CM5,15}, {0,30}, {CM3,30}, {0,15}, {CM6,30}, {CM7,15},{0,15},{CM7-27,15},{CM6,30},  //11
  {CM5,30}, {CH3,30}, {CH5,30}, {CH6,30}, {CH4,15}, {CH5,15}, {0,15}, {CH3,30}, {CH1,15}, {CH2,15}, {CM7,30}, {0,15}, //12
  {0,30}, {CH5,15},  {CH4+85,15}, {CH4,15}, {CH3-67,30}, {CH3,15}, {0,15}, {CM5+48,15}, {CM6,15}, {CH1,15}, {0,15}, {CM6,15},{CH1,15},{CH2,15}, //14

  {0,30}, {CH5,15},  {CH4+85,15}, {CH4,15}, {CH3-67,30}, {CH3,15}, {0,15}, {CM1*4,30}, {CM1*4,15}, {CM1*4,30}, {0,30}, //11
  {0,30}, {CH5,15},  {CH4+85,15}, {CH4,15}, {CH3-67,30}, {CH3,15}, {0,15}, {CM5+48,15}, {CM6,15}, {CH1,15}, {0,15}, {CM6,15},{CH1,15},{CH2,15}, //14
  {0,30}, {CH5,30},  {0,15}, {CH4,30},  {0,15}, {CH3,30},  {0,30}, {0,60},    //8

  {0,30}, {CH5,15},  {CH4+85,15}, {CH4,15}, {CH3-67,30}, {CH3,15}, {0,15}, {CM5+48,15}, {CM6,15}, {CH1,15}, {0,15}, {CM6,15},{CH1,15},{CH2,15}, //14
  {0,30}, {CH5,15},  {CH4+85,15}, {CH4,15}, {CH3-67,30}, {CH3,15}, {0,15}, {CM1*4,30}, {CM1*4,15}, {CM1*4,30}, {0,30}, //11
  {0,30}, {CH5,15},  {CH4+85,15}, {CH4,15}, {CH3-67,30}, {CH3,15}, {0,15}, {CM5+48,15}, {CM6,15}, {CH1,15}, {0,15}, {CM6,15},{CH1,15},{CH2,15}, //14

  {0,30}, {CH5,30},  {0,15}, {CH4,30},  {0,15}, {CH3,30},  {0,30}, {0,60},    //8
  {CH1,15}, {CH1,30},  {CH1,15}, {0,15}, {CH1,15}, {CH2,30},  {CH3,15}, {CH1,30},{CM6,15},{CM5,30},{0,30}, //11
  {CH1,15}, {CH1,30},  {CH1,15}, {0,15}, {CH1,15}, {CH2,15}, {CH3,15},{0,120},  //8

  {CH1,15}, {CH1,30},  {CH1,15}, {0,15}, {CH1,15}, {CH2,30},  {CH3,15}, {CH1,30},{CM6,15},{CM5,30},{0,30}, //11
  {CH3,15}, {CH3,30}, {CH3,15}, {0,15}, {CH1,15}, {CH3,30}, {CH5,30}, {0,30}, {CM5,30}, {0,30},  //10
  {CH1,30}, {0,15}, {CM5,15}, {0,30}, {CM3,30}, {0,15}, {CM6,30}, {CM7,15},{0,15},{CM7-27,15},{CM6,30},  //11

  {CM5,30}, {CH3,30}, {CH5,30}, {CH6,30}, {CH4,15}, {CH5,15}, {0,15}, {CH3,30}, {CH1,15}, {CH2,15}, {CM7,30}, {0,15}, //12
  {CH1,30}, {0,15}, {CM5,15}, {0,30}, {CM3,30}, {0,15}, {CM6,30}, {CM7,15},{0,15},{CM7-27,15},{CM6,30},  //11
  {CM5,30}, {CH3,30}, {CH5,30}, {CH6,30}, {CH4,15}, {CH5,15}, {0,15}, {CH3,30}, {CH1,15}, {CH2,15}, {CM7,30}, {0,15}, //12


  {CH3,15}, {CH1,30}, {CM5,15},{0,30},{CM5+48,30},{CM6,15},{CH4,30},{CH4,15},{CM6,30},{0,30},  //10
  {CM7,30},{CH6,30},{CH6,30},{CH6,30},{CH5,30},{CH4,30},{CH3,15},{CH1,30},{CM6,15},{CM5,30},{0,30},  //10
  {CH3,15}, {CH1,30}, {CM5,15},{0,30},{CM5+48,30},{CM6,15},{CH4,30},{CH4,15},{CM6,30},{0,30},  //10

  {CM7,15},{CH4,30},{CH4,15},{CH4,30},{CH3,30},{CH2,30},{CH1,30},{0,30},{0,60},  //9
  {CH3,15}, {CH1,30}, {CM5,15},{0,30},{CM5+48,30},{CM6,15},{CH4,30},{CH4,15},{CM6,30},{0,30},  //10
  {CM7,30},{CH6,30},{CH6,30},{CH6,30},{CH5,30},{CH4,30},{CH3,15},{CH1,30},{CM6,15},{CM5,30},{0,30},  //10

  {CH3,15}, {CH1,30}, {CM5,15},{0,30},{CM5+48,30},{CM6,15},{CH4,30},{CH4,15},{CM6,30},{0,30},  //10
  {CM7,15},{CH4,30},{CH4,15},{CH4,30},{CH3,30},{CH2,30},{CH1,30},{0,30},{0,60},  //9
  {CH1,15}, {CH1,30},  {CH1,15}, {0,15}, {CH1,15}, {CH2,30},  {CH3,15}, {CH1,30},{CM6,15},{CM5,30},{0,30}, //11

  {CH1,15}, {CH1,30},  {CH1,15}, {0,15}, {CH1,15}, {CH2,15}, {CH3,15},{0,120},  //8
  {CH1,15}, {CH1,30},  {CH1,15}, {0,15}, {CH1,15}, {CH2,30},  {CH3,15}, {CH1,30},{CM6,15},{CM5,30},{0,30}, //11
  {CH3,15}, {CH3,30}, {CH3,15}, {0,15}, {CH1,15}, {CH3,30}, {CH5,30}, {0,30}, {CM5,30}, {0,30},  //10

  {CH3,15}, {CH1,30}, {CM5,15},{0,30},{CM5+48,30},{CM6,15},{CH4,30},{CH4,15},{CM6,30},{0,30},  //10
  {CM7,30},{CH6,30},{CH6,30},{CH6,30},{CH5,30},{CH4,30},{CH3,15},{CH1,30},{CM6,15},{CM5,30},{0,30},  //10
  {CH3,15}, {CH1,30}, {CM5,15},{0,30},{CM5+48,30},{CM6,15},{CH4,30},{CH4,15},{CM6,30},{0,30},  //10

  {CM7,15},{CH4,30},{CH4,15},{CH4,30},{CH3,30},{CH2,30},{CH1,30},{0,30},{0,60},  //9

  {0,30},
};
//���ֶ���������
const Note_TypeDef DouDiZhuMusic[]=
{
	{0,82},//��һ��Ԫ�ص�ʱ��ֵΪ���鳤�� �������������鳤��С1
  {CM3,60},{CM3,30},{CM2,30},{CM1,60},{CM1,30},{CL6,30},  //6
  {CM2,30},{CM3,30},{CM2,30},{CM3,30},{CL5,60},{CL5,60},   //6
  {CL6,60}, {CL6,30}, {CL5,30}, {CL6,60}, {CM1,60},            //5
  {CM5,30},{CM6,30},{CM3,30},{CM5,30},{CM2,60},{CM2,60},   //6

  {CM3,60},{CM3,30},{CM2,30},{CM3,60},{CM5,60},     //5
  {CM6,30},{CM6,30},{CM6,30},{CH1,30},{CM6,60},{CM5,30},{CM3,30},   //7
  {CM2,60},{CM2,30},{CM3,30},{CM5,60},{CL5,60},     //5
  {CM2,30},{CM3,30},{CM2,30},{CM3,30},{CM1,60},{CM1,60},   //6

  {CM3,60},{CM3,30},{CM2,30},{CM3,60},{CM5,60},     //5
  {CM6,30},{CH1,30},{CM6,30},{CM5,30},{CM6,60},{CM5,30},{CM3,30},   //7
  {CM2,60},{CM2,30},{CM3,30},{CM5,60},{CL5,60},     //5
  {CM2,30},{CM3,30},{CM2,30},{CM3,30},{CM1,60},{CM1,60},   //6

  {CM2,30},{CM2,30},{CM2,30},{CM3,30},{CM5,60},{CM5,30},{CM6,30},   //7
  {CH1,60},{CM6,60},  {CH1,60},  {CH1,60},              //4
  {0,60},
};