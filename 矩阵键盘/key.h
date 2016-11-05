/*
	NAME:	key.h
	DATE:	2016.10.28
	AUTHOR:	牛亚犇
	E-MAIL:	niuyabeng@126.com
*/

/*	
	说明:
	1、适用于4*4矩阵键盘
	2、采用查询方式扫描按键
	3、行反转法扫描按键
	4、每个按键支持3种工作方式
		1>单击:单击时间不能超过T_KEY_HOLD_MIN毫秒;
			   按键抬起后处理单击事件.
			   
		2>双击:两次单击的间隔不超过T_KEY_DOUBLE_CLICK毫秒;
			   第二次单击时间不能超过T_KEY_HOLD_MIN毫秒;
			   第二次单击按键抬起后处理双击事件。
			   
		3>长按:单击时间超过T_KEY_HOLD_MIN毫秒;
			   按键抬起后处理长按事件。
			   
	
	测试平台:STC90C516RD+ 、晶振11.0592MHz。
*/

#ifndef _KEY_H_
#define _KEY_H_

#include <reg51.h>

#define KEY_PORT			P2
#define KEY_LINE    		0x0f 
#define KEY_COLUMN			0xf0

#define MODE_NONECLICK		0x00
#define MODE_ONECLICK		0x01
#define MODE_DBCLICK 		0x02
#define MODE_HOLD			0x03

//定时器定时1ms时
#define T_KEY_DOUBLE_CLICK	200		//双击时间间隔设置40ms
#define T_KEY_HOLD_MIN		1200	//按下时间超过1.2s为长按

typedef unsigned char u8;
typedef unsigned int  u16;			//51的short和int都是两个字节长度

/*
	t_key_dbck、t_key_hold、t_db_delay
*/
extern u16 t_key_dbck;		
extern u16 t_key_hold;
extern u16 t_db_delay;


void key_delay(u16 n_ms);					//消抖延时n_ms毫秒
u8 is_key_down(u8 *line,u8 *column);		//键按下返回1，没有返回0
u8 is_key_up(void);							//键抬起返回1，没有返回0
u8 key_mode_value(u8 *line,u8 *column);		//返回值有MODE_NONECLICK、MODE_ONECLICK、MODE_DBCLICK、MODE_HOLD
void t_flag_count_up(void);
/*
函数:void t_flag_count_up(void) 
	1>需在定时器中调用此函数。
	2>因为要在定时器中断函数中调用\
	  这里寄存器组使用默认using 0,可能会出现程序运行不正常的问题。\
	  测试时并没有出现问题。
	
	  如果出现了问题
	  1>>不要使用这个函数
	  2>>直接将函数体内的语句复制，粘贴到定时器中断函数中即可。
	  
定时器中调用如下:

void Timer0Interrupt(void) interrupt 1 //寄存器组使用默认0，测试没有出现问题
{
    TH0 = 0xFC;
    TL0 = 0x18;
	...;
	
	t_flag_count_up();
	
	...; 
}
*/

#endif
