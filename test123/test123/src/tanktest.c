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
	unsigned int temp_dac_value;
	unsigned long no_of_samples;
	unsigned long avghv_signal_input;
	unsigned long avghv_anode_value;
	unsigned long avghv_cathode_value;
	unsigned long avgbleeder_anode_value;
	unsigned long avgbleeder_cathode_value;
	unsigned long hv_input_value;
	unsigned long hv_anode_value;
	unsigned long hv_cathode_value;
	unsigned long bleeder_anode_value;
	unsigned long bleeder_cathode_value;
	unsigned long avgfilament_input_value;
	unsigned long avgshort_filament_value;
	unsigned long avglong_filament_value;
	unsigned long filament_input_value;
	unsigned long short_filament_value;
	unsigned long long_filament_value;
	unsigned long prevavghv_signal_input;
	unsigned long prevavghv_anode_value;
	unsigned long prevavghv_cathode_value;
	unsigned long prevavgbleeder_anode_value;
	unsigned long prevavgbleeder_cathode_value;
	unsigned long prevhv_input_value;
	unsigned long prevhv_anode_value;
	unsigned long prevhv_cathode_value;
	unsigned long prevbleeder_anode_value;
	unsigned long prevbleeder_cathode_value;
	unsigned long prevavgfilament_input_value;
	unsigned long prevavgshort_filament_value;
	unsigned long prevavglong_filament_value;
	unsigned long prevfilament_input_value;
	unsigned long prevshort_filament_value;
	unsigned long prevlong_filament_value;
	unsigned char nob;
	unsigned int color;
	unsigned char filament_check_flag;
	unsigned long no_of_averages;
	unsigned long prev_average_no;
	unsigned long cur_average_no;
	unsigned char filament_short_flag;
	unsigned char filament_long_flag;
	filament_short_flag = false;
	filament_long_flag = 0;
	HV_CHECK_PIN = LOW;
	Draw_Main_Window();
	Write_Center_Text(0,TFT_SIZE_X,5,&model_name[0],(unsigned far char *)&arial_narrow_bold16[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
	Write_Center_Text_Bar(20,TFT_SIZE_X-20,32,&hv_turns_text[0],(unsigned far char *)&arial_narrow_bold16[0],BRIGHTBLUE,LIGHTGRAY,TFT_MODE_FULL);
	Write_Text(5,60,&hv_input_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,60,&vpk_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	//Write_Text(267,60,&vac_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,86,&anode_hv_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(180,86,&plus_symbol[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,86,&vdc_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,112,&cathode_hv_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(180,112,&minus_symbol[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,112,&vdc_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Center_Text_Bar(20,TFT_SIZE_X-20,140,&bleeder_turns_text[0],(unsigned far char *)&arial_narrow_bold16[0],BRIGHTBLUE,LIGHTGRAY,TFT_MODE_FULL);
	Write_Text(5,168,&bleeder_anode_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(180,168,&plus_symbol[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,168,&mvdc_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,194,&bleeder_cathode_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(180,194,&minus_symbol[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,194,&mvdc_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Display_HVTKey_Menu(0);
	FREQUENCY_CHANGE_PIN = LOW;
	trdgra1 = KHZ20_PERIOD_CONST;
	trdgrb1 = KHZ20_VALUE_CONST;
	trdstr = 0x0F;
	Init_Data_Values();
	avghv_signal_input = 0;
	no_of_samples = 0;
	avghv_anode_value = 0;
	avghv_cathode_value = 0;
	avgbleeder_anode_value = 0;
	avgbleeder_cathode_value = 0;
	prevavghv_signal_input = 0;
	prevavghv_anode_value = 0;
	prevavghv_cathode_value = 0;
	prevavgbleeder_anode_value = 0;
	prevavgbleeder_cathode_value = 0;
	prevhv_input_value = 0;
	prevhv_anode_value = 0;
	prevhv_cathode_value = 0;
	prevbleeder_anode_value = 0;
	prevbleeder_cathode_value = 0;
	half_sec_ctr = HALF_SEC_CONST;
	half_sec_flag = false;
	temp_dac_value = (unsigned int)INITIAL_HV_DAC_VALUE;
	Write2_DAC_Register(temp_dac_value);
	key_timeout_ctr = KEY_TIMEOUT_CONST1;
	key_timeout_flag = true;
	key_press_flag = false;
	data_read_flag = false;
	result = false;
	filament_check_flag = false;
	no_of_averages = 1;
	prev_average_no = 1;
	cur_average_no = 1;
	while (true)
	{
		hv_input_value = ReadN_Average_ADC(0x80,1,HV_SIGNAL_INPUT_CONST);
		hv_anode_value = ReadN_Average_ADC(0x83,1,HV_ANODECATHODE_CONST);
		hv_cathode_value = ReadN_Average_ADC(0x82,1,HV_ANODECATHODE_CONST);
		bleeder_anode_value = ReadN_Average_ADC(0x84,1,HV_BLEEDER_AC_CONST);
		bleeder_cathode_value = ReadN_Average_ADC(0x85,1,HV_BLEEDER_AC_CONST);
		if (hv_input_value > (unsigned long)REQUIRED_HV_HICONST)//REQUIRED_HV_CONST)
		{
			if (temp_dac_value > (unsigned int)0)
			{
				temp_dac_value--;
			}
			Write2_DAC_Register(temp_dac_value);
		}
		else if (hv_input_value < (unsigned long)REQUIRED_HV_LOCONST)//REQUIRED_HV_CONST)
		{
			if (temp_dac_value < (unsigned int)MAX_DAC_CONST)
			{
				temp_dac_value++;
			}
			Write2_DAC_Register(temp_dac_value);
		}
		//hv_input_value = hv_input_value / (unsigned long)10;
		if (half_sec_flag == true)
		{
			half_sec_flag = false;
			no_of_averages--;
			if (no_of_averages == 0)
			{
				prevavghv_signal_input = (prevavghv_signal_input + (avghv_signal_input / no_of_samples));
				prevavghv_anode_value = (prevavghv_anode_value + (avghv_anode_value / no_of_samples));
				prevavghv_cathode_value = (prevavghv_cathode_value + (avghv_cathode_value / no_of_samples));
				prevavgbleeder_anode_value = (prevavgbleeder_anode_value + (avgbleeder_anode_value / no_of_samples));
				prevavgbleeder_cathode_value = (prevavgbleeder_cathode_value + (avgbleeder_cathode_value / no_of_samples));
				avghv_signal_input = 0;
				avghv_anode_value = 0;
				avghv_cathode_value = 0;
				avgbleeder_anode_value = 0;
				avgbleeder_cathode_value = 0;
				no_of_samples = 0;
				if (cur_average_no == 10)
				{
					no_of_averages = 10;
				}
				/*else if (cur_average_no == 1)
				{
					prev_average_no = 2;
					no_of_averages = prev_average_no;
					if (data_read_flag == false)
					{
						Display_HVTKey_Menu(1);
						data_read_flag = true;
					}
				}*/
				else
				{
					prev_average_no = prev_average_no + 1;
					no_of_averages = prev_average_no;
					if (no_of_averages == 4)
					{
						if (data_read_flag == false)
						{
							Display_HVTKey_Menu(1);
							data_read_flag = true;
						}
					}
				}
				prevavghv_signal_input = prevavghv_signal_input / (unsigned long)10;
				test_data_buffer.hv_signal_input = prevavghv_signal_input / cur_average_no;
				nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_signal_input,&temp_disp_array[0]);
				Write_Right_Text_Bar(265,61,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
				test_data_buffer.hv_anode_volt = prevavghv_anode_value / cur_average_no;
				if (test_data_buffer.hv_anode_volt < HV_ANODE_MINCONST)
				{
					test_data_buffer.hv_anode_volt = 0;
				}
				color = Check_Range(test_data_buffer.hv_anode_volt,HVT_LO_CONST,HVT_HI_CONST);
				nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_anode_volt,&temp_disp_array[0]);
				Write_Right_Text_Bar(265,87,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
				test_data_buffer.hv_cathode_volt = prevavghv_cathode_value / cur_average_no;
				if (test_data_buffer.hv_cathode_volt < HV_CATHODE_MINCONST)
				{
					test_data_buffer.hv_cathode_volt = 0;
				}
				color = Check_Range(test_data_buffer.hv_cathode_volt,HVT_LO_CONST,HVT_HI_CONST);
				nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_cathode_volt,&temp_disp_array[0]);
				Write_Right_Text_Bar(265,113,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
				test_data_buffer.bleeder_anode_volt = prevavgbleeder_anode_value / cur_average_no;
				if (test_data_buffer.bleeder_anode_volt < BLEEDER_ANODE_MINCONST)
				{
					test_data_buffer.bleeder_anode_volt = 0;
				}
				color = Check_Range(test_data_buffer.bleeder_anode_volt,BLEEDER_LO_CONST,BLEEDER_HI_CONST);
				nob = Conv_Data2_AsciiDecimal(test_data_buffer.bleeder_anode_volt,&temp_disp_array[0]);
				Write_Right_Text_Bar(265,169,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
				test_data_buffer.bleeder_cathode_volt = prevavgbleeder_cathode_value / cur_average_no;
				if (test_data_buffer.bleeder_cathode_volt < BLEEDER_CATHODE_MINCONST)
				{
					test_data_buffer.bleeder_cathode_volt = 0;
				}
				color = Check_Range(test_data_buffer.bleeder_cathode_volt,BLEEDER_LO_CONST,BLEEDER_HI_CONST);
				nob = Conv_Data2_AsciiDecimal(test_data_buffer.bleeder_cathode_volt,&temp_disp_array[0]);
				Write_Right_Text_Bar(265,195,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
				prevavghv_signal_input = 0;
				prevavghv_anode_value = 0;
				prevavghv_cathode_value = 0;
				prevavgbleeder_anode_value = 0;
				prevavgbleeder_cathode_value = 0;
				cur_average_no = no_of_averages;
			}
			else
			{
				prevavghv_signal_input = (prevavghv_signal_input + (avghv_signal_input / no_of_samples));
				prevavghv_anode_value = (prevavghv_anode_value + (avghv_anode_value / no_of_samples));
				prevavghv_cathode_value = (prevavghv_cathode_value + (avghv_cathode_value / no_of_samples));
				prevavgbleeder_anode_value = (prevavgbleeder_anode_value + (avgbleeder_anode_value / no_of_samples));
				prevavgbleeder_cathode_value = (prevavgbleeder_cathode_value + (avgbleeder_cathode_value / no_of_samples));
				avghv_signal_input = 0;
				avghv_anode_value = 0;
				avghv_cathode_value = 0;
				avgbleeder_anode_value = 0;
				avgbleeder_cathode_value = 0;
				no_of_samples = 0;
			}
			/*test_data_buffer.hv_signal_input = avghv_signal_input / no_of_samples;
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_signal_input,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,61,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			test_data_buffer.hv_anode_volt = avghv_anode_value / no_of_samples;
			color = Check_Range(test_data_buffer.hv_anode_volt,HVT_LO_CONST,HVT_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_anode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,87,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
			test_data_buffer.hv_cathode_volt = avghv_cathode_value / no_of_samples;
			color = Check_Range(test_data_buffer.hv_cathode_volt,HVT_LO_CONST,HVT_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_cathode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,113,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
			test_data_buffer.bleeder_anode_volt = avgbleeder_anode_value / no_of_samples;
			color = Check_Range(test_data_buffer.bleeder_anode_volt,BLEEDER_LO_CONST,BLEEDER_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.bleeder_anode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,169,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
			test_data_buffer.bleeder_cathode_volt = avgbleeder_cathode_value / no_of_samples;
			color = Check_Range(test_data_buffer.bleeder_cathode_volt,BLEEDER_LO_CONST,BLEEDER_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.bleeder_cathode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,195,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
			if (data_read_flag == false)
			{
				Display_HVTKey_Menu(1);
				data_read_flag = true;
			}
			avghv_signal_input = 0;
			avghv_anode_value = 0;
			avghv_cathode_value = 0;
			avgbleeder_anode_value = 0;
			avgbleeder_cathode_value = 0;
			no_of_samples = 0;*/
		}
		else
		{
			avghv_signal_input = avghv_signal_input + hv_input_value;
			avghv_anode_value = avghv_anode_value + hv_anode_value;
			avghv_cathode_value = avghv_cathode_value + hv_cathode_value;
			avgbleeder_anode_value = avgbleeder_anode_value + bleeder_anode_value;
			avgbleeder_cathode_value = avgbleeder_cathode_value + bleeder_cathode_value;
			no_of_samples++;
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
				//if (data_read_flag == true)
				//{
					////result = Store_HV_Data_Values();
					//result = true;
					//break;
				//}
			}
		}
		else if (key_timeout_flag == false)
		{
			data_save_flag = false;
			break;
		}
		//Read_HV_Ratio_Inputs();
	}
	Write2_DAC_Register(0);
	trdstr = 0x0C;
	if (result == false)
	{
		return;
	}
	HV_CHECK_PIN = HIGH;
	//FILAMENT_CHECK_PIN = LOW;
	Draw_Main_Window();
	Write_Center_Text(0,TFT_SIZE_X,5,&model_name[0],(unsigned far char *)&arial_narrow_bold16[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
	Write_Center_Text_Bar(20,TFT_SIZE_X-20,32,&filament_turns_text[0],(unsigned far char *)&arial_narrow_bold16[0],BRIGHTBLUE,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text(5,60,&filament_input_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,60,&vac_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,86,&large_filament_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,86,&vac_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,112,&small_filament_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(267,112,&vac_text[0],(unsigned far char *)&arial_narrow_bold16[0],BLACK,WHITE,TFT_MODE_FULL);
	Display_HVTKey_Menu(0);
	FREQUENCY_CHANGE_PIN = HIGH;
	trdgra1 = KHZ30_PERIOD_CONST;
	trdgrb1 = KHZ30_VALUE_CONST;
	trdstr = 0x0F;
	half_sec_ctr = HALF_SEC_CONST;
	half_sec_flag = false;
	avgfilament_input_value = 0;
	avgshort_filament_value = 0;
	avglong_filament_value = 0;
	prevavgfilament_input_value = 0;
	prevavgshort_filament_value = 0;
	prevavglong_filament_value = 0;
	prevfilament_input_value = 0;
	prevshort_filament_value = 0;
	prevlong_filament_value = 0;
	no_of_samples = 0;
	temp_dac_value = (unsigned int)INITIAL_FIL_DAC_VALUE;
	Write2_DAC_Register(temp_dac_value);
	key_timeout_ctr = KEY_TIMEOUT_CONST1;
	key_timeout_flag = true;
	key_press_flag = false;
	data_read_flag = false;
	filament_check_flag = false;
	no_of_averages = 1;
	prev_average_no = 1;
	cur_average_no = 1;
	while (true)
	{
		filament_input_value = ReadN_Average_ADC(0x81,1,FIL_SIGNAL_INPUT_CONST);
		long_filament_value = ReadN_Average_ADC(0x86,1,FIL_ANODECATHODE_CONST);
		short_filament_value = ReadN_Average_ADC(0x87,1,FIL_ANODECATHODE_CONST);
		if (filament_input_value > (unsigned long)REQUIRED_FILAMENT_HICONST)//REQUIRED_FILAMENT_CONST)
		{
			if (temp_dac_value > (unsigned int)0)
			{
				temp_dac_value--;
			}
			Write2_DAC_Register(temp_dac_value);
		}
		else if (filament_input_value < (unsigned long)REQUIRED_FILAMENT_LOCONST)//REQUIRED_FILAMENT_CONST)
		{
			if (temp_dac_value < (unsigned int)MAX_DAC_CONST)
			{
				temp_dac_value++;
			}
			Write2_DAC_Register(temp_dac_value);
		}
		//filament_input_value = filament_input_value / (unsigned int)10;
		if (half_sec_flag == true)
		{
			half_sec_flag = false;
			no_of_averages--;
			if (no_of_averages == 0)
			{
				prevavgfilament_input_value = (prevavgfilament_input_value + (avgfilament_input_value / no_of_samples));
				prevavglong_filament_value = (prevavglong_filament_value + (avglong_filament_value / no_of_samples));
				prevavgshort_filament_value = (prevavgshort_filament_value + (avgshort_filament_value / no_of_samples));
				avgfilament_input_value = 0;
				avgshort_filament_value = 0;
				avglong_filament_value = 0;
				no_of_samples = 0;
				if (cur_average_no == 10)
				{
					no_of_averages = 10;
					if (filament_long_flag == 1)
					{
						filament_long_flag = 2;
					}
				}
				/*else if (cur_average_no == 1)
				{
					prev_average_no = 2;
					no_of_averages = prev_average_no;
					if (data_read_flag == false)
					{
						Display_HVTKey_Menu(2);
						data_read_flag = true;
					}
					filament_check_flag = true;
				}*/
				else
				{
					prev_average_no = prev_average_no + 1;
					no_of_averages = prev_average_no;
					if (no_of_averages == 4)
					{
						if (data_read_flag == false)
						{
							Display_HVTKey_Menu(2);
							data_read_flag = true;
						}
						if (filament_short_flag == false)
						{
							FILAMENT_CHECK_PIN = LOW;
							filament_short_flag = true;
							filament_check_flag = true;
							filament_long_flag = 1;
						}
					}
					if (no_of_averages > 5)
					{
						if (filament_long_flag == 1)
						{
							filament_long_flag = 2;
						}
					}
				}
				prevavgfilament_input_value = prevavgfilament_input_value / (unsigned int)10;
				test_data_buffer.filament_signal_input = prevavgfilament_input_value / cur_average_no;
				nob = Conv_Data2_AsciiDecimal(test_data_buffer.filament_signal_input,&temp_disp_array[0]);
				Write_Right_Text_Bar(265,61,195,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
				test_data_buffer.long_filament_volt = prevavglong_filament_value / cur_average_no;
				if (test_data_buffer.long_filament_volt < FIL_EXIST_CONST)
				{
					test_data_buffer.long_filament_volt = 0;
				}
				color = Check_Range(test_data_buffer.long_filament_volt,FILAMENT_LO_CONST,FILAMENT_HI_CONST);
				nob = Conv_Data2_AsciiDecimal(test_data_buffer.long_filament_volt,&temp_disp_array[0]);
				Write_Right_Text_Bar(265,87,195,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
				test_data_buffer.short_filament_volt = prevavgshort_filament_value / cur_average_no;
				if (test_data_buffer.short_filament_volt < FIL_EXIST_CONST)
				{
					test_data_buffer.short_filament_volt = 0;
				}
				color = Check_Range(test_data_buffer.short_filament_volt,FILAMENT_LO_CONST,FILAMENT_HI_CONST);
				nob = Conv_Data2_AsciiDecimal(test_data_buffer.short_filament_volt,&temp_disp_array[0]);
				Write_Right_Text_Bar(265,113,195,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
				prevavgfilament_input_value = 0;
				prevavglong_filament_value = 0;
				prevavgshort_filament_value = 0;
				cur_average_no = no_of_averages;
			}
			else
			{
				prevavgfilament_input_value = (prevavgfilament_input_value + (avgfilament_input_value / no_of_samples));
				prevavglong_filament_value = (prevavglong_filament_value + (avglong_filament_value / no_of_samples));
				prevavgshort_filament_value = (prevavgshort_filament_value + (avgshort_filament_value / no_of_samples));
				avgfilament_input_value = 0;
				avgshort_filament_value = 0;
				avglong_filament_value = 0;
				no_of_samples = 0;
			}
			/*filament_check_flag = true;
			test_data_buffer.filament_signal_input = avgfilament_input_value / no_of_samples;
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.filament_signal_input,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,61,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],BLACK,WHITE,TFT_MODE_FULL);
			test_data_buffer.long_filament_volt = avglong_filament_value / no_of_samples;
			color = Check_Range(test_data_buffer.long_filament_volt,FILAMENT_LO_CONST,FILAMENT_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.long_filament_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,87,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
			test_data_buffer.short_filament_volt = avgshort_filament_value / no_of_samples;
			color = Check_Range(test_data_buffer.short_filament_volt,FILAMENT_LO_CONST,FILAMENT_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.short_filament_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(265,113,190,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold20SI[0],color,WHITE,TFT_MODE_FULL);
			if (data_read_flag == false)
			{
				Display_HVTKey_Menu(2);
				data_read_flag = true;
			}
			avgfilament_input_value = 0;
			avgshort_filament_value = 0;
			avglong_filament_value = 0;
			no_of_samples = 0;*/
		}
		else
		{
			avgfilament_input_value = avgfilament_input_value + filament_input_value;
			avgshort_filament_value = avgshort_filament_value + short_filament_value;
			avglong_filament_value = avglong_filament_value + long_filament_value;
			no_of_samples++;
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
					Store_Data2_Eeprom();
					break;
				}
			}
		}
		else if (key_timeout_flag == false)
		{
			data_save_flag = false;
			break;
		}
		//if ((filament_check_flag == true) && (long_filament_value < FIL_EXIST_CONST))
		if ((filament_check_flag == true) && (test_data_buffer.short_filament_volt < FIL_EXIST_CONST))
		{
			Display_Filament_Interchange();
			break;
		}
		else if ((filament_long_flag == 2) && (test_data_buffer.long_filament_volt < FIL_EXIST_CONST))
		{
			Display_Filament_Interchange();
			break;
		}
	}
	trdstr = 0x0C;
	FREQUENCY_CHANGE_PIN = LOW;
	FILAMENT_CHECK_PIN = HIGH;
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

void Store_Data2_Eeprom()
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
	Display_Updated_Message(test_data_buffer.test_number);
}
