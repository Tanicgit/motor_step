#ifndef __MOTOR_H
#define __MOTOR_H
#include "main.h"



#define M_A_PORT	GPIOA
#define M_A_PIN 	GPIO_Pin_0
#define M_B_PORT	GPIOA
#define M_B_PIN 	GPIO_Pin_1
#define M_C_PORT	GPIOA
#define M_C_PIN 	GPIO_Pin_2
#define M_D_PORT	GPIOA
#define M_D_PIN 	GPIO_Pin_3

#define MOTOR_ALL_PIN	  (M_A_PIN|M_B_PIN|M_C_PIN|M_D_PIN)


#define MOTOR_NONE	0
#define MOTOR_STOP	1
#define MOTOR_ADD	2
#define MOTOR_DEL	3
#define MOTOR_RUN	4



void MOTOR_Config(void);
void motor_stop(void);
void motor_start(uint16_t speed,int position);
void motor_get_status(uint8_t *sta,int32_t *dir,int32_t *step);
#endif
