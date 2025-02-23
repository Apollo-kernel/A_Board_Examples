/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//static void play_BGM(uint8_t volume_level,Note_TypeDef* BGM);

//static void BGM_handler(void);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for ProcTask */
osThreadId_t ProcTaskHandle;
const osThreadAttr_t ProcTask_attributes = {
  .name = "ProcTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 64 * 4
};
/* Definitions for ComQueue */
osMessageQueueId_t ComQueueHandle;
const osMessageQueueAttr_t ComQueue_attributes = {
  .name = "ComQueue"
};
/* Definitions for KeyQueue */
osMessageQueueId_t KeyQueueHandle;
const osMessageQueueAttr_t KeyQueue_attributes = {
  .name = "KeyQueue"
};
/* Definitions for msTimer */
osTimerId_t msTimerHandle;
const osTimerAttr_t msTimer_attributes = {
  .name = "msTimer"
};
/* Definitions for TimeMutex */
osMutexId_t TimeMutexHandle;
const osMutexAttr_t TimeMutex_attributes = {
  .name = "TimeMutex"
};
/* Definitions for AlarmBinarySem */
osSemaphoreId_t AlarmBinarySemHandle;
const osSemaphoreAttr_t AlarmBinarySem_attributes = {
  .name = "AlarmBinarySem"
};
/* Definitions for DispEvent */
osEventFlagsId_t DispEventHandle;
const osEventFlagsAttr_t DispEvent_attributes = {
  .name = "DispEvent"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartProcTask(void *argument);
void msTimerCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of TimeMutex */
  TimeMutexHandle = osMutexNew(&TimeMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of AlarmBinarySem */
  AlarmBinarySemHandle = osSemaphoreNew(1, 0, &AlarmBinarySem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of msTimer */
  msTimerHandle = osTimerNew(msTimerCallback, osTimerPeriodic, NULL, &msTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
	osTimerStart(msTimerHandle,10);
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of ComQueue */
  ComQueueHandle = osMessageQueueNew (3, 9, &ComQueue_attributes);

  /* creation of KeyQueue */
  KeyQueueHandle = osMessageQueueNew (2, sizeof(uint8_t), &KeyQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ProcTask */
  ProcTaskHandle = osThreadNew(StartProcTask, NULL, &ProcTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of DispEvent */
  DispEventHandle = osEventFlagsNew(&DispEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartProcTask */
/**
  * @brief  Function implementing the ProcTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartProcTask */
void StartProcTask(void *argument)
{
  /* USER CODE BEGIN StartProcTask */
  /* Infinite loop */
  for(;;)
  {
		if(t_led>61)
		{
			t_led=0;
			Refresh_LED(~led_state<<k);
			k++;
		}
		number=minute*100+second;//100;
		
		
		if(!alarm)
		{
			Hundred_Display(number,display_birghness);
			if(Music_Off==0)
				Music_Flag=MUSIC_FINISH;
		}
		else
		{
//			if(Music_Flag==MUSIC_FINISH)
//			{
//				if(Music_List==0)
//				{
//					Hundred_Display(100,display_birghness);
//					music_player(MoleMusicVolum,POLICE_MUSIC);
//				}
//				else if(Music_List==1)
//				{
//					Hundred_Display(100,display_birghness);
//					music_player(MoleMusicVolum,TEST1_MUSIC);
//				}
//				else if(Music_List==2)
//				{
//					Hundred_Display(100,display_birghness);
//					music_player(MoleMusicVolum,TWO_TIGERS_MUSIC);
//				}
//				else if(Music_List==3)
//				{
//					Hundred_Display(1,display_birghness);
//					music_player(MoleMusicVolum,DOU_DI_ZHU_MUSIC);
//				}
//				else if(Music_List==4)
//				{
//					Hundred_Display(1,display_birghness);
//					music_player(MoleMusicVolum,PEPPA_PIG_MUSIC);
//				}
//				else
//				{
//					Hundred_Display(1,display_birghness);
//					music_player(MoleMusicVolum,AMBULANCE_MUSIC);
//				}
//			}
//				
//			else
//			{
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
			
//			if(alarm)
//			{
//				buzzer_off();

				
			if(Music_Flag==MUSIC_FINISH)
			{
				if(Music_List==0)
					music_player(MoleMusicVolum,TEST1_MUSIC);
				else if(Music_List==1)
					music_player(MoleMusicVolum,DOU_DI_ZHU_MUSIC);
				else if(Music_List==2)
					music_player(MoleMusicVolum,TWO_TIGERS_MUSIC);
				else if(Music_List==3)
					music_player(MoleMusicVolum,POLICE_MUSIC);
				else if(Music_List==4)
					music_player(MoleMusicVolum,PEPPA_PIG_MUSIC);
				else
					music_player(MoleMusicVolum,AMBULANCE_MUSIC);
			}
//			}
		}
			
//		//蜂鸣器开启SUPER_MARIO_MUSIC
//		buzzer_on(psc, pwm);
//		}
		if(g_iButtonState_SW3)
		{
			Count_Down_Flag=!Count_Down_Flag;
			if(alarm)
			{
				alarm=0;
				minute=5;
			}
			
				g_iButtonState_SW3=0;
		}

		if(g_iButtonState_SW1)
		{
			if(minute!=9)
				minute++;
			g_iButtonState_SW1=0;
		}
		if(g_iButtonState_SW2)
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
			
//			if(minute!=0)
//				minute--;
			g_iButtonState_SW2=0;
		}	
	
//		if(g_iButtonState_SW2)
//		{
//			if(minute!=0)
//				minute--;
//			g_iButtonState_SW2=0;
//		}	
		
		if(g_iButtonStatel_SW1)
		{
			if(minute!=0)
				minute--;
			g_iButtonStatel_SW1=0;
		}	
		
		if(g_iButtonStatel_SW2)
		{
		Music_Off=!Music_Off;
		if(Music_Off)
			Music_Flag=MUSIC_FINISH;
		else
		{
			Cycle_Flag=!Cycle_Flag;
			Music_Flag=MUSIC_BEGIN;
		}
			g_iButtonStatel_SW2=0;
		}			
		
		
		
//	if(g_iButtonState_SW1)
//	{
//		if(Music_List==2)
//			Music_List=0;
//		else
//			Music_List++;
//		if(Music_List==0)
//			music_player(MoleMusicVolum,TEST_MUSIC);
//		else if(Music_List==1)
//			music_player(MoleMusicVolum,CREATE_MUSIC);
////		else if(Music_List==2)
////			music_player(MoleMusicVolum,MISS_MUSIC);
////		else if(Music_List==3)
////			music_player(MoleMusicVolum,POLICE_MUSIC);
////		else if(Music_List==4)
////			music_player(MoleMusicVolum,CREATE_MUSIC);
//		else
//			music_player(MoleMusicVolum,TEST1_MUSIC);

		
		
		
		
		
//		if(Music_List==0)
//			music_player(MoleMusicVolum,POLICE_MUSIC);
//		else if(Music_List==1)
//			music_player(MoleMusicVolum,SUPER_MARIO_MUSIC);
//		else if(Music_List==2)
//			music_player(MoleMusicVolum,TWO_TIGERS_MUSIC);
//		else if(Music_List==3)
//			music_player(MoleMusicVolum,DOU_DI_ZHU_MUSIC);
//		else if(Music_List==4)
//			music_player(MoleMusicVolum,PEPPA_PIG_MUSIC);
//		else
//			music_player(MoleMusicVolum,AMBULANCE_MUSIC);

//		g_iButtonState_SW1=0;
//	}

		
		
		
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
				if(iButtonCount_SW1>=500)
				{
					g_iButtonStates_SW1=0;
					if(iButtonFlag_SW1==0) //判断有没有重按键，1为有，0为没有
					{
						iButtonFlag_SW1=1; //设置重按键标志
						g_iButtonStatel_SW1=1; //设置按键标志
					}
					iButtonCount_SW1=0;
				}
				else if(iButtonFlag_SW1==0)
						g_iButtonStates_SW1=1; //设置按键标志
			}
			else //如果没有稳定按下7ms，则代表没有按下按键
				g_iButtonState_SW1=0;
		}
		else //如果一直无检测到低电平，即一直无按键按下
		{
			if(g_iButtonStates_SW1==1)
			{
				g_iButtonState_SW1=1; //清除按键标志
				g_iButtonStates_SW1=0;
			}
			iButtonCount_SW1=0; //清零iButtonCount
			iButtonFlag_SW1=0; //清除重按键标志
		}
		if( HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == GPIO_PIN_RESET )//如果引脚检测到          低电平
		{
			iButtonCount_SW2++; //按键按下，计数iButtonCount加1
			
			if(iButtonCount_SW2>=7) //1ms中断服务函数里运行一次，iButtonCount大于等于16，即按键已稳定按下16ms
			{
				if(iButtonCount_SW2>=500)
				{
					g_iButtonStates_SW2=0;
					if(iButtonFlag_SW2==0) //判断有没有重按键，1为有，0为没有
					{
						iButtonFlag_SW2=1; //设置重按键标志
						g_iButtonStatel_SW2=1; //设置按键标志
					}
					iButtonCount_SW2=0;
				}
				else if(iButtonFlag_SW2==0)
						g_iButtonStates_SW2=1; //设置按键标志
			}
			else //如果没有稳定按下7ms，则代表没有按下按键
				g_iButtonState_SW2=0;
		}
		else //如果一直无检测到低电平，即一直无按键按下
		{
			if(g_iButtonStates_SW2==1)
			{
				g_iButtonState_SW2=1; //清除按键标志
				g_iButtonStates_SW2=0;
			}
			iButtonCount_SW2=0; //清零iButtonCount
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
    osDelay(1);
  }
  /* USER CODE END StartProcTask */
}

/* msTimerCallback function */
void msTimerCallback(void *argument)
{
  /* USER CODE BEGIN msTimerCallback */
	
////		设 置 pwm 的值在MIN_BUZZER_PWM和 MAX_BUZZER_PWM之间变动
//		if(pwm > MAX_BUZZER_PWM)
//		{
//			pwm = MIN_BUZZER_PWM;
//		}
//		//psc 的值在0 和 MAX_PSC之间变动
//		if(psc > MAX_PSC)
//		{
//			psc = 0;
//		}
if((Music_Flag == MUSIC_BEGIN)||(Music_Flag == MUSIC_CONTINUE))
	{
	 BGM_handler();
	}
	else//播放静止音
	{
		play_a_note(0,Music_Volume);
	}
  /* USER CODE END msTimerCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */

