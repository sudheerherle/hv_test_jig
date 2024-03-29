/***********************************************************************/
/*                                                                     */
/*  FILE        :heap.c                                                */
/*  DATE        :Thu, Jul 08, 2010                                     */
/*  DESCRIPTION :define the size of heap.                              */
/*  CPU GROUP   :2A                                                    */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.18).    */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/***********************************************************************/

/***********************************************************
* COMPILER for R8C/Tiny
* Copyright (C) 2004 (2010) Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
*
* heap.c
*
* allocate heap area
*
* $Date: 2006/06/13 10:45:18 $
* $Revision: 1.4 $
**********************************************************/
#include "typedefine.h"
#include "cstartdef.h"

#if __HEAPSIZE__ != 0
#pragma SECTION	bss	heap

_UBYTE heap_area[__HEAPSIZE__];
#endif
