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

void Initialise_Oscillator()
{
	prc0 = 1;							// WRITE ENABLE = PROTECT DISABLE
	//on chip oscillator initialisation
    cm1 = 0x06;                             
    cm0 = 0x20; 
    fra00 = 1;                      // order of the setting is imp. 
    fra2 = 0x00;                    // on chip high speed osc divided by 2; - 20MHz Clock to CPU and Peripheral Function
    fra01 = 1;                      // select high speed on chip osc. 
    ocd2 = 1; 
	prc0 = 0;						// WRITE DISABLE = PROTECT ENABLE
}

void Initialise_Port_Structure()
{
	//port information
	//p0_0 - p0_7 Analog inputs
	//p1_0 - p1_3 spare
	//p1_4 - p1_5 uart0 - pc
	//p1_6 - p1_7 spare
	//p2_0 - filament check pin output
	//p2_1 - p2_2 iic bus
	//p2_3 - p2_7 spare
	//p3_0 - p3_7 tft db10 - db17 output
	//p4_3 - p4_7 spare
	//p5_0 - tft cs output
	//p5_1 - tft rs output
	//p5_2 - tft wr output
	//p5_3 - tft rd output
	//p5_4 - tft reset output
	//p6_0 - p6_7 tft db0 - db7 output
	//p8_0 - p8_2 key inputs
	//p8_2 - p8_6 spare

	//Input ports assignment
	//p1_0 - p1_3 - keyboard
	//p1_5 - rx0
	//p1_7 - rtc square wave input
	//p4_5 - touch panel adc interrupt
	//p4_6 - p4_7 spare inputs
	//p6_4 - rx2
	//p6_7 - rx1
	//p8_5 - spi data input
	
	//data direction
	prc2 = 1;		//port direction write enable off
	pd0 = 0x00;
	pd1 = 0xdf;
	pd2 = 0xff;
	pd3 = 0xff;
	pd4 = 0x3f;
	pd5 = 0xff;
	pd6 = 0xff;
	pd8 = 0xe8;
	prc2 = 0;		//port direction write enable off	
}

void Initialise_Port_Pins()
{
	//output
	p0 = 0x00;
	p1 = 0x00;
	p2 = 0x20;
	p3 = 0x00;
	p4 = 0x00;
	p5 = 0x00;
	p6 = 0x00;
	p8 = 0x00;
	SCL = 1;
	SDA = 1;
	Set_SCL_As_Output();
	FREQUENCY_CHANGE_PIN = LOW;
	FILAMENT_CHECK_PIN = HIGH;
	HV_CHECK_PIN = HIGH;
}

void Initialise_Variables()
{
	half_sec_flag = true;
	half_sec_ctr = (unsigned int)HALF_SEC_CONST;
	//key_beep_counter = 50;
	value_count_loc =1;
	firstkey_bounce_ctr = KEY_BOUNCE_CONST;
	first_contpress_ctr = FIRST_CONTPRESS_CONST;
	second_contpress_ctr = SECOND_CONTPRESS_CONST;
	secondkey_bounce_ctr = KEY_BOUNCE_CONST;
	thirdkey_bounce_ctr = KEY_BOUNCE_CONST;
	adc_timeout_flag = false;
	adc_timeout_ctr = ADC_TIMEOUT_CONST;
	cur_machine_state = IDLE_TEST_STATE;
}

void Start_Watch_Dog_Timer()
{
	//starting watch dog timer to avoid the hanging of the microcontroller
	wdc7 = 1;
	wdts = 0;
	prc1 = 1;
	pm12 = 1;
	prc1 = 0;
}

void Check_E2prom_Signature()
{
	unsigned char e2_sig;
	unsigned char eeprom_status_flag;
	unsigned char usc;
	eeprom_error_code = NO_EEPROM_ERROR;
	eeprom_status_flag = true;
	for (usc=0;usc<5;usc++)
	{
		e2_sig = 0;
		gsc_i2c_retsts = ReadFromi2c(E2_ADD_CONST,0x02,E2_SIGADD_CONST,&e2_sig,1);
		if (gsc_i2c_retsts == 0)
		{
			eeprom_status_flag = false;
			break;
		}
		Delay_Half_Seconds(1);
	}
	if (eeprom_status_flag == false)
	{
		if (e2_sig == E2_SIGNATURE_CONST)
		{
			Retrieve_E2_Values();
		}
		else
		{
			gsc_i2c_retsts = ReadFromi2c(E2_ADD_CONST,0x02,EXTRA_E2SIGADD_CONST,&e2_sig,1);
			if (e2_sig == E2_SIGNATURE_CONST)
			{
				Retrieve_E2_Values();
			}
			else
			{
				Store_Factory_Values();
			}
			Store_E2_Signature();
		}
	}
	if (eeprom_status_flag == true)
	{
		eeprom_error_code = BOARD_EEPROM_ERROR;
	}
}

void Retrieve_E2_Values()
{
	gsc_i2c_retsts = ReadFromi2c(E2_ADD_CONST,0x02,POINTERINFO_ADD_CONST,(unsigned char *)&pointer_info_buffer.total_bytes,POINTER_INFO_BYTES);
	gsc_i2c_retsts = ReadFromi2c(E2_ADD_CONST,0x02,pointer_info_buffer.cur_data_pointer,(unsigned char *)&test_data_buffer.test_number,TRAN_DATA_BYTES);
	gsc_i2c_retsts = ReadFromi2c(E2_ADD_CONST,0x02,E2_DACADD_CONST,(unsigned char *)&dac_info_buffer.hv_dac_value,DAC_DATA_BYTES);
}

void Store_Factory_Values()
{
	/*unsigned int usi;
	test_data_buffer.test_number = 0;
	test_data_buffer.hv_signal_input = 0;
	test_data_buffer.hv_anode_volt = 0;
	test_data_buffer.hv_cathode_volt = 0;
	test_data_buffer.bleeder_anode_volt = 0;
	test_data_buffer.bleeder_cathode_volt = 0;
	test_data_buffer.filament_signal_input = 0;
	test_data_buffer.short_filament_volt = 0;
	test_data_buffer.long_filament_volt = 0;
	pointer_info_buffer.cur_data_pointer = E2EVENTSTART_ADD_CONST;
	for (usi=0;usi<TOTAL_TRANS_CONST;usi++)
	{
		gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,pointer_info_buffer.cur_data_pointer,(unsigned char *)&test_data_buffer.test_number,TRAN_DATA_BYTES);
		pointer_info_buffer.cur_data_pointer = pointer_info_buffer.cur_data_pointer + TRAN_DATA_BYTES;
	}*/
	pointer_info_buffer.total_bytes = 0;
	pointer_info_buffer.cur_data_pointer = E2EVENTSTART_ADD_CONST;
	pointer_info_buffer.last_download_pointer = E2EVENTSTART_ADD_CONST;
	pointer_info_buffer.test_no = 0;
	gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,POINTERINFO_ADD_CONST,(unsigned char *)&pointer_info_buffer.total_bytes,POINTER_INFO_BYTES);
	dac_info_buffer.hv_signature = (unsigned int)0;
	dac_info_buffer.fil_signature = (unsigned int)0;
	dac_info_buffer.hv_dac_value = (unsigned int)INITIAL_HV_DAC_VALUE;
	dac_info_buffer.filament_dac_value = (unsigned int)INITIAL_FIL_DAC_VALUE;
	gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_DACADD_CONST,(unsigned char *)&dac_info_buffer.hv_dac_value,DAC_DATA_BYTES);
}

void Store_E2_Signature()
{
	unsigned char temp_char;
	temp_char = E2_SIGNATURE_CONST;
	gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_SIGADD_CONST,&temp_char,1);
	gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,EXTRA_E2SIGADD_CONST,&temp_char,1);
}

void Get_HVT_SerialNo()
{
	unsigned far char * temp_ptr;
	unsigned char usc;
	temp_ptr = (unsigned far char *)HVT_SL_NO;
	hvt_slno_loc = 0;
	for (usc=0;usc<4;usc++)
	{
		hvt_slno_loc <<= 8;
		hvt_slno_loc |= temp_ptr[usc];
	}
}

