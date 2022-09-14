/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include "rthw.h"
#include "rtthread.h"
#include "../drv/lcd.h"
#include "main.h"
#include "../drv/delay.h"
#include "../drv/icm20608.h"
#include "../drv/MPL/driver/eMPL/inv_icm20608.h"

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (15*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART1_UART_Init(void);
SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart1;
void SysTick_Handler(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{

    
    HAL_Init();
    SystemClock_Config();
    SystemCoreClockUpdate();

    /* 
     * 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RT_TICK_PER_SECOND);

    MX_GPIO_Init();
    MX_USART1_UART_Init();

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif

}

#ifdef RT_USING_CONSOLE


void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    __HAL_UNLOCK(&huart1);

    size = rt_strlen(str);

    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&huart1, (uint8_t *)(str + i), 1, 1);
    }
}
#endif

//}
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

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */


    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
//    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
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
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

    /*Configure GPIO pins : PC0 PC1 */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PE7 PE8 PE9 */
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : PB4 PB6 PB7 */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);
}


int main(void)
{
    delay_init(80);
    LCD_Init();
    ICM20608_Init();
    float pitch,roll,yaw; 		//
    short aacx,aacy,aacz;		//
    short gyrox,gyroy,gyroz;	//
    short temp;
    short p,r,y,left,right;
    u8 t=0;
    left=0;
    right=0;
//    LCD_ShowString(30, 80, 200, 16, 16, "Pandora STM32L4 IOT");
//    LCD_ShowString(30, 96, 200, 16, 16, "IIC ICM20608 TEST");
//    LCD_ShowString(30, 112, 200, 16, 16, "ATOM@ALIENTEK");
//    LCD_ShowString(30, 128, 200, 16, 16, "2018/10/27");
//    LCD_ShowString(30,144,200,16,16,"UPLOAD ON ");
    while(mpu_mpl_init())
    {
        LCD_ShowString(30,155,200,16,16,"ICM20608 Error");
        delay_ms(200);
        LCD_Fill(30,155,239,157+16,WHITE);
        delay_ms(200);
    }

    POINT_COLOR = BLUE;
    LCD_ShowString(30,162,200,16,16," Temp:    . C");
    LCD_ShowString(30,182,200,16,16,"Pitch:    . ");
    LCD_ShowString(30,202,200,16,16," Roll:    . ");
    LCD_ShowString(30,222,200,16,16," Yaw :    . ");
    while (1)
    {
        if(mpu_mpl_get_data(&pitch,&roll,&yaw)==0)
        {
            temp=MPU_Get_Temperature();
            MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
            MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
            p=pitch;
            r=roll;
            y=yaw;
            rt_kprintf("%d %d %d \n", p,r,y);

            if (r < -10){
                left=-r;
            }
            if (r>10){
                right=r*10;
            }

            if((t%10)==0)
            {
                if(temp<0)
                {
                    LCD_ShowChar(30+48,162,'-',16);
                }else LCD_ShowChar(30+48,162,' ',16);

                LCD_ShowNum(30+48+8,162,temp/100,3,16);
                LCD_ShowNum(30+48+40,162,temp%10,1,16);
                temp=pitch*10;
                if(temp<0)
                {
                    LCD_ShowChar(30+48,182,'-',16);
                    temp=-temp;
                }else LCD_ShowChar(30+48,182,' ',16);
                LCD_ShowNum(30+48+8,182,temp/10,3,16);
                LCD_ShowNum(30+48+40,182,temp%10,1,16);
                temp=roll*10;
                if(temp<0)
                {
                    LCD_ShowChar(30+48,202,'-',16);
                    temp=-temp;
                }else LCD_ShowChar(30+48,202,' ',16);

                LCD_ShowNum(30+48+8,202,temp/10,3,16);
                LCD_ShowNum(30+48+40,202,temp%10,1,16);
                temp=yaw*10;
                if(temp<0)
                {
                    LCD_ShowChar(30+48,220,'-',16);
                    temp=-temp;
                }else LCD_ShowChar(30+48,220,' ',16);
                LCD_ShowNum(30+48+8,220,temp/10,3,16);
                LCD_ShowNum(30+48+40,220,temp%10,1,16);
                t=0;

                if (right>0){
                    LCD_ShowString(30, 80, 200, 16, 16, "right");
                }
                if (left>0){
                    LCD_ShowString(30, 80, 200, 16, 16, "left ");
                }
                if (right==0 && left==0){
                    LCD_ShowString(30, 80, 200, 16, 16, "mid  ");
                }

            }
        }
        right=0;
        left=0;
//        LCD_ShowString(30, 80, 200, 16, 16, "mid  ");
        t++;
        delay_ms(20);

    }
    /* USER CODE END 3 */
}
#ifdef RT_USING_FINSH
char rt_hw_console_getchar(void)
{
    /* Note: the initial value of ch must < 0 */
    int ch = -1;

    if (__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE) != RESET)
    {
        ch = UartHandle.Instance->DR & 0xff;
    }
    else
    {
        rt_thread_mdelay(10);
    }
    return ch;
}
#endif
