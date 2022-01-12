#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "clock.h"
extern struct clock MyClock;
 
//��������:���빫�����ڵõ�����
//������������������� 
//����ֵ�����ں�																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 date)
{	
	u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=MyClock.year/100;	yearL=MyClock.year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+MyClock.date+table_week[MyClock.month-1];
	if (yearL%4==0&&MyClock.month<3)temp2--;
	return(temp2%7);
}			  

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)   //�ж��Ƿ�������
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0){ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	

void button_up(void){                                       //���Ӻ���
	const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//���������ݱ�	
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
	if(MyClock.month==2){            //���յĽ�λ�жϣ���Ϊ���ºͲ��Ƕ��£�Ҫ���ǵ������ƽ�꣬ 
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

 void button_down(void){                          //����ť��λ�жϺ���
						
						const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//ƽ������
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
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��

	  KEY_Init();//��ʼ��������Ӧioģʽ

    //GPIOC.5 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    //GPIOA.15	  �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    //GPIOA.0	  �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

   	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
 
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
}
void EXTI0_IRQHandler(void)
{
  delay_ms(10);    //����
	if(WK_UP==1)     //ģ��ѡ�񰴼�
	{	  
		MyClock.xuanze++;
		if(MyClock.xuanze==7)  //����ģ��ѭ��
			MyClock.xuanze=0;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //���EXTI0��·����λ
}
 void EXTI9_5_IRQHandler(void)
{			
	delay_ms(10);   //����			 
	if(KEY0==0)	{ //���Ӱ���
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
 	 EXTI_ClearITPendingBit(EXTI_Line5);    //���LINE5�ϵ��жϱ�־λ  
}

void EXTI15_10_IRQHandler(void)
{
  delay_ms(10);    //����			 
  if(KEY1==0)	{   //������
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
	 EXTI_ClearITPendingBit(EXTI_Line15);  //���LINE15��·����λ
}

