//1602的初始化及写命令、写数据、读控制字、读数据程序探究
#include <reg51.h>
#include "lcd1602.h"



void delay1ms(unsigned int t)   
{
	unsigned char a=0,b=0,c=0;
	while(t--)
	{
		for(c=1;c>0;c--)
			for(b=142;b>0;b--)
				for(a=2;a>0;a--);
	}
}

void lcd1602_writecommand(const unsigned char Wcommand)
{
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_E = 0;
	DATAPORT = Wcommand;
	delay1ms(1);
	LCD1602_E = 1;  //LCD1602_E高脉冲
	delay1ms(1);
	LCD1602_E = 0;
}

void lcd1602_writedata(const unsigned char Wdata)
{
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_E = 0;
	DATAPORT = Wdata;
	delay1ms(1);
	LCD1602_E = 1;  //LCD1602_E高脉冲
	delay1ms(1);
	LCD1602_E = 0;
}

void lcd1602_init(void)
{
	lcd1602_writecommand(0x38);
	lcd1602_writecommand(0x08);  //关闭显示
	lcd1602_writecommand(0x01);
	lcd1602_writecommand(0x06);
	lcd1602_writecommand(0x0c);
	lcd1602_writecommand(0xC0);
}


void show_line(bit line, const unsigned char *str)
{
		if(0 == line)
			lcd1602_writecommand(0x80);            
		else
			lcd1602_writecommand(0xc0);
		while(*str != '\0')
		{
				lcd1602_writedata(*str);
				str++;
		}
}

