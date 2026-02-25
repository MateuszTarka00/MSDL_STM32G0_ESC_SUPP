/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32c0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define __HAL_GPIO_EXTI_DISABLE_IT(__EXTI_LINE__) \
  (EXTI->IMR1 &= ~(__EXTI_LINE__))

#define __HAL_GPIO_EXTI_ENABLE_IT(__EXTI_LINE__) \
  (EXTI->IMR1 |= (__EXTI_LINE__))
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TEACH_INPUT_Pin GPIO_PIN_3
#define TEACH_INPUT_GPIO_Port GPIOA
#define ROTATION_CONTROL_Pin GPIO_PIN_4
#define ROTATION_CONTROL_GPIO_Port GPIOA
#define STAND_CONTROL_Pin GPIO_PIN_5
#define STAND_CONTROL_GPIO_Port GPIOA
#define SAFETY_END_Pin GPIO_PIN_0
#define SAFETY_END_GPIO_Port GPIOB
#define INSPECTION_MODE_Pin GPIO_PIN_1
#define INSPECTION_MODE_GPIO_Port GPIOB
#define SPEED_READY_Pin GPIO_PIN_13
#define SPEED_READY_GPIO_Port GPIOB
#define TEACH_FAST_Pin GPIO_PIN_14
#define TEACH_FAST_GPIO_Port GPIOB
#define FACTORY_RESET_Pin GPIO_PIN_15
#define FACTORY_RESET_GPIO_Port GPIOB
#define IN_FREQ_Pin GPIO_PIN_7
#define IN_FREQ_GPIO_Port GPIOC
#define WATCHDOG_OUT_Pin GPIO_PIN_15
#define WATCHDOG_OUT_GPIO_Port GPIOA
#define K2_EN_Pin GPIO_PIN_0
#define K2_EN_GPIO_Port GPIOD
#define ROTATION_S2_Pin GPIO_PIN_1
#define ROTATION_S2_GPIO_Port GPIOD
#define MIS_ST2_Pin GPIO_PIN_2
#define MIS_ST2_GPIO_Port GPIOD
#define MIS_ST1_Pin GPIO_PIN_3
#define MIS_ST1_GPIO_Port GPIOD
#define TEACH_SLOW_Pin GPIO_PIN_3
#define TEACH_SLOW_GPIO_Port GPIOB
#define SAFETY_OK_Pin GPIO_PIN_4
#define SAFETY_OK_GPIO_Port GPIOB
#define STAND_OK_Pin GPIO_PIN_5
#define STAND_OK_GPIO_Port GPIOB
#define ROTATION_OK_Pin GPIO_PIN_6
#define ROTATION_OK_GPIO_Port GPIOB
#define CHECK_UP_Pin GPIO_PIN_8
#define CHECK_UP_GPIO_Port GPIOB
#define CHECK_DOWN_Pin GPIO_PIN_9
#define CHECK_DOWN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
typedef enum
{
	FALSE,
	TRUE
}bool;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
