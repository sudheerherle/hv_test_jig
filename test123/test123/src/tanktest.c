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

void Start_New_Test()
{
	unsigned char result;
	signed int temp_dac_value;
	unsigned long no_of_samples;
	unsigned long avghv_signal_input;
	unsigned long avghv_anode_value;
	unsigned long avghv_cathode_value;
	unsigned long avgbleeder_anode_value;
	unsigned long avgbleeder_cathode_value;
	unsigned long avgfilament_input_value;
	unsigned long avgshort_filament_value;
	unsigned long avglong_filament_value;
	unsigned long finalhv_signal_input;
	unsigned long finalhv_anode_value;
	unsigned long finalhv_cathode_value;
	unsigned long finalbleeder_anode_value;
	unsigned long finalbleeder_cathode_value;
	unsigned long finalfilament_input_value;
	unsigned long finalshort_filament_value;
	unsigned long finallong_filament_value;
	unsigned char nob;
	unsigned long no_of_averages;
	unsigned long temp_hv_input; 
	unsigned long temp_hv_cathode; 
	unsigned long temp_hv_anode; 
	unsigned long temp_bleeder_anode; 
	unsigned long temp_bleeder_cathode; 
	unsigned long temp_filament_input; 
	unsigned long temp_long_filament; 
	unsigned long temp_short_filament; 
	unsigned char usc;
	unsigned char filament_interchange_flag;
	unsigned long temp_signal_input;
	unsigned char dac_adj_flag = false;
	unsigned long prevhv_cathode_value;
	unsigned long prevhv_anode_value;
	unsigned char e2_write_flag = false;
	unsigned char dac_time_ctr;
	dac_time_ctr = MAXDAC_STARTTIME_CONST;
	cur_machine_state = IDLE_TEST_STATE;
	HV_CHECK_PIN = LOW;
	filament_interchange_flag = false;
	discard_adc_flag = true;
	FREQUENCY_CHANGE_PIN = LOW;
	trdgra1 = KHZ20_PERIOD_CONST;
	trdgrb1 = KHZ20_VALUE_CONST;
	trdstr = 0x0F;
	hv_input_value = 0;
	hv_cathode_value = 0;
	hv_anode_value = 0;
	bleeder_anode_value = 0;
	bleeder_cathode_value = 0;
	adc_avg_ctr = 0;
	adcon0 = 0x80;
	hv_input_flag = true;
	Draw_Main_Window();
	temp_dac_value = dac_info_buffer.hv_dac_value;//(unsigned int)INITIAL_HV_DAC_VALUE;
	Write2_DAC_Register(temp_dac_value);
	Write_Center_Text(0,TFT_SIZE_X,5,&model_name[0],(unsigned far char *)&arial_narrow_bold16[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
	Write_Center_Text_Bar(20,TFT_SIZE_X-20,32,&hv_turns_text[0],(unsigned far char *)&arial_narrow_bold16[0],WHITE,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text(5,60,&hv_input_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,60,&vpk_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,86,&anode_hv_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(180,86,&plus_symbol[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,86,&vdc_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,112,&cathode_hv_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(180,112,&minus_symbol[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,112,&vdc_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Center_Text_Bar(20,TFT_SIZE_X-20,140,&bleeder_turns_text[0],(unsigned far char *)&arial_narrow_bold16[0],WHITE,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text(5,168,&bleeder_anode_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(180,168,&plus_symbol[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,168,&mvdc_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,194,&bleeder_cathode_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(180,194,&minus_symbol[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,194,&mvdc_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Display_HVTKey_Menu(0);
	adc_timeout_ctr = ADC_TIMEOUT_CONST;
	adc_timeout_flag = false;
	adc_read_flag = false;
	half_sec_ctr = HALF_SEC_CONST;
	half_sec_flag = false;
	adst = 1;
	adic = 0x01;
	Init_Data_Values();
	avghv_signal_input = 0;
	no_of_samples = 0;
	avghv_anode_value = 0;
	avghv_cathode_value = 0;
	avgbleeder_anode_value = 0;
	avgbleeder_cathode_value = 0;
	key_timeout_ctr = KEY_TIMEOUT_CONST1;
	key_timeout_flag = true;
	key_press_flag = false;
	data_read_flag = false;
	result = false;
	no_of_averages = 0;
	adc_array_ctr = 0;
	max_adc_arrayctr = 0;
	finalhv_signal_input = (unsigned long)REQUIRED_HV_CONST;
	prevhv_cathode_value = 0;
	prevhv_anode_value = 0;
	while (true)
	{
		if (adc_read_flag == true)
		{
			adc_read_flag = false;
			hv_input_array[adc_array_ctr] = (hv_input_value / adc_avg_ctr); 
			hv_cathode_array[adc_array_ctr] = (hv_cathode_value / adc_avg_ctr); 
			hv_anode_array[adc_array_ctr] = (hv_anode_value / adc_avg_ctr); 
			bleeder_anode_array[adc_array_ctr] = (bleeder_anode_value / adc_avg_ctr); 
			bleeder_cathode_array[adc_array_ctr] = (bleeder_cathode_value / adc_avg_ctr); 
			hv_input_value = 0;
			hv_cathode_value = 0;
			hv_anode_value = 0;
			bleeder_anode_value = 0;
			bleeder_cathode_value = 0;
			adc_avg_ctr = 0;
			adc_array_ctr++;
			if (adc_array_ctr > (unsigned long)((unsigned long)ADC_ARRAY_CONST - (unsigned long)1))
			{
				adc_array_ctr = 0;
			}
			if (max_adc_arrayctr < (unsigned long)ADC_ARRAY_CONST)
			{
				max_adc_arrayctr++;
			}
			if (max_adc_arrayctr > 0)
			{
				temp_hv_input = 0;
				temp_hv_cathode = 0;
				temp_hv_anode = 0;
				temp_bleeder_anode = 0;
				temp_bleeder_cathode = 0;
				for (usc=0;usc<max_adc_arrayctr;usc++)
				{
					temp_hv_input = temp_hv_input + hv_input_array[usc];
					temp_hv_cathode = temp_hv_cathode + hv_cathode_array[usc];
					temp_hv_anode = temp_hv_anode + hv_anode_array[usc];
					temp_bleeder_anode = temp_bleeder_anode + bleeder_anode_array[usc];
					temp_bleeder_cathode = temp_bleeder_cathode + bleeder_cathode_array[usc];
				}
				temp_hv_input = temp_hv_input / max_adc_arrayctr;
				temp_hv_cathode = temp_hv_cathode / max_adc_arrayctr;
				temp_hv_anode = temp_hv_anode / max_adc_arrayctr;
				temp_bleeder_anode = temp_bleeder_anode / max_adc_arrayctr;
				temp_bleeder_cathode = temp_bleeder_cathode / max_adc_arrayctr;
				/*temp_signal_input = temp_hv_input % (unsigned long)10;
				temp_hv_input = temp_hv_input / (unsigned long)10;
				if (temp_signal_input > (unsigned long)4)
				{
					temp_hv_input++;
				}*/
			}
			avghv_signal_input = avghv_signal_input + temp_hv_input;
			avghv_cathode_value = avghv_cathode_value + temp_hv_cathode;
			avghv_anode_value = avghv_anode_value + temp_hv_anode;
			avgbleeder_anode_value = avgbleeder_anode_value + temp_bleeder_anode;
			avgbleeder_cathode_value = avgbleeder_cathode_value + temp_bleeder_cathode;
			no_of_samples++;
			if (dac_adj_flag == false)
			{
				finalhv_signal_input = (unsigned long)REQUIRED_HV_CONST;
				prevhv_cathode_value = temp_hv_cathode;
				prevhv_anode_value = temp_hv_anode;
			}
		}
		if (half_sec_flag == true)
		{
			half_sec_flag = false;
			cur_machine_state = HV_TEST_STATE;
			no_of_averages++;
			if (no_of_samples > 0)
			{
				finalhv_signal_input = (avghv_signal_input / no_of_samples);
				finalhv_cathode_value = (avghv_cathode_value / no_of_samples);
				finalhv_cathode_value = ((finalhv_cathode_value + prevhv_cathode_value) / (unsigned long)2);
				prevhv_cathode_value = finalhv_cathode_value;
				finalhv_anode_value = (avghv_anode_value / no_of_samples);
				finalhv_anode_value = ((finalhv_anode_value + prevhv_cathode_value) / (unsigned long)2);
				prevhv_cathode_value = finalhv_anode_value;
				finalbleeder_anode_value = (avgbleeder_anode_value / no_of_samples);
				finalbleeder_cathode_value = (avgbleeder_cathode_value / no_of_samples);
			}
			avghv_signal_input = 0;
			avghv_cathode_value = 0;
			avghv_anode_value = 0;
			avgbleeder_anode_value = 0;
			avgbleeder_cathode_value = 0;
			no_of_samples = 0;
			if (no_of_averages == (unsigned long)4)
			{
				dac_adj_flag = true;
				if (data_read_flag == false)
				{
					Display_HVTKey_Menu(1);
					data_read_flag = true;
				}
			}
			//test_data_buffer.hv_signal_input = finalhv_signal_input;
			test_data_buffer.hv_signal_input = (finalhv_signal_input / (unsigned long)10);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_signal_input,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,61,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			test_data_buffer.hv_anode_volt = finalhv_anode_value;
			if (test_data_buffer.hv_anode_volt < (unsigned long)HV_ANODE_MINCONST)
			{
				test_data_buffer.hv_anode_volt = 0;
			}
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_anode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,87,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			test_data_buffer.hv_cathode_volt = finalhv_cathode_value;
			if (test_data_buffer.hv_cathode_volt < (unsigned long)HV_CATHODE_MINCONST)
			{
				test_data_buffer.hv_cathode_volt = 0;
			}
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_cathode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,113,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			test_data_buffer.bleeder_anode_volt = finalbleeder_anode_value;
			if (test_data_buffer.bleeder_anode_volt < (unsigned long)BLEEDER_ANODE_MINCONST)
			{
				test_data_buffer.bleeder_anode_volt = 0;
			}
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.bleeder_anode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,169,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			test_data_buffer.bleeder_cathode_volt = finalbleeder_cathode_value;
			if (test_data_buffer.bleeder_cathode_volt < (unsigned long)BLEEDER_CATHODE_MINCONST)
			{
				test_data_buffer.bleeder_cathode_volt = 0;
			}
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.bleeder_cathode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,195,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			if (dac_adj_flag == true)
			{
				dac_time_ctr--;
				if (dac_time_ctr == 0)
				{
					if (finalhv_signal_input > (unsigned long)REQUIRED_HV_HICONST)
					{
						temp_hv_input = finalhv_signal_input - (unsigned long)REQUIRED_HV_CONST;
						/*if ((temp_hv_input > (unsigned long)DIFFHV_5TIMES_CONST) && (dac_info_buffer.hv_signature == (unsigned int)DAC_SIGNATURE_CONST))
						{
							temp_dac_value = dac_info_buffer.hv_dac_value;
						}
						else */
						if (temp_hv_input >= (unsigned long)DIFFHV_20TIMES_CONST)
						{
							temp_dac_value = temp_dac_value - (signed int)20;
						}
						else if (temp_hv_input >= (unsigned long)DIFFHV_10TIMES_CONST)
						{
							temp_dac_value = temp_dac_value - (signed int)10;
						}
						else if (temp_hv_input >= (unsigned long)DIFFHV_5TIMES_CONST)
						{
							temp_dac_value = temp_dac_value - (signed int)5;
						}
						else if (temp_hv_input > (unsigned long)DIFFHV_2TIMES_CONST)
						{
							temp_dac_value = temp_dac_value - (signed int)2;
						}
						else
						{
							temp_dac_value--;
						}
						if (temp_dac_value < (signed int)MINIMUM_DAC_VALUE)
						{
							temp_dac_value = (signed int)MINIMUM_DAC_VALUE;
						}
						Write2_DAC_Register(temp_dac_value);
					}
					else if (finalhv_signal_input < (unsigned long)REQUIRED_HV_LOCONST)
					{
						temp_hv_input = (unsigned long)REQUIRED_HV_CONST - finalhv_signal_input;
						/*if ((temp_hv_input < (unsigned long)DIFFHV_5TIMES_CONST) && (dac_info_buffer.hv_signature == (unsigned int)DAC_SIGNATURE_CONST))
						{
							temp_dac_value = dac_info_buffer.hv_dac_value;
						}
						else */
						if (temp_hv_input >= (unsigned long)DIFFHV_20TIMES_CONST)
						{
							temp_dac_value = temp_dac_value + (signed int)20;
						}
						else if (temp_hv_input >= (unsigned long)DIFFHV_10TIMES_CONST)
						{
							temp_dac_value = temp_dac_value + (signed int)10;
						}
						else if (temp_hv_input >= (unsigned long)DIFFHV_5TIMES_CONST)
						{
							temp_dac_value = temp_dac_value + (signed int)5;
						}
						else if (temp_hv_input > (unsigned long)DIFFHV_2TIMES_CONST)
						{
							temp_dac_value = temp_dac_value + (signed int)2;
						}
						else
						{
							temp_dac_value++;
						}
						if (temp_dac_value > (signed int)MAXIMUM_DAC_VALUE)
						{
							temp_dac_value = (signed int)MAXIMUM_DAC_VALUE;
						}
						Write2_DAC_Register(temp_dac_value);
					}
					else
					{
						//equal
						if ((no_of_averages > (unsigned long)DACSAVE_TIMER_CONST) && (e2_write_flag == false))
						{
							if ((temp_dac_value > (dac_info_buffer.hv_dac_value + (unsigned int)2)) || (temp_dac_value < (dac_info_buffer.hv_dac_value - (unsigned int)2)))
							{
								dac_info_buffer.hv_signature = (unsigned int)DAC_SIGNATURE_CONST;
								dac_info_buffer.hv_dac_value = temp_dac_value;
								gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_DACADD_CONST,(unsigned char *)&dac_info_buffer.hv_dac_value,DAC_DATA_BYTES);
								//gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_HVDACADD_CONST,(unsigned char *)&dac_info_buffer.hv_dac_value,HVDAC_DATA_BYTES);
								e2_write_flag = true;
							}
							else if (dac_info_buffer.hv_signature != (unsigned int)DAC_SIGNATURE_CONST)
							{
								dac_info_buffer.hv_signature = (unsigned int)DAC_SIGNATURE_CONST;
								dac_info_buffer.hv_dac_value = temp_dac_value;
								gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_DACADD_CONST,(unsigned char *)&dac_info_buffer.hv_dac_value,DAC_DATA_BYTES);
								//gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_HVDACADD_CONST,(unsigned char *)&dac_info_buffer.hv_dac_value,HVDAC_DATA_BYTES);
								e2_write_flag = true;
							}
						}
					}
					dac_time_ctr = MAXDAC_TIME_CONST;
				}
			}
		}
		if (key_press_flag == true)
		{
			key_timeout_ctr = KEY_TIMEOUT_CONST1;
			key_press_flag = false;
			if (first_key_flag == true)
			{
				if (data_read_flag == true)
				{
					result = true;
					break;
				}
			}
			if (second_key_flag == true)
			{
				data_save_flag = false;
				break;
			}
			if (third_key_flag == true)
			{
				//nothing assigned
			}
		}
		else if (key_timeout_flag == false)
		{
			data_save_flag = false;
			break;
		}
		else
		{
			//check for the serial data from the PC
			Chk_Serial_Data();
			if (shift_machine_state == FIL_TEST_STATE)
			{
				shift_machine_state = IDLE_TEST_STATE;
				if (data_read_flag == true)
				{
					result = true;
					break;
				}
			}
			if (shift_machine_state == STOP_TEST_STATE)
			{
				shift_machine_state = IDLE_TEST_STATE;
				data_save_flag = false;
				break;
			}
			if (shift_machine_state == READ_TEST_STATE)
			{
				//send data to pc
				shift_machine_state = IDLE_TEST_STATE;
				Send_Current_HVData();
			}
		}
	}
	cur_machine_state = IDLE_TEST_STATE;
	adst = 0;
	adic = 0x00;
	Write2_DAC_Register(0);
	trdstr = 0x0C;
	if (result == false)
	{
		return;
	}
	e2_write_flag = false;
	dac_time_ctr = MAXDAC_STARTTIME_CONST;
	HV_CHECK_PIN = HIGH;
	dac_adj_flag = false;
	hv_input_flag = false;
	discard_adc_flag = true;
	FREQUENCY_CHANGE_PIN = HIGH;
	trdgra1 = KHZ30_PERIOD_CONST;
	trdgrb1 = KHZ30_VALUE_CONST;
	trdstr = 0x0F;
	filament_input_value = 0;
	long_filament_value = 0;
	short_filament_value = 0;
	adc_avg_ctr = 0;
	adcon0 = 0x81;
	Draw_Main_Window();
	temp_dac_value = dac_info_buffer.filament_dac_value;//(unsigned int)INITIAL_FIL_DAC_VALUE;
	Write2_DAC_Register(temp_dac_value);
	Write_Center_Text(0,TFT_SIZE_X,5,&model_name[0],(unsigned far char *)&arial_narrow_bold16[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
	Write_Center_Text_Bar(20,TFT_SIZE_X-20,32,&filament_turns_text[0],(unsigned far char *)&arial_narrow_bold16[0],WHITE,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text(5,60,&filament_input_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,60,&vac_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,86,&large_filament_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,86,&vac_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,112,&small_filament_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,112,&vac_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Display_HVTKey_Menu(0);
	adc_timeout_ctr = ADC_TIMEOUT_CONST;
	adc_timeout_flag = false;
	adc_read_flag = false;
	half_sec_ctr = HALF_SEC_CONST;
	half_sec_flag = false;
	adst = 1;
	adic = 0x01;
	avgfilament_input_value = 0;
	avgshort_filament_value = 0;
	avglong_filament_value = 0;
	no_of_samples = 0;
	key_timeout_ctr = KEY_TIMEOUT_CONST1;
	key_timeout_flag = true;
	key_press_flag = false;
	data_read_flag = false;
	no_of_averages = 0;
	adc_array_ctr = 0;
	max_adc_arrayctr = 0;
	finalfilament_input_value = (unsigned long)REQUIRED_FILAMENT_CONST;
	while (true)
	{
		if (adc_read_flag == true)
		{
			adc_read_flag = false;
			filament_input_array[adc_array_ctr] = (filament_input_value / adc_avg_ctr); 
			long_filament_array[adc_array_ctr] = (long_filament_value / adc_avg_ctr); 
			short_filament_array[adc_array_ctr] = (short_filament_value / adc_avg_ctr); 
			filament_input_value = 0;
			long_filament_value = 0;
			short_filament_value = 0;
			adc_avg_ctr = 0;
			adc_array_ctr++;
			if (adc_array_ctr > (unsigned long)((unsigned long)ADC_ARRAY_CONST - (unsigned long)1))
			{
				adc_array_ctr = 0;
			}
			if (max_adc_arrayctr < (unsigned long)ADC_ARRAY_CONST)
			{
				max_adc_arrayctr++;
			}
			if (max_adc_arrayctr > 0)
			{
				temp_filament_input = 0;
				temp_long_filament = 0;
				temp_short_filament = 0;
				for (usc=0;usc<max_adc_arrayctr;usc++)
				{
					temp_filament_input = temp_filament_input + filament_input_array[usc];
					temp_long_filament = temp_long_filament + long_filament_array[usc];
					temp_short_filament = temp_short_filament + short_filament_array[usc];
				}
				temp_filament_input = temp_filament_input / max_adc_arrayctr;
				temp_long_filament = temp_long_filament / max_adc_arrayctr;
				temp_short_filament = temp_short_filament / max_adc_arrayctr;
			}
			avgfilament_input_value = avgfilament_input_value + temp_filament_input;
			avglong_filament_value = avglong_filament_value + temp_long_filament;
			avgshort_filament_value = avgshort_filament_value + temp_short_filament;
			no_of_samples++;
		}
		if (half_sec_flag == true)
		{
			half_sec_flag = false;
			cur_machine_state = FIL_TEST_STATE;
			no_of_averages++;
			if (no_of_samples > 0)
			{
				finalfilament_input_value = (avgfilament_input_value / no_of_samples);
				finallong_filament_value = (avglong_filament_value / no_of_samples);
				finalshort_filament_value = (avgshort_filament_value / no_of_samples);
			}
			avgfilament_input_value = 0;
			avglong_filament_value = 0;
			avgshort_filament_value = 0;
			no_of_samples = 0;
			if (no_of_averages == (unsigned long)4)
			{
				dac_adj_flag = true;
				if (data_read_flag == false)
				{
					Display_HVTKey_Menu(2);
					data_read_flag = true;
				}
				if (finallong_filament_value > (unsigned long)FIL_EXIST_CONST)
				{
					filament_interchange_flag = true;
				}
				FILAMENT_CHECK_PIN = LOW;
			}
			if (no_of_averages > (unsigned long)10000)
			{
				no_of_averages = (unsigned long)10;
			}
			//check filament interchange
			//test_data_buffer.filament_signal_input = finalfilament_input_value;
			test_data_buffer.filament_signal_input = (finalfilament_input_value / (unsigned long)10);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.filament_signal_input,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,61,195,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			test_data_buffer.long_filament_volt = finallong_filament_value;
			if (test_data_buffer.long_filament_volt < (unsigned long)FIL_EXIST_CONST)
			{
				test_data_buffer.long_filament_volt = 0;
			}
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.long_filament_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,87,195,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			test_data_buffer.short_filament_volt = finalshort_filament_value;
			if (test_data_buffer.short_filament_volt < (unsigned long)FIL_EXIST_CONST)
			{
				test_data_buffer.short_filament_volt = 0;
			}
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.short_filament_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,113,195,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			if (dac_adj_flag == true)
			{
				dac_time_ctr--;
				if (dac_time_ctr == 0)
				{
					if (finalfilament_input_value > (unsigned long)REQUIRED_FILAMENT_HICONST)
					{
						temp_filament_input = finalfilament_input_value - (unsigned long)REQUIRED_FILAMENT_CONST;
						/*if ((temp_filament_input > (unsigned long)DIFFFIL_5TIMES_CONST) && (dac_info_buffer.fil_signature == (unsigned int)DAC_SIGNATURE_CONST))
						{
							temp_dac_value = dac_info_buffer.filament_dac_value;
						}
						else */
						if (temp_filament_input >= (unsigned long)DIFFFIL_20TIMES_CONST)
						{
							temp_dac_value = temp_dac_value - (signed int)20;
						}
						else if (temp_filament_input >= (unsigned long)DIFFFIL_10TIMES_CONST)
						{
							temp_dac_value = temp_dac_value - (signed int)10;
						}
						else if (temp_filament_input >= (unsigned long)DIFFFIL_5TIMES_CONST)
						{
							temp_dac_value = temp_dac_value - (signed int)5;
						}
						else if (temp_filament_input > (unsigned long)DIFFFIL_2TIMES_CONST)
						{
							temp_dac_value = temp_dac_value - (signed int)2;
						}
						else
						{
							temp_dac_value--;
						}
						if (temp_dac_value < (signed int)MINIMUM_DAC_VALUE)
						{
							temp_dac_value = (signed int)MINIMUM_DAC_VALUE;
						}
						Write2_DAC_Register(temp_dac_value);
					}
					else if (finalfilament_input_value < (unsigned long)REQUIRED_FILAMENT_LOCONST)
					{
						temp_filament_input = (unsigned long)REQUIRED_FILAMENT_CONST - finalfilament_input_value;
						/*if ((temp_filament_input < (unsigned long)DIFFFIL_5TIMES_CONST) && (dac_info_buffer.fil_signature == (unsigned int)DAC_SIGNATURE_CONST))
						{
							temp_dac_value = dac_info_buffer.filament_dac_value;
						}
						else */
						if (temp_filament_input >= (unsigned long)DIFFFIL_20TIMES_CONST)
						{
							temp_dac_value = temp_dac_value + (signed int)20;
						}
						else if (temp_filament_input >= (unsigned long)DIFFFIL_10TIMES_CONST)
						{
							temp_dac_value = temp_dac_value + (signed int)10;
						}
						else if (temp_filament_input >= (unsigned long)DIFFFIL_5TIMES_CONST)
						{
							temp_dac_value = temp_dac_value + (signed int)5;
						}
						else if (temp_filament_input > (unsigned long)DIFFFIL_2TIMES_CONST)
						{
							temp_dac_value = temp_dac_value + (signed int)2;
						}
						else
						{
							temp_dac_value++;
						}
						if (temp_dac_value > (signed int)MAXIMUM_DAC_VALUE)
						{
							temp_dac_value = (signed int)MAXIMUM_DAC_VALUE;
						}
						Write2_DAC_Register(temp_dac_value);
					}
					else
					{
						//equal
						if ((no_of_averages > (unsigned long)DACSAVE_TIMER_CONST) && (e2_write_flag == false))
						{
							if ((temp_dac_value > (dac_info_buffer.filament_dac_value + (unsigned int)2)) || (temp_dac_value < (dac_info_buffer.filament_dac_value - (unsigned int)2)))
							{
								dac_info_buffer.fil_signature = (unsigned int)DAC_SIGNATURE_CONST;
								dac_info_buffer.filament_dac_value = temp_dac_value;
								gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_DACADD_CONST,(unsigned char *)&dac_info_buffer.hv_dac_value,DAC_DATA_BYTES);
								//gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_FILDACADD_CONST,(unsigned char *)&dac_info_buffer.filament_dac_value,FILDAC_DATA_BYTES);
								e2_write_flag = true;
							}
							else if (dac_info_buffer.filament_dac_value != (unsigned int)DAC_SIGNATURE_CONST)
							{
								dac_info_buffer.fil_signature = (unsigned int)DAC_SIGNATURE_CONST;
								dac_info_buffer.filament_dac_value = temp_dac_value;
								gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_DACADD_CONST,(unsigned char *)&dac_info_buffer.hv_dac_value,DAC_DATA_BYTES);
								//gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,E2_FILDACADD_CONST,(unsigned char *)&dac_info_buffer.filament_dac_value,FILDAC_DATA_BYTES);
								e2_write_flag = true;
							}
						}
					}
					//nob = Conv_Data2_AsciiDecimal(temp_dac_value,&temp_disp_array[0]);
					//Write_Right_Text_Bar(265,139,195,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
					dac_time_ctr = MAXDAC_TIME_CONST;
				}
			}
		}
		if (key_press_flag == true)
		{
			key_timeout_ctr = KEY_TIMEOUT_CONST1;
			key_press_flag = false;
			if (first_key_flag == true)
			{
				//nothing assigned
			}
			if (second_key_flag == true)
			{
				data_save_flag = false;
				break;
			}
			if (third_key_flag == true)
			{
				if (data_read_flag == true)
				{
					Store_Data2_Eeprom(1);
					break;
				}
			}
		}
		else if (key_timeout_flag == false)
		{
			data_save_flag = false;
			break;
		}
		else
		{
			//check for the serial data from the PC
			Chk_Serial_Data();
			if (shift_machine_state == SAVE_TEST_STATE)
			{
				shift_machine_state = IDLE_TEST_STATE;
				if (data_read_flag == true)
				{
					Store_Data2_Eeprom(0);
					break;
				}
			}
			if (shift_machine_state == STOP_TEST_STATE)
			{
				shift_machine_state = IDLE_TEST_STATE;
				//data_save_flag = false;
				break;
			}
			if (shift_machine_state == READ_TEST_STATE)
			{
				//send data to pc
				shift_machine_state = IDLE_TEST_STATE;
				Send_Current_FILData();
			}
		}
		if (filament_interchange_flag == true)
		{
			Display_Filament_Interchange();
			//send the filament interchange information to the PC
			break;
		}
	}
	cur_machine_state = IDLE_TEST_STATE;
	trdstr = 0x0C;
	FREQUENCY_CHANGE_PIN = LOW;
	FILAMENT_CHECK_PIN = HIGH;
	adst = 0;
	adic = 0x00;
	Write2_DAC_Register(0);
}

void Init_Data_Values()
{
	test_data_buffer.hv_signal_input = 0;
	test_data_buffer.hv_anode_volt = 0;
	test_data_buffer.hv_cathode_volt = 0;
	test_data_buffer.bleeder_anode_volt = 0;
	test_data_buffer.bleeder_cathode_volt = 0;
	test_data_buffer.filament_signal_input = 0;
	test_data_buffer.short_filament_volt = 0;
	test_data_buffer.long_filament_volt = 0;
}

void Store_Data2_Eeprom(unsigned char continue_flag)
{
	test_data_buffer.test_number = pointer_info_buffer.test_no + 1;
	if (test_data_buffer.test_number == 0)
	{
		test_data_buffer.test_number = 1;
	}
	gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,pointer_info_buffer.cur_data_pointer,(unsigned char *)&test_data_buffer.test_number,TRAN_DATA_BYTES);
	pointer_info_buffer.cur_data_pointer = pointer_info_buffer.cur_data_pointer + TRAN_DATA_BYTES;
	if (pointer_info_buffer.cur_data_pointer >= E2EVENTEND_ADD_CONST)
	{
		pointer_info_buffer.cur_data_pointer = E2EVENTSTART_ADD_CONST;
	}
	pointer_info_buffer.test_no = test_data_buffer.test_number;
	gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,POINTERINFO_ADD_CONST,(unsigned char *)&pointer_info_buffer.total_bytes,POINTER_INFO_BYTES);
	Display_Updated_Message(test_data_buffer.test_number,continue_flag);
}

void Send_Current_HVData()
{
	struct hv_cctalk_data pc_data_buffer;
	pc_data_buffer.nob = 24;
	//pc_data_buffer.unit_address = hvt_slno_loc;
	pc_data_buffer.pchv_test_data.hv_signal_input = test_data_buffer.hv_signal_input;
	pc_data_buffer.pchv_test_data.hv_anode_volt = test_data_buffer.hv_anode_volt;
	pc_data_buffer.pchv_test_data.hv_cathode_volt = test_data_buffer.hv_cathode_volt;
	pc_data_buffer.pchv_test_data.bleeder_anode_volt = test_data_buffer.bleeder_anode_volt;
	pc_data_buffer.pchv_test_data.bleeder_cathode_volt = test_data_buffer.bleeder_cathode_volt;
	Send_CCTalk_Data((struct general_cctalk_data *)&pc_data_buffer.dest_addr,READ_TEST_HEADER,false,MAXPC_TIMEOUT_CONST);
}

void Send_Current_FILData()
{
	struct fil_cctalk_data pc_data_buffer;
	pc_data_buffer.nob = 16;
	//pc_data_buffer.unit_address = hvt_slno_loc;
	pc_data_buffer.pcfil_test_data.filament_signal_input = test_data_buffer.filament_signal_input;
	pc_data_buffer.pcfil_test_data.short_filament_volt = test_data_buffer.short_filament_volt;
	pc_data_buffer.pcfil_test_data.long_filament_volt = test_data_buffer.long_filament_volt;
	Send_CCTalk_Data((struct general_cctalk_data *)&pc_data_buffer.dest_addr,READ_TEST_HEADER,false,MAXPC_TIMEOUT_CONST);
}
