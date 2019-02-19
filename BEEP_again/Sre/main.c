/*******************************************************************************
* 文件名：main.c
* 描  述：
* 作  者：CLAY
* 版本号：v1.0.0
* 日  期: 2019年2月19日
* 备  注：修改后的LCD例程
*         按键B1切换LED1状态并且蜂鸣器鸣叫0.1s
*		  按键B2按下发生长鸣;按键B3停止鸣叫
*******************************************************************************
*/

#include "stm32f10x.h"
#include "lcd.h"
#include "e2prom.h"
#include "stdio.h"
#include "i2c.h"
#include "adc.h"
#include "rtc.h"
#include "usart2.h"
#include "pwm.h"
#include "pwm_oc.h"
#include "pwm_ic.h"
#include "timer.h"
#include "led.h"
#include "key.h"
#include "beep.h"

u32 TimingDelay = 0;
u8 RxdCnt = 0;
u8 RxdOver = 0;
u8 RxdBuf[20];
void Delay_Ms(u32 nTime);
u8 RTC_Flag = 0;

//Main Body
int main(void)
{
	
	STM3210B_LCD_Init();
	LCD_Clear(Blue);
	LCD_SetBackColor(Blue);
	LCD_SetTextColor(White);
	
	SysTick_Config(SystemCoreClock/1000);
	
	//PWM_Init(500, 60);//500Hz 60%方波//PA1
	PWM_OC_Init(500, 60);//500Hz 60%方波 PA1
	PWM_IC_Init();//PA7
	
	TIM4_Init(2000, 72);//定时2ms
	LED_Init();//LED配置
	KeyInit();
	BeepInit();
	
	while(1)
	{
		KeyDriver();
	}
}

void KeyAction(int code)
{
	if(code == 1)//按下B1，LED1状态改变
	{
		GPIOD->ODR |= (1<<2);//使能573
		GPIOC->ODR ^= (1<<8);//LED1改变
		GPIOD->ODR &= (1<<2);//失能573
		Beep(100);//伴随滴的一声
	}
	else if(code == 2)//B2
	{
		Beep(-1);//长鸣
	}
	else if(code == 3)
	{
		Beep(0);//停止鸣叫
	}
}

//
void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
