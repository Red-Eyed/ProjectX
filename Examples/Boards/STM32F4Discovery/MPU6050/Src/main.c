/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "usb_device.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "usbd_custom_hid_if.h"
#include "mpu6050.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

uint8_t buf[CUSTOM_HID_EPIN_SIZE];
uint16_t MSB, LSB;

uint8_t mpu6050_Read(uint16_t addr)
{
  uint8_t data = 0;
  const uint8_t dev_addr = MPU6050_ADDRESS_AD0_LOW >> 1;
//  HAL_I2C_Mem_Read(&hi2c1, dev_addr, addr, 1, &data, 1, 0x1000);
  HAL_I2C_Master_Transmit(&hi2c1, dev_addr, &addr, 1, 0xF000);
  HAL_I2C_Master_Receive(&hi2c1, dev_addr, &data, 1, 0xF000);

  return data;
}


void mpu6050_Write(uint16_t addr, uint8_t data)
{
  const uint8_t dev_addr = MPU6050_ADDRESS_AD0_LOW >> 1;
//  HAL_I2C_Mem_Write(&hi2c1, dev_addr, addr, 1, &data, 1, 0x1000);
    HAL_I2C_Master_Transmit(&hi2c1, dev_addr, &addr, 1, 0xF000);
    HAL_I2C_Master_Transmit(&hi2c1, dev_addr, &data, 1, 0xF000);
}

void mpu6050_init()
{
  //Sets sample rate to 8000/1+7 = 1000Hz
  mpu6050_Write(MPU6050_RA_SMPLRT_DIV, 0x07);
  //Disable FSync, 256Hz DLPF
  mpu6050_Write(MPU6050_RA_CONFIG, 0x00);
  //Disable gyro self tests, scale of 500 degrees/s
  mpu6050_Write(MPU6050_RA_GYRO_CONFIG, 0b00001000);
  //Disable accel self tests, scale of +-2g, no DHPF
  mpu6050_Write(MPU6050_RA_ACCEL_CONFIG, 0x00);
  //Freefall threshold of |0mg|
  mpu6050_Write(MPU6050_RA_FF_THR, 0x00);
  //Freefall duration limit of 0
  mpu6050_Write(MPU6050_RA_FF_DUR, 0x00);
  //Motion threshold of 0mg
  mpu6050_Write(MPU6050_RA_MOT_THR, 0x00);
  //Motion duration of 0s
  mpu6050_Write(MPU6050_RA_MOT_DUR, 0x00);
  //Zero motion threshold
  mpu6050_Write(MPU6050_RA_ZRMOT_THR, 0x00);
  //Zero motion duration threshold
  mpu6050_Write(MPU6050_RA_ZRMOT_DUR, 0x00);
  //Disable sensor output to FIFO buffer
  mpu6050_Write(MPU6050_RA_FIFO_EN, 0x00);
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */
  mpu6050_init();
  uint8_t who_am_i = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_Delay(100);

    if(!who_am_i){
      who_am_i = mpu6050_Read(0x75);
      mpu6050_Write(MPU6050_RA_SMPLRT_DIV, 0x07);
    }

    MSB = mpu6050_Read(0x3B);
    LSB = mpu6050_Read(0x3C);
    uint16_t acc_x = (MSB << 8) | LSB; // Read X accelerometer

    MSB = mpu6050_Read(0x3D);
    LSB = mpu6050_Read(0x3E);
    uint16_t acc_y = (MSB << 8) | LSB; // Read Y accelerometer

    MSB = mpu6050_Read(0x3F);
    LSB = mpu6050_Read(0x40);
    uint16_t acc_z = (MSB << 8) | LSB; // Read Z accelerometer

    sprintf(buf, "who am i = %d\nAccelerometer:\n    X = %d\n    Y = %d\n    Z = %d\n", who_am_i, MSB, LSB, acc_z);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, buf, CUSTOM_HID_EPIN_SIZE);

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
