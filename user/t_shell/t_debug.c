/**
  ******************************************************************************
  * @file           : t_debug.c
  * @brief          : shell �ײ�ӿ�
  ******************************************************************************
  *
  * Copyright (c) X.N. 
  * All rights reserved.
  *
  * ��Ҫ�������溯��
  *
	* 1. LOG_Init(void);
	*        
	* 2. int DbgConsole_Printf(const char *fmt_s, ...);
	*    
	* 3. int DbgConsole_Scanf(char *fmt_ptr, ...);
	*  
	* 4. void t_recv_data(uint8_t *data,uint32_t len);
	*    
  * 5. void t_send_data(uint8_t *data,uint32_t len);
	*
	*
	* ��Ҫ��������ȫ�ֱ���
	* 1. g_MutexLogHandle �����ź��� δ�õ�
	* 2. g_CountingSemLogHandle �����ź���  �ж��з� Ӧ������
	*
	*
	*
  * ��������                                                ������:
	* �޸ļ�¼
	*    1.������xxxxxxxxxx��ɾ����xxxxxxxx
	*      �Ż���xxxxxxxxxx                      ����:        �޸���:
  *  
  ******************************************************************************
  */


#include "main.h"
#include "t_debug.h"
#include <stdarg.h>
#include <stdlib.h>
#include "t_str.h"

#ifdef RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "main.h"
#include "cmsis_os.h"
extern SemaphoreHandle_t MutexMotorHandle;
extern SemaphoreHandle_t CountingSemLogHandle;
#endif
#define RX_BUFFER_SIZE	64
static uint8_t PAfifo[RX_BUFFER_SIZE];
static uint32_t tail=0;
static uint32_t head=RX_BUFFER_SIZE-1;
static int8_t in(uint8_t a)
{
	uint32_t p;
	p=tail;
	if(p==head)return -1;
	else
	{
		PAfifo[p] = a;
		tail = p<RX_BUFFER_SIZE-1 ? p+1 : 0;
		return 0;
	}
}


static int8_t Fifoout(uint8_t *a)
{
	uint32_t p;
	p= head;
	p= p<RX_BUFFER_SIZE-1 ? p+1:0;
	if(p==tail)
	{
		return -1;
	}
	else
	{
		*a = PAfifo[p];
		head = p;
		return 0;
	}
}

static void Uart1_Char(unsigned char i) 
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, i);
}

static void Uart1_MultiData(uint8_t *p,uint16_t len)
{
	while(len--)
	{
		Uart1_Char(*p++);
	}
}

/**
  * @brief  �����ж�
  *         ��ȡ�ж�����,����FIFO
  * @param  ��
  * @return ��
  * @retval ��
  */
void USART1_IRQHandler(void)
{
	uint8_t res=0;
  /* USART in mode Idle interrupt --------------------------------------------------*/
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
			res = USART1->RDR;			
		#ifdef RTOS
			BaseType_t xHigherPriorityTaskWoken;
			xHigherPriorityTaskWoken = pdFALSE;
			in(res);
			xSemaphoreGiveFromISR( CountingSemLogHandle, &xHigherPriorityTaskWoken );
		#else
			in(res);
		#endif
	}  

}

static int LOG_ReadOneCharacter(uint8_t *ch)
{
	uint8_t t=0;
	#ifdef RTOS
	if( xSemaphoreTake( CountingSemLogHandle, portMAX_DELAY ) == pdTRUE )
	{
		if(0==Fifoout(&t))
		*ch = t;
		return 0;
	} 
	else
	{
		return -1;
	}
	#else
	while(Fifoout(&t)<0)
	{
		delay_ms(5);
	}
	*ch = t;
	return 0;
	#endif
}


static int LOG_ReadLine(uint8_t *buf,uint8_t size)
{

    int i = 0;


    for (i = 0; i < size; i++)
    {
        /* recieve one char every time */
        if (LOG_ReadOneCharacter(&buf[i]) != 0)
        {
            return -1;
        }
#if DEBUG_CONSOLE_ENABLE_ECHO_FUNCTION
        LOG_EchoCharacter(&buf[i], false, &i);
#endif
        /* analysis data */
        if ((buf[i] == '\r') || (buf[i] == '\n'))
        {
            /* End of Line. */
            if (i == 0)
            {
                buf[i] = '\0';
                i = -1;
            }
            else
            {
                break;
            }
        }
    }
    /* get char should not add '\0'*/
    if (i == size)
    {
        buf[i] = '\0';
    }
    else
    {
        buf[i + 1] = '\0';
    }


    return i;
}


int DbgConsole_Scanf(char *fmt_ptr, ...)
{
    va_list ap;
    int result;
    char scanfBuf[DEBUG_CONSOLE_SCANF_MAX_LOG_LEN + 1U] = {0U};

    /* scanf log */
    LOG_ReadLine((uint8_t *)scanfBuf, DEBUG_CONSOLE_SCANF_MAX_LOG_LEN);
    /* get va_list */
    va_start(ap, fmt_ptr);
    /* format scanf log */
    result = StrFormatScanf(scanfBuf, fmt_ptr, ap);

    va_end(ap);

    return result;
}

static void DbgConsole_RelocateLog(char *buf, int32_t *indicator, char val, int len)
{
    int i = 0;

    for (i = 0; i < len; i++)
    {
        if ((*indicator + 1) >= DEBUG_CONSOLE_PRINTF_MAX_LOG_LEN)
        {
           // LOG_Push((uint8_t *)buf, *indicator);
					Uart1_MultiData((uint8_t*)buf,*indicator);
					//	HAL_UART_Transmit(&huart3,(uint8_t*)buf,*indicator,0xffff);
            *indicator = 0U;
        }
        buf[*indicator] = val;
        (*indicator)++;
    }
}

int DbgConsole_Printf(const char *fmt_s, ...)
{
    va_list ap;
    int logLength = 0U, result = 0U;
    char printBuf[DEBUG_CONSOLE_PRINTF_MAX_LOG_LEN] = {0U};
    va_start(ap, fmt_s);
    /* format print log first */
    logLength = StrFormatPrintf(fmt_s, ap, printBuf, DbgConsole_RelocateLog);
    /* print log */
   // result = LOG_Push((uint8_t *)printBuf, logLength);
		Uart1_MultiData((uint8_t*)printBuf,logLength);
	//	result = HAL_UART_Transmit(&huart3,(uint8_t*)printBuf,logLength,0xffff);
    va_end(ap);

    return result;
}
/**
  * @brief  ���ڷ�������
  * @param  *a:����ָ��
  * @param  len:���ݳ���
  * @return ��
  * @retval ��
  */
void t_send_data(uint8_t *data,uint32_t len)
{
	Uart1_MultiData((uint8_t*)data,len);
}
/**
  * @brief  ���ڽ���
  * @param  *data:����ָ��
  * @param  len:���ݳ���
  * @return ��
  * @retval ��
  */
void t_recv_data(uint8_t *data,uint32_t len)
{
	uint32_t i=0;
	if(len==1)
	{
		if(LOG_ReadOneCharacter(data)<0)
			*data=(uint8_t)-1;
	}
	else{
		for(i=0;i<len;i++)
		{
			LOG_ReadOneCharacter(data+i);
		}
	}
}


/**
  * @brief  ����1��ʼ��
  * @param  ��
  * @return ��
  * @retval ��
  */
void Usart1_config()
{
 GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	
	/* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	/* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);
	
	/* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 /* Enable USART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART configuration */
  USART_Init(USART1, &USART_InitStructure);
	
	/**/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	

	/*Enable IDLE interrupt*/
	//USART_ITConfig(USART2, USART_IT_IDLE,ENABLE);	
	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
	
	//USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);	
}

/**
  * @brief  shell�ײ㴮�ڳ�ʼ��
  *         �����Ż������ ���������ź���
  *         �󶨻ص�����������3�ж�
  *         ��������3
  * @param  ��
  * @return ��
  * @retval ��
  */
void LOG_Init()
{

	Usart1_config();
}





