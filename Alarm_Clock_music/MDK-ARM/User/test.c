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
#include "music_note.h"


#define MAX_PSC             500			//分频系数
#define MAX_BUZZER_PWM      2500		//重载值上限
#define MIN_BUZZER_PWM      500			//重载值下限

//用于PWM输出，演奏音符的定时器
#define NOTE_PWM_TIM         htim1
//PWM输出的通道
#define NOTE_PWM_CHANNEL     TIM_CHANNEL_1
//音符持续时间定时器
#define NOTE_CONTINUE_TIM    htim16

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
uint8_t Count_Down_Flag=0;


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
  HAL_TIM_Base_Start_IT(&NOTE_CONTINUE_TIM);
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
static void play_BGM(uint8_t volume_level,Note_TypeDef* BGM)
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
static void BGM_handler(void)
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
	}
}

/***********************************************************************
** 函 数 名： setup()
** 函数说明： 程序初始化函数，上电后执行一次
**---------------------------------------------------------------------
** 输入参数： 无
** 返回参数： 无
***********************************************************************/
void Setup()
{
	//开启定时器中断
  HAL_TIM_Base_Start_IT(&htim3);

  // 数码管清屏，防止意外点亮
  Clear_Display();
	
	MoleMusicVolum = 6;						//音量,建议范围1~10,1大10小
	enable_music_player();

	//start tim
	//开启定时器
//	HAL_TIM_Base_Start(&htim1);
	//start pwm channel
	//开启PWM通道
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	//关闭蜂鸣器
//	buzzer_off();

	Refresh_LED(~0);

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
	//设 置 pwm 的值在MIN_BUZZER_PWM和 MAX_BUZZER_PWM之间变动
//	if(pwm > MAX_BUZZER_PWM)
//	{
//		pwm = MIN_BUZZER_PWM;
//	}
	//psc 的值在0 和 MAX_PSC之间变动
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
		//蜂鸣器开启
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
//  Hundred_Display(000, display_birghness);
//	// 视觉暂留
//  HAL_Delay(200);
//	// 数码管显示
//  Hundred_Display(666, display_birghness);
//	// 视觉暂留
//  HAL_Delay(200);


	
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
		if( HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == GPIO_PIN_RESET )//如果引脚检测到          低电平
		{
			iButtonCount_SW1++; //按键按下，计数iButtonCount加1
			if(iButtonCount_SW1>=7) //1ms中断服务函数里运行一次，iButtonCount大于等于16，即按键已稳定按下16ms
			{
				if(iButtonFlag_SW1==0) //判断有没有重按键，1为有，0为没有
				{
					g_iButtonState_SW1=1; //设置按键标志
					iButtonCount_SW1=0;
					iButtonFlag_SW1=1; //设置重按键标志
				}
				else //如果重按键，则重新计数
					iButtonCount_SW1=0;
			}
			else //如果没有稳定按下7ms，则代表没有按下按键
				g_iButtonState_SW1=0;
		}
		else //如果一直无检测到低电平，即一直无按键按下
		{
			iButtonCount_SW1=0; //清零iButtonCount
			g_iButtonState_SW1=0; //清除按键标志
			iButtonFlag_SW1=0; //清除重按键标志
		}
		if( HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == GPIO_PIN_RESET )//如果引脚检测到          低电平
		{
			iButtonCount_SW2++; //按键按下，计数iButtonCount加1
			if(iButtonCount_SW2>=7) //1ms中断服务函数里运行一次，iButtonCount大于等于16，即按键已稳定按下16ms
			{
				if(iButtonFlag_SW2==0) //判断有没有重按键，1为有，0为没有
				{
					g_iButtonState_SW2=1; //设置按键标志
					iButtonCount_SW2=0;
					iButtonFlag_SW2=1; //设置重按键标志
				}
				else //如果重按键，则重新计数
					iButtonCount_SW2=0;
			}
			else //如果没有稳定按下7ms，则代表没有按下按键
				g_iButtonState_SW2=0;
		}
		else //如果一直无检测到低电平，即一直无按键按下
		{
			iButtonCount_SW2=0; //清零iButtonCount
			g_iButtonState_SW2=0; //清除按键标志
			iButtonFlag_SW2=0; //清除重按键标志
		}
		if( HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == GPIO_PIN_RESET )//如果引脚检测到          低电平
		{
			iButtonCount_SW3++; //按键按下，计数iButtonCount加1
			if(iButtonCount_SW3>=7) //1ms中断服务函数里运行一次，iButtonCount大于等于16，即按键已稳定按下16ms
			{
				if(iButtonFlag_SW3==0) //判断有没有重按键，1为有，0为没有
				{
					g_iButtonState_SW3=1; //设置按键标志
					iButtonCount_SW3=0;
					iButtonFlag_SW3=1; //设置重按键标志
				}
				else //如果重按键，则重新计数
					iButtonCount_SW3=0;
			}
			else //如果没有稳定按下7ms，则代表没有按下按键
				g_iButtonState_SW3=0;
		}
		else //如果一直无检测到低电平，即一直无按键按下
		{
			iButtonCount_SW3=0; //清零iButtonCount
			g_iButtonState_SW3=0; //清除按键标志
			iButtonFlag_SW3=0; //清除重按键标志
		}
	}
	if(htim == &NOTE_CONTINUE_TIM)
  {
    music_callback();
  }

	
}
