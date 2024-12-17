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
#include "usart.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BIT_0  ( 1 << 0 )
#define BIT_1  ( 1 << 1 )

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId thread1Handle;
osThreadId thread2Handle;
osThreadId thread3Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void led_thread(void const * argument);
void usart_thread(void const * argument);
void signal_gen(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  osThreadDef(thread1, led_thread, osPriorityNormal, 0, 128);
  thread1Handle = osThreadCreate(osThread(thread1), NULL);

  /* definition and creation of thread2 */
  osThreadDef(thread2, usart_thread, osPriorityBelowNormal, 0, 128);
  thread2Handle = osThreadCreate(osThread(thread2), NULL);

  /* definition and creation of thread3 */
  osThreadDef(thread3, signal_gen, osPriorityBelowNormal, 0, 128);
  thread3Handle = osThreadCreate(osThread(thread3), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_led_thread */
/**
  * @brief  Function implementing the thread1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_led_thread */
void led_thread(void const * argument)
{
  /* USER CODE BEGIN led_thread */
	(void) argument;
  osEvent event;
  /* Infinite loop */
  for(;;)
  {
    event = osSignalWait( BIT_0, osWaitForever);
    if(event.value.signals == BIT_0) {
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		}
  }
  /* USER CODE END led_thread */
}

/* USER CODE BEGIN Header_usart_thread */
/**
* @brief Function implementing the thread2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_usart_thread */
void usart_thread(void const * argument)
{
  /* USER CODE BEGIN usart_thread */
  /* USER CODE BEGIN usart_thread */
	(void) argument;
  osEvent event;
	char tx[] = "FreeRTOS usart_thread signal test...\r\n";
  /* Infinite loop */
  for(;;)
  {
    event = osSignalWait( BIT_1, osWaitForever);
    if(event.value.signals == BIT_1) {
				HAL_UART_Transmit(&huart1, (uint8_t *)tx, sizeof(tx), 0xffff);
		}
  }
  /* USER CODE END usart_thread */
}

/* USER CODE BEGIN Header_signal_gen */
/**
* @brief Function implementing the thread3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_signal_gen */
void signal_gen(void const * argument)
{
  /* USER CODE BEGIN signal_gen */
	(void) argument;
  /* Infinite loop */
  for(;;)
  {
    osSignalSet(thread1Handle, BIT_0 );
    osDelay(500);
    osSignalSet(thread2Handle, BIT_1 );
    osDelay(500);
  }
  /* USER CODE END signal_gen */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
