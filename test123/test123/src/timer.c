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

// Example interrupt every 0.5msec
// f1 = 20MHz
// 0.5e-3 * 20e+6 = 10000
// trapre and tra cannot be loaded with value greater than 0xFF
// p = value in prescalar register; q = value in timer register
// [(p + 1)*(q + 1) = 10000] (duration * clock freq)
// Assume p = 199, then, 200*(q + 1) = 10000;
// therefore q = (10000 / 200) - 1 = 49;

void Initialise_TimerA()
{
	tramr = 0x00;						// TimerA = Timer Mode ; Clock = f2
	traic = 0x07;						// Interrupt control register - 
										// Bit3   - (IR) Interrupt Request Bit = Initialise to 0
										// Bit2~0 - (IPL)Interrupt Priority level = value between 1 ~ 7 (0 is disable)
										// NOTE: IR bit is set on underflow even if IPL = 0, 
										// IR can be cleared to '0' after reading '1' hence can use interrupt Polling 
										// even if IPL is 0
	trapre = 199;
	tra = 199;							//interrupt for every 2 msec)249
	tstart_tracr = 1;					// Start Count									
}

/************************************************************************************
Name:        timer_rd_init
Parameters:  None
Returns:     None
Description: Initial setting of Timer RD SFR
************************************************************************************/
void Initialise_TimerD()
{
	msttrd = 1;
	trdstr = 0x0C;							/* Stop TRD0 Count */
	trd0ic = 0x00;							/* Interrupt disabled */
	sync_trdmr = 0;							/* TRD0 and TRD1 registers operate independently */
	trdpmr = 0x77;							/* TRDIOB0:PWM mode */
											/* TRDIOC0:PWM mode */
											/* TRDIOD0:PWM mode */
											/* TRDIOB1:PWM mode */
											/* TRDIOC1:PWM mode */
											/* TRDIOD1:PWM mode */
	trdfcr = 0x80;	 						/* CMD0-CMD1:Set to "00b" in PWM mode */
											/* STCLK:External clock input disabled */
											/* PWM3:Set this bit to "1" in PWM mode */
	polb_trdpocr0 = 1;	//					/* TRDIOB0 output level:"H" active */
	polc_trdpocr0 = 1;						/* TRDIOC0 output level:"H" active */
	pold_trdpocr0 = 1;						/* TRDIOD0 output level:"H" active */
	polb_trdpocr1 = 1;	//					/* TRDIOB1 output level:"H" active */
	polc_trdpocr1 = 1;						/* TRDIOC1 output level:"H" active */
	pold_trdpocr1 = 1;						/* TRDIOD1 output level:"H" active */
	toa0_trdocr = 0;						/* Set this bit to "0"(enable output)in PWM mode  */ 
	tob0_trdocr = 0;//						/* TRDIOB0 Initial Output Level is inactive level */ 
	toc0_trdocr = 0;						/* TRDIOC0 Initial Output Level is inactive level */ 
	tod0_trdocr = 0;						/* TRDIOD0 Initial Output Level is inactive level */ 
	toa1_trdocr = 0;						/* Set this bit to "0"(enable output)in PWM mode  */ 
	tob1_trdocr = 0;//						/* TRDIOB1 Initial Output Level is inactive level */ 
	toc1_trdocr = 0;						/* TRDIOC1 Initial Output Level is inactive level */ 
	tod1_trdocr = 0;						/* TRDIOD1 Initial Output Level is inactive level */ 
	ea0_trdoer1 = 1;						/* TRDIOA0 Output Disable Bit: Set this bit to "1" in PWM mode */
	eb0_trdoer1 = 1;//						/* TRDIOB0 Pin Output enable */
	ec0_trdoer1 = 1;					//	/* TRDIOC0 Pin Output enable */
	ed0_trdoer1 = 1;					//	/* TRDIOD0 Pin Output enable */
	ea1_trdoer1 = 1;						/* TRDIOA1 Output Disable Bit: Set this bit to "1" in PWM mode */
	eb1_trdoer1 = 0;						/* TRDIOB1 Pin Output enable */
	ec1_trdoer1 = 1;					//	/* TRDIOC1 Pin Output enable */
	ed1_trdoer1 = 1;					//	/* TRDIOD1 Pin Output enable */
	pto_trdoer2 = 0;						/* INT0 of Pulse output forced cutoff input disabled */
	tck0_trdcr0 = 0;						/* Count Source Selection Bit:f8("011") */
	tck1_trdcr0 = 1;
	tck2_trdcr0 = 1;
	//tck0_trdcr0 = 0;						/* Count Source Selection Bit:f8("011") */
	//tck1_trdcr0 = 0;
	//tck2_trdcr0 = 1;
	cclr0_trdcr0 = 1;						/* TRD0 Counter Clear Selection Bit:Set to "001b" in PWM mode */
	cclr1_trdcr0 = 0;
	cclr2_trdcr0 = 0;
	tck0_trdcr1 = 0;						/* Count Source Selection Bit:fOCO40M("110") */
	tck1_trdcr1 = 1;
	tck2_trdcr1 = 1;
	//tck0_trdcr1 = 0;						/* Count Source Selection Bit:fOCO40M("110") */
	//tck1_trdcr1 = 0;
	//tck2_trdcr1 = 1;
	cclr0_trdcr1 = 1;						/* TRD1 Counter Clear Selection Bit:Set to "001b" in PWM mode */
	cclr1_trdcr1 = 0;
	cclr2_trdcr1 = 0;
	//trd0 = 0;								/* The Count value of TRD0 is initialized to "0" */
	trd1 = 0;								/* The Count value of TRD1 is initialized to "0" */
	trdgra1 = KHZ20_PERIOD_CONST;					/* 25ns * 40000 = 1000000ns */
	//trdgra0 = TIMERD_COMPARE_CONST1 - 1;					/* 100ns * 40000 = 16s */
	//trdgrb0 = TIMERD_COMPARE_CONST2 - 1;//0;							/* 25ns * 1000 =  25us */
	trdgrb1 = KHZ20_VALUE_CONST;							/* 25ns * 1000 =  25us */
	//trdgrc0 = 2000 - 1;					/* 25ns * 2000 =  50us */
	//trdgrd0 = 3000 - 1;					/* 25ns * 3000 =  75us */
	trdier0 = 0x00;							/* Disable interrupts, ovie/imied/imiec/imieb/imiea */
	trdier1 = 0x00;							/* Disable interrupts, ovie/imied/imiec/imieb/imiea */
	//trdstr = 0x0F;							/* Start TRD0 & TRD1 Count */
}

/************************************************************************************
Name:        key_press
Parameters:  None
Returns:     None
************************************************************************************/
void Chk_Key_Press(void)
{
	Chk_First_Key();
	Chk_Second_Key();
	Chk_Third_Key();
}

void Chk_First_Key()
{
	if (continuous_mode_flag == true)
	{
		key_status_flag = FIRST_KEY_PIN;
		if (key_status_flag == false)
		{
			firstkey_bounce_ctr--;
			if (firstkey_bounce_ctr == 0)
			{
				key_press_flag = true;
				first_key_flag = true;
				firstkey_release_flag = true;
				second_key_flag = false;
				third_key_flag = false;
				first_press_flag = true;
				firstkey_bounce_ctr = KEY_BOUNCE_CONST;
				first_contpress_ctr--;
				if (first_contpress_ctr == 0)
				{
					first_contpress_ctr = FIRST_CONTPRESS_CONST;
					if (value_count_loc == 1)
					{
						value_count_loc = 5;
					}
					second_contpress_ctr--;
					if (second_contpress_ctr == 0)
					{
						first_contpress_ctr = FIRST_CONTPRESS_CONST;
						second_contpress_ctr = SECOND_CONTPRESS_CONST;
						if (value_count_loc == 5)
						{
							value_count_loc = 10;
						}
					}
				}
			}
		}
		else
		{
			first_press_flag = false;
			firstkey_bounce_ctr = KEY_BOUNCE_CONST;
			if (second_press_flag == false)
			{
				value_count_loc = 1;
				first_contpress_ctr = FIRST_CONTPRESS_CONST;
				second_contpress_ctr = SECOND_CONTPRESS_CONST;
			}
		}
	}
	else
	{
		if (key_press_flag == false)
		{
			key_status_flag = FIRST_KEY_PIN;
			if (key_status_flag == false)
			{
				if (firstkey_release_flag == true)
				{
					firstkey_bounce_ctr--;
					if (firstkey_bounce_ctr == 0)
					{
						key_press_flag = true;
						first_key_flag = true;
						firstkey_release_flag = false;
						//BUZZER = 1;
						//key_beep_counter = 50;
						//key_beep_flag = true;
						second_key_flag = false;
						third_key_flag = false;
						firstkey_bounce_ctr = KEY_BOUNCE_CONST;
					}
				}
			}
			else
			{
				firstkey_bounce_ctr--;
				if (firstkey_bounce_ctr == 0)
				{
					firstkey_bounce_ctr = KEY_BOUNCE_CONST;
					firstkey_release_flag = true;
				}
			}
		}
	}
}

void Chk_Second_Key()
{
	if (continuous_mode_flag == true)
	{
		key_status_flag = SECOND_KEY_PIN;
		if (key_status_flag == false)
		{
			secondkey_bounce_ctr--;
			if (secondkey_bounce_ctr == 0)
			{
				key_press_flag = true;
				second_key_flag = true;
				second_press_flag = true;
				secondkey_release_flag = true;
				first_key_flag = false;
				third_key_flag = false;
				secondkey_bounce_ctr = KEY_BOUNCE_CONST;
				first_contpress_ctr--;
				if (first_contpress_ctr == 0)
				{
					first_contpress_ctr = FIRST_CONTPRESS_CONST;
					if (value_count_loc == 1)
					{
						value_count_loc = 5;
					}
					second_contpress_ctr--;
					if (second_contpress_ctr == 0)
					{
						first_contpress_ctr = FIRST_CONTPRESS_CONST;
						second_contpress_ctr = SECOND_CONTPRESS_CONST;
						if (value_count_loc == 5)
						{
							value_count_loc = 10;
						}
					}
				}
			}
		}
		else
		{
			second_press_flag = false;
			secondkey_bounce_ctr = KEY_BOUNCE_CONST;
			if (first_press_flag == false)
			{
				value_count_loc = 1;
				first_contpress_ctr = FIRST_CONTPRESS_CONST;
				second_contpress_ctr = SECOND_CONTPRESS_CONST;
			}
		}
	}
	else
	{
		if (key_press_flag == false)
		{
			key_status_flag = SECOND_KEY_PIN;
			if (key_status_flag == false)
			{
				if (secondkey_release_flag == true)
				{
					secondkey_bounce_ctr--;
					if (secondkey_bounce_ctr == 0)
					{
						key_press_flag = true;
						second_key_flag = true;
						secondkey_release_flag = false;
						//BUZZER = 1;
						//key_beep_counter = 50;
						//key_beep_flag = true;
						first_key_flag = false;
						third_key_flag = false;
						secondkey_bounce_ctr = KEY_BOUNCE_CONST;
					}
				}
			}
			else
			{
				secondkey_bounce_ctr--;
				if (secondkey_bounce_ctr == 0)
				{
					secondkey_bounce_ctr = KEY_BOUNCE_CONST;
					secondkey_release_flag = true;
				}
			}
		}
	}
}

void Chk_Third_Key()
{
	if (key_press_flag == false)
	{
		key_status_flag = THIRD_KEY_PIN;
		if (key_status_flag == false)
		{
			if (thirdkey_release_flag == true)
			{
				thirdkey_bounce_ctr--;
				if (thirdkey_bounce_ctr == 0)
				{
					key_press_flag = true;
					third_key_flag = true;
					thirdkey_release_flag = false;
					//BUZZER = 1;
					//key_beep_counter = 50;
					//key_beep_flag = true;
					second_key_flag = false;
					first_key_flag = false;
					thirdkey_bounce_ctr = KEY_BOUNCE_CONST;
				}
			}
		}
		else
		{
			thirdkey_bounce_ctr--;
			if (thirdkey_bounce_ctr == 0)
			{
				thirdkey_bounce_ctr = KEY_BOUNCE_CONST;
				thirdkey_release_flag = true;
			}
		}
	}
}


