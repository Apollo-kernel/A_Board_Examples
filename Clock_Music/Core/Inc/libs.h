#ifndef _LIBS_H_
#define _LIBS_H_

#define MAX_PSC             500			//分频系数
#define MAX_BUZZER_PWM      2500		//重载值上限
#define MIN_BUZZER_PWM      500			//重载值下限

//用于PWM输出，演奏音符的定时器
#define NOTE_PWM_TIM         htim1
//PWM输出的通道
#define NOTE_PWM_CHANNEL     TIM_CHANNEL_1


#include "main.h"
#include "stdint.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

#include "music_note.h"

extern void Refresh_LED(uint16_t state); // 刷新LED灯（0亮1灭）
extern void Refresh_LED_Neg(uint16_t state); // 刷新LED灯（0灭1亮）
extern void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness); // 刷新数码管
extern void Hundred_Display(uint32_t hundred, uint16_t brightness); // 数码管999
extern void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness);
extern void buzzer_on(uint16_t psc, uint16_t pwm);
extern void buzzer_off(void);

extern void Clear_Display(); // 数码管清屏

extern void BGM_handler(void);
extern void enable_music_player(void);
extern void play_a_note(uint16_t tone,uint8_t volumeLevel);
extern void play_BGM(uint8_t volume_level,Note_TypeDef* BGM);
extern void music_player(uint8_t volume_level , enum MusicList num);

extern uint16_t t_ms,t_led,t_tube,number,led_state;
extern uint8_t alarm,minute,second,Count_Down_Flag;
extern int k;
extern uint8_t display_codes[10];
extern uint16_t psc;
extern uint16_t pwm;

//用于音乐播放的全局变量
extern uint8_t Music_Volume;     //音量
extern uint8_t Music_Flag;       //切换音乐的标志位
extern uint8_t Music_Off;     	//音乐停止的标志位
extern uint8_t Cycle_Flag;     //单曲循环的标志位
extern uint8_t Music_List;     //音乐列表序号
extern uint16_t BGM_Length;      //BGM背景音乐的长度
extern Note_TypeDef *BGM_Current;      //指向当前BGM的指针
extern unsigned char MoleMusicVolum;   //音量,建议范围1~10,1大10小


// 数码管亮度，0~1000（htim3.Instance->ARR）
extern uint16_t display_birghness;

// 数码管上显示的数字
extern uint16_t num;
extern int iButtonCount_SW1,iButtonCount_SW2,iButtonCount_SW3;//i代表int型变量，ButtonCount表示按键计数变量
extern int iButtonFlag_SW1,iButtonFlag_SW2,iButtonFlag_SW3;//i代表int型变量，ButtonFlag表示重按键标志，1代表重新按键，0为没有重新按键
extern int g_iButtonState_SW1,g_iButtonState_SW2,g_iButtonState_SW3;//g是globle代表全局变量，会在其他地方引用；i代表int型变量，ButtonState表示按键标志，1代表按下，0代表松开
extern int g_iButtonStates_SW1,g_iButtonStates_SW2,g_iButtonStates_SW3;//g是globle代表全局变量，会在其他地方引用；i代表int型变量，ButtonState表示按键标志，1代表按下，0代表松开
extern int g_iButtonStatel_SW1,g_iButtonStatel_SW2,g_iButtonStatel_SW3;//g是globle代表全局变量，会在其他地方引用；i代表int型变量，ButtonState表示按键标志，1代表按下，0代表松开
extern int setting_mode;


extern uint8_t Count_Down_Flag;




#endif
