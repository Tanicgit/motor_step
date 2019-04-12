/**
  ******************************************************************************
  * @file           : t_debug.h
  * @brief          : shell 底层接口
  ******************************************************************************
  *
  * Copyright (c) X.N. 
  * All rights reserved.
  *
  * 主要包含下面函数
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
	* 主要包含以下全局变量
	* 1. g_MutexLogHandle 互斥信号量 未用到
	* 2. g_CountingSemLogHandle 计数信号量  中断中发 应用中收
	*
	*
	*
  * 创建日期                                                创建人:
	* 修改记录
	*    1.增加了xxxxxxxxxx，删除了xxxxxxxx
	*      优化了xxxxxxxxxx                      日期:        修改人:
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
