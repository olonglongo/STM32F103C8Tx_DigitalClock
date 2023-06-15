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
#include "pca9685.h"
#include "DS3231.h"

// #include "stm32f1xx_hal.c"
// #include <stdint.h>
// #include <stm32f1xx.h>
// #include <stm32f1xx_hal.h>
// #include <stm32f1xx_hal_gpio.h>

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
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

#include <stdio.h>
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
PUTCHAR_PROTOTYPE
{

    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
int _write(int file, char *ptr, int len)
{
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}

#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
#endif

void printr(uint8_t reg)
{
    printf("Reg 0x%02x = ", reg);
    uint8_t val;
    HAL_I2C_Master_Transmit(_ds3231_ui2c, DS3231_I2C_ADDR << 1, &reg, 1,
                            DS3231_TIMEOUT);
    HAL_StatusTypeDef s = HAL_I2C_Master_Receive(_ds3231_ui2c,
                                                 DS3231_I2C_ADDR << 1, &val, 1, DS3231_TIMEOUT);
    for (uint8_t i = 0; i < 8; i++)
    {
        printf("%d", (val >> (7 - i)) & 1);
    }
    printf(" With status %d", s);
    printf("\n");
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// const int SEGMENT_INTERVALS[7][2] = {
//     /*
//         0
//     1       2
//         3
//     4       5
//         6
//     */
//     {110, 180}, // 0
//     {110, 180}, // 1
//     {10, 110},  // 2
//     {110, 180}, // 3
//     {10, 110},  // 4
//     {110, 180}, // 5
//     {110, 180}, // 6

// };
// int min, max;
// min, max = SEGMENT_INTERVALS[Channel];

const int COLON_INTERVAL[2][2] = {
    {141, 62},
    {137, 30},
};

// const int DIGIT_STARTING_SEGMENT_INDEX[4] = {2, 9, 22, 29};
// const int DIGIT_STARTING_SEGMENT_INDEX[4] = {0, 9, 22, 29};
const int COLON_STARTING_INDEX = 16;

const int START_POS = 0;
const int COLON = 2;
const int DIGITS = 4;
const int SEGMENTS_PER_DIGIT = 7;
const int STEP_MS = 20;
const int COUNT_MS = 2000;
const int NUM_SERVOS = DIGITS * SEGMENTS_PER_DIGIT;

const uint8_t DIGIT_TO_SEGMENT_MAPPING[10][7] = {
    //     /*
    //         0
    //     1       2
    //         3
    //     4       5
    //         6
    //     */
    /*      0  1  2  3  4  5  6     */
    {1, 1, 1, 0, 1, 1, 1}, // 0
    {0, 0, 1, 0, 0, 1, 0}, // 1
    {1, 0, 1, 1, 1, 0, 1}, // 2
    {1, 0, 1, 1, 0, 1, 1}, // 3
    {0, 1, 1, 1, 0, 1, 0}, // 4
    {1, 1, 0, 1, 0, 1, 1}, // 5
    {1, 1, 0, 1, 1, 1, 1}, // 6
    {1, 0, 1, 0, 0, 1, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

// int servoTargetDestination[DIGITS][NUM_SERVOS];
// int servoTargetDestinationColon[COLON];

int count = 1200;
int timeMS = 0;

void printNum(uint8_t DevAddress, uint8_t date)
{
    uint8_t tmpNum = 0;
    uint8_t tmpDate = 0;
    uint8_t tmpServo = 0;

    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint8_t num = 0; num < SEGMENTS_PER_DIGIT; num++)
        {
            tmpDate = num;
            if (i == 0)
            {
                tmpNum = date / 10 % 10;
                tmpServo = num;
            }
            if (i == 1)
            {
                tmpNum = date / 1 % 10;
                tmpServo = num + 7;
            }
            // tmp 要显示的数字
            PCA9685_SetServoStat(DevAddress, tmpServo, DIGIT_TO_SEGMENT_MAPPING[tmpNum][tmpDate]);

            // int displayNumber = stringCount.charAt(timeString.length() - 1 - activeDigit) - '0';
            // int placement = (activeDigit == 3 && displayNumber == 0) ? 0 : DIGIT_TO_SEGMENT_MAPPING[displayNumber][i];
            // servoTargetDestination[activeDigit][i] = SEGMENT_INTERVALS[activeDigit][i][placement];
        }
    }
}

// #include <stdlib.h>
// Driver

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
    MX_I2C2_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    // uint32_t TimerUART = HAL_GetTick();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    // DS3231 init function. Pass I2C handle.
    DS3231_Init(&hi2c2);

    /* set times */
    // // Disable interrupts while we set interrupt configs.
    // __disable_irq();
    // // Set interrupt mode to square wave mode, enable square wave interrupt at pin 3.
    // DS3231_SetInterruptMode(DS3231_SQUARE_WAVE_INTERRUPT);
    // // Set interrupting frequency to 1 Hz.
    // DS3231_SetRateSelect(DS3231_1HZ);
    // // Set time.
    // DS3231_SetFullTime(20, 18, 30);
    // // Set date.
    // DS3231_SetFullDate(2022, 10, 23, 7);
    // // Print all register values, for demonstration purpose
    // for (uint8_t i = 0x00; i < 0x13; i++)
    //     printr(i);
    // // Enable interrupts after finishing.
    // __enable_irq();

    // // Should show 5 interrupts, once every second.

    // First Set Hour
    PCA9685_Init(SERVO_LEFT, &hi2c2);
    printNum(SERVO_LEFT, DS3231_GetHour());
    HAL_Delay(1 * 1000);
    PCA9685_Init(SERVO_RIGHT, &hi2c2);
    printNum(SERVO_RIGHT, DS3231_GetMinute());
    HAL_Delay(3 * 1 * 1000);
    // Info Main Date
    uint8_t timeMinute = 00;
    while (1)
    {
        timeMinute = DS3231_GetMinute();
        printf("Date: %.2x/%.2x/%.2x - %.2x:%.2x:%.2x - Week:%.2x\r\n",
               DS3231_GetYear(),
               DS3231_GetMonth(),
               DS3231_GetDate(),
               DS3231_GetHour(),
               DS3231_GetMinute(),
               DS3231_GetSecond(),
               DS3231_GetDayOfWeek());
        if (timeMinute == 0 || timeMinute == 00)
        {
            // hour
            PCA9685_Init(SERVO_LEFT, &hi2c2);
            printNum(SERVO_LEFT, DS3231_GetHour());
            HAL_Delay(1 * 1000);
            // minute
            PCA9685_Init(SERVO_RIGHT, &hi2c2);
        }
        else
        {
            printNum(SERVO_RIGHT, timeMinute);
            HAL_Delay(1 * 1000);
        }
        // sleep 59s
        /* USER CODE END WHILE */

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

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void)
{

    /* USER CODE BEGIN I2C2_Init 0 */

    /* USER CODE END I2C2_Init 0 */

    /* USER CODE BEGIN I2C2_Init 1 */

    /* USER CODE END I2C2_Init 1 */
    hi2c2.Instance = I2C2;
    hi2c2.Init.ClockSpeed = 100000;
    hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c2.Init.OwnAddress1 = 0;
    hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.OwnAddress2 = 0;
    hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C2_Init 2 */

    /* USER CODE END I2C2_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
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
