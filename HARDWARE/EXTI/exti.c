#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "clock.h"
extern struct clock MyClock;
 
//功能描述:输入公历日期得到星期
//输入参数：公历年月日 
//返回值：星期号																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 date)
{	
	u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=MyClock.year/100;	yearL=MyClock.year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+MyClock.date+table_week[MyClock.month-1];
	if (yearL%4==0&&MyClock.month<3)temp2--;
	return(temp2%7);
}			  

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)   //判断是否是闰年
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0){ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	

void button_up(void){                                       //增加函数
	const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//月修正数据表	
	if(MyClock.sec==60){
			          MyClock.min++;
			          MyClock.sec=0;
			       }
	if(MyClock.min==60){
			           MyClock.hour++;
			           MyClock.min=0;
			       }
	if(MyClock.hour==24){
					      MyClock.date++;
					      MyClock.hour=0;
		       	}
	if(MyClock.month==2){            //对日的进位判断，分为二月和不是二月，要考虑到瑞年和平年， 
		   if(Is_Leap_Year(MyClock.year)){						
						 if(MyClock.date>29){							 
					       MyClock.month++;
					       MyClock.date=1;			   
						 }				
		    }
			 if(!Is_Leap_Year(MyClock.year)){
				   if(MyClock.date>28){  
					    MyClock.month++;
					    MyClock.date=1;
				   }	
			}
	}			 
	   if(MyClock.month!=2){
				 if(MyClock.date>mon_table[MyClock.month-1]){
									   MyClock.month++;
					           MyClock.date=1;
					}
		 }
			if(MyClock.month>12){				
           MyClock.year++;
					 MyClock.month=1;
			}
			if(MyClock.year>2099)MyClock.year=2099;
}

 void button_down(void){                          //减按钮退位判断函数
						
						const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//平年天数
						if(MyClock.sec==-1){
			        MyClock.min--;
			        MyClock.sec=59;
						}					  
						if(MyClock.min==-1){
			          MyClock.hour--;
			          MyClock.min=59;
			       }						 
						if(MyClock.hour==-1){
						   MyClock.date--;
					     MyClock.hour=23;
						 }						
						 if(MyClock.date<1){
							   MyClock.month--;
							 if(MyClock.month==0)
							 {
								 MyClock.month=12;
								  MyClock.year--;
							 }
							 if(Is_Leap_Year(MyClock.year)&&MyClock.month==2)
							 {
							     MyClock.date=29;
							 }
							 else{
								     MyClock.date=mon_table[MyClock.month-1];
							 }
						 }		
						if(MyClock.month<1){            
					  MyClock.year--;
						MyClock.month=12;
						}
						if(MyClock.year<1970)MyClock.year=1970;
					}
//外部中断初始化函数
void EXTIX_Init(void)
{
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟

	  KEY_Init();//初始化按键对应io模式

    //GPIOC.5 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.15	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.0	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

   	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}
void EXTI0_IRQHandler(void)
{
  delay_ms(10);    //消抖
	if(WK_UP==1)     //模块选择按键
	{	  
		MyClock.xuanze++;
		if(MyClock.xuanze==7)  //六个模块循环
			MyClock.xuanze=0;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
}
 void EXTI9_5_IRQHandler(void)
{			
	delay_ms(10);   //消抖			 
	if(KEY0==0)	{ //增加按键
		switch(MyClock.xuanze){
			case 0:break;
			case 1:MyClock.year++;button_up();break;
			case 2:MyClock.month++;button_up();break;
			case 3:MyClock.date++;button_up();break;
			case 4:MyClock.hour++;button_up();break;
			case 5:MyClock.min++;button_up();break;
			case 6:MyClock.sec++;button_up();break;
		}
	}
 	 EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位  
}

void EXTI15_10_IRQHandler(void)
{
  delay_ms(10);    //消抖			 
  if(KEY1==0)	{   //减按键
	switch(MyClock.xuanze){
			case 0:break;
			case 1:MyClock.year--;button_down();break;
			case 2:MyClock.month--;button_down();break;
			case 3:MyClock.date--;button_down();break;
			case 4:MyClock.hour--;button_down();break;
			case 5:MyClock.min--;button_down();break;
			case 6:MyClock.sec--; button_down();break;
		}	
	}
	 EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
}

