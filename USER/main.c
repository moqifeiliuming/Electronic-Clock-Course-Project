#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "key.h"
#include "usmart.h" 
#include "malloc.h"  
#include "MMC_SD.h" 
#include "exfuns.h"
#include "text.h"	
#include "math.h"
#include "clock.h"

 int main(void)
 { 
	u8 key;
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	 //串口初始化为9600
	TIM3_Int_Init(2000-1,36000-1);  
	EXTIX_Init();
	KEY_Init();  				//按键函数初始化
 	LCD_Init();  				//LCD函数初始化
		 
	usmart_dev.init(72);	//usmart初始化	
 	mem_init();				//初始化内存池	    
 	exfuns_init();			//为fatfs相关变量申请内存  
  f_mount(fs[0],"0:",1); 	//挂载SD卡 
 	f_mount(fs[1],"1:",1); 	//挂载FLASH.
	
	while(font_init()) 	//检查字库
	{    
		LCD_Clear(WHITE);		   	//清屏
 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
		LCD_ShowString(60,50,200,16,16,"mini STM32");
		while(SD_Initialize())		//检测SD卡
		{
			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(60,70,200+60,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
	 	key=update_font(20,110,16);//更新字库
		while(key)//更新失败		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(1500);	
		LCD_Clear(WHITE);//清屏	       
	}
	
		Clock_Init(2019,6,27,11,10,32);	
		clock_circle_drawpanel(120,120,200,8);
		
		POINT_COLOR=YELLOW;
		Show_Str(90,242,400,24,"年",24,0);         //显示字体
		POINT_COLOR=BRRED;
		Show_Str(138,242,400,24,"月",24,0);
		POINT_COLOR=MAGENTA;
		Show_Str(185,242,400,24,"日",24,0);
	
		POINT_COLOR=GREEN;
		Show_Str(75,295,400,24,"时",24,0);
		POINT_COLOR=BLUE;
		Show_Str(125,295,400,24,"分",24,0);
		POINT_COLOR=RED;
		Show_Str(173,295,400,24,"秒",24,0);
		
		while(1)
	 {	
		  handle_clock();  //时钟处理函数
	 }				
}
 
