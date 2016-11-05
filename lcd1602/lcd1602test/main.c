#include <reg51.h>
#include "lcd1602.h"

void main(void)
{
	char str1[] = "helloworld";
	char str2[] = "worldhello";
	lcd1602_init();
	while(1)
	{
		show_line(0,str1);
		show_line(1,str2);
	}
}