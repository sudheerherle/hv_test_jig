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
#include "transactions.h"
#include "adc.h"
#include "cctalk.h"

unsigned char Send_CCTalk_Data(struct general_cctalk_data * cctalk_info_buffer,unsigned char cctalk_header,unsigned char reply_flag, unsigned int time_value)
{
	unsigned char usc;
	unsigned char result;
	unsigned int nob;
	result = false;
	cctalk_info_buffer->dest_addr = DEST_PC_ADDRESS;
	nob = cctalk_info_buffer->nob + 5;
	cctalk_info_buffer->header = cctalk_header;	//cctalk header
	cctalk_info_buffer->crc = 0;	//initial crc value
	Calculate_CCTalkData_Chksum(cctalk_info_buffer);
	if (reply_flag == false)
	{
		Send_Data2_UART0((unsigned char *)cctalk_info_buffer,nob);
	}
	else
	{
		for (usc=0;usc<SERIALDATA_RETRY_CONST;usc++)
		{
			pc_timeout_ctr = time_value;
			pc_timeout_flag = true;
			pc_data_ctr = 0;
			pc_data_recd = false;
			Send_Data2_UART0((unsigned char *)cctalk_info_buffer,nob);
			result = Wait4_UART0_Data();
			if (result == true)
			{
				break;
			}
		}
	}
	return (result);
}

unsigned char Wait4_UART0_Data()
{
	unsigned long temp_ctr = 0;
	unsigned char return_value;
	return_value = false;
	while (true)
	{
		if (pc_data_recd == true)
		{
			pc_data_recd = false;
			pc_timeout_flag = false;
			return_value = Compare_CCTalkData_Chksum();
			break;
		}
		else if (pc_timeout_flag == false)
		{
			return_value = false;
			break;
		}
	}
	return (return_value);
}

void Calculate_CCTalkData_Chksum(struct general_cctalk_data * cctalk_info_buffer)
{
	unsigned int crc;
	unsigned char nob;
	nob = cctalk_info_buffer->nob + 5;
	crc = Calculate_CCITT_CRC((unsigned char *)cctalk_info_buffer,nob);
	cctalk_info_buffer->crc = crc;
}

unsigned char Compare_CCTalkData_Chksum()
{
	unsigned char return_value;
	unsigned int recd_crc_value;
	unsigned int crc;
	recd_crc_value = pc_recd_buffer.crc;
	pc_recd_buffer.crc = 0;
	crc = Calculate_CCITT_CRC((unsigned char *)&pc_recd_buffer.dest_addr,pc_recd_buffer.nob+5);
	return_value = false;
	if (crc == recd_crc_value)
	{
		return_value = true;
	}
	return (return_value);
}

unsigned int Calculate_CCITT_CRC(unsigned far char * data_buffer_ptr,unsigned int nob)
{
	unsigned int i;
	unsigned int crc;
	crc = 0;
	for (i=0;i<nob;++i)
	{
		crc = ( crc << 8 ) ^ CCITT_CRC_LookupTable[(crc >> 8) ^ data_buffer_ptr[i]];
	}
	return (crc);
}


