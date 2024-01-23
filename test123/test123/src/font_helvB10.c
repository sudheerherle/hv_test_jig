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

// helvB10 = "Helvetica Bold"
const unsigned char font_helvB10[] = {
0x20,0x5E,   // characters 32 to 126
#ifdef FAST_FONT_INDEX
0xC0,0x00,
0xC4,0x00,
0xCA,0x00,
0xD1,0x00,
0xE5,0x00,
0xF5,0x00,
0x0F,0x01,
0x23,0x01,
0x27,0x01,
0x31,0x01,
0x3B,0x01,
0x42,0x01,
0x4C,0x01,
0x52,0x01,
0x57,0x01,
0x5C,0x01,
0x66,0x01,
0x76,0x01,
0x80,0x01,
0x90,0x01,
0xA0,0x01,
0xB2,0x01,
0xC2,0x01,
0xD2,0x01,
0xE2,0x01,
0xF2,0x01,
0x02,0x02,
0x06,0x02,
0x0E,0x02,
0x16,0x02,
0x1F,0x02,
0x27,0x02,
0x37,0x02,
0x53,0x02,
0x69,0x02,
0x7B,0x02,
0x8F,0x02,
0xA3,0x02,
0xB3,0x02,
0xC3,0x02,
0xD7,0x02,
0xE9,0x02,
0xEF,0x02,
0xFF,0x02,
0x13,0x03,
0x23,0x03,
0x3B,0x03,
0x4F,0x03,
0x65,0x03,
0x77,0x03,
0x8D,0x03,
0xA1,0x03,
0xB3,0x03,
0xC5,0x03,
0xD9,0x03,
0xEF,0x03,
0x0D,0x04,
0x21,0x04,
0x37,0x04,
0x49,0x04,
0x53,0x04,
0x5D,0x04,
0x67,0x04,
0x6F,0x04,
0x7A,0x04,
0x7F,0x04,
0x88,0x04,
0x98,0x04,
0xA0,0x04,
0xB0,0x04,
0xB8,0x04,
0xC4,0x04,
0xD4,0x04,
0xE4,0x04,
0xEA,0x04,
0xF2,0x04,
0x00,0x05,
0x06,0x05,
0x12,0x05,
0x1B,0x05,
0x24,0x05,
0x34,0x05,
0x44,0x05,
0x4B,0x05,
0x53,0x05,
0x5F,0x05,
0x68,0x05,
0x72,0x05,
0x7E,0x05,
0x87,0x05,
0x99,0x05,
0xA1,0x05,
0xAD,0x05,
0xB1,0x05,
0xBD,0x05,
#endif // FAST_FONT_INDEX

// space (32): w=1, h=1
0x80,0x0A,0x48,
0x00,

// exclam (33): w=2, h=11
0x21,0x0F,
0xFF,0x3F,
0x06,0x06,

// quotedbl (34): w=5, h=3
0x04,0x0F,
0x07,0x03,0x00,0x07,0x03,

// numbersign (35): w=9, h=10
0x28,0x28,
0x40,0xC8,0xF8,0x7F,0xCF,0xF8,0x7F,0x4F,0x08,
0x00,0x03,0x03,0x00,0x03,0x03,0x00,0x00,0x00,

// dollar (36): w=7, h=13
0x26,0x09,
0x1E,0x3F,0x31,0xFF,0x61,0xC7,0x86,
0x03,0x07,0x04,0x3F,0x04,0x07,0x03,

// percent (37): w=12, h=11
0x2B,0x09,
0x06,0x0F,0x09,0x09,0x8F,0xE6,0x30,0x9C,0x86,0x83,
0x80,0x00,
0x00,0x00,0x00,0x06,0x03,0x00,0x03,0x07,0x04,0x04,
0x07,0x03,

// ampersand (38): w=9, h=10
0x28,0x2F,
0xE0,0xF6,0x3F,0x39,0x6F,0xC6,0xF0,0x30,0x00,
0x01,0x03,0x02,0x02,0x03,0x03,0x01,0x03,0x02,

// quotesingle (39): w=2, h=3
0x01,0x0F,
0x07,0x03,

// parenleft (40): w=4, h=14
0x23,0x0E,
0xF8,0xFE,0x07,0x01,
0x07,0x1F,0x38,0x20,

// parenright (41): w=4, h=14
0x23,0x09,
0x01,0x07,0xFE,0xF8,
0x20,0x38,0x1F,0x07,

// asterisk (42): w=5, h=4
0x04,0x09,
0x0A,0x0E,0x07,0x0E,0x0A,

// plus (43): w=8, h=7
0x07,0x69,
0x08,0x08,0x08,0x7F,0x7F,0x08,0x08,0x08,

// comma (44): w=3, h=3
0x82,0x09,0x46,
0x04,0x07,0x03,

// hyphen (45): w=3, h=1
0x02,0xC9,
0x01,0x01,0x01,

// period (46): w=2, h=2
0x81,0x09,0x56,
0x03,0x03,

// slash (47): w=4, h=11
0x23,0x08,
0x00,0xE0,0x3C,0x07,
0x07,0x01,0x00,0x00,

// zero (48): w=7, h=11
0x26,0x09,
0xFC,0xFE,0x03,0x01,0x03,0xFE,0xFC,
0x01,0x03,0x06,0x04,0x06,0x03,0x01,

// one (49): w=4, h=11
0x23,0x11,
0x02,0x02,0xFF,0xFF,
0x00,0x00,0x07,0x07,

// two (50): w=7, h=11
0x26,0x09,
0x06,0x07,0x81,0xC1,0x71,0x3F,0x1E,
0x06,0x07,0x05,0x04,0x04,0x04,0x04,

// three (51): w=7, h=11
0x26,0x09,
0x06,0x07,0x21,0x21,0x21,0xFF,0xDE,
0x03,0x07,0x04,0x04,0x04,0x07,0x03,

// four (52): w=8, h=11
0x27,0x08,
0xE0,0xF0,0x98,0x8C,0x86,0xFF,0xFF,0x80,
0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x00,

// five (53): w=7, h=11
0x26,0x09,
0x18,0x1F,0x17,0x11,0x31,0xF1,0xE1,
0x03,0x07,0x04,0x04,0x06,0x03,0x01,

// six (54): w=7, h=11
0x26,0x09,
0xF8,0xFE,0x27,0x11,0x11,0xF7,0xE6,
0x03,0x07,0x04,0x04,0x04,0x07,0x03,

// seven (55): w=7, h=11
0x26,0x09,
0x01,0x01,0xC1,0xF1,0x3D,0x0F,0x03,
0x00,0x07,0x07,0x00,0x00,0x00,0x00,

// eight (56): w=7, h=11
0x26,0x09,
0xDE,0xFF,0x21,0x21,0x21,0xFF,0xDE,
0x03,0x07,0x04,0x04,0x04,0x07,0x03,

// nine (57): w=7, h=11
0x26,0x09,
0x3E,0x7F,0x41,0x41,0x41,0xFF,0xFE,
0x03,0x07,0x04,0x04,0x06,0x03,0x01,

// colon (58): w=2, h=8
0x01,0x75,
0xC3,0xC3,

// semicolon (59): w=3, h=9
0x22,0x6F,
0x00,0xC3,0xC3,
0x01,0x01,0x00,

// less (60): w=6, h=5
0x05,0x8F,
0x04,0x0E,0x0A,0x1B,0x11,0x11,

// equal (61): w=7, h=3
0x06,0xAF,
0x05,0x05,0x05,0x05,0x05,0x05,0x05,

// greater (62): w=6, h=5
0x05,0x8F,
0x11,0x11,0x1B,0x0A,0x0E,0x04,

// question (63): w=7, h=11
0x26,0x0F,
0x06,0x07,0xC1,0xE1,0x31,0x1F,0x0E,
0x00,0x00,0x06,0x06,0x00,0x00,0x00,

// at (64): w=13, h=12
0x2C,0x09,
0xF0,0xFC,0x0E,0xE6,0xF3,0x19,0xC9,0xF1,0x3B,0x06,
0x8E,0xFC,0x70,
0x01,0x03,0x06,0x0C,0x09,0x09,0x09,0x0C,0x05,0x01,
0x01,0x00,0x00,

// A (65): w=10, h=11
0x29,0x08,
0x00,0xC0,0xF0,0xBC,0x87,0x87,0xBC,0xF0,0xC0,0x00,
0x06,0x07,0x01,0x00,0x00,0x00,0x00,0x01,0x07,0x06,

// B (66): w=8, h=11
0x27,0x0F,
0xFF,0xFF,0x21,0x21,0x21,0x73,0xDF,0x8E,
0x07,0x07,0x04,0x04,0x04,0x06,0x07,0x03,

// C (67): w=9, h=11
0x28,0x0F,
0xF8,0xFE,0x06,0x03,0x03,0x01,0x03,0x03,0x06,
0x00,0x03,0x03,0x06,0x06,0x04,0x06,0x06,0x03,

// D (68): w=9, h=11
0x28,0x0F,
0xFF,0xFF,0x01,0x01,0x01,0x03,0x06,0xFE,0xF8,
0x07,0x07,0x04,0x04,0x04,0x06,0x03,0x03,0x00,

// E (69): w=7, h=11
0x26,0x0F,
0xFF,0xFF,0x21,0x21,0x21,0x21,0x21,
0x07,0x07,0x04,0x04,0x04,0x04,0x04,

// F (70): w=7, h=11
0x26,0x0F,
0xFF,0xFF,0x21,0x21,0x21,0x21,0x01,
0x07,0x07,0x00,0x00,0x00,0x00,0x00,

// G (71): w=9, h=11
0x28,0x0F,
0xF8,0xFE,0x06,0x03,0x03,0x21,0x23,0xE3,0xE6,
0x00,0x03,0x03,0x06,0x06,0x04,0x06,0x03,0x07,

// H (72): w=8, h=11
0x27,0x0F,
0xFF,0xFF,0x20,0x20,0x20,0x20,0xFF,0xFF,
0x07,0x07,0x00,0x00,0x00,0x00,0x07,0x07,

// I (73): w=2, h=11
0x21,0x0F,
0xFF,0xFF,
0x07,0x07,

// J (74): w=7, h=11
0x26,0x09,
0x80,0x80,0x00,0x00,0x00,0xFF,0xFF,
0x03,0x07,0x06,0x04,0x06,0x07,0x03,

// K (75): w=9, h=11
0x28,0x0E,
0xFF,0xFF,0x30,0x78,0xCC,0x86,0x03,0x01,0x00,
0x07,0x07,0x00,0x00,0x00,0x01,0x03,0x06,0x04,

// L (76): w=7, h=11
0x26,0x0E,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x07,0x07,0x04,0x04,0x04,0x04,0x04,

// M (77): w=11, h=11
0x2A,0x0F,
0xFF,0xFF,0x1C,0xF0,0x80,0x00,0x80,0xF0,0x1C,0xFF,
0xFF,
0x07,0x07,0x00,0x00,0x03,0x06,0x03,0x00,0x00,0x07,
0x07,

// N (78): w=9, h=11
0x28,0x0F,
0xFF,0xFF,0x06,0x18,0x70,0xC0,0x00,0xFF,0xFF,
0x07,0x07,0x00,0x00,0x00,0x00,0x03,0x07,0x07,

// O (79): w=10, h=11
0x29,0x0F,
0xF8,0xFE,0x06,0x03,0x01,0x01,0x03,0x06,0xFE,0xF8,
0x00,0x03,0x03,0x06,0x04,0x04,0x06,0x03,0x03,0x00,

// P (80): w=8, h=11
0x27,0x0F,
0xFF,0xFF,0x21,0x21,0x21,0x33,0x3F,0x1E,
0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,

// Q (81): w=10, h=11
0x29,0x0F,
0xF8,0xFE,0x06,0x03,0x01,0x81,0x83,0x06,0xFE,0xF8,
0x00,0x03,0x03,0x06,0x04,0x04,0x07,0x03,0x07,0x04,

// R (82): w=9, h=11
0x28,0x0F,
0xFF,0xFF,0x21,0x21,0x21,0x73,0xFF,0xCE,0x00,
0x07,0x07,0x00,0x00,0x00,0x00,0x03,0x07,0x04,

// S (83): w=8, h=11
0x27,0x0F,
0x0E,0x1F,0x1B,0x31,0x31,0x63,0xE7,0xC6,
0x03,0x07,0x06,0x04,0x06,0x06,0x03,0x01,

// T (84): w=8, h=11
0x27,0x08,
0x01,0x01,0x01,0xFF,0xFF,0x01,0x01,0x01,
0x00,0x00,0x00,0x07,0x07,0x00,0x00,0x00,

// U (85): w=9, h=11
0x28,0x0F,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0x01,0x03,0x06,0x04,0x04,0x04,0x06,0x03,0x01,

// V (86): w=10, h=11
0x29,0x08,
0x03,0x1F,0x7C,0xF0,0x80,0x80,0xF0,0x7C,0x1F,0x03,
0x00,0x00,0x00,0x01,0x07,0x07,0x01,0x00,0x00,0x00,

// W (87): w=14, h=11
0x2D,0x08,
0x07,0x7F,0xF8,0x00,0xC0,0xF8,0x0F,0x0F,0xF8,0xC0,
0x00,0xF8,0x7F,0x07,
0x00,0x00,0x01,0x07,0x07,0x00,0x00,0x00,0x00,0x07,
0x07,0x01,0x00,0x00,

// X (88): w=9, h=11
0x28,0x08,
0x03,0x87,0xCC,0x78,0x30,0x78,0xCC,0x87,0x03,
0x06,0x07,0x01,0x00,0x00,0x00,0x01,0x07,0x06,

// Y (89): w=10, h=11
0x29,0x08,
0x01,0x07,0x1E,0x78,0xE0,0xE0,0x78,0x1E,0x07,0x01,
0x00,0x00,0x00,0x00,0x07,0x07,0x00,0x00,0x00,0x00,

// Z (90): w=8, h=11
0x27,0x09,
0x01,0x81,0xC1,0xF1,0x39,0x1D,0x07,0x03,
0x06,0x07,0x05,0x04,0x04,0x04,0x04,0x04,

// bracketleft (91): w=4, h=14
0x23,0x0E,
0xFF,0xFF,0x01,0x01,
0x3F,0x3F,0x20,0x20,

// backslash (92): w=4, h=11
0x23,0x08,
0x07,0x3C,0xE0,0x00,
0x00,0x00,0x01,0x07,

// bracketright (93): w=4, h=14
0x23,0x09,
0x01,0x01,0xFF,0xFF,
0x20,0x20,0x3F,0x3F,

// asciicircum (94): w=6, h=5
0x05,0x0F,
0x18,0x1E,0x03,0x03,0x1E,0x18,

// underscore (95): w=8, h=1
0x87,0x0D,0x45,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,

// grave (96): w=3, h=2
0x02,0x0F,
0x01,0x03,0x02,

// a (97): w=7, h=8
0x06,0x6E,
0x72,0xFB,0x89,0xC9,0x7F,0xFE,0x80,

// b (98): w=7, h=11
0x26,0x0F,
0xFF,0xFF,0x10,0x08,0x18,0xF0,0xE0,
0x07,0x07,0x02,0x04,0x06,0x03,0x01,

// c (99): w=6, h=8
0x05,0x6F,
0x3C,0x7E,0xC3,0x81,0xE7,0x66,

// d (100): w=7, h=11
0x26,0x0F,
0xE0,0xF0,0x18,0x08,0x10,0xFF,0xFF,
0x01,0x03,0x06,0x04,0x02,0x07,0x07,

// e (101): w=6, h=8
0x05,0x6F,
0x7E,0xFF,0xC9,0x89,0xCF,0x4E,

// f (102): w=5, h=11
0x24,0x07,
0x08,0xFE,0xFF,0x09,0x01,
0x00,0x07,0x07,0x00,0x00,

// g (103): w=7, h=11
0x26,0x6F,
0x3C,0x7E,0xC3,0x81,0x43,0xFE,0xFF,
0x02,0x06,0x04,0x04,0x06,0x07,0x03,

// h (104): w=7, h=11
0x26,0x0F,
0xFF,0xFF,0x10,0x08,0x18,0xF8,0xF0,
0x07,0x07,0x00,0x00,0x00,0x07,0x07,

// i (105): w=2, h=11
0x21,0x0F,
0xFB,0xFB,
0x07,0x07,

// j (106): w=3, h=14
0x22,0x09,
0x00,0xFB,0xFB,
0x30,0x3F,0x1F,

// k (107): w=6, h=11
0x25,0x0F,
0xFF,0xFF,0x60,0xF0,0x98,0x08,
0x07,0x07,0x00,0x01,0x07,0x06,

// l (108): w=2, h=11
0x21,0x0F,
0xFF,0xFF,
0x07,0x07,

// m (109): w=10, h=8
0x09,0x6F,
0xFF,0xFF,0x02,0x01,0xFF,0xFE,0x03,0x01,0xFF,0xFE,

// n (110): w=7, h=8
0x06,0x6F,
0xFF,0xFF,0x02,0x01,0x03,0xFF,0xFE,

// o (111): w=7, h=8
0x06,0x6F,
0x3C,0x7E,0xC3,0x81,0xC3,0x7E,0x3C,

// p (112): w=7, h=11
0x26,0x6F,
0xFF,0xFF,0x42,0x81,0xC3,0x7E,0x3C,
0x07,0x07,0x00,0x00,0x00,0x00,0x00,

// q (113): w=7, h=11
0x26,0x6F,
0x3C,0x7E,0xC3,0x81,0x42,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x07,0x07,

// r (114): w=5, h=8
0x04,0x6E,
0xFF,0xFF,0x02,0x03,0x03,

// s (115): w=6, h=8
0x05,0x6F,
0x46,0xCF,0xCD,0x99,0xFB,0x72,

// t (116): w=5, h=10
0x24,0x28,
0x04,0xFF,0xFF,0x04,0x04,
0x00,0x01,0x03,0x02,0x01,

// u (117): w=7, h=8
0x06,0x6F,
0x7F,0xFF,0xC0,0x80,0x40,0xFF,0xFF,

// v (118): w=8, h=8
0x07,0x68,
0x03,0x0F,0x3C,0xE0,0xE0,0x3C,0x0F,0x03,

// w (119): w=10, h=8
0x09,0x68,
0x07,0x1F,0xF8,0xE0,0x1F,0x1F,0xE0,0xF8,0x1F,0x07,

// x (120): w=7, h=8
0x06,0x68,
0xC3,0xE7,0x3C,0x18,0x3C,0xE7,0xC3,

// y (121): w=8, h=11
0x27,0x68,
0x03,0x0F,0x3C,0xE0,0xE0,0x3C,0x0F,0x03,
0x00,0x04,0x06,0x07,0x01,0x00,0x00,0x00,

// z (122): w=6, h=8
0x05,0x68,
0xC1,0xE1,0xB9,0x9D,0x87,0x83,

// braceleft (123): w=5, h=14
0x24,0x09,
0x40,0xE0,0xBE,0x1F,0x01,
0x00,0x00,0x1F,0x3F,0x20,

// bar (124): w=1, h=14
0x20,0x15,
0xFF,
0x3F,

// braceright (125): w=5, h=14
0x24,0x0E,
0x01,0x1F,0xBE,0xE0,0x40,
0x20,0x3F,0x1F,0x00,0x00,

// asciitilde (126): w=7, h=3
0x06,0xAF,
0x06,0x03,0x01,0x03,0x06,0x06,0x03

};
// 1288 bytes
// 1478 bytes with FAST_FONT_INDEX
