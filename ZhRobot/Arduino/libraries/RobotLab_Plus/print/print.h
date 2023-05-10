#ifndef _PRINT_H_
#define _PRINT_H_

#include"Arduino.h"
#include <stdarg.h>

#define __PRINT_FLOAT 0

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifdef __cplusplus
 extern "C" {
#endif //__cplusplus


#ifndef printf_char
int _printcharflag=0;
char printf_char(char c)//显示字符
{
	if(_printcharflag==0)
	{
		_printcharflag=1;
		Serial.begin(9600);
	}
	Serial.write(c);
	//setUart(9600,c);
	//setUart(2,9600,c);
	return 0;
}
#endif
void Printf(char* fmt, ...);
void printf_ch(char ch);
//void printf_dec(int dec,int numfirst);
void printf_dec(long dec,int numfirst,int numsize);
//void printf_udec(unsigned int dec,int numfirst);
void printf_udec(unsigned long dec,int numfirst,int numsize);
#if __PRINT_FLOAT>0
void printf_flt(double flt,int numfirst);
#endif
void printf_bin(unsigned long bin,int numfirst);
void printf_hex(unsigned long hex,char c,int numfirst,int numsize);
void printf_octal(unsigned long octal,int numfirst);
void printf_string(char* str);

#define console_print(ch)    printf_char(ch)//putchar(ch)

void Printf(char* fmt, ...)
{
#if __PRINT_FLOAT>0
    double vargflt = 0;
#else
		long vargflt = 0;
#endif//#if __PRINT_FLOAT>0
    long  vargint = 0,sta=0;
    char* vargpch = NULL;
    char vargch = 0;
    char* pfmt = NULL;
    va_list vp;
    unsigned char c;
    int numfirst=0;//用于标志显示的数字是不是第一次显示
    //int numflag=0;//符号标志位
		int numsize=0;
		unsigned int mhex,mlen;

    va_start(vp, fmt);
    pfmt = fmt;
    while(*pfmt)
    {
        if(*pfmt == '%')
        {
      NextStep:
            switch(*(++pfmt))
            {

                case 'c'://ascii
                    vargch = va_arg(vp, long);
                    /*  va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI
                        mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */
                    printf_ch(vargch);
                    break;
                case 'd'://十进制整数
                case 'i'://十进制整数
PrintDec_Step:
                	  vargint = va_arg(vp, long);
                    //printf_dec(vargint,numfirst);
                		printf_dec(vargint,numfirst,numsize);
                    break;
                case 'u'://%u 不带正负号的十进制值
PrintUDec_Step:
                	  vargint = va_arg(vp, unsigned long);
                    //printf_udec(vargint,numfirst);
                		printf_udec(vargint,numfirst,numsize);
                    break;
#if __PRINT_FLOAT>0
                case 'f'://浮点格式
                    vargflt = va_arg(vp, double);
                    /*  va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI
                        mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */
                    printf_flt(vargflt,numfirst);
                    break;
#endif//__PRINT_FLOAT
                case 's'://字符串
                    vargpch = va_arg(vp, char*);
                    printf_string(vargpch);
                    break;
                //case 'b'://二进制
                case 'B':
                    vargint = va_arg(vp, unsigned long);
                    printf_bin(vargint,numfirst);
                    break;
                case 'o'://不带正负号的八进制
                    vargint = va_arg(vp, unsigned long);
                    printf_octal(vargint,numfirst);
                    break;
                case 'x':// 不带正负号的十六进制
                case 'X':
PrintHex_Step:
                    vargint = va_arg(vp, unsigned long);
                    printf_hex(vargint,*pfmt,numfirst,numsize);
                    break;
                case '%'://%
                    printf_ch('%');
                    break;
                case '#'://# 二/八/十六进制数前缀0b/o/0x：对c,s,d,u类无影响；对o类，在输出时加前缀o；对x类，在输出时加前缀0x；
                    if(*(pfmt+1)=='x'||*(pfmt+1)=='X')
                    {
                    	//numflag=1;
                    	printf_string("0x");
                    	goto NextStep;
                    }
                    else if(*(pfmt+1)=='B')
                    {
                    	//numflag=1;
                    	printf_string("0b");
                    	goto NextStep;
                    }
                    else if(*(pfmt+1)=='o')
                    {
                    	//numflag=1;
                    	printf_string("o");
                    	goto NextStep;
                    }
                    break;
                default:
									if(*pfmt>='0'&&*pfmt<='9'&&*(pfmt+1)>='0'&&*(pfmt+1)<='9')
									{
										if(*(pfmt+2)=='x'||*(pfmt+2)=='X')
										{
											
											numsize=(*pfmt-'0')*10+(*(pfmt+1)-'0');
											pfmt+=2;
											goto PrintHex_Step;
										}
										else if(*(pfmt+2)=='d'||*(pfmt+2)=='i')
										{

											numsize=(*pfmt-'0')*10+(*(pfmt+1)-'0');
											pfmt+=2;
											goto PrintDec_Step;
										}
										else if(*(pfmt+2)=='u')
										{

											numsize=(*pfmt-'0')*10+(*(pfmt+1)-'0');
											pfmt+=2;
											goto PrintUDec_Step;
										}
									}
                    break;
            }//switch(*(++pfmt))
            pfmt++;
        }//if(*pfmt == '%')
        else
        {
        	c=*pfmt;
        	{
        		printf_ch(*pfmt++);
        	}
        }
    }//while(*pfmt)
    va_end(vp);
}

void printf_ch(char ch)
{
  console_print(ch);
}
void printf_dec(long dec,int numfirst,int numsize)//numsize=0 任意长度,numsize>0 固定长度
{
	int mlen=0;
	int mdat;
  if(dec==0&&numfirst)
  {
  	numfirst=0;
    return;
  }
  if(numfirst==0)
  {
  	if(numsize)
  	{
			mdat=abs(dec);	mlen=0;
			do{mdat/=10;mlen++;}while(mdat>0);
			if(mlen<numsize)
			{
				for(mlen;mlen<numsize;mlen++)printf_ch(' ');
			}
  	}
  	numfirst=1;
  }
	if(dec<0)
	{
		dec=-dec;
		printf_ch('-');
	}
	else
	{
		if(numsize)
		{
			printf_ch(' ');
			numsize=0;
		}
	}
  printf_dec(dec/10,numfirst,0);
  printf_ch((char)(dec%10 + '0'));
}
void printf_udec(unsigned long dec,int numfirst,int numsize)//numsize=0 任意长度,numsize>0 固定长度
{
	int mlen=0;
	unsigned int mdat;
  if(dec==0&&numfirst)
  {
  	numfirst=0;
    return;
  }
  if(numfirst==0)
  {
  	if(numsize)
  	{
			mdat=(unsigned int)dec;	mlen=0;
			do{mdat/=10;mlen++;}while(mdat>0);
			if(mlen<numsize)
			{
				for(mlen;mlen<numsize;mlen++)printf_ch(' ');
			}
  	}
  	numfirst=1;
  }
	if(numsize)
	{
		numsize=0;
	}
	printf_udec(dec/10,numfirst,0);
  printf_ch((char)(dec%10 + '0'));
}
#if __PRINT_FLOAT >0
void printf_flt(double flt,int numfirst)
{
    int icnt = 0;
    int tmpint = 0;
    int n;

    tmpint = (int)flt;
    printf_dec(tmpint,numfirst,0);
    printf_ch('.');
    flt = flt - tmpint+0.0000005;// 四舍五入
    tmpint = (int)(flt * 1000000);

    for(n=0;n<5;n++)//去掉后面的0
    {
    	if(tmpint%10)break;
    	else tmpint/=10;
    }

    printf_dec(tmpint,numfirst,0);
}
#endif//__PRINT_FLOAT
void printf_string(char* str)
{
	int sta;
    while(*str)
    {
      printf_ch(*str++);
    }
}
void printf_bin(unsigned long bin,int numfirst)
{
    if(bin == 0&&numfirst)
    {
    	numfirst=0;
      return;
    }
    if(numfirst==0)
    {
    	numfirst=1;
    }
    printf_bin(bin/2,numfirst);
    printf_ch((char)(bin%2 + '0'));
}
void printf_hex(unsigned long hex,char c,int numfirst,int numsize)//numsize=0 长度任意,numsize>0 长度固定
{
	int mlen=0;
	unsigned int mhex;
  if(hex==0&&numfirst)
  {
  	numfirst=0;
    return;
  }
  if(numfirst==0)
  {
		mhex=(unsigned int)hex;	mlen=0;
		do{mhex/=16;mlen++;}while(mhex>0);
		if(mlen<numsize)
		{
			for(mlen;mlen<numsize;mlen++)printf_ch(' ');
		}
		
  	numfirst=1;
  }
  printf_hex((hex>>4),c,numfirst,numsize);
  if((hex&0XF) < 10)
  {
    printf_ch((char)((hex&0X0F) + '0'));
  }
  else
  {
    if(c=='X')	printf_ch((char)(((hex&0X0F) - 10) + 'A' ));
    else 	printf_ch((char)(((hex&0X0F) - 10) + 'a' ));
  }
}
void printf_octal(unsigned long octal,int numfirst)//八进制数
{
  if(octal==0&&numfirst)
  {
  	numfirst=0;
    return;
  }
  if(numfirst==0)
  {
  	numfirst=1;
  }
  printf_octal(octal/8,numfirst);
  printf_ch((char)(octal%8 + '0'));
}
#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_PRINTF_H_
