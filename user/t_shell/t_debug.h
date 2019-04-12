/**
  ******************************************************************************
  * @file           : t_debug.h
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
#ifndef __T_DEBUG_H
#define __T_DEBUG_H

#include "stdint.h"


typedef struct{
	char name[16];
	int (*fun)(char *f,...);
}_cmd;

#define DEBUG_CONSOLE_SCANF_MAX_LOG_LEN 64
#define DEBUG_CONSOLE_PRINTF_MAX_LOG_LEN 128
#define Ac_log(...) DbgConsole_Printf(__VA_ARGS__)

void LOG_Init(void);

int DbgConsole_Printf(const char *fmt_s, ...);
int DbgConsole_Scanf(char *fmt_ptr, ...);

void t_recv_data(uint8_t *data,uint32_t len);
void t_send_data(uint8_t *data,uint32_t len);




#endif
