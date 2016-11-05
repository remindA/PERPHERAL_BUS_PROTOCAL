#ifndef _LCD1602_H_
#define _LCD1602_H_


#define DATAPORT P2

//typedef unsigned char u8;
//typedef unsigned int  u16;

sbit LCD1602_RS = P0^7;
sbit LCD1602_RW = P0^6;
sbit LCD1602_E  = P0^5;


void delay1ms(unsigned int t);
void lcd1602_writecommand(const char Wcommand);
void lcd1602_writedata(const char Wdata);
void lcd1602_init(void);
void show_line(bit line, const unsigned char *str);


#endif
