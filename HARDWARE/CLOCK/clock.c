#include "clock.h"
#include "lcd.h"
#include "delay.h"
#include "exti.h"
#include "math.h"
#define pi 3.1415926535897932384626433832795

//MyClockType MyClock;
struct clock MyClock;

void Clock_Init(u32 year, u32 month, u32 date, u32 hour, u32 min, u32 sec){
	MyClock.year = year;
	MyClock.month = month;
	MyClock.date = date;
	MyClock.hour = hour;
	MyClock.min = min;
	MyClock.sec = sec;
	MyClock.xuanze = 0;
	MyClock.week = RTC_Get_Week(year,month,date);
	MyClock.DrawClockPoint = calendar_circle_clock_showtime; 
	MyClock.DrawDial = clock_circle_drawpanel;  //画表盘
	MyClock.ShowDigitalTime = show_number_time;  //显示数字时间
	MyClock.ShowWeek = show_week;  //显示星期
	MyClock.DrawClockPoint(120,120,200,8,MyClock.hour,MyClock.min,MyClock.sec);  //画指针
}

//显示时间
//x,y:坐标中心点
//size:表盘大小(直径)
//d:表盘分割,秒钟的高度
//hour:时钟
//min:分钟
//sec:秒钟
void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec)
{
	static u8 oldhour=0;	//最近一次进入该函数的时分秒信息
	static u8 oldmin=0;
	static u8 oldsec=0;
	float temp;
	u16 r=size/2;//得到半径 
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1;  
	u8 r1; 
	if(hour>11)hour-=12;    //24小时制的时间表
///////////////////////////////////////////////
	//清除小时
	r1=d/2+4;
	//清除上一次的数据
	temp=(float)oldmin/60;
	temp+=oldhour;
	px0=sx+r+(r-3*d-40)*sin((pi/6)*temp); 
	py0=sy+r-(r-3*d-40)*cos((pi/6)*temp); 
	px1=sx+r+r1*sin((pi/6)*temp); 
	py1=sy+r-r1*cos((pi/6)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,2,LGRAY);
	//清除分钟
	r1=d/2+3;
	temp=(float)oldsec/60;
	temp+=oldmin;
	//清除上一次的数据
	px0=sx+r+(r-2*d-30)*sin((pi/30)*temp); 
	py0=sy+r-(r-2*d-30)*cos((pi/30)*temp); 
	px1=sx+r+r1*sin((pi/30)*temp); 
	py1=sy+r-r1*cos((pi/30)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,1,LGRAY); 
	//清除秒钟 
	r1=d/2+3;
	//清除上一次的数据
	px0=sx+r+(r-d-25)*sin((pi/30)*oldsec); 
	py0=sy+r-(r-d-25)*cos((pi/30)*oldsec); 
	px1=sx+r+r1*sin((pi/30)*oldsec); 
	py1=sy+r-r1*cos((pi/30)*oldsec); 
	gui_draw_bline1(px0,py0,px1,py1,0,LGRAY); 
///////////////////////////////////////////////
	//显示小时 
	r1=d/2+4; 
	//显示新的时钟
	temp=(float)min/60;
	temp+=hour;
	px0=sx+r+(r-3*d-40)*sin((pi/6)*temp); 
	py0=sy+r-(r-3*d-40)*cos((pi/6)*temp); 
	px1=sx+r+r1*sin((pi/6)*temp); 
	py1=sy+r-r1*cos((pi/6)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,2,0x0000); 
	//显示分钟 
	r1=d/2+3; 
	temp=(float)sec/60;
	temp+=min;
	//显示新的分钟
	px0=sx+r+(r-2*d-30)*sin((pi/30)*temp); 
	py0=sy+r-(r-2*d-30)*cos((pi/30)*temp); 
	px1=sx+r+r1*sin((pi/30)*temp); 
	py1=sy+r-r1*cos((pi/30)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,1,0x0000); 	
	//显示秒钟  
	r1=d/2+3;
	//显示新的秒钟
	px0=sx+r+(r-d-25)*sin((pi/30)*sec); 
	py0=sy+r-(r-d-25)*cos((pi/30)*sec); 
	px1=sx+r+r1*sin((pi/30)*sec); 
	py1=sy+r-r1*cos((pi/30)*sec); 
	gui_draw_bline1(px0,py0,px1,py1,0,0x0000); 
	oldhour=hour;	//保存时
	oldmin=min;		//保存分
	oldsec=sec;		//保存秒
}	   

//显示数字时间
void show_number_time(){
			POINT_COLOR=RED;
			LCD_ShowNum(40,241,MyClock.year,4,24);	
			POINT_COLOR=BLUE;
			LCD_ShowNum(115,241,MyClock.month,2,24);		
			POINT_COLOR=GREEN;
			LCD_ShowNum(162,241,MyClock.date,2,24);					
			POINT_COLOR=LIGHTGREEN;
			LCD_ShowNum(50,295,MyClock.hour,2,24);	
			POINT_COLOR=YELLOW;
			LCD_ShowNum(100,295,MyClock.min,2,24);	
			POINT_COLOR=BRRED;
			LCD_ShowNum(148,295,MyClock.sec,2,24);	
}

//显示星期
void show_week(u16 year,u8 month,u8 date){
		  MyClock.week = RTC_Get_Week(year,month,date);
			POINT_COLOR=MAGENTA;
			switch(MyClock.week)
			{
				case 0:
					LCD_ShowString(75,266,400,32,24,"Sunday   ");
					break;
				case 1:
					LCD_ShowString(75,266,400,32,24,"Monday   ");
					break; 
				case 2:
					LCD_ShowString(75,266,400,32,24,"Tuesday  ");
					break;
				case 3:
					LCD_ShowString(75,266,400,32,24,"Wednesday");
					break;
				case 4:
					LCD_ShowString(75,266,400,32,24,"Thursday ");                                                                               
					break;
				case 5:
					LCD_ShowString(75,266,400,32,24,"Friday   ");
					break;
				case 6:
					LCD_ShowString(75,266,400,32,24,"Saturday ");
					break;  
			}		
}

void handle_clock(){
	if(MyClock.xuanze == 0){
		//说明是正常模式
						MyClock.DrawClockPoint(120,120,200,8,MyClock.hour,MyClock.min,MyClock.sec);	
						MyClock.ShowDigitalTime();
						MyClock.ShowWeek(MyClock.year,MyClock.month,MyClock.date);
		return;
	}
	switch(MyClock.xuanze){
				 case 1:
							MyClock.ShowWeek(MyClock.year,MyClock.month,MyClock.date);
								POINT_COLOR = WHITE;				//按键后的闪烁画面为白色
									LCD_ShowNum(40,241,MyClock.year,4,24);	
									delay_ms(300);
								POINT_COLOR = RED;
									LCD_ShowNum(40,241,MyClock.year,4,24);	
									delay_ms(300);
				 break;				 	
				 case 2:
					 		MyClock.ShowWeek(MyClock.year,MyClock.month,MyClock.date);
				 				POINT_COLOR = WHITE;
										LCD_ShowNum(115,241,MyClock.month,2,24);									 
									delay_ms(300);
								POINT_COLOR = BLUE;
										LCD_ShowNum(115,241,MyClock.month,2,24);		
									delay_ms(300);
				       break;
				 case 3:
					 		MyClock.ShowWeek(MyClock.year,MyClock.month,MyClock.date);
				 				 POINT_COLOR = WHITE;
								 LCD_ShowNum(163,241,MyClock.date,2,24);		
								 delay_ms(300);
								 POINT_COLOR = GREEN;
								LCD_ShowNum(163,241,MyClock.date,2,24);		
								delay_ms(300);
				        break;
				 case 4:	
				 				 POINT_COLOR = WHITE;
									LCD_ShowNum(50,295,MyClock.hour,2,24);		
									delay_ms(300);
								 POINT_COLOR = LIGHTGREEN;
									LCD_ShowNum(50,295,MyClock.hour,2,24);								
									delay_ms(300);
				        break;
				 case 5:
				 				 POINT_COLOR = WHITE;
								 	LCD_ShowNum(100,295,MyClock.min,2,24);		
								 delay_ms(300);
								 POINT_COLOR = YELLOW;
									LCD_ShowNum(100,295,MyClock.min,2,24);
									delay_ms(300);
				        break;
				  case 6:
				 				 	POINT_COLOR = WHITE;
									LCD_ShowNum(148,295,MyClock.sec,2,24);
									delay_ms(300);
									POINT_COLOR =BRRED;
									LCD_ShowNum(148,295,MyClock.sec,2,24);
									delay_ms(300);
				        break;			
	        }
	
					calendar_circle_clock_showtime(120,120,200,8,MyClock.hour,MyClock.min,MyClock.sec);
}

void LCD_DrawFullCircle1(u16 x0,u16 y0,u16 r)
{
	u16 x,y;
	for(y=y0 - r;y<y0 +r;y++){
		for(x=x0 - r;x<x0+r;x++){
			if(((x-x0)*(x-x0)+(y-y0)*(y-y0))<=r*r){
					LCD_DrawPoint(x,y);
			}
		}
	}
}
//画圆形指针表盘
//x,y:坐标中心点
//size:表盘大小(直径)
//d:表盘分割,秒钟的高度
void clock_circle_drawpanel(u16 x,u16 y,u16 size,u16 d)
{
	int colorBuff[4] = {GREEN,BROWN,MAGENTA,GRAYBLUE};
	u16 r=size/2;//得到半径 
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1;  
	float i;
	int j= 0;
	LCD_Clear(LGRAY);		//清屏填充色为浅灰色
	POINT_COLOR=BRRED;//设置字体为棕红色
	for(i=96;i<100;i=i+0.02){
		POINT_COLOR++;
		LCD_Draw_Circle(120,120,i);	  //画圆
	}
  for (j = 102,i= 102; i<= 118; i = i+0.02){   //分区间段画出背景颜色
		POINT_COLOR = colorBuff[((int)ceilf(i)-j)/4];  //ceilf();向上取整
		LCD_Draw_Circle(120,120,i);
	}		
		LCD_DrawLine(119,24,119,29,RED); 
		LCD_DrawLine(120,24,120,29,RED);
		LCD_DrawLine(121,24,121,29,RED);//画线  12
		
		LCD_DrawLine(119,211,119,216,RED); 
		LCD_DrawLine(120,211,120,216,RED);
		LCD_DrawLine(121,211,121,216,RED);//画线   6
		
		LCD_DrawLine(24,119,29,119,RED); 
		LCD_DrawLine(24,120,29,120,RED); 
		LCD_DrawLine(24,121,29,121,RED); //画线  9
		
		LCD_DrawLine(211,119,216,119,RED);
		LCD_DrawLine(211,120,216,120,RED);
		LCD_DrawLine(211,121,216,121,RED);//画线	3
		
		BACK_COLOR=LGRAY;									//画背景颜色
		LCD_ShowNum(113,38,12,2,16);    //显示数字  12
		LCD_ShowNum(195,112,3,1,16);    //3
		LCD_ShowNum(116,187,6,1,16);   //6
		LCD_ShowNum(38,112,9,1,16);     //9
		
		LCD_ShowNum(158,45,1,1,16);
		LCD_ShowNum(184,70,2,1,16);
		LCD_ShowNum(186,152,4,1,16);
		LCD_ShowNum(158,180,5,1,16);
		LCD_ShowNum(78,184,7,1,16);
		LCD_ShowNum(46,155,8,1,16);
		LCD_ShowNum(45,70,10,2,16);
		LCD_ShowNum(78,45,11,2,16);
	
		LCD_DrawLine(168,38,164,45,BLACK);//1
		LCD_DrawLine(202,72,195,76,BLACK);//2
		LCD_DrawLine(202,168,195,164,BLACK);//4
		LCD_DrawLine(168,202,164,195,BLACK);//5
		LCD_DrawLine(72,202,76,195,BLACK);//7
		LCD_DrawLine(38,168,45,164,BLACK);//8
		LCD_DrawLine(38,72,45,76,BLACK);//10
		LCD_DrawLine(72,38,76,45,BLACK);//11
	
		POINT_COLOR=RED;
		LCD_Draw_Circle(120,120,2);//显示表盘中间的里面的圈
		LCD_Draw_Circle(120,120,3);//显示表盘中间外面的圈	
	
	 for(i=0;i<60;i++)//画秒钟格
	 { 
		  px0=sx+r+(r-4)*sin((pi/30)*i);  //一个pi，30秒走完；
	  	py0=sy+r-(r-4)*cos((pi/30)*i); 
	  	px1=sx+r+(r-d)*sin((pi/30)*i); 
		  py1=sy+r-(r-d)*cos((pi/30)*i);  
		  LCD_DrawLine(px0,py0,px1,py1,BLACK);		
	 }
		LCD_DrawLine(119,30,119,37,RED); 
		LCD_DrawLine(120,30,120,37,RED);
		LCD_DrawLine(121,30,121,37,RED);//画线  12
		
		LCD_DrawLine(119,203,119,210,RED); 
		LCD_DrawLine(120,203,120,210,RED);
		LCD_DrawLine(121,203,121,210,RED);//画线   6
		
		LCD_DrawLine(30,119,37,119,RED); 
		LCD_DrawLine(30,120,37,120,RED); 
		LCD_DrawLine(30,121,37,121,RED); //画线  9
		
		LCD_DrawLine(203,119,210,119,RED);
		LCD_DrawLine(203,120,210,120,RED);
		LCD_DrawLine(203,121,210,121,RED);//画线	3		
		
		LCD_ShowNum(113,38,12,2,16);    //显示数字  12
		LCD_ShowNum(195,112,3,1,16);    //3
		LCD_ShowNum(116,187,6,1,16);   //6
		LCD_ShowNum(38,112,9,1,16);     //9
	 	 	
}

//画一条粗线
//x0,y0:起点 x1,y1:终点
//size:线粗细,仅支持:0~2.
//color:颜色
void gui_draw_bline1(u16 x0,u16 y0,u16 x1,u16 y1,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x1-x0; //计算坐标增量 
	delta_y=y1-y0; 
	uRow=x0; 
	uCol=y0; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		if(size==0)LCD_Fast_DrawPoint(uRow,uCol,color);//画点 
		if(size==1){
			LCD_Fast_DrawPoint(uRow,uCol,color);//画点 
			LCD_Fast_DrawPoint(uRow+1,uCol,color);//画点 
			LCD_Fast_DrawPoint(uRow,uCol+1,color);//画点 
			LCD_Fast_DrawPoint(uRow+1,uCol+1,color);//画点 
		}
		if(size==2){
			LCD_Fast_DrawPoint(uRow,uCol,color);//画点 
			LCD_Fast_DrawPoint(uRow+1,uCol,color);//画点 
			LCD_Fast_DrawPoint(uRow,uCol+1,color);//画点 
			LCD_Fast_DrawPoint(uRow+1,uCol+1,color);//画点 
			LCD_Fast_DrawPoint(uRow-1,uCol+1,color);//画点 
			LCD_Fast_DrawPoint(uRow+1,uCol-1,color);//画点
			LCD_Fast_DrawPoint(uRow-1,uCol-1,color);//画点  
			LCD_Fast_DrawPoint(uRow-1,uCol,color);//画点 
			LCD_Fast_DrawPoint(uRow,uCol-1,color);//画点  
		}
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance){ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance){ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	} 				 
}

