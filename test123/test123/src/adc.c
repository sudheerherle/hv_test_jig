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

void InitialiseADC()
{              
	prc2 =1;					// port 0 protection bit off
	pd0 = 0x00;					// input port	
	prc2=0;						// port 0 protection bit on
	adcon0 = 0x80;				// AN0,AN1, one shot mode, fAD/2
	adcon1 = 0x28;				// 10-bit mode, Internal Vref connected.
	adcon2 = 0x01;				// Sample and hold enabled
	adic = 0x00;
}

unsigned int ReadADC(unsigned char channel)
{
	unsigned char usc;
	for (usc=0;usc<250;usc++);
	adcon0 = channel;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	adst= 1;					// Start conversion signal
	while(adst);				// Wait until end of conversion 
	adst = 0;					// Clear End of conversion flag
	return((ad0) & 0x3ff);   	// Return the Data Read
}

