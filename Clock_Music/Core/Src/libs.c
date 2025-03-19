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
#include "libs.h"


extern uint16_t t_tube;

void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness); // 刷新数码管
void Clear_Display(); // 数码管清屏
void Hundred_Display(uint32_t hundred, uint16_t brightness); // 数码管999
void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness);
void Refresh_LED(uint16_t state); // 刷新LED灯（0亮1灭）
void Refresh_LED_Neg(uint16_t state); // 刷新LED灯（0灭1亮）

//用于音乐播放的全局变量
uint8_t Music_Volume;     //音量
uint8_t Music_Flag;       //切换音乐的标志位
uint8_t Music_Off=0;     	//音乐停止的标志位
uint8_t Cycle_Flag=0;     //单曲循环的标志位
uint8_t Music_List=0;     //音乐列表序号
uint16_t BGM_Length;      //BGM背景音乐的长度
Note_TypeDef *BGM_Current;      //指向当前BGM的指针
unsigned char MoleMusicVolum;   //音量,建议范围1~10,1大10小


uint16_t psc = 0;
uint16_t pwm = MIN_BUZZER_PWM;
uint16_t  t_ms = 0;
uint16_t	t_led = 0;
uint16_t	t_tube = 0;
uint8_t	  alarm =0;
uint8_t	  minute =5;	//倒计时：分
uint8_t	  second =0;	//倒计时：秒
uint16_t  number =0;
// 使用一个16位2进制数存放16个led灯的状态，高位给标号大的LED
uint16_t led_state = 0b0000000000000001;
int k = 0;
int iButtonCount_SW1,iButtonCount_SW2,iButtonCount_SW3;//i代表int型变量，ButtonCount表示按键计数变量
int iButtonFlag_SW1,iButtonFlag_SW2,iButtonFlag_SW3;//i代表int型变量，ButtonFlag表示重按键标志，1代表重新按键，0为没有重新按键
int g_iButtonState_SW1=0,g_iButtonState_SW2=0,g_iButtonState_SW3=0;//g是globle代表全局变量，会在其他地方引用；i代表int型变量，ButtonState表示按键标志，1代表按下，0代表松开
int g_iButtonStates_SW1=0,g_iButtonStates_SW2=0,g_iButtonStates_SW3=0;//g是globle代表全局变量，会在其他地方引用；i代表int型变量，ButtonState表示按键标志，1代表按下，0代表松开
int g_iButtonStatel_SW1=0,g_iButtonStatel_SW2=0,g_iButtonStatel_SW3=0;//g是globle代表全局变量，会在其他地方引用；i代表int型变量，ButtonState表示按键标志，1代表按下，0代表松开
int setting_mode=0;

uint8_t Count_Down_Flag=0;

// 数码管字模 0 1 2 3 4 5 6 7 8 9
uint8_t display_codes[10] = {0x22, 0xEE, 0x34, 0xA4, 0xE8, 0xA1, 0x21, 0xE6, 0x20, 0xA0};

// 数码管亮度，0~1000（htim3.Instance->ARR）
uint16_t display_birghness = 999;

// 数码管上显示的数字
uint16_t num;

/**
  * @brief 使能音乐播放器
  * @param
  * @note  使能不代表立即播放音乐
  * @retval None
  */
void enable_music_player(void)
{
//  HAL_TIM_Base_Start_IT(&NOTE_CONTINUE_TIM);
  HAL_TIM_PWM_Start(&NOTE_PWM_TIM,NOTE_PWM_CHANNEL);
}

/**
  * @brief 播放1个音符，根据音调让蜂鸣器发出声音
  * @param 音调（频率），音量
  * @note  音量建议范围1~10，1是最大，10几乎听不清了。
  *        NOTE_PWM_TIM是用于输出PWM的定时器
  *        NOTE_PWM_CHANNEL是该定时器中用于输出PWM的通道
  * @retval None
  */
void play_a_note(uint16_t tone,uint8_t volumeLevel)
{
	uint16_t autoReload;
	if((tone<20)||(tone>20000))//太低与太高的频率都当做无声
	{
    //比较值设置为0，静音
		__HAL_TIM_SET_COMPARE(&NOTE_PWM_TIM,NOTE_PWM_CHANNEL,0);
		__HAL_TIM_SET_COUNTER(&NOTE_PWM_TIM,0);
	}
	else
	{
    //根据频率计算自动重装值
		autoReload=(BEEP_TIM_CLOCK/tone)-1;
    //设置自动重装值
		__HAL_TIM_SET_AUTORELOAD(&NOTE_PWM_TIM,autoReload);
    //将自动重装值右移，成倍变小，作为比较值
		__HAL_TIM_SET_COMPARE(&NOTE_PWM_TIM,NOTE_PWM_CHANNEL,autoReload>>volumeLevel);
    //在不使用缓冲的情况下，必须把计数值清零
		__HAL_TIM_SET_COUNTER(&NOTE_PWM_TIM,0);
	}
}

/**
  * @brief 以阻塞的方式演奏两只老虎
  * @param 音量
  * @note  音量建议范围1~10，1是最大，10几乎听不清了
  * @retval None
  */
void play_music_block(uint8_t volume_level)
{
  //两只老虎乐谱
  const Note_TypeDef TwoTigersMusic[]=
  {
    {0,37},//第一个元素的时间值为数组长度 音符数量比数组长度小1
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
	while(i < length) //i<length表示当前音乐还没有播放完
	{
	  //从乐谱数组中拿出一个音符并播放
    play_a_note(TwoTigersMusic[i].tone,volume_level);
    //根据乐谱中的某个音符的持续时间进行阻塞式延时
    HAL_Delay(TwoTigersMusic[i].time * 10);
    i++;
	}
}

/**
  * @brief 播放背景音乐
  * @param 音量，乐谱数组
  * @note  音量建议范围1~10，1是最大，10几乎听不清了
  *        此函数无需在别的源文件中调用
  * @retval None
  */
void play_BGM(uint8_t volume_level,Note_TypeDef* BGM)
{
  Music_Flag = MUSIC_BEGIN;    //设置更换BGM标志
  BGM_Length = BGM[0].time;
  BGM_Current = BGM;
  Music_Volume = volume_level;
	//printf("播放背景音乐。\n");
}

/**
  * @brief 背景音乐的处理函数
  * @param None
  * @note  根据全局标志位Music_Flag的状态，判断是否需要停止播放音乐
  * @retval None
  */
void BGM_handler(void)
{
	//静态变量，函数执行完也不会释放
  static uint16_t i = 0;//索引，表示乐谱的第几个音符
	static uint16_t passed_time = 0;//当前音符已经播放了多少倍10毫秒的时间
	static uint16_t curretnt_note_time = 0; //当前音符需要持续多少倍10毫秒的时间
	passed_time++;//每次调用此函数，则此变量+1，则说明已经过了10毫秒
	//如果当前的音符没有演奏完毕，则不需要做任何操作，继续演奏即可
	if(passed_time >= curretnt_note_time) //如果当前音符演奏完毕
	{
		if(Music_Flag == MUSIC_BEGIN)//如果第一次播放BGM或更换BGM，索引从头开始
		{
			i = 1;
			Music_Flag = MUSIC_CONTINUE;
		}

		if(i < BGM_Length) //当前BGM未演奏完，则演奏下一个音符
		{
			play_a_note(BGM_Current[i].tone,Music_Volume);
			curretnt_note_time = BGM_Current[i].time; //设置当前音符持续时间
			i++;
		}
		else//演奏结束
		{
			i = 1;
			if(!Cycle_Flag)
				Music_Flag = MUSIC_FINISH;
		}
		passed_time = 0;//时间重置
	}
}

/**
  * @brief 背景音乐播放回调函数，在定时器的中断函数中，每隔10ms调用一次
  * @param
  * @note  根据全局标志位Music_Flag的状态，判断是否需要停止播放音乐
  * @retval None
  */
void music_callback(void)
{
	if((Music_Flag == MUSIC_BEGIN)||(Music_Flag == MUSIC_CONTINUE))
	 BGM_handler();
 else//播放静止音符
	 play_a_note(0,Music_Volume);
}

/**
  * @brief 音乐播放器
  * @param 音量，音乐的序号
  * @note  音量建议范围1~10，1是最大，10几乎听不清了
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

//void buzzer_on(uint16_t psc, uint16_t pwm)
//{
//	//设置分频
//	__HAL_TIM_PRESCALER(&htim1, psc);
//	//设置 PWM 的占空比
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm);
//}
//void buzzer_off(void)
//{
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
//}


//test乐谱
const Note_TypeDef testMusic[]=
{
	{0,63},//第一个元素的时间2值为数组长度 音符数量比数组长度小1
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

//test1乐谱
const Note_TypeDef test1Music[]=
{
	{0,33},//第一个元素的时间值为数组长度 音符数量比数组长度小1
	{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,50},
	{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,50},
	{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,50},
	{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,3},{FH3,10},{0,50}
};

////《你》-GALA 乐谱（修正版）
//const Note_TypeDef GalaYouMusic[] =
//{
//    {0, 567,},// 总音符数 = 566，数组长度 = 566 + 1 = 567
//    // 前奏
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
// 《你》-GALA 乐谱（需对照简谱逐行精确填充）
const Note_TypeDef GalaYouMusic[] = {
//    {0, 390}, // 假设统计总音符数767，实际需精准统计后修改
//    // 前奏部分
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
//未击中乐谱
const Note_TypeDef MissMusic[]=
{
	{0,4},//第一个元素的时间值为数组长度 音符数量比数组长度小1
	{CM3,15},{CM2,15},{CM1,15},{0,30},
};

//生成新地鼠乐谱
const Note_TypeDef CreatMusic[]=
{
	{0,4},//第一个元素的时间值为数组长度 音符数量比数组长度小1
	{CH1,15},{CH2,15},{CH3,15},{0,30},
};

//双音转换警笛乐谱
const Note_TypeDef PoliceMusic[]=
{
	{0,40},//第一个元素的时间值为数组长度 音符数量比数组长度小1
	{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},
	{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},
	{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},
	{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},{800,25},{1000,25},
};

//双音转换救护车乐谱
const Note_TypeDef AmbulanceMusic[]=
{
	{0,40},//第一个元素的时间值为数组长度 音符数量比数组长度小1
	{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},
	{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},
	{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},
	{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},{1000,100},{800,100},
};

//两只老虎乐谱
const Note_TypeDef TwoTigersMusic[]=
{
	{0,37},//第一个元素的时间值为数组长度 音符数量比数组长度小1
	{CM1,40},{CM2,40},{CM3,40},{CM1,40},
	{CM1,40},{CM2,40},{CM3,40},{CM1,40},
	{CM3,40},{CM4,40},{CM5,40},{0,40},
	{CM3,40},{CM4,40},{CM5,40},{0,40},
	{CM5,20},{CM6,20},{CM5,20},{CM4,20},{CM3,40},{CM1,40},
	{CM5,20},{CM6,20},{CM5,20},{CM4,20},{CM3,40},{CM1,40},
	{CM1,40},{CL5,40},{CM1,40},{0,40},
	{CM1,40},{CL5,40},{CM1,40},{0,40},
};

//小猪佩奇乐谱
const Note_TypeDef PeppaPigMusic[]=
{
  {0,13},//第一个元素的时间值为数组长度 音符数量比数组长度小1
	{CH5,25},{CH3,13},{CH1,13},{CH2,25},{CM5,25},
	{CM5,13},{CM7,13},{CH2,13},{CH4,13},{CH3,25},{CH1,25},{0,25},
};

//超级玛丽乐谱
const Note_TypeDef SuperMarioMusic[]=
{
	{0,396},//第一个元素的时间值为数组长度 音符数量比数组长度小1
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
//欢乐斗地主乐谱
const Note_TypeDef DouDiZhuMusic[]=
{
	{0,82},//第一个元素的时间值为数组长度 音符数量比数组长度小1
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