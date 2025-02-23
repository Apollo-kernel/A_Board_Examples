/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "libs.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED14_Pin GPIO_PIN_13
#define LED14_GPIO_Port GPIOC
#define LED15_Pin GPIO_PIN_14
#define LED15_GPIO_Port GPIOC
#define LED16_Pin GPIO_PIN_15
#define LED16_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_3
#define LED4_GPIO_Port GPIOA
#define LED5_Pin GPIO_PIN_4
#define LED5_GPIO_Port GPIOA
#define LED6_Pin GPIO_PIN_5
#define LED6_GPIO_Port GPIOA
#define LED7_Pin GPIO_PIN_6
#define LED7_GPIO_Port GPIOA
#define LED8_Pin GPIO_PIN_7
#define LED8_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_0
#define SW2_GPIO_Port GPIOB
#define SW3_Pin GPIO_PIN_1
#define SW3_GPIO_Port GPIOB
#define SW1_Pin GPIO_PIN_12
#define SW1_GPIO_Port GPIOB
#define LED9_Pin GPIO_PIN_13
#define LED9_GPIO_Port GPIOB
#define LED10_Pin GPIO_PIN_14
#define LED10_GPIO_Port GPIOB
#define LED11_Pin GPIO_PIN_15
#define LED11_GPIO_Port GPIOB
#define LED12_Pin GPIO_PIN_6
#define LED12_GPIO_Port GPIOC
#define LED13_Pin GPIO_PIN_7
#define LED13_GPIO_Port GPIOC
#define Display_DG3_Pin GPIO_PIN_0
#define Display_DG3_GPIO_Port GPIOD
#define Display_DG2_Pin GPIO_PIN_1
#define Display_DG2_GPIO_Port GPIOD
#define Display_DG1_Pin GPIO_PIN_2
#define Display_DG1_GPIO_Port GPIOD
#define Display_B_Pin GPIO_PIN_3
#define Display_B_GPIO_Port GPIOD
#define Display_G_Pin GPIO_PIN_3
#define Display_G_GPIO_Port GPIOB
#define Display_F_Pin GPIO_PIN_4
#define Display_F_GPIO_Port GPIOB
#define Display_A_Pin GPIO_PIN_5
#define Display_A_GPIO_Port GPIOB
#define Display_C_Pin GPIO_PIN_6
#define Display_C_GPIO_Port GPIOB
#define Display_DP_Pin GPIO_PIN_7
#define Display_DP_GPIO_Port GPIOB
#define Display_D_Pin GPIO_PIN_8
#define Display_D_GPIO_Port GPIOB
#define Display_E_Pin GPIO_PIN_9
#define Display_E_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
