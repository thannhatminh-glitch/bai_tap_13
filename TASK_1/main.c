#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"                   // RTOS:Core&&Cortex-M
#include "stm32f10x_exti.h"             // Device:StdPeriph Drivers:EXTI
#include "stm32f10x_rcc.h"              // Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Device:StdPeriph Drivers:GPIO
#include "task.h"                       // RTOS:Core&&Cortex-M
#include "semphr.h"                     // RTOS:Core&&Cortex-M

static SemaphoreHandle_t xButtonSem = NULL;

void GPIO_Config(void);
void exti0_config(void);
void task_blink_led(void *pvParameters);
void task_handle(void *pvParameters);

int main(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	
	GPIO_Config();
	exti0_config();
	
	xButtonSem = xSemaphoreCreateBinary();
	
	xTaskCreate(task_blink_led, "BlinkPC13", 128, NULL, 1, NULL);
	xTaskCreate(task_handle, "ButtonPA0", 128, NULL,2, NULL);
	
	vTaskStartScheduler();
	
	while(1);
}

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef gpio;
	
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &gpio);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void exti0_config(void)
{
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	
	exti.EXTI_Line = EXTI_Line0;
	exti.EXTI_LineCmd = ENABLE;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&exti);
	
	EXTI_ClearITPendingBit(EXTI_Line0);
	
	nvic.NVIC_IRQChannel = EXTI0_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 10;
	nvic.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic);
}

void EXTI0_IRQHandler(void)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE; 
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line0);
			if(xButtonSem != NULL)
			{
				xSemaphoreGiveFromISR(xButtonSem, &xHigherPriorityTaskWoken);
			}
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// task blink led chu ky 1s;
void task_blink_led(void *pvParameters)
{
	(void) pvParameters;
	TickType_t xDelay = pdMS_TO_TICKS(500);
	while(1)
	{
		GPIOC -> ODR ^= GPIO_Pin_13;
		vTaskDelay(xDelay);
	}
}

void task_handle(void *pvParameters)
{
	(void) pvParameters;
	while(1)
	{
		if(xSemaphoreTake(xButtonSem, portMAX_DELAY) == pdTRUE)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			vTaskDelay(pdMS_TO_TICKS(1000));
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		}
	}
}
