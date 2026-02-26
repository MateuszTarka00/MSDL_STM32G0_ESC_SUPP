/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, INSPECTION_MODE_Pin|SPEED_READY_Pin|SAFETY_OK_Pin|STAND_OK_Pin
                          |ROTATION_OK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WATCHDOG_OUT_GPIO_Port, WATCHDOG_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(K2_EN_GPIO_Port, K2_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : TEACH_INPUT_Pin ROTATION_CONTROL_Pin STAND_CONTROL_Pin FAST_SPEED_Pin
                           SLOW_SPEED_Pin */
  GPIO_InitStruct.Pin = TEACH_INPUT_Pin|ROTATION_CONTROL_Pin|STAND_CONTROL_Pin|FAST_SPEED_Pin
                          |SLOW_SPEED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SAFETY_END_Pin TEACH_FAST_Pin FACTORY_RESET_Pin TEACH_SLOW_Pin
                           CHECK_UP_Pin CHECK_DOWN_Pin */
  GPIO_InitStruct.Pin = SAFETY_END_Pin|TEACH_FAST_Pin|FACTORY_RESET_Pin|TEACH_SLOW_Pin
                          |CHECK_UP_Pin|CHECK_DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : INSPECTION_MODE_Pin SPEED_READY_Pin SAFETY_OK_Pin STAND_OK_Pin
                           ROTATION_OK_Pin */
  GPIO_InitStruct.Pin = INSPECTION_MODE_Pin|SPEED_READY_Pin|SAFETY_OK_Pin|STAND_OK_Pin
                          |ROTATION_OK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : END_TEACHING_Pin IN_FREQ_Pin */
  GPIO_InitStruct.Pin = END_TEACHING_Pin|IN_FREQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : WATCHDOG_OUT_Pin */
  GPIO_InitStruct.Pin = WATCHDOG_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WATCHDOG_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : K2_EN_Pin */
  GPIO_InitStruct.Pin = K2_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(K2_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ROTATION_S2_Pin MIS_ST2_Pin MIS_ST1_Pin */
  GPIO_InitStruct.Pin = ROTATION_S2_Pin|MIS_ST2_Pin|MIS_ST1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
