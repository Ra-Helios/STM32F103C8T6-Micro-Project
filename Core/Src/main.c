/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "ssd1306.h"
#include "fonts.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// ---- KEYPAD PIN DEFINITIONS ----
// Rows: PA0–PA3 (Output)
// Cols: PA4–PA6 (Input + Pull-up)
#define ROW1_Pin GPIO_PIN_0
#define ROW2_Pin GPIO_PIN_1
#define ROW3_Pin GPIO_PIN_2
#define ROW4_Pin GPIO_PIN_3
#define COL1_Pin GPIO_PIN_4
#define COL2_Pin GPIO_PIN_5
#define COL3_Pin GPIO_PIN_6
#define KEYPAD_PORT GPIOA

// ---- FUNCTION DECLARATIONS ----
char Keypad_GetKey(void);
int get_int_from_keypad(char* label);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  SSD1306_Init();
  SSD1306_Clear();
  SSD1306_GotoXY(0, 0);
  SSD1306_Puts("BMI Calculator", &Font_7x10, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
  HAL_Delay(1500);

  // Get Weight (integer)
      int weight = get_int_from_keypad("W(kg):");
      // Get Height (integer)
      int height_cm = get_int_from_keypad("H(cm):");
      float height_m = height_cm / 100.0f;
      float bmi = (float)weight / (height_m * height_m);

      // ---------------------- BMI DISPLAY ----------------------
      char line[30];
      const char* category;
      const char* remark;

      // Determine BMI category and remark
      if (bmi < 18.5f) {
          category = "Underweight";
          remark = "<18.5 = Underweight";
      }
      else if (bmi < 25.0f) {
          category = "Normal";
          remark = "18.5-24.9 = Normal";
      }
      else if (bmi < 30.0f) {
          category = "Overweight";
          remark = "25-29.9 = Overweight";
      }
      else {
          category = "Obese";
          remark = ">=30 = Obese";
      }

      // Clear and display
      SSD1306_Clear();
      SSD1306_GotoXY(0, 0);
      SSD1306_Puts("Result:", &Font_11x18, SSD1306_COLOR_WHITE);

      // Combine BMI + category like “31.12 (Obese)”
      SSD1306_GotoXY(0, 25);
      int integer = (int)bmi;
      int decimal = (int)((bmi - integer) * 100);
      sprintf(line, "%d.%02d (%s)", integer, abs(decimal), category);
      SSD1306_Puts(line, &Font_7x10, SSD1306_COLOR_WHITE);

      // Add bottom description
      SSD1306_GotoXY(0, 50);
      SSD1306_Puts((char*)remark, &Font_7x10, SSD1306_COLOR_WHITE);

      SSD1306_UpdateScreen();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

// ---- INTEGER KEYPAD INPUT ----
int get_int_from_keypad( char* label)
{
    char input[4] = ""; // only 3 digits + null
    uint8_t index = 0;
    char key;

    SSD1306_Clear();
    SSD1306_GotoXY(0, 0);
    SSD1306_Puts(label, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();

    while (1)
        {
            key = Keypad_GetKey();
            if (key != 0)
            {
                if (key >= '0' && key <= '9')
                {
                    if (index < 3)
                    {
                        input[index++] = key;
                        input[index] = '\0';
                    }
                }
                else if (key == '#') // Enter key
                {
                    if (index > 0) break; // only accept if something entered
                }
                else if (key == '*') // Clear key
                {
                    index = 0;
                    input[0] = '\0';
                }

                // Update display
                SSD1306_GotoXY(0, 30);
                SSD1306_Puts("               ", &Font_11x18, SSD1306_COLOR_BLACK);
                SSD1306_GotoXY(0, 30);
                SSD1306_Puts(input, &Font_11x18, SSD1306_COLOR_WHITE);
                SSD1306_UpdateScreen();
            }
        }

        return atoi(input);
    }

// ---- KEYPAD SCAN FUNCTION ----
const char keypad[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

char Keypad_GetKey(void)
{
    for (int row = 0; row < 4; row++)
    {
        // Set all rows HIGH
        HAL_GPIO_WritePin(KEYPAD_PORT, ROW1_Pin | ROW2_Pin | ROW3_Pin | ROW4_Pin, GPIO_PIN_SET);
        // Pull one row LOW
        HAL_GPIO_WritePin(KEYPAD_PORT, (ROW1_Pin << row), GPIO_PIN_RESET);

        for (int col = 0; col < 3; col++)
        {
            if (HAL_GPIO_ReadPin(KEYPAD_PORT, (COL1_Pin << col)) == GPIO_PIN_RESET)
            {
                HAL_Delay(150); // debounce
                while (HAL_GPIO_ReadPin(KEYPAD_PORT, (COL1_Pin << col)) == GPIO_PIN_RESET);
                return keypad[row][col];
            }
        }
    }
    return 0;
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
