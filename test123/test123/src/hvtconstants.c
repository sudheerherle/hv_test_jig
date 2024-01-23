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

const unsigned char company_name[] = {"GE BE LTD"};
const unsigned char firmware_version[] = {"FW Ver 02.00 R 01"};
const unsigned char model_name[] = {"HV TANK TURNS RATIO TEST JIG"};
const unsigned char menu_name_text[] = {"MAIN MENU"};
//const unsigned char serialno_mesg[] = {"Serial No.:"};
//const unsigned char modelname_mesg[] = {"Model Name:"};
//const unsigned char version_mesg[] = {"Firmware Version:"};

//const unsigned char normal_text[] 	=	{"SELECT TEST TYPE"};
const unsigned char hv_turns_text[] 	=	{"HV TURNS RATIO TEST"};
//const unsigned char hv_input_text[] 	=	{"HV IP 20KHz PULSEWAVE"};
const unsigned char hv_input_text[] 	=	{"HV I/P 20KHz (Vac)"};
const unsigned char anode_hv_text[] 	=	{"ANODE HV O/P"};
const unsigned char cathode_hv_text[] 	=	{"CATHODE HV O/P"};
const unsigned char bleeder_turns_text[] 	=	{"BLEEDER TURNS RATIO TEST"};
const unsigned char bleeder_anode_text[] 	=	{"ANODE BLDR O/P"};
const unsigned char bleeder_cathode_text[] 	=	{"CATHODE BLDR O/P"};
const unsigned char filament_turns_text[] 	=	{"FILAMENT TURNS RATIO TEST"};
//const unsigned char filament_input_text[] 	=	{"FIL IP 30KHz PULSEWAVE"};
const unsigned char filament_input_text[] 	=	{"FIL I/P 30KHz"};
const unsigned char large_filament_text[] 	=	{"LARGE FILAMENT O/P"};
const unsigned char small_filament_text[] 	=	{"SMALL FILAMENT O/P"};

const unsigned char filament_changed_text[]	=	{"FILAMENT INTERCHANGED"};

const unsigned char test_passed_text[] =	{"TEST PASSED"};
const unsigned char test_failed_text[] =	{"TEST FAILED"};

const unsigned char main_menu_text[] 	=	{"START NEW TEST"};
const unsigned char main_menu_text1[] 	=	{"TEST REPORTS  "};

const unsigned char vpk_text[] 	=	{"Vpk"};//{"Vpkac"};
const unsigned char vac_text[] 	=	{"Vac"};
const unsigned char vdc_text[] 	=	{"Vdc"};
const unsigned char mvdc_text[] =	{"mVdc"};
const unsigned char pass_text[] 	=	{"PASS"};
const unsigned char fail_text[] 	=	{"FAIL"};
const unsigned char plus_symbol[] 	=	{"+"};
const unsigned char minus_symbol[] 	=	{"-"};

const unsigned char press_key_text[] 	=	{"Press any key to continue..."};
const unsigned char filnot_connected_text[] 	=	{"Filament not properly connected"};

//const unsigned char hv_test_text[] 	=	{"HV TANK TEST "};
//const unsigned char filament_test_text1[] 	=	{"FILAMENT TEST"};




