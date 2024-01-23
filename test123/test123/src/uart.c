#include "sfr_r82b.h"
#include "main.h"
#include "allvars.h"
#include "general.h"
#include "iic.h"
#include "init.h"
#include "hvtj.h"
#include "tft.h"
#include "timer.h"
#include "uart.h"
#include "tanktest.h"
#include "cctalk.h"
#include "transactions.h"
#include "adc.h"

/* Serial Port Initialization */
void Set_UART0_Port()
{
	u0mr = 0x05;		// No parity, One stop bit, Internal Clock 
						// 8 bits data length
	u0rb = 0x00;
	u0c0 = 0x00;		// Select f1, LSB first
	u0brg = 129;		// Select 9600 Baud rate at 20MHZ
	u0c1 = 0x05;		// Enable Transmission & Reception
	s0ric = 0x0f;//0x07;//0x0f;		//uart0 intererrupt enable
}

void Send_Data2_UART0(unsigned char far *message,unsigned int nob)
{
	unsigned char cur_char;
	cur_char = 0;
	while (cur_char < nob)
	{
		Send_CurByte2_UART0(message[cur_char]);
		cur_char++;
	}
} 

void Send_CurByte2_UART0(unsigned char a)
{
	while(ti_u0c1 == 0);			//Wait until buffer becomes empty
	u0tb = a;
}

