

#include "Delay.h"



#if 0
void Delay1us()		//@ 14.745MHz
{
	unsigned char i;

	_nop_();
	i = 1;
	while (--i);
}

#endif

#if 0
void Delay5us()		//@ 14.745MHz
{
	unsigned char i;

	i = 16;
	while (--i);
}
#endif

#if 1
void Delay30us()		//@ 14.745MHz
{
	unsigned char i;

	i = 108;
	while (--i);
}
#endif

#if 0
void Delay45us()		//@ 14.745MHz
{
	unsigned char i, j;

	i = 1;
	j = 162;
	do
	{
		while (--j);
	} while (--i);
}

#endif

#if 0

void Delay50us()		//@ 14.745MHz
{
	unsigned char i, j;

	_nop_();
	i = 1;
	j = 180;
	do
	{
		while (--j);
	} while (--i);
}
#endif



void Delay700us()		//@ 14.745MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 11;
	j = 6;
	do
	{
		while (--j);
	} while (--i);
}


#if 0
void Delay5ms()		//@ 14.745MHz
{
	unsigned char i, j;

	_nop_();
	i = 72;
	j = 180;
	do
	{
		while (--j);
	} while (--i);
}

#endif

void Delay10ms()		//@ 14.745MHz
{
	unsigned char i, j, k;

	i = 1;
	j = 144;
	k = 106;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


