#include "reg52.h"
#include "intrins.h"
typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

#define GPIO_KEY P1	//定义矩阵按键
u8 KeyValue;	//用来存放读取到的键值

//接入3-8译码器，选择要点亮的灯
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit beep=P1^5;	 //蜂鸣器

sbit button1 = P3^1; 	//设置
sbit button2 = P3^0;	//加按键
sbit button3 = P3^2;	//减按键
sbit button4 = P3^3;	//确认按键

//定义按键按下的次数，不同次数选择不同设置
static char button_num1 = 0; 	//判断选则时分秒

u8 code smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f};//显示0~9的

//num1:秒初始值 num2:分初始值 num3:时初始值
u16 num1=55,num2=59,num3=13;
//闹铃时间
u16 num11=59,num22=59,num33=23;

//串口接收的数据
u8 receive1=0,receive2=0,receive3=0,receive4=0,receive5=0;


static int i = 0;

//函数声明
void KeyDown(void);

/***********************************************************
*函数名		：delay
*功能		：延时
*参数		：i 延时多少个us
************************************************************/
void delay(u16 i)
{
	while(i--);
}

/***********************************************************
*函数名		：delay_us
*功能		：延时
*参数		：i 延时的us数
************************************************************/
void delay_us(int i)
{
	while(i--);
}

/*******************************************************************************
* 函 数 名         : button_setting
* 函数功能		   : 实现按键设置/确认功能，按四下设置完毕 重新开始计数
* 参数			   ：无
*******************************************************************************/
void button_setting()
{	
	//进行设置
	if(button1 == 0 || receive1 == 1) 
	{
		receive1 = 0;	//清除标志位
		delay_us(10);	
		while(!button1);	//直到松开才向下执行

		button_num1++;	//选择设置不同位（时 分 秒）
		if(button_num1 == 4)
		{
			button_num1 = 1;	//将被设置位定位到 秒
		}
		
		ET0=0;
		//如果是闹钟状态则定时器不关闭
		if(KeyValue == 1)
		{
			ET0=1;	//打开定时器中断
			//EA = 1;
		}
//		else if(butt)
//		{
//			ET0=0;	//关闭定时器中断
//			//EA = 0;	//关闭总中断，停止计时
//		}	
				
	}

	//确认设置
	if(button4 == 0 || receive4 == 1)
	{
		receive4 = 0;
		receive5 = 0;
		delay_us(10);
		while(!button4);	//直到松开才向下执行
		button_num1 = 0;	//取消设置
		//EA=1;	//开中断
		ET0=1;	 //开定时器中断
		KeyValue = 0;	//闹钟标志清零				
	}
}


/*******************************************************************************
* 函 数 名         : button_up_down
* 函数功能		   : 时间加/减 按键逻辑处理
* 参数			   ：无
*******************************************************************************/
void button_up_down()
{
	//增加
	if(button2 == 0 || receive2 == 1)
	{
		receive2 = 0;
		if(KeyValue == 1 || receive5 == 1)
		{
			switch(button_num1)
			{
				case 0:	break;
				case 1:	delay_us(10); while(~button2); num11++;  break; 	//等待按键释放	时加一
				case 2:	delay_us(10); while(~button2); num22++;	break;		//等待按键释放	分加一
				case 3:	delay_us(10); while(~button2); num33++;	break;		//等待按键释放	秒加一
				default: break;	
			}	
		}
		else
		{
			switch(button_num1)
			{
				case 0:	break;
				case 1:	delay_us(10); while(~button2); num1++;  break; 		//等待按键释放	时加一
				case 2:	delay_us(10); while(~button2); num2++;	break;		//等待按键释放	分加一
				case 3:	delay_us(10); while(~button2); num3++;	break;		//等待按键释放	秒加一
				default: break;	
			}	
		}

		if(num3 == 24) num3 = 0;	//超出归零
		if(num2 == 60) num2 = 0;
		if(num1 == 60) num1 = 0;
		if(num33 == 24) num33 = 0;	//超出归零
		if(num22 == 60) num22 = 0;
		if(num11 == 60) num11 = 0;	
	}

	//减少
	if(button3 == 0  || receive3 == 1)
	{
		receive3 = 0;
		if(KeyValue == 1 || receive5 == 1)
		{
			switch(button_num1)
			{
				case 0:		break;
				case 1:	delay_us(10);  num11--; while(~button3);    break;	//时减一
				case 2:	delay_us(10);  num22--; while(~button3);	break;	//分减一
				case 3:	delay_us(10);  num33--; while(~button3);	break;	//秒减一
				default: break;
			}	
		}
		else
		{
			switch(button_num1)
			{
				case 0:		break;
				case 1:	delay_us(10);  num1--; while(~button3); break;	//时减一
				case 2:	delay_us(10);  num2--; while(~button3);	break;	//分减一
				case 3:	delay_us(10);  num3--; while(~button3);	break;	//秒减一
				default: break;
			}
		}
		
		if(num3 == -1) num3 = 23;	//超出归零
		if(num2 == -1) num2 = 59;
		if(num1 == -1) num1 = 59;
		if(num33 == -1) num33 = 23;	//超出归零
		if(num22 == -1) num22 = 59;
		if(num11 == -1) num11 = 59;
	}
}

/*******************************************************************************
* 函 数 名         : KeyDown
* 函数功能		   : 检测有按键按下并读取键值
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KeyDown(void)
{
	char a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)//读取按键是否按下
	{
		delay(1000);//延时10ms进行消抖
		if(GPIO_KEY!=0x0f)//再次检测键盘是否按下
		{	
			//测试列
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
			}
			//测试行
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
			while((a<50)&&(GPIO_KEY!=0xf0))	 //检测按键松手检测
			{
				delay(1000);
				a++;
			}
		}
	}
}

/***********************************************************
*函数名		：display
*功能		：对传如的时分秒进行处理计算，转化为七段数码管要显示的值
*参数		：num1 秒	num2 分	  num3 时
************************************************************/
void display(u16 num1,u16 num2,u16 num3)
{
	static u16 shi1,ge1,shi2,ge2,shi3,ge3;
	shi1=num1/10;
	ge1=num1%10;

	shi2=num2/10;
	ge2=num2%10;

	shi3=num3/10;
	ge3=num3%10;

	LSA=1;
	LSB=1;
	LSC=1;//显示第8位
	P0=smgduan[shi3];	  
//	P0=0x00; 	
	delay(100);

	LSA=0;
	LSB=1;
	LSC=1;//显示第7位
	P0=smgduan[ge3];
//	P0=0x00; 
	delay(100);	

	LSA=1;
	LSB=0;
	LSC=1;//显示第6位
	P0=0x40;
//	P0=0x00; 
	delay(100);	

	LSA=0;
	LSB=0;
	LSC=1;//显示第5位
	P0=smgduan[shi2];
//	P0=0x00;
	delay(100);	

	LSA=1;
	LSB=1;
	LSC=0;//显示第4位
	P0=smgduan[ge2];
//	P0=0x00; 
	delay(100);	
	
	LSA=0;
	LSB=1;
	LSC=0;//显示第3位
	P0=0x40;
//	P0=0x00; 
	delay(100);	
	
	LSA=1;
	LSB=0;
	LSC=0;//显示第2位
	P0=smgduan[shi1];
//	P0=0x00; 
	delay(100);
	
	LSA=0;
	LSB=0;
	LSC=0;//显示第1位
	P0=smgduan[ge1];
//	P0=0x00; 
	delay(100);	

			
}

/*******************************************************************************
* 函数名         :UsartInit()
* 函数功能		   :设置串口
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void UsartInit()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XF3;				//计数器初始值设置，注意波特率是4800的
	TL1=0XF3;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;					//打开计数器
}

/*******************************************************************************
* 函 数 名         : Timer0Init
* 函数功能		   : 定时器0初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TimerInit()
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1，仅用TR0打开启动。
	TH0=0X3C;	//给定时器赋初值，定时50ms
	TL0=0XB0;	//0X3CB0的十进制是15536 从15536计数到65536计数50000次 即50000us=50ms	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器	

}

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	UsartInit();
	TimerInit();
	
	while(1)
	{
		//按键扫描
		KeyDown();
		button_up_down();
		button_setting();
		
			
	 	if(i>=20)//20个50毫秒即一秒			//此处条件别卡太死，容易超出使程序无法进入
		{
			i=0;
			num1++;
			if(num1==60)
			{
				num1=0;
				num2++;
				if(num2==60)//定时一小时自动清零
				{
					num2=0;
					num3++;
					if(num3==24)
					{
						num3=0;
					}
				}
				
			}
		}
		if(KeyValue == 1 || receive5 == 1)
		{
			display(num11,num22,num33);	
		}
		else
		{
			display(num1,num2,num3); 
		}
		
		//闹钟
		if(num1==num11&&num2==num22&&num3==num33)
		{
			beep = 1;	
			if(num1>num11+3)
				beep = 0;
		}		 
	}
}

/*******************************************************************************
* 函 数 名       : Timer0
* 函数功能		 : 中断函数，计时，方式1需手动重装载值
* 参数			 : 无
*******************************************************************************/		
void Timer0() interrupt 1
{
	 TH0=0x3c;	 //手动重装载
	 TL0=0xb0;
	 i++;		
}

/*******************************************************************************
* 函数名         : Usart() interrupt 4
* 函数功能		  : 串口通信中断函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Usart() interrupt 4
{
	u8 receiveData;

	receiveData=SBUF;//出去接收到的数据
	RI = 0;//清除接收中断标志位
	SBUF=receiveData;//将接收到的数据放入到发送寄存器
	while(!TI);			 //等待发送数据完成
	TI=0;
							 //清除发送完成标志位

	switch(receiveData)
	{
		 case '1': receive1=1;break;
		 case '2': receive2=1;break;
		 case '3': receive3=1;break;
		 case '4': receive4=1;break;
		 case '5': receive5=1;break;
		 default:break;
	}
	receiveData = 0;

}
