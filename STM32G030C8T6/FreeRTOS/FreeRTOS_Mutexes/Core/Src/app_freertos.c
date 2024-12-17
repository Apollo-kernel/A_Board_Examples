/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId thread1Handle;
osThreadId thread2Handle;
osMutexId osMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void mutex_high_thread(void const * argument);
void mutex_low_thread(void const * argument);

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
  /* definition and creation of osMutex */
  osMutexDef(osMutex);
  osMutexHandle = osMutexCreate(osMutex(osMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of thread1 */
  osThreadDef(thread1, mutex_high_thread, osPriorityHigh, 0, 128);
  thread1Handle = osThreadCreate(osThread(thread1), NULL);

  /* definition and creation of thread2 */
  osThreadDef(thread2, mutex_low_thread, osPriorityLow, 0, 128);
  thread2Handle = osThreadCreate(osThread(thread2), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_mutex_high_thread */
/**
  * @brief  Function implementing the thread1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_mutex_high_thread */
void mutex_high_thread(void const * argument)
{
  /* USER CODE BEGIN mutex_high_thread */
	(void) argument;
  /* Infinite loop */
  for(;;)
  {
    /* ��һ������ʱ���߳��������ȼ���ߣ������Ȼ�û�������
			 �ٽ����������й����У����ڵ����ȼ��̳߳��л����������̻߳ᴦ�ڹ���״̬�ȴ��ź��� */
    osMutexWait(osMutexHandle, osWaitForever);
 
    /* ����һ��ʱ��ȷ�������ȼ��߳���ִ��ʱ�� */
    osDelay(100);

    /* �ͷŻ����� */
		osMutexRelease(osMutexHandle);

		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    
		/* ������߳� */
    osThreadSuspend(NULL);
  }
  /* USER CODE END mutex_high_thread */
}

/* USER CODE BEGIN Header_mutex_low_thread */
/**
* @brief Function implementing the thread2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_mutex_low_thread */
void mutex_low_thread(void const * argument)
{
  /* USER CODE BEGIN mutex_low_thread */
	(void) argument;
  /* Infinite loop */
  for(;;)
  {
    /* ��ȡ������ */
    osMutexWait(osMutexHandle, osWaitForever);
		
		/* �ָ���һ���߳����У���Ȼ��һ���߳��и������ȼ����������޷���ȡ������������ */
    osThreadResume(thread1Handle);

		osDelay(300);
		
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		
		/* �ͷŻ���������һ���߳̿������� */
		osMutexRelease(osMutexHandle);
  }
  /* USER CODE END mutex_low_thread */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
