#include "reg52.h"
#include "intrins.h"
typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

#define GPIO_KEY P1	//������󰴼�
u8 KeyValue;	//������Ŷ�ȡ���ļ�ֵ

//����3-8��������ѡ��Ҫ�����ĵ�
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit beep=P1^5;	 //������

sbit button1 = P3^1; 	//����
sbit button2 = P3^0;	//�Ӱ���
sbit button3 = P3^2;	//������
sbit button4 = P3^3;	//ȷ�ϰ���

//���尴�����µĴ�������ͬ����ѡ��ͬ����
static char button_num1 = 0; 	//�ж�ѡ��ʱ����

u8 code smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f};//��ʾ0~9��

//num1:���ʼֵ num2:�ֳ�ʼֵ num3:ʱ��ʼֵ
u16 num1=55,num2=59,num3=13;
//����ʱ��
u16 num11=59,num22=59,num33=23;

//���ڽ��յ�����
u8 receive1=0,receive2=0,receive3=0,receive4=0,receive5=0;


static int i = 0;

//��������
void KeyDown(void);

/***********************************************************
*������		��delay
*����		����ʱ
*����		��i ��ʱ���ٸ�us
************************************************************/
void delay(u16 i)
{
	while(i--);
}

/***********************************************************
*������		��delay_us
*����		����ʱ
*����		��i ��ʱ��us��
************************************************************/
void delay_us(int i)
{
	while(i--);
}

/*******************************************************************************
* �� �� ��         : button_setting
* ��������		   : ʵ�ְ�������/ȷ�Ϲ��ܣ�������������� ���¿�ʼ����
* ����			   ����
*******************************************************************************/
void button_setting()
{	
	//��������
	if(button1 == 0 || receive1 == 1) 
	{
		receive1 = 0;	//�����־λ
		delay_us(10);	
		while(!button1);	//ֱ���ɿ�������ִ��

		button_num1++;	//ѡ�����ò�ͬλ��ʱ �� �룩
		if(button_num1 == 4)
		{
			button_num1 = 1;	//��������λ��λ�� ��
		}
		
		ET0=0;
		//���������״̬��ʱ�����ر�
		if(KeyValue == 1)
		{
			ET0=1;	//�򿪶�ʱ���ж�
			//EA = 1;
		}
//		else if(butt)
//		{
//			ET0=0;	//�رն�ʱ���ж�
//			//EA = 0;	//�ر����жϣ�ֹͣ��ʱ
//		}	
				
	}

	//ȷ������
	if(button4 == 0 || receive4 == 1)
	{
		receive4 = 0;
		receive5 = 0;
		delay_us(10);
		while(!button4);	//ֱ���ɿ�������ִ��
		button_num1 = 0;	//ȡ������
		//EA=1;	//���ж�
		ET0=1;	 //����ʱ���ж�
		KeyValue = 0;	//���ӱ�־����				
	}
}


/*******************************************************************************
* �� �� ��         : button_up_down
* ��������		   : ʱ���/�� �����߼�����
* ����			   ����
*******************************************************************************/
void button_up_down()
{
	//����
	if(button2 == 0 || receive2 == 1)
	{
		receive2 = 0;
		if(KeyValue == 1 || receive5 == 1)
		{
			switch(button_num1)
			{
				case 0:	break;
				case 1:	delay_us(10); while(~button2); num11++;  break; 	//�ȴ������ͷ�	ʱ��һ
				case 2:	delay_us(10); while(~button2); num22++;	break;		//�ȴ������ͷ�	�ּ�һ
				case 3:	delay_us(10); while(~button2); num33++;	break;		//�ȴ������ͷ�	���һ
				default: break;	
			}	
		}
		else
		{
			switch(button_num1)
			{
				case 0:	break;
				case 1:	delay_us(10); while(~button2); num1++;  break; 		//�ȴ������ͷ�	ʱ��һ
				case 2:	delay_us(10); while(~button2); num2++;	break;		//�ȴ������ͷ�	�ּ�һ
				case 3:	delay_us(10); while(~button2); num3++;	break;		//�ȴ������ͷ�	���һ
				default: break;	
			}	
		}

		if(num3 == 24) num3 = 0;	//��������
		if(num2 == 60) num2 = 0;
		if(num1 == 60) num1 = 0;
		if(num33 == 24) num33 = 0;	//��������
		if(num22 == 60) num22 = 0;
		if(num11 == 60) num11 = 0;	
	}

	//����
	if(button3 == 0  || receive3 == 1)
	{
		receive3 = 0;
		if(KeyValue == 1 || receive5 == 1)
		{
			switch(button_num1)
			{
				case 0:		break;
				case 1:	delay_us(10);  num11--; while(~button3);    break;	//ʱ��һ
				case 2:	delay_us(10);  num22--; while(~button3);	break;	//�ּ�һ
				case 3:	delay_us(10);  num33--; while(~button3);	break;	//���һ
				default: break;
			}	
		}
		else
		{
			switch(button_num1)
			{
				case 0:		break;
				case 1:	delay_us(10);  num1--; while(~button3); break;	//ʱ��һ
				case 2:	delay_us(10);  num2--; while(~button3);	break;	//�ּ�һ
				case 3:	delay_us(10);  num3--; while(~button3);	break;	//���һ
				default: break;
			}
		}
		
		if(num3 == -1) num3 = 23;	//��������
		if(num2 == -1) num2 = 59;
		if(num1 == -1) num1 = 59;
		if(num33 == -1) num33 = 23;	//��������
		if(num22 == -1) num22 = 59;
		if(num11 == -1) num11 = 59;
	}
}

/*******************************************************************************
* �� �� ��         : KeyDown
* ��������		   : ����а������²���ȡ��ֵ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void KeyDown(void)
{
	char a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)//��ȡ�����Ƿ���
	{
		delay(1000);//��ʱ10ms��������
		if(GPIO_KEY!=0x0f)//�ٴμ������Ƿ���
		{	
			//������
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
			}
			//������
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
			while((a<50)&&(GPIO_KEY!=0xf0))	 //��ⰴ�����ּ��
			{
				delay(1000);
				a++;
			}
		}
	}
}

/***********************************************************
*������		��display
*����		���Դ����ʱ������д�����㣬ת��Ϊ�߶������Ҫ��ʾ��ֵ
*����		��num1 ��	num2 ��	  num3 ʱ
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
	LSC=1;//��ʾ��8λ
	P0=smgduan[shi3];	  
//	P0=0x00; 	
	delay(100);

	LSA=0;
	LSB=1;
	LSC=1;//��ʾ��7λ
	P0=smgduan[ge3];
//	P0=0x00; 
	delay(100);	

	LSA=1;
	LSB=0;
	LSC=1;//��ʾ��6λ
	P0=0x40;
//	P0=0x00; 
	delay(100);	

	LSA=0;
	LSB=0;
	LSC=1;//��ʾ��5λ
	P0=smgduan[shi2];
//	P0=0x00;
	delay(100);	

	LSA=1;
	LSB=1;
	LSC=0;//��ʾ��4λ
	P0=smgduan[ge2];
//	P0=0x00; 
	delay(100);	
	
	LSA=0;
	LSB=1;
	LSC=0;//��ʾ��3λ
	P0=0x40;
//	P0=0x00; 
	delay(100);	
	
	LSA=1;
	LSB=0;
	LSC=0;//��ʾ��2λ
	P0=smgduan[shi1];
//	P0=0x00; 
	delay(100);
	
	LSA=0;
	LSB=0;
	LSC=0;//��ʾ��1λ
	P0=smgduan[ge1];
//	P0=0x00; 
	delay(100);	

			
}

/*******************************************************************************
* ������         :UsartInit()
* ��������		   :���ô���
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void UsartInit()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XF3;				//��������ʼֵ���ã�ע�Ⲩ������4800��
	TL1=0XF3;
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
}

/*******************************************************************************
* �� �� ��         : Timer0Init
* ��������		   : ��ʱ��0��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TimerInit()
{
	TMOD|=0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1������TR0��������
	TH0=0X3C;	//����ʱ������ֵ����ʱ50ms
	TL0=0XB0;	//0X3CB0��ʮ������15536 ��15536������65536����50000�� ��50000us=50ms	
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
	TR0=1;//�򿪶�ʱ��	

}

/*******************************************************************************
* �� �� ��       : main
* ��������		 : ������
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void main()
{	
	UsartInit();
	TimerInit();
	
	while(1)
	{
		//����ɨ��
		KeyDown();
		button_up_down();
		button_setting();
		
			
	 	if(i>=20)//20��50���뼴һ��			//�˴�������̫�������׳���ʹ�����޷�����
		{
			i=0;
			num1++;
			if(num1==60)
			{
				num1=0;
				num2++;
				if(num2==60)//��ʱһСʱ�Զ�����
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
		
		//����
		if(num1==num11&&num2==num22&&num3==num33)
		{
			beep = 1;	
			if(num1>num11+3)
				beep = 0;
		}		 
	}
}

/*******************************************************************************
* �� �� ��       : Timer0
* ��������		 : �жϺ�������ʱ����ʽ1���ֶ���װ��ֵ
* ����			 : ��
*******************************************************************************/		
void Timer0() interrupt 1
{
	 TH0=0x3c;	 //�ֶ���װ��
	 TL0=0xb0;
	 i++;		
}

/*******************************************************************************
* ������         : Usart() interrupt 4
* ��������		  : ����ͨ���жϺ���
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void Usart() interrupt 4
{
	u8 receiveData;

	receiveData=SBUF;//��ȥ���յ�������
	RI = 0;//��������жϱ�־λ
	SBUF=receiveData;//�����յ������ݷ��뵽���ͼĴ���
	while(!TI);			 //�ȴ������������
	TI=0;
							 //���������ɱ�־λ

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
