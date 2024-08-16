#define 	true  						1
#define 	false  						0

#define 	FAST_FONT_INDEX
#define		MAX_SERIAL_CONST			1000 //200ms

#define		HVT_SL_NO					0x23000

#define 	FIRST_KEY_PIN 				p8_0
#define 	SECOND_KEY_PIN 				p8_1
#define 	THIRD_KEY_PIN 				p8_2

#define 	FILAMENT_CHECK_PIN			p2_0
#define		FREQUENCY_CHANGE_PIN		p2_3
#define		HV_CHECK_PIN				p2_6

//i2c routine declarations
#define		IIC_ROUTINE_PINS
#define		SDA							p2_2
#define		SCL							p2_1
#define		SDA_Direction				pd2_2 
#define		SCL_Direction				pd2_1
#define		WRITE_ENABLE				prc2
#define		HIGH						0x01
#define		LOW							0x00
//end of i2c routine declaration

#define		KEY_BOUNCE_CONST			60
#define		KEY_TIMEOUT_CONST			120		//1minute
#define		KEY_TIMEOUT_CONST1			1200	//10minute
#define		FIRST_CONTPRESS_CONST		30		//20
#define		SECOND_CONTPRESS_CONST		20		//2

#define		KHZ20_PERIOD_CONST			(unsigned int)1999
#define		KHZ20_VALUE_CONST			(unsigned int)1000
#define		KHZ30_PERIOD_CONST			(unsigned int)1331
#define		KHZ30_VALUE_CONST			(unsigned int)666

#define		HV_ANODE_MINCONST			(unsigned long)300
#define		HV_CATHODE_MINCONST			(unsigned long)300
#define		BLEEDER_ANODE_MINCONST		(unsigned long)1000
#define		BLEEDER_CATHODE_MINCONST	(unsigned long)1000

#define		MAX_ADC_REF_CONST			(unsigned long)1000
#define		HV_SIGNAL_INPUT_CONST		(unsigned long)1000
#define		HV_ANODECATHODE_CONST		(unsigned long)50000
#define		HV_BLEEDER_AC_CONST			(unsigned long)5000
#define		FIL_SIGNAL_INPUT_CONST		(unsigned long)6000
#define		FIL_ANODECATHODE_CONST		(unsigned long)500
#define		FIL_EXIST_CONST				(unsigned long)25

#define		REQUIRED_HV_CONST			(unsigned long)85
#define		REQUIRED_HV_HICONST			(unsigned long)859
#define		REQUIRED_HV_LOCONST			(unsigned long)850
#define		REQUIRED_FILAMENT_CONST		(unsigned long)510
#define		REQUIRED_FILAMENT_HICONST	(unsigned long)5119
#define		REQUIRED_FILAMENT_LOCONST	(unsigned long)5105
#define		INITIAL_HV_DAC_VALUE		(unsigned int)1000//580
#define		INITIAL_FIL_DAC_VALUE		(unsigned int)500//3480
#define		MAX_DAC_CONST				(unsigned int)4095

#define		HVT_LO_CONST				(unsigned long)34049//39049
#define		HVT_HI_CONST				(unsigned long)45051
#define		BLEEDER_LO_CONST			(unsigned long)3449//3949
#define		BLEEDER_HI_CONST			(unsigned long)4551
#define		FILAMENT_LO_CONST			(unsigned long)389
#define		FILAMENT_HI_CONST			(unsigned long)421

/*#define		SECONDS1					400
#define		SECONDS2					800
#define		SECONDS5					2000
#define		SECONDS10					4000
#define		SECONDS15					6000
#define		SECONDS50					20000*/

#define		E2_ADD_CONST				0xa0
#define		E2_SIGNATURE_CONST			80	//'N'+VERNO
#define 	HALF_SEC_CONST				250//100	//200

#define		DEST_PC_ADDRESS				1
#define		SOURCE_UNIT_ADDRESS			2
#define		SERIALDATA_RETRY_CONST		5
#define		MAXPC_TIMEOUT_CONST			500
#define		DATA_DOWNLOAD_HEADER		99
#define		FULLDATA_DOWNLOAD_HEADER	98

#define		E2SIG_BYTES_CONST			1
#define		EXTRAE2SIG_BYTES_CONST		1

#define		CONFIG_DATA_BYTES			20
#define		POINTER_INFO_BYTES			8
#define		TRAN_DATA_BYTES				34
#define		TOTAL_TRANS_CONST			200

#define		FREE_DATA_CONST				5

#define		E2_SIGADD_CONST				0X0001
#define		EXTRA_E2SIGADD_CONST		0x0003
#define		E2_CONFIGADD_CONST			0x0004
#define		POINTERINFO_ADD_CONST		0X0010
#define		EVENT_DATA_START			0X0020

//ending at 
//   bytes free

#define		E2EVENTSTART_ADD_CONST		EVENT_DATA_START
#define		E2EVENTFIRST_ADD_CONST		E2EVENTSTART_ADD_CONST - TRAN_DATA_BYTES
#define		E2EVENTEND_ADD_CONST		(E2EVENTSTART_ADD_CONST + (TRAN_DATA_BYTES * TOTAL_TRANS_CONST))
#define		E2LASTEVENT_ADD_CONST		E2EVENTEND_ADD_CONST - TRAN_DATA_BYTES

//5 bytes free

enum eeprom_fault_code
{
	NO_EEPROM_ERROR,
	BOARD_EEPROM_ERROR,
	CHANGE_EEPROM
};

struct test_data		//34 bytes
{
	unsigned int test_number;
	unsigned long hv_signal_input;
	unsigned long hv_anode_volt;
	unsigned long hv_cathode_volt;
	unsigned long bleeder_anode_volt;
	unsigned long bleeder_cathode_volt;
	unsigned long filament_signal_input;
	unsigned long short_filament_volt;
	unsigned long long_filament_volt;
};

struct pointer_info			//8 bytes
{
	unsigned int total_bytes;
	unsigned int cur_data_pointer;
	unsigned int last_download_pointer;
	unsigned int test_no;
};

struct cctalk_data			//45 bytes
{
	unsigned char dest_addr;
	unsigned char nob;
	unsigned char header;
	unsigned int crc;
	unsigned long unit_address;
	unsigned int data_address;
	struct test_data pc_test_data;
};
