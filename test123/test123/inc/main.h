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
#define		KEY_TIMEOUT_CONST1			1800	//15minute	//1200	//10minute
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

#define		MAX_ADC_REF_CONST			(unsigned long)1023//1000
#define		HV_SIGNAL_INPUT_CONST		(unsigned long)1000
#define		HV_ANODECATHODE_CONST		(unsigned long)50000
#define		HV_BLEEDER_AC_CONST			(unsigned long)5000
#define		FIL_SIGNAL_INPUT_CONST		(unsigned long)6000
#define		FIL_ANODECATHODE_CONST		(unsigned long)500
#define		FIL_EXIST_CONST				(unsigned long)25

#define		REQUIRED_HV_CONST			(unsigned long)854
#define		REQUIRED_HV_HICONST			(unsigned long)858
#define		REQUIRED_HV_LOCONST			(unsigned long)851
#define		REQUIRED_FILAMENT_CONST		(unsigned long)5104
#define		REQUIRED_FILAMENT_HICONST	(unsigned long)5108
#define		REQUIRED_FILAMENT_LOCONST	(unsigned long)5101
#define		INITIAL_HV_DAC_VALUE		(unsigned int)1613//1600
#define		MINIMUM_HV_DAC_VALUE		(unsigned int)1400
#define		MAXIMUM_HV_DAC_VALUE		(unsigned int)1800//1650
#define		INITIAL_FIL_DAC_VALUE		(unsigned int)1446//1516
#define		MINIMUM_FIL_DAC_VALUE		(unsigned int)1300
#define		MAXIMUM_FIL_DAC_VALUE		(unsigned int)1700
#define		MINIMUM_DAC_VALUE			(unsigned int)0
#define		MAXIMUM_DAC_VALUE			(unsigned int)4095

#define		DACSAVE_TIMER_CONST			(unsigned long)100
#define		MAXDAC_STARTTIME_CONST		(unsigned char)6
#define		MAXDAC_TIME_CONST			(unsigned char)3

#define		DIFFHV_2TIMES_CONST			(unsigned long)30
#define		DIFFHV_5TIMES_CONST			(unsigned long)50
#define		DIFFHV_10TIMES_CONST		(unsigned long)70
#define		DIFFHV_20TIMES_CONST		(unsigned long)90
#define		DIFFFIL_2TIMES_CONST		(unsigned long)200
#define		DIFFFIL_5TIMES_CONST		(unsigned long)400
#define		DIFFFIL_10TIMES_CONST		(unsigned long)600
#define		DIFFFIL_20TIMES_CONST		(unsigned long)800

#define		HISIDEHV_2TIMES_CONST		(unsigned long)870
#define		HISIDEHV_5TIMES_CONST		(unsigned long)890
#define		HISIDEHV_10TIMES_CONST		(unsigned long)910
#define		HISIDEHV_20TIMES_CONST		(unsigned long)950
#define		LOSIDEHV_2TIMES_CONST		(unsigned long)840
#define		LOSIDEHV_5TIMES_CONST		(unsigned long)820
#define		LOSIDEHV_10TIMES_CONST		(unsigned long)800
#define		LOSIDEHV_20TIMES_CONST		(unsigned long)750
#define		HISIDEFIL_2TIMES_CONST		(unsigned long)5130
#define		HISIDEFIL_5TIMES_CONST		(unsigned long)5150
#define		HISIDEFIL_10TIMES_CONST		(unsigned long)5200
#define		HISIDEFIL_20TIMES_CONST		(unsigned long)5300
#define		LOSIDEFIL_2TIMES_CONST		(unsigned long)5070
#define		LOSIDEFIL_5TIMES_CONST		(unsigned long)5050
#define		LOSIDEFIL_10TIMES_CONST		(unsigned long)5000
#define		LOSIDEFIL_20TIMES_CONST		(unsigned long)4900

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
#define		E2_SIGNATURE_CONST			82	//'N'+VERNO
#define 	HALF_SEC_CONST				250//100	//200
#define 	ADC_TIMEOUT_CONST			50
#define 	DOUBLEADC_TIMEOUT_CONST		100
#define		DAC_SIGNATURE_CONST			0xaa55

#define		DEST_PC_ADDRESS				1
#define		SOURCE_UNIT_ADDRESS			2
#define		SERIALDATA_RETRY_CONST		5
#define		MAXPC_TIMEOUT_CONST			500
#define		START_TEST_HEADER			'S'
#define		STOP_TEST_HEADER			'X'
#define		CONTINUE_TEST_HEADER		'C'
#define		SAVE_TEST_HEADER			'V'
#define		READ_TEST_HEADER			'R'
#define		DATA_DOWNLOAD_HEADER		'D'//99
#define		NAK_TEST_HEADER				'E'

#define		E2SIG_BYTES_CONST			1
#define		EXTRAE2SIG_BYTES_CONST		1

#define		HVDAC_DATA_BYTES			4
#define		FILDAC_DATA_BYTES			4
#define		DAC_DATA_BYTES				sizeof(dac_info_buffer)
#define		POINTER_INFO_BYTES			sizeof(pointer_info_buffer)//8
#define		TRAN_DATA_BYTES				sizeof(test_data_buffer)//34
#define		TOTAL_TRANS_CONST			200

#define		FREE_DATA_CONST				5

#define		E2_SIGADD_CONST				0x0001
#define		EXTRA_E2SIGADD_CONST		0x0003
#define		E2_DACADD_CONST				0x0005
#define		E2_HVDACADD_CONST			0x0005
#define		E2_FILDACADD_CONST			0x0007
#define		POINTERINFO_ADD_CONST		0x0010
#define		EVENT_DATA_START			0x0020

//ending at 
//   bytes free

#define		E2EVENTSTART_ADD_CONST		EVENT_DATA_START
#define		E2EVENTFIRST_ADD_CONST		E2EVENTSTART_ADD_CONST - TRAN_DATA_BYTES
#define		E2EVENTEND_ADD_CONST		(E2EVENTSTART_ADD_CONST + (TRAN_DATA_BYTES * TOTAL_TRANS_CONST))
#define		E2LASTEVENT_ADD_CONST		E2EVENTEND_ADD_CONST - TRAN_DATA_BYTES

//5 bytes free

#define		ADC_ARRAY_CONST				5//10

#define		IDLE_TEST_STATE				0
#define		START_TEST_STATE			1
#define		STOP_TEST_STATE				2
#define		HV_TEST_STATE				3
#define		FIL_TEST_STATE				4
#define		READ_TEST_STATE				5
#define		SAVE_TEST_STATE				6
#define		DATA_TEST_STATE				7
#define		PRINT_TEST_STATE			8

#define		SERIAL_START_CONST			0x02
#define		SERIAL_END_CONST			0x03
#define		SERIAL_ACK_CONST			0x06
#define		SERIAL_NACK_CONST			0x15
#define		START_TEST_CONST			0x11
#define		STOP_TEST_CONST				0x12
#define		NEXT_TEST_CONST				0x13
#define		REQUEST_DATA_CONST			0x14

#define		START_BYTES_CONST			3
#define		STOP_BYTES_CONST			3
#define		NEXT_BYTES_CONST			3
#define		REQUEST_BYTES_CONST			3
#define		ACK_BYTES_CONST				3

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

struct cctalk_data			//41 bytes
{
	unsigned char dest_addr;
	unsigned char nob;
	unsigned char header;
	unsigned int crc;
	//unsigned long unit_address;
	unsigned int data_address;
	struct test_data pc_test_data;
};

struct hv_test_data		//20 bytes
{
	unsigned long hv_signal_input;
	unsigned long hv_anode_volt;
	unsigned long hv_cathode_volt;
	unsigned long bleeder_anode_volt;
	unsigned long bleeder_cathode_volt;
};

struct hv_cctalk_data			//29 bytes
{
	unsigned char dest_addr;
	unsigned char nob;
	unsigned char header;
	unsigned int crc;
	//unsigned long unit_address;
	struct hv_test_data pchv_test_data;
};

struct fil_test_data		//12 bytes
{
	unsigned long filament_signal_input;
	unsigned long short_filament_volt;
	unsigned long long_filament_volt;
};

struct fil_cctalk_data			//21 bytes
{
	unsigned char dest_addr;
	unsigned char nob;
	unsigned char header;
	unsigned int crc;
	//unsigned long unit_address;
	struct fil_test_data pcfil_test_data;
};

struct general_cctalk_data			//41 bytes
{
	unsigned char dest_addr;
	unsigned char nob;
	unsigned char header;
	unsigned int crc;
	//unsigned long unit_address;
	unsigned char general_data[38];
};

struct dac_info			//8 bytes
{
	unsigned int hv_signature;
	signed int hv_dac_value;
	unsigned int fil_signature;
	signed int filament_dac_value;
};

