/**
  ******************************************************************************
  * @file           : myAc.c
  * @brief          : 常用基础函数
  ******************************************************************************
  *
  * Copyright (c) X.N. 
  * All rights reserved.
  *
  * 主要包含下面函数
  *
	* 1. uint8_t str_find(char *a, char *b,uint16_t *c,uint8_t *d)
	*    字符串查找
	* 2. _stringList *mtStrDiv(char *a, char *b)
	*    字符串分割
  *
  * 创建日期                                                创建人:
	* 修改记录
	*    1.增加了xxxxxxxxxx，删除了xxxxxxxx
	*      优化了xxxxxxxxxx                      日期:        修改人:
  *  
  ******************************************************************************
  */
#ifndef __MYAC_H
#define __MYAC_H
#ifdef __cplusplus
		extern "C" {
#endif 			
#include "stdint.h"	
		
			
#define LIST_MAX_NUM	10
typedef struct{
	uint8_t num;
	char *d[LIST_MAX_NUM];
}_stringList;


int Ac_pf(char *format,...);
			
//uint16_t Crc16_ModbusFarst(uint8_t *puchMsg,uint16_t usDataLen);	
//uint16_t Crc16_Modbus(uint8_t *puchMsg,uint16_t usDataLen);
//uint8_t U8checkSum(uint8_t *a,uint8_t len);	
//uint8_t Bin2HexStr(uint32_t bin,uint8_t bits,char* ret);			
//uint8_t HexStr2Bin(char *a ,uint8_t bits,void *ret);
uint8_t str_find(char *a, char *b,uint16_t *c,uint8_t *d);
_stringList *mtStrDiv(char *a, char *b);
void FreeStringList(_stringList *list);
/**/			
			
#ifdef __cplusplus
				}
    #endif
#endif
