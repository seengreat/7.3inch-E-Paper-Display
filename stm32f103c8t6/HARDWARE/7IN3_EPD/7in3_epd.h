#ifndef __7INCH3_EPD_H
#define	__7INCH3_EPD_H	

#include "7in3_epd.h"
#include "gpio.h"
#include "stdio.h"
#include <stdint.h>
//#define EPD_7IN3F_WIDTH   800
//#define EPD_7IN3F_HEIGHT  480

/**********************************
Color Index
**********************************/
#define EPD_7IN3F_BLACK   0x0   /// 000
#define EPD_7IN3F_WHITE   0x1   /// 001
#define EPD_7IN3F_GREEN   0x2   /// 010
#define EPD_7IN3F_BLUE    0x3   /// 011
#define EPD_7IN3F_RED     0x4   /// 100
#define EPD_7IN3F_YELLOW  0x5   /// 101
#define EPD_7IN3F_ORANGE  0x6   /// 110
#define EPD_7IN3F_CLEAN   0x7   /// 111  


#define PSR         0x00
#define PWRR        0x01
#define POF         0x02
#define POFS        0x03
#define PON         0x04
#define BTST1       0x05
#define BTST2       0x06
#define DSLP        0x07
#define BTST3       0x08
#define DTM         0x10
#define DRF         0x12
#define PLL         0x30
#define CDI         0x50
#define TCON        0x60
#define TRES        0x61
#define REV         0x70
#define VDCS        0x82
#define T_VDCS      0x84
#define PWS         0xE3
//EPD
void EPD_W21_Init(void);
void EPD_init(void);
void EPD_sleep(void);
void lcd_chkstatus(void);
void EPD_7IN3F_Clear(unsigned char color);
void Display_color(void);
void EPD_horizontal(void);
void EPD_vertical(void);
void EPD_7IN3F_DisplayPart(uint8_t *Image, uint16_t xstart, uint16_t ystart, uint16_t image_width, uint16_t image_heigh);
void EPD_7IN3F_Display( const unsigned char *Image, uint16_t xstart, uint16_t ystart, uint16_t image_width, uint16_t image_heigh);
#endif
/***********************************************************
						end file
***********************************************************/


