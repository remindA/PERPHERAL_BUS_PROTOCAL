/*
	NAME:	伪代码.c
	DATE:	2016.10.28
	AUTHOR:	牛亚犇
	E-MAIL:	niuyabeng@126.com
*/

/*	
	说明:
	下面都是草稿，最初的程序运行逻辑，有不少错误\其他人最好不要看，本人最好也不要看
*/

u8 key_mode_value(u8 *line,u8 *column)
{
	su16 linetemp1 	 = 0;
	su16 columntemp2 = 0;
	su16 linetemp2 	 = 0;
	su16 columntemp2 = 0;
	
	if(is_key_down(line,column))
	{
		linetemp2 = linetemp1;			
		columntemp2 = columntemp1;
		linetemp1 = *line;
		columntemp1 = *column;
		t_hold = 0;
		t_temp = t_dbck;
		KEYDOWN = 1;
	}
	else
		return NONECLICK;
	
	
	
	if(KEYDOWN==1)
	{
		while(1)
		{
			if(is_key_up())										//1、按键抬起了
			{
				t_dbck = 0;
				while(t_db_delay<T_DB_DELAY)				
				{
					if(is_key_down(line,column))				//T_DB_DELAY时间内按键又按下，很可能是连击，二进宫
						return key_mode_value(line,column);
				}
					
				if((t_temp<T_KEY_DOUBLE_CLICK/*400ms*/)&&(linetemp2==linetemp1)&&(columntemp2==columntemp1))
					return DBCLICK;								//①连击事件；
				else
					return ONECLICK;							//②单击事件；
			}
				
			else												//2、按键还没抬起
			{
				if(t_hold>T_HOLD_MIN)
					return HOLD;								//③长按事件；
			}
				
		}
		
	}	
}
