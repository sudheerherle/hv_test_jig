/***********************************************************************/
/*                                                                     */
/*  FILE        :resetprg.c                                            */
/*  DATE        :Thu, Jul 08, 2010                                     */
/*  DESCRIPTION :initialize for C language.                            */
/*  CPU GROUP   :2A                                                    */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.18).    */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/***********************************************************************/
/*********************************************************************
*
* Device     : R8C/Tiny/2A,2B
*
* File Name  : resetprg.c
*
* Abstract   : Initialize for C language.
*
* History    : 1.30  (2009-01-27)  [Hardware Manual Revision : 2.00]
*
* Copyright (C) 2009 (2010) Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
*
*********************************************************************/
#include "sfr_r82b.h"
#include "typedefine.h"
#include "resetprg.h"

DEF_SBREGISTER;
extern _UINT	_stack_top,_istack_top;
#pragma entry start
void start(void);
extern void initsct(void);
extern void _init(void);
//void exit(void);
void main(void);

#pragma section program interrupt 

void start(void)
{
	_isp_	= &_istack_top; 	// set interrupt stack pointer
	prcr	= 0x02U;		// change protect mode register
	pm0	= 0x00U;		// set processor mode register
	prcr	= 0x00U;		// change protect mode register
	_flg_	= __F_value__;	// set flag register
#if __STACKSIZE__!=0
	_sp_	= &_stack_top; 	// set user stack pointer
#endif
	_sb_	= 0x400U;	// 400H fixation (Do not change)
	_intbh_ = 0x00U; 	// set variable vector's address
	_asm("	ldc	#(topof vector)&0FFFFh,INTBL");

	initsct();		// initlalize each sections

#if __HEAPSIZE__ != 0
	heap_init();		// initialize heap 
#endif
#if __STANDARD_IO__ != 0
	_init();			// initialize standard I/O
#endif
	_fb_ = 0U;		// initialize FB registe for debugger
	main();			// call main routine

//	exit();		// call exit
}
/*
void exit(void)
{
	while(1)
	{
		;		//infinite loop
	}
}
*/

