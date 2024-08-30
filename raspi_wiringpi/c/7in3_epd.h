#ifndef __EPD_7IN3F_H_
#define __EPD_7IN3F_H_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "epd_gui.h"

// Display resolution
#define EPD_7IN3F_WIDTH       800
#define EPD_7IN3F_HEIGHT      480




#define PIN_CS      10   
#define PIN_RST     0   
#define PIN_DC      6  
#define PIN_BUSY    5
#define PIN_PWR     1 
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
#define EPD_7IN3F_CLEAN   0x7   /// 111   unavailable  Afterimage


/*Bitmap file header   14bit*/
typedef struct BMP_FILE_HEADER {
    uint16_t btype;        //File identifier
    uint32_t bsize;      //The size of the file
    uint16_t breserved1;   //Reserved value, must be set to 0
    uint16_t breserved2;   //Reserved value, must be set to 0
    uint32_t boffset;    //The offset from the beginning of the file header to the beginning of the image data bit
} __attribute__ ((packed)) BMPFILEHEADER;    // 14bit

/*Bitmap information header  40bit*/
typedef struct BMP_INFO {
    uint32_t biinfoSize;      //The size of the header
    uint32_t biwidth;         //The width of the image
    uint32_t biheight;        //The height of the image
    uint16_t biplanes;          //The number of planes in the image
    uint16_t bibitcount;        //The number of bits per pixel
    uint32_t bicompression;   //Compression type
    uint32_t bimpimagesize;   //The size of the image, in bytes
    uint32_t bixpelspermeter; //Horizontal resolution
    uint32_t biypelspermeter; //Vertical resolution
    uint32_t biclrused;       //The number of colors used
    uint32_t biclrimportant;  //The number of important colors
} __attribute__ ((packed)) BMPINFOHEADER;

/*Color table: palette */
typedef struct RGB_QUAD {
    uint8_t rgbblue;               //Blue intensity
    uint8_t rgbgreen;              //Green strength
    uint8_t rgbred;                //Red intensity
    uint8_t rgbreversed;           //Reserved value
} __attribute__ ((packed)) BMPRGBQUAD;
/**************************************** end ***********************************************/
uint8_t GUI_ReadBmp_RGB_7Color(const char *path, uint16_t x, uint16_t y);

void EPD_GPIO_Init(void);
void EPD_7IN3F_Init(void);
void EPD_7IN3F_Clear(uint8_t color);
void EPD_7IN3F_Show7Block(void);
void EPD_7IN3F_Display(uint8_t *Image);
void EPD_7IN3F_Sleep(void);
void Epaper_READBUSY(void);
void delay_xms(unsigned int xms);
void EPD_GPIO_Exit(void); 
static void EPD_7IN3F_TurnOnDisplay(void);

void Epaper_Write_Command(unsigned char cmd);
void Epaper_Write_Data(unsigned char data);
#endif
