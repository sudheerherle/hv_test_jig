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

// helvB18 = "Helvetica Bold"
const unsigned char font_helvB18[] = {
0x20,0x5E,   // characters 32 to 126
#ifdef FAST_FONT_INDEX
0xC0,0x00,
0xC4,0x00,
0xCF,0x00,
0xD6,0x00,
0xFC,0x00,
0x22,0x01,
0x63,0x01,
0x95,0x01,
0x99,0x01,
0xAD,0x01,
0xC1,0x01,
0xCB,0x01,
0xE5,0x01,
0xEB,0x01,
0xF5,0x01,
0xFB,0x01,
0x12,0x02,
0x38,0x02,
0x50,0x02,
0x76,0x02,
0x9C,0x02,
0xC2,0x02,
0xE8,0x02,
0x0E,0x03,
0x34,0x03,
0x5A,0x03,
0x80,0x03,
0x88,0x03,
0x93,0x03,
0xAF,0x03,
0xBC,0x03,
0xD8,0x03,
0xFB,0x03,
0x3F,0x04,
0x71,0x04,
0xA0,0x04,
0xD2,0x04,
0x04,0x05,
0x2D,0x05,
0x53,0x05,
0x88,0x05,
0xB7,0x05,
0xC2,0x05,
0xE5,0x05,
0x11,0x06,
0x37,0x06,
0x72,0x06,
0xA1,0x06,
0xD6,0x06,
0x02,0x07,
0x37,0x07,
0x63,0x07,
0x8F,0x07,
0xBE,0x07,
0xED,0x07,
0x1F,0x08,
0x60,0x08,
0x92,0x08,
0xC1,0x08,
0xED,0x08,
0xFE,0x08,
0x15,0x09,
0x26,0x09,
0x3E,0x09,
0x4F,0x09,
0x57,0x09,
0x71,0x09,
0x97,0x09,
0xAF,0x09,
0xD5,0x09,
0xEF,0x09,
0x06,0x0A,
0x2C,0x0A,
0x4F,0x0A,
0x5A,0x0A,
0x6B,0x0A,
0x8E,0x0A,
0x99,0x0A,
0xBD,0x0A,
0xD5,0x0A,
0xEF,0x0A,
0x15,0x0B,
0x3B,0x0B,
0x4B,0x0B,
0x63,0x0B,
0x7A,0x0B,
0x92,0x0B,
0xAC,0x0B,
0xD4,0x0B,
0xEC,0x0B,
0x15,0x0C,
0x2D,0x0C,
0x44,0x0C,
0x4C,0x0C,
0x63,0x0C,
#endif // FAST_FONT_INDEX

// space (32): w=1, h=1
0x80,0x12,0x4A,
0x00,

// exclam (33): w=3, h=19
0x42,0x16,
0xFF,0xFF,0xFF,
0x3F,0x3F,0x07,
0x07,0x07,0x07,

// quotedbl (34): w=5, h=6
0x04,0x16,
0x3F,0x1F,0x00,0x3F,0x1F,

// numbersign (35): w=12, h=18
0x4B,0x2F,
0x00,0x60,0x60,0xE0,0xFF,0x7F,0x60,0xE0,0xFF,0x7F,
0x60,0x60,
0x18,0x18,0xF8,0xFF,0x1F,0x18,0xF8,0xFF,0x1F,0x18,
0x18,0x00,
0x00,0x00,0x03,0x03,0x00,0x00,0x03,0x03,0x00,0x00,
0x00,0x00,

// dollar (36): w=12, h=21
0x4B,0x09,
0x78,0xFC,0xFE,0xCE,0x86,0xFF,0xFF,0x06,0x0E,0x3C,
0x3C,0x30,
0xE0,0xE0,0xE1,0x01,0x03,0xFF,0xFF,0x0E,0x1C,0xFC,
0xF8,0xF0,
0x01,0x03,0x03,0x07,0x06,0x1F,0x1F,0x06,0x07,0x07,
0x03,0x01,

// percent (37): w=21, h=18
0x54,0x29,
0x78,0xFC,0xCE,0x86,0x86,0x86,0xCE,0xFC,0x78,0x00,
0xC0,0xF0,0xFC,0x3F,0x0F,0x03,0x00,0x00,0x00,0x00,
0x00,
0x00,0x00,0x01,0x01,0x01,0x01,0xC1,0xF0,0xFC,0x3F,
0x0F,0x03,0xF0,0xF8,0x9C,0x0C,0x0C,0x0C,0x9C,0xF8,
0xF0,
0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x00,0x00,
0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x01,
0x00,

// ampersand (38): w=16, h=18
0x4F,0x2F,
0x00,0x00,0x3C,0x7E,0xFF,0xC7,0x83,0xC7,0xFF,0x7E,
0x3C,0x00,0x00,0x00,0x00,0x00,
0xF0,0xFC,0xFE,0x8F,0x07,0x03,0x07,0x8F,0xDE,0xFC,
0xF8,0xFE,0xDE,0x8E,0x00,0x00,
0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
0x00,0x00,0x01,0x03,0x03,0x02,

// quotesingle (39): w=2, h=6
0x01,0x16,
0x3F,0x1F,

// parenleft (40): w=6, h=24
0x45,0x0F,
0x80,0xF0,0xFC,0x3E,0x0F,0x03,
0xFF,0xFF,0xFF,0x00,0x00,0x00,
0x01,0x0F,0x3F,0x7C,0xF0,0xC0,

// parenright (41): w=6, h=24
0x45,0x0F,
0x03,0x0F,0x3E,0xFC,0xF0,0x80,
0x00,0x00,0x00,0xFF,0xFF,0xFF,
0xC0,0xF0,0x7C,0x3F,0x0F,0x01,

// asterisk (42): w=8, h=7
0x07,0x0F,
0x0C,0x6C,0x78,0x1F,0x1F,0x78,0x6C,0x0C,

// plus (43): w=12, h=12
0x2B,0xD0,
0x60,0x60,0x60,0x60,0x60,0xFF,0xFF,0x60,0x60,0x60,
0x60,0x60,
0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,
0x00,0x00,

// comma (44): w=3, h=6
0x82,0x10,0x67,
0x27,0x3F,0x1F,

// hyphen (45): w=7, h=3
0x86,0x0A,0x46,
0x07,0x07,0x07,0x07,0x07,0x07,0x07,

// period (46): w=3, h=3
//0x82,0x10,0x67,
0xC2,0x10,0x67,
0x07,0x07,0x07,

// slash (47): w=7, h=19
0x46,0x0E,
0x00,0x00,0x00,0xC0,0xF8,0x3F,0x07,
0x00,0xE0,0xFC,0x1F,0x03,0x00,0x00,
0x07,0x07,0x00,0x00,0x00,0x00,0x00,

// zero (48): w=12, h=18
0x4B,0x29,
0xE0,0xFC,0xFE,0x1F,0x07,0x03,0x03,0x07,0x1F,0xFE,
0xFC,0xE0,
0x1F,0xFF,0xFF,0xE0,0x80,0x00,0x00,0x80,0xE0,0xFF,
0xFF,0x1F,
0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
0x00,0x00,

// one (49): w=7, h=18
0xC6,0x01,0x69,
0x18,0x18,0x18,0x1C,0xFF,0xFF,0xFF,
0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x03,0x03,0x03,

// two (50): w=12, h=18
0x4B,0x29,
0x38,0x3E,0x3E,0x07,0x03,0x03,0x03,0x07,0x8E,0xFE,
0xFC,0x70,
0xC0,0xE0,0xF0,0x78,0x38,0x1C,0x0E,0x0F,0x07,0x03,
0x01,0x00,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,

// three (51): w=12, h=18
0x4B,0x29,
0x38,0x3E,0x3E,0x07,0x03,0x03,0x03,0x87,0xFE,0xFE,
0x78,0x00,
0x60,0xE0,0xE0,0x80,0x03,0x03,0x03,0x83,0xC7,0xFE,
0xFE,0x78,
0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
0x01,0x00,

// four (52): w=12, h=18
0x4B,0x29,
0x00,0x00,0x00,0xC0,0xE0,0x78,0x1E,0xFF,0xFF,0xFF,
0x00,0x00,
0x38,0x3E,0x3F,0x33,0x30,0x30,0x30,0xFF,0xFF,0xFF,
0x30,0x30,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,
0x00,0x00,

// five (53): w=12, h=18
0x4B,0x29,
0x00,0xFF,0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0x83,
0x03,0x00,
0xE0,0xE1,0xE1,0x81,0x00,0x00,0x00,0x81,0xC3,0xFF,
0xFF,0x7C,
0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
0x00,0x00,

// six (54): w=12, h=18
0x4B,0x29,
0xF0,0xFC,0xFE,0x0E,0x87,0x83,0x83,0x83,0x07,0x1E,
0x1E,0x18,
0x3F,0xFF,0xFF,0xC7,0x83,0x01,0x01,0x83,0xC7,0xFF,
0xFE,0x38,
0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
0x00,0x00,

// seven (55): w=12, h=18
0x4B,0x29,
0x03,0x03,0x03,0x03,0x03,0x03,0xC3,0xF3,0xFF,0x3F,
0x0F,0x07,
0x00,0x00,0x80,0xF0,0xFC,0xFF,0x1F,0x03,0x00,0x00,
0x00,0x00,
0x00,0x00,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,

// eight (56): w=12, h=18
0x4B,0x29,
0x00,0x78,0xFE,0xFE,0x87,0x03,0x03,0x87,0xFE,0xFE,
0x78,0x00,
0x78,0xFC,0xFE,0x87,0x03,0x03,0x03,0x03,0x87,0xFE,
0xFC,0x78,
0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
0x01,0x00,

// nine (57): w=12, h=18
0x4B,0x29,
0xF8,0xFE,0xFE,0x0F,0x07,0x03,0x03,0x07,0x0F,0xFE,
0xFC,0xF0,
0xC1,0xC7,0xC7,0x8F,0x0E,0x0C,0x8C,0x8E,0xC7,0xFF,
0xFF,0x3F,
0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x01,0x01,
0x00,0x00,

// colon (58): w=3, h=14
0x22,0xB6,
0x07,0x07,0x07,
0x38,0x38,0x38,

// semicolon (59): w=3, h=17
0x42,0xB6,
0x07,0x07,0x07,
0x38,0xF8,0xF8,
0x01,0x01,0x00,

// less (60): w=13, h=12
0x2C,0xCA,
0x60,0x60,0xF0,0xF0,0x98,0x98,0x9C,0x0C,0x0E,0x06,
0x07,0x03,0x03,
0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x03,0x07,0x06,
0x0E,0x0C,0x0C,

// equal (61): w=10, h=5
0x89,0x09,0x67,
0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,

// greater (62): w=13, h=12
0x2C,0xCE,
0x03,0x03,0x07,0x06,0x0E,0x0C,0x9C,0x98,0x98,0xF0,
0xF0,0x60,0x60,
0x0C,0x0C,0x0E,0x06,0x07,0x03,0x03,0x01,0x01,0x00,
0x00,0x00,0x00,

// question (63): w=11, h=19
0x4A,0x16,
0x38,0x3E,0x3E,0x0F,0x07,0x03,0x83,0xE7,0xFF,0xFE,
0x3C,
0x00,0x00,0x00,0x00,0x3C,0x3F,0x3F,0x03,0x01,0x00,
0x00,
0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x00,0x00,0x00,
0x00,

// at (64): w=22, h=22
0x55,0x2F,
0x80,0xE0,0xF0,0xF8,0x3C,0x1E,0x0E,0x87,0xC7,0xE3,
0x63,0x63,0x63,0xE3,0xC3,0xE7,0x66,0x0E,0x1C,0xFC,
0xF8,0xE0,
0xFF,0xFF,0xFF,0x80,0x00,0x3C,0xFF,0xFF,0xC1,0x80,
0x80,0xF0,0xFE,0xFF,0x87,0xC1,0xC0,0xF0,0x7C,0x3F,
0x0F,0x03,
0x00,0x03,0x07,0x0F,0x1E,0x1C,0x38,0x39,0x31,0x31,
0x31,0x31,0x30,0x39,0x19,0x19,0x00,0x00,0x00,0x00,
0x00,0x00,

// A (65): w=16, h=19
0x4F,0x0F,
0x00,0x00,0x00,0x80,0xF0,0xFC,0x7F,0x0F,0x0F,0x7F,
0xFC,0xE0,0x80,0x00,0x00,0x00,
0x00,0xE0,0xFC,0xFF,0x3F,0x33,0x30,0x30,0x30,0x30,
0x33,0x3F,0xFF,0xFC,0xE0,0x00,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0x07,0x07,

// B (66): w=15, h=19
0x4E,0x15,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x07,
0x87,0xFE,0xFE,0x78,0x00,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x87,0xFE,0xFC,0xF8,
0x07,0x07,0x07,0x06,0x06,0x06,0x06,0x06,0x06,0x07,
0x07,0x03,0x03,0x01,0x00,

// C (67): w=16, h=19
0x4F,0x0F,
0xE0,0xF8,0xFC,0x3E,0x0E,0x07,0x07,0x03,0x03,0x03,
0x07,0x07,0x0E,0x1E,0x1C,0x18,
0x3F,0xFF,0xFF,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0xE0,0xE0,0xE0,
0x00,0x00,0x01,0x03,0x03,0x07,0x07,0x06,0x06,0x06,
0x07,0x07,0x03,0x03,0x01,0x00,

// D (68): w=16, h=19
0x4F,0x15,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x07,
0x07,0x0E,0x3E,0xFC,0xF8,0xE0,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x80,0xE0,0xFF,0xFF,0x3F,
0x07,0x07,0x07,0x06,0x06,0x06,0x06,0x06,0x06,0x07,
0x07,0x03,0x03,0x01,0x00,0x00,

// E (69): w=13, h=19
0x4C,0x15,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x00,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x00,0x00,
0x07,0x07,0x07,0x06,0x06,0x06,0x06,0x06,0x06,0x06,
0x06,0x06,0x06,

// F (70): w=12, h=19
0x4B,0x15,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x00,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,

// G (71): w=17, h=19
0x50,0x0F,
0xE0,0xF8,0xFC,0x3E,0x0E,0x07,0x07,0x03,0x03,0x03,
0x07,0x07,0x0E,0x1E,0x1C,0x18,0x00,
0x3F,0xFF,0xFF,0xE0,0x80,0x00,0x00,0x00,0x00,0x06,
0x06,0x06,0x86,0xC6,0xFE,0xFE,0xFE,
0x00,0x00,0x01,0x03,0x03,0x07,0x07,0x06,0x06,0x06,
0x07,0x07,0x03,0x01,0x07,0x07,0x07,

// H (72): w=15, h=19
0x4E,0x16,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0xFF,0xFF,0xFF,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0x07,0x07,

// I (73): w=3, h=19
0x42,0x16,
0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,
0x07,0x07,0x07,

// J (74): w=11, h=19
0x4A,0x10,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0xFF,
0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0xFF,
0x00,0x03,0x07,0x07,0x06,0x06,0x06,0x07,0x07,0x03,
0x01,

// K (75): w=14, h=19
0x4D,0x16,
0xFF,0xFF,0xFF,0x80,0xC0,0xE0,0xF0,0x78,0x3C,0x1E,
0x0F,0x07,0x03,0x01,
0xFF,0xFF,0xFF,0x07,0x03,0x07,0x0F,0x3E,0x7C,0xF0,
0xE0,0x80,0x00,0x00,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x07,0x07,0x07,0x04,

// L (76): w=12, h=19
0x4B,0x15,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,
0x07,0x07,0x07,0x06,0x06,0x06,0x06,0x06,0x06,0x06,
0x06,0x06,

// M (77): w=19, h=19
0x52,0x16,
0xFF,0xFF,0xFF,0x3E,0xF8,0xE0,0x80,0x00,0x00,0x00,
0x00,0x00,0x80,0xE0,0xF8,0x3E,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x00,0x00,0x07,0x3F,0xFE,0xF0,0xC0,
0xF0,0xFE,0x3F,0x07,0x00,0x00,0xFF,0xFF,0xFF,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x07,0x07,
0x07,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x07,

// N (78): w=15, h=19
0x4E,0x16,
0xFF,0xFF,0xFF,0x3E,0xF8,0xE0,0xC0,0x00,0x00,0x00,
0x00,0x00,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x00,0x00,0x01,0x07,0x0F,0x3E,0x78,
0xE0,0xC0,0xFF,0xFF,0xFF,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0x03,0x07,0x07,0x07,

// O (79): w=17, h=19
0x50,0x0F,
0xE0,0xF8,0xFC,0x3E,0x0E,0x07,0x07,0x03,0x03,0x03,
0x07,0x07,0x0E,0x3E,0xFC,0xF8,0xE0,
0x3F,0xFF,0xFF,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0xE0,0xFF,0xFF,0x3F,
0x00,0x00,0x01,0x03,0x03,0x07,0x07,0x06,0x06,0x06,
0x07,0x07,0x03,0x03,0x01,0x00,0x00,

// P (80): w=14, h=19
0x4D,0x15,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x87,0xFE,0xFE,0x78,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x01,0x00,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,

// Q (81): w=17, h=20
0x50,0x0F,
0xE0,0xF8,0xFC,0x3E,0x0E,0x07,0x07,0x03,0x03,0x03,
0x07,0x07,0x0E,0x3E,0xFC,0xF8,0xE0,
0x3F,0xFF,0xFF,0xE0,0x80,0x00,0x00,0x00,0x00,0x40,
0xE0,0xC0,0x80,0xE0,0xFF,0xFF,0x3F,
0x00,0x00,0x01,0x03,0x03,0x07,0x07,0x06,0x06,0x06,
0x06,0x07,0x03,0x07,0x0F,0x04,0x00,

// R (82): w=14, h=19
0x4D,0x15,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x87,0xFE,0xFE,0x78,
0xFF,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x07,
0x0F,0xFF,0xFD,0xF0,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x07,0x07,0x07,

// S (83): w=14, h=19
0x4D,0x15,
0x78,0xFC,0xFE,0xCE,0x87,0x83,0x03,0x03,0x07,0x07,
0x1E,0x1E,0x1C,0x00,
0xE0,0xE0,0xE1,0x01,0x01,0x03,0x03,0x03,0x07,0x07,
0x8E,0xFE,0xFC,0xF8,
0x01,0x03,0x03,0x07,0x07,0x06,0x06,0x06,0x07,0x07,
0x03,0x03,0x01,0x00,

// T (84): w=15, h=19
0x4E,0x08,
0x03,0x03,0x03,0x03,0x03,0x03,0xFF,0xFF,0xFF,0x03,
0x03,0x03,0x03,0x03,0x03,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,
0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x00,
0x00,0x00,0x00,0x00,0x00,

// U (85): w=15, h=19
0x4E,0x16,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xFF,0xFF,0xFF,
0x7F,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xC0,0xFF,0xFF,0x7F,
0x00,0x01,0x03,0x03,0x07,0x07,0x06,0x06,0x06,0x07,
0x07,0x03,0x03,0x01,0x00,

// V (86): w=16, h=19
0x4F,0x0F,
0x07,0x1F,0xFF,0xFC,0xF0,0x80,0x00,0x00,0x00,0x00,
0x80,0xF0,0xFC,0xFF,0x1F,0x07,
0x00,0x00,0x00,0x07,0x3F,0xFF,0xFC,0xC0,0xC0,0xFC,
0xFF,0x3F,0x07,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x07,
0x00,0x00,0x00,0x00,0x00,0x00,

// W (87): w=21, h=19
0x54,0x0F,
0x0F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x80,0xF0,0xFF,
0x3F,0xFF,0xF0,0x80,0x00,0x00,0x00,0xF0,0xFF,0xFF,
0x0F,
0x00,0x00,0x0F,0xFF,0xFE,0xC0,0xF8,0xFF,0x3F,0x03,
0x00,0x03,0x3F,0xFF,0xF8,0xC0,0xFE,0xFF,0x0F,0x00,
0x00,
0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x00,0x00,0x00,
0x00,

// X (88): w=16, h=19
0x4F,0x0F,
0x03,0x07,0x0F,0x1E,0x7C,0xF0,0xE0,0xC0,0xC0,0xE0,
0xF0,0x7C,0x1E,0x0F,0x07,0x03,
0x00,0x00,0xC0,0xE0,0xF8,0x7C,0x1F,0x0F,0x0F,0x1F,
0x7C,0xF8,0xE0,0xC0,0x00,0x00,
0x06,0x07,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x03,0x07,0x07,0x06,

// Y (89): w=15, h=19
0x4E,0x0F,
0x03,0x0F,0x3F,0xFE,0xF8,0xE0,0x00,0x00,0x00,0xC0,
0xF0,0xFC,0x3F,0x0F,0x03,
0x00,0x00,0x00,0x00,0x03,0x0F,0xFF,0xFC,0xFF,0x0F,
0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x00,
0x00,0x00,0x00,0x00,0x00,

// Z (90): w=14, h=19
0x4D,0x0F,
0x03,0x03,0x03,0x03,0x03,0x03,0x83,0xE3,0xF3,0xFB,
0x7F,0x1F,0x0F,0x07,
0x00,0x80,0xE0,0xF8,0xFC,0x3F,0x1F,0x07,0x03,0x00,
0x00,0x00,0x00,0x00,
0x07,0x07,0x07,0x07,0x06,0x06,0x06,0x06,0x06,0x06,
0x06,0x06,0x06,0x06,

// bracketleft (91): w=5, h=24
0x44,0x10,
0xFF,0xFF,0xFF,0x03,0x03,
0xFF,0xFF,0xFF,0x00,0x00,
0xFF,0xFF,0xFF,0xC0,0xC0,

// backslash (92): w=7, h=19
0x46,0x09,
0x07,0x3F,0xF8,0xC0,0x00,0x00,0x00,
0x00,0x00,0x03,0x1F,0xFC,0xE0,0x00,
0x00,0x00,0x00,0x00,0x00,0x07,0x07,

// bracketright (93): w=5, h=24
0x44,0x15,
0x03,0x03,0xFF,0xFF,0xFF,
0x00,0x00,0xFF,0xFF,0xFF,
0xC0,0xC0,0xFF,0xFF,0xFF,

// asciicircum (94): w=11, h=9
0x2A,0x10,
0x80,0xE0,0xF0,0x7C,0x1F,0x07,0x1F,0x7C,0xF0,0xE0,
0x80,
0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,
0x01,

// underscore (95): w=14, h=2
0x8D,0x16,0x45,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x03,0x03,

// grave (96): w=6, h=4
0x05,0x0F,
0x01,0x03,0x07,0x0E,0x0C,0x08,

// a (97): w=12, h=14
0x2B,0xAF,
0x00,0x8C,0xCE,0xCF,0xC3,0xE3,0x63,0x67,0xFF,0xFE,
0xFC,0x00,
0x0F,0x1F,0x3F,0x39,0x30,0x30,0x38,0x1C,0x1F,0x3F,
0x3F,0x30,

// b (98): w=12, h=19
0x4B,0x15,
0xFF,0xFF,0xFF,0xC0,0xE0,0x60,0x60,0xE0,0xE0,0xC0,
0x80,0x00,
0xFF,0xFF,0xFF,0x81,0x00,0x00,0x00,0x00,0x81,0xFF,
0xFF,0x7E,
0x07,0x07,0x07,0x03,0x07,0x06,0x06,0x07,0x07,0x03,
0x01,0x00,

// c (99): w=11, h=14
0x2A,0xAF,
0xF0,0xFC,0xFE,0x0F,0x07,0x03,0x03,0x07,0x0F,0x0E,
0x0C,
0x03,0x0F,0x1F,0x3C,0x38,0x30,0x30,0x38,0x3C,0x1C,
0x0C,

// d (100): w=12, h=19
0x4B,0x10,
0x00,0x80,0xC0,0xE0,0xE0,0x60,0x60,0xE0,0xC0,0xFF,
0xFF,0xFF,
0x7E,0xFF,0xFF,0x81,0x00,0x00,0x00,0x00,0x81,0xFF,
0xFF,0xFF,
0x00,0x01,0x03,0x07,0x07,0x06,0x06,0x07,0x03,0x07,
0x07,0x07,

// e (101): w=12, h=14
0x2B,0xAF,
0xF0,0xFC,0xFE,0xCE,0xC7,0xC3,0xC3,0xC7,0xCE,0xFE,
0xFC,0xF0,
0x03,0x0F,0x1F,0x1C,0x38,0x30,0x30,0x30,0x38,0x1C,
0x1C,0x0C,

// f (102): w=7, h=19
0x46,0x0F,
0x60,0x60,0xFE,0xFF,0xFF,0x63,0x63,
0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x00,0x07,0x07,0x07,0x00,0x00,

// g (103): w=12, h=19
0x4B,0xB0,
0xF0,0xFC,0xFE,0x0F,0x07,0x03,0x03,0x07,0x0E,0xFF,
0xFF,0xFF,
0x83,0x8F,0x9F,0x3C,0x38,0x30,0x30,0x38,0x1C,0xFF,
0xFF,0xFF,
0x01,0x03,0x03,0x07,0x06,0x06,0x06,0x06,0x07,0x03,
0x03,0x00,

// h (104): w=11, h=19
0x4A,0x16,
0xFF,0xFF,0xFF,0xC0,0x60,0x60,0x60,0xE0,0xC0,0xC0,
0x00,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0xFF,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x07,0x07,
0x07,

// i (105): w=3, h=19
0x42,0x16,
0xE7,0xE7,0xE7,
0xFF,0xFF,0xFF,
0x07,0x07,0x07,

// j (106): w=5, h=24
0x44,0x0A,
0x00,0x00,0xE7,0xE7,0xE7,
0x00,0x00,0xFF,0xFF,0xFF,
0xC0,0xC0,0xFF,0xFF,0x7F,

// k (107): w=11, h=19
0x4A,0x15,
0xFF,0xFF,0xFF,0x00,0x00,0x80,0xC0,0xE0,0xE0,0x60,
0x00,
0xFF,0xFF,0xFF,0x0E,0x1F,0x7F,0xFB,0xF1,0xC0,0x80,
0x00,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x03,0x07,0x07,
0x06,

// l (108): w=3, h=19
0x42,0x16,
0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,
0x07,0x07,0x07,

// m (109): w=17, h=14
0x30,0xB6,
0xFF,0xFF,0xFF,0x06,0x03,0x03,0x07,0xFF,0xFE,0xFE,
0x07,0x03,0x03,0x07,0xFF,0xFE,0xFC,
0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,
0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,

// n (110): w=11, h=14
0x2A,0xB6,
0xFF,0xFF,0xFF,0x06,0x03,0x03,0x03,0x07,0xFF,0xFE,
0xF8,
0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,
0x3F,

// o (111): w=12, h=14
0x2B,0xAF,
0xF0,0xFC,0xFE,0x0E,0x07,0x03,0x03,0x07,0x0E,0xFE,
0xFC,0xF0,
0x03,0x0F,0x1F,0x1C,0x38,0x30,0x30,0x38,0x1C,0x1F,
0x0F,0x03,

// p (112): w=12, h=19
0x4B,0xB5,
0xFF,0xFF,0xFF,0x0E,0x07,0x03,0x03,0x07,0x0F,0xFE,
0xFC,0xF0,
0xFF,0xFF,0xFF,0x1C,0x38,0x30,0x30,0x38,0x3C,0x1F,
0x0F,0x03,
0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,

// q (113): w=12, h=19
0x4B,0xB0,
0xF0,0xFC,0xFE,0x0F,0x07,0x03,0x03,0x07,0x0E,0xFF,
0xFF,0xFF,
0x03,0x0F,0x1F,0x3C,0x38,0x30,0x30,0x38,0x1C,0xFF,
0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,
0x07,0x07,

// r (114): w=7, h=14
0x26,0xB5,
0xFF,0xFF,0xFF,0x0E,0x07,0x07,0x07,
0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,

// s (115): w=11, h=14
0x2A,0xAF,
0x3C,0x7E,0x7F,0x67,0xE3,0xE3,0xC7,0xCF,0xCE,0x8C,
0x00,
0x0E,0x1E,0x3E,0x38,0x30,0x30,0x30,0x39,0x3F,0x1F,
0x0F,

// t (116): w=7, h=18
0x46,0x2F,
0x30,0x30,0xFF,0xFF,0xFF,0x30,0x30,
0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x00,0x01,0x03,0x03,0x03,0x03,

// u (117): w=11, h=14
0x2A,0xB6,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0xFF,
0x07,0x1F,0x1F,0x38,0x30,0x30,0x18,0x0C,0x3F,0x3F,
0x3F,

// v (118): w=12, h=14
0x2B,0xAF,
0x07,0x3F,0xFF,0xF8,0xC0,0x00,0x00,0xC0,0xF8,0xFF,
0x3F,0x07,
0x00,0x00,0x01,0x07,0x3F,0x3E,0x3E,0x3F,0x07,0x01,
0x00,0x00,

// w (119): w=19, h=14
0x32,0xA8,
0x03,0x1F,0xFF,0xF8,0x80,0x00,0xC0,0xF8,0xFF,0x1F,
0xFF,0xF8,0xC0,0x00,0x80,0xF8,0xFF,0x1F,0x03,
0x00,0x00,0x00,0x07,0x3F,0x3E,0x3F,0x07,0x00,0x00,
0x00,0x07,0x3F,0x3E,0x3F,0x07,0x00,0x00,0x00,

// x (120): w=11, h=14
0x2A,0xAF,
0x03,0x07,0x1F,0xBE,0xF8,0xF0,0xF8,0xBE,0x1F,0x07,
0x03,
0x30,0x3C,0x3E,0x1F,0x07,0x01,0x07,0x1F,0x3E,0x3C,
0x30,

// y (121): w=13, h=19
0x4C,0xAF,
0x03,0x0F,0x3F,0xFC,0xF8,0xE0,0x00,0x00,0xE0,0xF8,
0xFF,0x3F,0x07,
0x00,0x00,0x00,0x00,0x03,0xCF,0xFF,0xFF,0x3F,0x07,
0x00,0x00,0x00,
0x00,0x00,0x06,0x06,0x07,0x07,0x03,0x00,0x00,0x00,
0x00,0x00,0x00,

// z (122): w=11, h=14
0x2A,0xAF,
0x03,0x03,0x03,0x83,0xE3,0xF3,0xFB,0x3F,0x1F,0x07,
0x03,
0x38,0x3C,0x3F,0x3F,0x33,0x31,0x30,0x30,0x30,0x30,
0x30,

// braceleft (123): w=7, h=24
0x46,0x10,
0x00,0x00,0xFC,0xFE,0xFF,0x03,0x01,
0x18,0x3C,0xFF,0xE7,0xC3,0x00,0x00,
0x00,0x00,0x3F,0x7F,0xFF,0xC0,0x80,

// bar (124): w=2, h=24
0x41,0x1C,
0xFF,0xFF,
0xFF,0xFF,
0xFF,0xFF,

// braceright (125): w=7, h=24
0x46,0x15,
0x01,0x03,0xFF,0xFE,0xFC,0x00,0x00,
0x00,0x00,0xC3,0xE7,0xFF,0x3C,0x18,
0x80,0xC0,0xFF,0x7F,0x3F,0x00,0x00,

// asciitilde (126): w=11, h=4
0x8A,0x0A,0x57,
0x0E,0x0F,0x0F,0x03,0x07,0x06,0x0E,0x0C,0x0F,0x0F,
0x07

};
// 2995 bytes
// 3185 bytes with FAST_FONT_INDEX
