#include "motor.h"
#include "motor_data.h"


//PA0~3  0~7 顺时针
uint16_t motor_step_value[]={M_A_PIN,M_A_PIN|M_B_PIN,M_B_PIN,M_B_PIN|M_C_PIN,M_C_PIN,M_C_PIN|M_D_PIN,M_D_PIN,M_D_PIN|M_A_PIN};
__IO int32_t motor_step=0;//转子当前位置  只在中断段里面改变 其他位置不允许修改
__IO int32_t purpose_step=0;//转子目标位置 用户通过API设置
__IO int32_t motor_dir = 0;//-1 0 1 运转方向
uint8_t motor_sta=MOTOR_STOP;//状态
uint8_t need_add_del=0;//是否需要加减速过程
__IO uint32_t motor_t_step=0;//临时用  记录加速减速了多少步
__IO int32_t motor_purpose_speed=0;//目标速度


extern uint16_t motor_data[];
/*
a 0~7
*/
void motor_set_step(uint16_t a)
{
	uint16_t temp=0;
	temp = GPIOA->ODR;
	temp &=  ~MOTOR_ALL_PIN;
	temp |= motor_step_value[a];
	GPIOA->ODR = temp;
}

void motor_irq(void)
{
	uint32_t temp=0;
	uint16_t arr=0;
	if(motor_step!=purpose_step)
	{
		motor_set_step(motor_step&0x07);
		motor_step += motor_dir;
		if(need_add_del==1)
		{
			if(motor_sta==MOTOR_ADD)
			{
				
				if(motor_t_step*MOTOR_RATE<MOTOR_ADD_DEL_STEP)//加速未走完
				{
					arr = MOTOR_START_SPEED - motor_data[motor_t_step];
					if(arr>motor_purpose_speed)//未达到目标速度
					{
						TIM3->ARR = arr;
					}
					else//已经达到目标速度 退出加速
					{
					  TIM3->ARR = motor_purpose_speed;
						motor_sta = MOTOR_RUN;
						motor_t_step=0;
					}
				}
				else//已经达到最大速度  开环加速跑完就当达到了设定的最大速度  退出加速
				{
					motor_sta = MOTOR_RUN;
					motor_t_step=0;
					TIM3->ARR = MOTOR_MAX_SPEED;
				}
				motor_t_step++;
			}	
			else if(motor_sta==MOTOR_RUN)
			{
				temp = purpose_step > motor_step ? purpose_step-motor_step : motor_step-purpose_step;
				if(temp<MOTOR_ADD_DEL_STEP)
				{
					motor_t_step = 0;
					motor_sta = MOTOR_DEL;
				}	
			}
			else if(motor_sta==MOTOR_DEL)
			{
				
				if(motor_t_step*MOTOR_RATE<=MOTOR_ADD_DEL_STEP)//减速中
				{
					arr = motor_purpose_speed + motor_data[motor_t_step];
					TIM3->ARR = arr;
				}
				else
				{
						motor_sta = MOTOR_NONE;//减速完成
						motor_t_step=0;			
				}		
				motor_t_step++;    //N步完成减速 相当于
			}	
			else
			{
				//等待停止
			}
		}
		
	}
	else
	{
		motor_stop();
	}
}

//未跑完之前调用 属于紧急停止
void motor_stop()
{
	TIM_Cmd(TIM3, DISABLE);
	GPIOA->ODR &= ~MOTOR_ALL_PIN;
	
	motor_dir = 0;
	motor_sta = MOTOR_STOP;
	purpose_step = 0;
}

void motor_start(uint16_t speed,int position)
{
	uint32_t temp;
	if(position==motor_step)return;
	
	if(position>motor_step)
	{
		motor_dir=1;
	}
	else
	{
		motor_dir=-1;
	}
	purpose_step = position;
	

	temp =	purpose_step > motor_step ? purpose_step-motor_step : motor_step-purpose_step;
	if(temp<MOTOR_ADD_DEL_STEP)
	{
		motor_sta = MOTOR_RUN;//运行距离过短  不加速
		motor_purpose_speed = MOTOR_START_SPEED;//
		need_add_del = 0;
	}
	else
	{
		motor_t_step = 0;
		motor_sta = MOTOR_ADD;
		motor_purpose_speed = speed;
		need_add_del=1;
	}
	
	TIM3->ARR = MOTOR_START_SPEED;
	TIM_Cmd(TIM3, ENABLE);	
}

void motor_step_clear()
{
	motor_stop();
	motor_step = 0;
}


void motor_get_status(uint8_t *sta,int32_t *dir,int32_t *step)
{
	*sta = motor_sta;
	*dir = motor_dir;
	*step = motor_step;
}


void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1000;
  TIM_TimeBaseStructure.TIM_Prescaler = 48;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
  /* TIM Interrupts enable */
  TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, DISABLE);
}

void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		motor_irq();	
	}
}


void MOTOR_Config()
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = MOTOR_ALL_PIN;//PA0~3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIOA->ODR &= ~0X0F;//拉低
	
	TIM_Config();
}



