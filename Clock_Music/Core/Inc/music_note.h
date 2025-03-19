#ifndef __MUSIC_NOTE_H__
#define __MUSIC_NOTE_H__


#include "main.h"

//播放新的音乐
#define MUSIC_BEGIN         1
//当前的音乐正在持续中
#define MUSIC_CONTINUE      2
//停止播放音乐
#define MUSIC_FINISH        3

//定义驱动蜂鸣器的定时器时钟频率
#define BEEP_TIM_CLOCK  9142857U	//8000000U
																	//9142857U
//背景音乐列表
enum MusicList
{
  CREATE_MUSIC = 1,
  MISS_MUSIC,
  TWO_TIGERS_MUSIC,
  PEPPA_PIG_MUSIC,
  SUPER_MARIO_MUSIC,
  DOU_DI_ZHU_MUSIC,
  POLICE_MUSIC,
  AMBULANCE_MUSIC,
	
	
	TEST_MUSIC,
	TEST1_MUSIC,
	GALAYOUMUSIC
	
};

typedef struct
{
  uint16_t tone; //音符
  uint16_t time; //时间 为1表示10ms
}Note_TypeDef;

void enable_music_player(void);
void play_a_note(uint16_t tone,uint8_t volumeLevel);
void play_music_block(uint8_t volume_level);
void music_callback(void);


//降A大调定义
#define note_A  220
#define note_3A 110  
#define note_5A 440  
#define note_sA 233  //233.082
#define note_B  247  //246.942
#define note_3B  123  //123.471
#define note_5B  494  //493.883
#define note_C  262  //261.626
#define note_5C  523  //523.251
#define note_sC 277  //277.183
#define note_D  294  //293.665
#define note_sD 311  //311.127
#define note_5D 587  //587.33
#define note_3sD 156  //155.563
#define note_E  330  //329.629
#define note_3E  165  //164.814
#define note_F  349  //349.228
#define note_3F  175  //174.614
#define note_sF 370  //369.994
#define note_3sF 185  //184.997
#define note_G  392  //391.995
#define note_sG 415  //415.305
#define note_3G 196  //195.998
#define note_5sG 831  //830.609


//C大调定义
//定义低音音名C
#define CL1 262
#define CL2 294
#define CL3 330
#define CL4 349
#define CL5 392
#define CL6 440
#define CL7 494

//定义中音音名C
#define CM1 523
#define CM2 587
#define CM3 659
#define CM4 698
#define CM5 784
#define CM6 880
#define CM7 988

//定义高音音名C
#define CH1 1047
#define CH2 1175
#define CH3 1319
#define CH4 1397
#define CH5 1568
#define CH6 1760
#define CH7 1976

//定义低音音名D
#define DL1 294
#define DL2 330
#define DL3 370
#define DL4 392
#define DL5 440
#define DL6 494
#define DL7 554

//定义中音音名D
#define DM1 587
#define DM2 659
#define DM3 740
#define DM4 784
#define DM5 880
#define DM6 988
#define DM7 1109

//定义高音音名D
#define DH1 1175
#define DH2 1319
#define DH3 1480
#define DH4 1568
#define DH5 1760
#define DH6 1976
#define DH7 2217

//定义低音音名E
#define EL1 330
#define EL2 370
#define EL3 415
#define EL4 440
#define EL5 494
#define EL6 554
#define EL65 587
#define EL7 622

//定义中音音名E
#define EM1 659
#define EM2 740
#define EM3 831
#define EM4 880
#define EM5 988
#define EM6 1109
#define EM7 1245

//定义高音音名E
#define EH1 1319
#define EH2 1480
#define EH3 1661
#define EH4 1760
#define EH5 1976

//定义低音音名F (单位是Hz)
#define FL1 349
#define FL2 392
#define FL3 440
#define FL4 466
#define FL5 523
#define FL6 587
#define FL7 659

//定义中音音名F
#define FM1 698
#define FM2 784
#define FM3 880
#define FM4 932
#define FM5 1047
#define FM6 1175
#define FM7 1319

//定义高音音名F
#define FH1 1397
#define FH2 1568
#define FH3 1760
#define FH4 1865


extern const Note_TypeDef testMusic[];
extern const Note_TypeDef test1Music[];
extern const Note_TypeDef GalaYouMusic[];

extern const Note_TypeDef MissMusic[];
extern const Note_TypeDef CreatMusic[];
extern const Note_TypeDef AmbulanceMusic[];
extern const Note_TypeDef PoliceMusic[];
extern const Note_TypeDef TwoTigersMusic[];
extern const Note_TypeDef PeppaPigMusic[];
extern const Note_TypeDef SuperMarioMusic[];
extern const Note_TypeDef DouDiZhuMusic[];


#ifdef __cplusplus
}
#endif

#endif
