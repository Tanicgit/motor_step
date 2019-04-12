/**
  ******************************************************************************
  * @file           : myAc.c
  * @brief          : ���û�������
  ******************************************************************************
  *
  * Copyright (c) X.N. 
  * All rights reserved.
  *
  * ��Ҫ�������溯��
  *
	* 1. uint8_t str_find(char *a, char *b,uint16_t *c,uint8_t *d)
	*    �ַ�������
	* 2. _stringList *mtStrDiv(char *a, char *b)
	*    �ַ����ָ�
  *
  * ��������                                                ������:
	* �޸ļ�¼
	*    1.������xxxxxxxxxx��ɾ����xxxxxxxx
	*      �Ż���xxxxxxxxxx                      ����:        �޸���:
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
