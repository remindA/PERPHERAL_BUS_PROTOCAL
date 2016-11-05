/*
	NAME:	key.c
	DATE:	2016.10.28
	AUTHOR:	牛亚犇
	E-MAIL:	niuyabeng@126.com
*/
//
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

#include "key.h"

u16 t_key_dbck = 0;
u16 t_key_hold = 0;
u16 t_db_delay = 0;

void t_flag_count_up(void)
{
	if(t_key_dbck < T_KEY_DOUBLE_CLICK)
		t_key_dbck++;
	else 
		t_key_dbck = T_KEY_DOUBLE_CLICK;		//避免出现t_key_double_click溢出
	
	if(t_key_hold < T_KEY_HOLD_MIN)
		t_key_hold++;
	else
		t_key_hold = T_KEY_HOLD_MIN;
	
	if(t_db_delay < T_KEY_DOUBLE_CLICK)
		t_db_delay++;
	else
		t_db_delay = T_KEY_DOUBLE_CLICK;
}

void key_delay(u16 n_ms)
{
	u8 a,b,c;
    while(n_ms--)
	{
		for(c=1;c>0;c--)
			for(b=142;b>0;b--)
				for(a=2;a>0;a--);
	}
}

u8 is_key_down(u8 *line,u8 *column)
{
	u8 keytemp = 0;
	
	KEY_PORT = KEY_LINE;
	keytemp  = KEY_PORT;
	keytemp &= 0x0f;
	if(keytemp != 0x0f)   //有键盘按下
	{
		key_delay(10);
		if(keytemp != 0x0f)  //消抖后按键still down
		{
			
			switch(keytemp)
			{
				//保存列号码
				case 0x0e: *column = 4; break;
				case 0x0d: *column = 3; break;
				case 0x0b: *column = 2; break;
				case 0x07: *column = 1; break;
				default  : *column = 0; break;
			}
				
			KEY_PORT = KEY_COLUMN;				//反转
			keytemp  = KEY_PORT;
			keytemp &= 0xf0;
			switch(keytemp)
			{
				//保存行号码
				case 0xe0: *line = 4; break;
				case 0xd0: *line = 3; break;
				case 0xb0: *line = 2; break;
				case 0x70: *line = 1; break;
				default  : *line = 0; break;
			}
				
			return 1;		//按键确实按下就返回1表示按键确实按下，并且编码识别成功。
				
		}
		else return 0;	
	}
	else return 0;	
}


u8 is_key_up(void)
{
	u8 keytemp = 0;
	
	KEY_PORT = KEY_LINE;
	keytemp  = KEY_PORT;
	keytemp &= 0x0f;
	if(0x0f == keytemp)   
	{
		key_delay(10);
		if(0x0f == keytemp)  //消抖后按键still up
		{
			return 1;
		}
		else 
			return 0;
	}
	else 
		return 0;
			
}


u8 key_mode_value(u8 *line,u8 *column)
{
	static unsigned char linetemp1 	 = 0;
	static unsigned char columntemp1 = 0;
	static unsigned char linetemp2 	 = 0;
	static unsigned char columntemp2 = 0;
	static unsigned char t_temp		 = 0;
		   unsigned char KEYDOWN 	 = 0;
	
	if(is_key_down(line,column))	//1、按键按下了
	{
		linetemp2 	= linetemp1;			
		columntemp2 = columntemp1;
		linetemp1 	= *line;
		columntemp1 = *column;
		t_key_hold 	= 0;
		t_temp 		= t_key_dbck;
		KEYDOWN 	= 1;
	}
	else							//2、按键没按下直接返回MODE_NONECLICK，后面的的都不用处理了。
		return MODE_NONECLICK;
	
	if(KEYDOWN==1)
	{
		while(1)
		{
			if(is_key_up())								//按键抬起了(没抬起就死循环等着抬起)
			{
				t_key_dbck = 0;
				t_db_delay = 0;
				
				if(t_key_hold>=T_KEY_HOLD_MIN)
					return MODE_HOLD;					//③长按事件；
				
				while(t_db_delay<T_KEY_DOUBLE_CLICK)				
				{
					if(is_key_down(line,column))		//T_DB_DELAY时间内按键又按下，很可能是连击，二进宫
						return key_mode_value(line,column);
				}
					
				if((t_temp<T_KEY_DOUBLE_CLICK)&&(linetemp2==linetemp1)&&(columntemp2==columntemp1))
					return MODE_DBCLICK;				//①连击事件；
				else	
					return MODE_ONECLICK;				//②单击事件；
			}	
		}
	}
	else
		return MODE_NONECLICK;
}
