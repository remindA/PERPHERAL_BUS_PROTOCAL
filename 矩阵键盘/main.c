/*
	NAME:	main.c
	DATE:	2016.10.29
	AUTHOR:	牛亚犇
	E-MAIL:	niuyabeng@126.com
*/

/*
	说明:
		这个程序用来测试key.c和key.h
		
	测试平台:STC90C516RD+ 、晶振11.0592MHz。
*/

#include <reg51.h>
#include "key.h"
#include "lcd1602.h"
#include <string.h>


#define LED P1
sbit LED0 = P1^0;
sbit LED1 = P1^1;
sbit LED2 = P1^2;
sbit LED3 = P1^3;
sbit LED4 = P1^4;
sbit LED5 = P1^5;
sbit LED6 = P1^6;
sbit LED7 = P1^7;


u8 code keyboard[4][4] = {'1','2','3','+','4','5','6','-','7','8','9','/','*','0','#','='};

void InitT0(void);
void str(char *str,char c);

static unsigned char cnt = 0;

void main(void)
{
	u8 mode = 0;
	u8 line = 0,column = 0;
	char strtemp[16] = "\0";
	LED = 0xff;
	InitT0();
	lcd1602_init();
	while(1)
	{
		/*
		show_line(0,arry);
		show_line(1,arry);
		*/
		
		/*
		if(is_key_down(&line,&column))
			LED = 0x00;
		else LED = 0XFF;
		*/
		
		
		mode = key_mode_value(&line,&column);
		switch(mode)
		{
			case MODE_NONECLICK:
			{
				//LED = 0xff;
				break; 
			}			
			case MODE_ONECLICK:
			{
				/*
				str(strtemp,keyboard[line-1][column-1]);
				lcd1602_writecommand(0x01);
				show_line(0,"one click:");
				show_line(1,strtemp); */
				
				
				LED0 = 0;
				delay1ms(200);
				LED0 = 1;
				delay1ms(200); 
				break;
			}
			case MODE_DBCLICK:
			{
				/*
				lcd1602_writecommand(0x01);
				show_line(0,"double click:"); 
				memset(strtemp,0,16);
				cnt = 0;*/
				
				LED4 = 0;
				delay1ms(200);
				LED4 = 1;
				delay1ms(200);
				LED4 = 0;
				delay1ms(200);
				LED4 = 1;
				delay1ms(200); 
				
				break;
			}
			case MODE_HOLD:
			{
				/*
				lcd1602_writecommand(0x01);
				show_line(0,"holdkey:");			
				memset(strtemp,0,16);
				cnt = 0;*/
				
				LED7 = 0;
				delay1ms(2000);
				LED7 = 1;
				delay1ms(200); 
				break;
			}
			default: break;
		}
		
	}
}

void str(char *str,char c)
{
	if(cnt<=15)
	{
		*(str+cnt) = c;
		cnt++;
	}
	else
	{
		cnt = 0;
		memset(str,0,16);
		*(str+cnt) = c;
		cnt++;
	}
}


void InitT0(void)		
{
    TMOD = 0x01;
    TH0 = 0x0FC;
    TL0 = 0x18;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}

void Timer0Interrupt(void) interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x18;
	t_flag_count_up();
}

