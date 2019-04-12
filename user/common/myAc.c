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

#include "myAc.h"
#include "string.h"
#include "stdlib.h"

//建议用 scanf实现
///**
//  * @brief  ASCII转bin  
//  *         '0'->0x00   'a'->0x0a   'A'->0x0a
//  * @param  numChar: '0'~'9' 'a'~'f' 'A'~'F'
//	* @return ok:0~16  err:0xff
//  * @retval 
//  */
//uint8_t NumChar2Bin(char numChar)
//{
//	
//	if(numChar>=0x30 && numChar<=0x39)
//	{
//		return numChar-0x30;
//	}
//	if(numChar>0x40&&numChar<0x47)
//	{
//		return numChar-0x41 + 10;
//	}
//	if(numChar>0x60&&numChar<0x67)
//	{
//		return numChar-0x61 +10;
//	}
//	return 0xff;
//}
//
///**
//  * @brief  bin转HEX ASCII  
//  *         '0'->0x00   'a'->0x0a   'A'->0x0a
//  * @param  a: 0<=a<16
//  * @retval ok:'0'~'9' 'A'~'F'  err:0xff
//  */
//char Bin2HexChar(uint8_t a)
//{
//	if(a<10)return a+0x30;
//	if(a>=10 ||a<16) return (a-10)+0x41;
//	return 0xff;
//}
//
///*
//bits = 1 2 3 4 5 6 7 8 
//*/
//uint8_t Bin2HexStr(uint32_t bin,uint8_t bits,char* ret)
//{
//	int i=0;
//	if(bits==0||bits>8)return 0xff;
//	for(i=0;i<bits;i++)
//	{
//		if((ret[i] = Bin2HexChar(bin>>((bits-i+1)*4))&0xf)==0)return 0xff;
//	}
//	return 0;
//}

///*
//bits = 1 2 3 4 5 6 7 8 
//*/
//uint8_t HexStr2Bin(char *a ,uint8_t bits,void *ret)
//{
//	int i;
//	uint32_t temp=0;
//	uint8_t t;
//	for(i=0;i<bits;i++)
//	{
//		t = NumChar2Bin(a[i]);
//		if(t==0xff)return 0xff;
//		temp |= t<<(i*4); 
//	}
//	return 0;
//}


/**
  * @brief  在 字符串b中查找a  
  *         
  * @param  *a:目标串
  * @param  *b:匹配串
  * @param  *c:匹配结果  每一个结果在*a中的偏移地址
  * @param  *d:期待匹配的结果个数   实际匹配的个数
	* @return ok:0  err:0xff
  * @retval 
  */
uint8_t str_find(char *a, char *b,uint16_t *c,uint8_t *d)
{
	uint16_t i,j,flag=0,m=0;
	uint16_t lena  = strlen(a);
	uint16_t lenb  = strlen(b);
	for(i=0;i<lena;i++)
	{
		   if(b[0]==a[i] && (i+lenb<=lena))
			 {
					for(j=0;j<lenb;j++)
				 {				  
						if(b[j]!=a[i+j] && b[j]!='?')
						{
							flag=1;
							break;
						}										
				 }
				 if(flag==0)
				 {
						c[m++]=i;
					  if(m>*d)break;
				 }
				 else
				 {
						flag = 0;
				 }
			 }
	}
	*d=m-1;
	if(m>0)return 0;
	else return 0xff;
}


/**
  * @brief  分割字符串a   分隔符b  不可重入
  *         最多支持分割成LIST_MAX_NUM个子串
  *         返回的子串用完后需要调用FreeStringList释放释放
  * @param  *a: 目标
  * @param  *b：分隔符
	* @return _stringList* 子字符串
  * @retval 
  */
_stringList *mtStrDiv(char *a, char *b)
{
	char *c;
	_stringList *list;
	list=malloc(sizeof(_stringList));
	if(list==NULL)return NULL;
	list->num=0;
	c = strtok(a,b);
	while(c!=NULL)
	{
		list->d[list->num] = malloc(strlen(c));	
		if(list->d[list->num]==NULL)
		{
			FreeStringList(list);
			return NULL;
		}
		strcpy(list->d[list->num],c);
		c = strtok(NULL,b);
		list->num++;
		if(list->num==LIST_MAX_NUM)break;
	}
	if(list->num==0)
	{
		FreeStringList(list);
		return NULL;
	}
	return list;
}

/**
  * @brief  释放子串空间
  *
  * @param  *list: 子串
	* @return 无
  * @retval 无
  */
void FreeStringList(_stringList *list)
{
	uint8_t i=0;
	for(i=0;i<list->num;i++)
	free(list->d[i]);
	free(list);
}

//fifo demo
//#define	FIFO_SIZE	64
//static uint8_t PAfifo[FIFO_SIZE];
//static uint16_t tail=0;
//static uint16_t head=FIFO_SIZE-1;
//int8_t in(uint8_t a)
//{
//	uint16_t p;
//	p=tail;
//	if(p==head)return -1;
//	else
//	{
//		PAfifo[p] = a;
//		tail = p<FIFO_SIZE-1 ? p+1 : 0;
//		return 0;
//	}
//}
//uint8_t Fifoout(uint8_t *a)
//{
//	uint16_t p;
//	p= head;
//	p= p<FIFO_SIZE-1 ? p+1:0;
//	if(p==tail)
//	{
//		return 1;
//	}
//	else
//	{
//		*a = PAfifo[p];
//		head = p;
//		return 0;
//	}
//}
