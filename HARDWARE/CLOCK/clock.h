#ifndef _CLOCK_H
#define _CLOCK_H
#include "sys.h"
struct clock{
	u32 year;
	u32 month;
	u32 date;
	u32 hour;
	u32 min;
	u32 sec;
	u8 week;
	u8 xuanze;       //星期控制显示变量，模块选择变量
	void (* DrawDial)(u16 x,u16 y,u16 size,u16 d); //画表盘
//显示指针
	void (* DrawClockPoint)(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec);
	//显示数字时间
	void (* ShowDigitalTime)(void);
	//显示星期
	void (* ShowWeek) (u16 year,u8 month,u8 date);
};

//typedef struct clock MyClockType;
//extern MyClockType  MyClock;
extern struct clock MyClock;

void Clock_Init(u32 year, u32 month, u32 date, u32 hour, u32 min, u32 sec);


void show_number_time(void);

void show_week(u16 year,u8 month,u8 date);

void handle_clock(void);
void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec);

void LCD_DrawFullCircle1(u16 x0,u16 y0,u16 r);
void clock_circle_drawpanel(u16 x,u16 y,u16 size,u16 d);

void clock_circle_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec);

void gui_draw_bline1(u16 x0,u16 y0,u16 x1,u16 y1,u8 size,u16 color);
#endif

