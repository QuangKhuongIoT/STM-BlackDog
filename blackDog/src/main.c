/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
/* Private includes ----------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "myButton.h"
#include "myOutput.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define df_LED_OFF 1
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
myButton btn1;
myOutput ledStt;
uint16_t ledBlinkTime = 0;
uint16_t ledPeriodTime = 0;
uint16_t ledDutyTime = 0;
uint32_t ledStartBlink = 0;
uint8_t ledStatus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
void btnInit();
uint8_t btnGetStatus();
void ledBlink();
void ledLoop();
uint32_t getCurrentTimeMs();
void clickHandler();
void doubleClickHandler();
void multiClickHandler();
void pressHandler();
void longPressHandler();
void depressHandler();

void ledInit();
void ledControl(output_status_t state);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* USER CODE BEGIN 1 */
    // uint32_t timeout;

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
    MX_RTC_Init();
    /* USER CODE BEGIN 2 */
    // timeout = HAL_GetTick();
    inputConfig(&btn1, 0, btnInit, btnGetStatus, getCurrentTimeMs);
    // inputAttachClick(&btn1, clickHandler);
    btn1.attachClick(&btn1, clickHandler);
    inputAttachDoubleClick(&btn1, clickHandler);
    inputAttachDoubleClick(&btn1, doubleClickHandler);
    inputAttachMultiClick(&btn1, multiClickHandler);
    inputAttachPress(&btn1, pressHandler);
    inputAttachLongPress(&btn1, longPressHandler);
    inputAttachDepress(&btn1, depressHandler);

    setOutputConfig(&ledStt, GPIO_PIN_RESET, ledInit, getCurrentTimeMs);
    // setOutputPulse(&ledStt, 10, 400, 200);
    setOutputPwm(&ledStt, 400, 200);
    outputOnOff(&ledStt, ledControl);

    // ledBlink(3, 1000, 500);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */
        // if (HAL_GetTick() - timeout > 1000ul) {
        //     timeout = HAL_GetTick();
        //     HAL_GPIO_TogglePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD_Pin);
        // }
        // buttonLoop(&btn);
        inputTick(&btn1);
        ledLoop();
        outputTick(&ledStt);
        // HAL_Delay(1000);
        // HAL_GPIO_TogglePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD_Pin);

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_ONBOARD_Pin */
  GPIO_InitStruct.Pin = LED_ONBOARD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(LED_ONBOARD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BTN_ONBOARD_Pin */
  GPIO_InitStruct.Pin = BTN_ONBOARD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BTN_ONBOARD_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void btnInit() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = BTN_ONBOARD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BTN_ONBOARD_GPIO_Port, &GPIO_InitStruct);
}

uint8_t btnGetStatus() {
    if (HAL_GPIO_ReadPin(BTN_ONBOARD_GPIO_Port, BTN_ONBOARD_Pin) == GPIO_PIN_SET)
        return 1;
    else
        return 0;
}

void ledBlink(int time, int period, int duty) {
    ledBlinkTime = time;
    ledPeriodTime = period;
    ledDutyTime = duty;
    ledStartBlink = HAL_GetTick();
}

void ledLoop() {
    if (ledBlinkTime > 0) {
        if ((HAL_GetTick() - ledStartBlink) < ledDutyTime) {
            if (ledStatus != 1) {
                HAL_GPIO_WritePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD_Pin, 0);
                ledStatus = 1;
            }
        } else {
            if (ledStatus != 0) {
                HAL_GPIO_WritePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD_Pin, 1);
                ledStatus = 0;
            }
        }
        if ((HAL_GetTick() - ledStartBlink) > ledPeriodTime) {
            if (ledBlinkTime != 0xFF) {
                ledBlinkTime--;
            }
            ledStartBlink = HAL_GetTick();
        }
    }
}

uint32_t getCurrentTimeMs() {
    return HAL_GetTick();
}

void clickHandler() {
    // ledBlink(1, 1000, 500);
    setOutputOnOff(&ledStt, OP_STT_OFF);
}
void doubleClickHandler() {
    setOutputOnOff(&ledStt, OP_STT_ON);
    // ledBlink(2, 1000, 500);
}
void multiClickHandler() {
    // ledBlink(inputGetClickCounter(&btn1), 1000, 500);
    if (inputGetClickCounter(&btn1) == 3) {
        setOutputPulse(&ledStt, 3, 400, 200);
    } else if (inputGetClickCounter(&btn1) == 4) {
        setOutputPwm(&ledStt, 400, 200);
    }
}
void pressHandler() {
    ledBlink(4, 1000, 500);
}
void longPressHandler() {
    ledBlink(5, 1000, 500);
}
void depressHandler() {
    ledBlink(6, 1000, 500);
}

void ledInit() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : LED_ONBOARD_Pin */
    GPIO_InitStruct.Pin = LED_ONBOARD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(LED_ONBOARD_GPIO_Port, &GPIO_InitStruct);
}

void ledControl(output_status_t state) {
    if (state == OP_STT_ON) {
        HAL_GPIO_WritePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD_Pin, GPIO_PIN_RESET);
    }
    if (state == OP_STT_OFF) {
        HAL_GPIO_WritePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD_Pin, GPIO_PIN_SET);
    }
}

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

#ifdef  USE_FULL_ASSERT
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
