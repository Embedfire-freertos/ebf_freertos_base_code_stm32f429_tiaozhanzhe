/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_led.h"
#include "bsp_debug_usart.h"
#include "bsp_key.h"
#include "bsp_exti.h"

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
//systick�жϷ�����
void SysTick_Handler(void)
{	
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif  /* INCLUDE_xTaskGetSchedulerState */  
    
    xPortSysTickHandler();
    
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  }
#endif  /* INCLUDE_xTaskGetSchedulerState */
}



/* ���������ⲿ���� & ��ֵ�ź��� */
extern QueueHandle_t Test_Queue;
extern SemaphoreHandle_t BinarySem_Handle;

static uint32_t send_data1 = 1;
static uint32_t send_data2 = 2;

/*********************************************************************************
  * @ ������  �� KEY1_IRQHandler
  * @ ����˵���� �жϷ�����
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  ********************************************************************************/
void KEY1_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
  //ȷ���Ƿ������EXTI Line�ж�
  uint32_t ulReturn;
  /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
    /* ������д�루���ͣ��������У��ȴ�ʱ��Ϊ 0  */
		xQueueSendFromISR(Test_Queue, /* ��Ϣ���еľ�� */
											&send_data1,/* ���͵���Ϣ���� */
											&pxHigherPriorityTaskWoken);
		
		//�����Ҫ�Ļ�����һ�������л�
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		
		//����жϱ�־λ
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

/*********************************************************************************
  * @ ������  �� KEY1_IRQHandler
  * @ ����˵���� �жϷ�����
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  ********************************************************************************/
void KEY2_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
  uint32_t ulReturn;
  /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
    /* ������д�루���ͣ��������У��ȴ�ʱ��Ϊ 0  */
		xQueueSendFromISR(Test_Queue, /* ��Ϣ���еľ�� */
											&send_data2,/* ���͵���Ϣ���� */
											&pxHigherPriorityTaskWoken);
		
		//�����Ҫ�Ļ�����һ�������л�
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		
		//����жϱ�־λ
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

/*********************************************************************************
  * @ ������  �� DEBUG_USART_IRQHandler
  * @ ����˵���� �����жϷ�����
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  ********************************************************************************/
void DEBUG_USART_IRQHandler(void)
{
  uint32_t ulReturn;
  /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();

	if(USART_GetITStatus(DEBUG_USART,USART_IT_IDLE)!=RESET)
	{		
		Uart_DMA_Rx_Data();       /* �ͷ�һ���ź�������ʾ�����ѽ��� */
		USART_ReceiveData(DEBUG_USART); /* �����־λ */
	}	 
  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}


