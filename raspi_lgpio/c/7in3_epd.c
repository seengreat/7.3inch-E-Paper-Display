#include <string.h>
#include <stdlib.h>	
#include <stdio.h>
#include <stdio.h>	
#include "7in3_epd.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "epd_gui.h"
#include "sg_lgpio.h"
//define pin conection in BCM number
/****************************
* PIN_CS      8
* PIN_RST     17 
* PIN_DC      25
* PIN_BUSY    24
***************************/

void EPD_GPIO_Init(void)
{
    Lgpio_export(); 
    Lgpio_set_pinmode(PIN_RST,LGPIO_OUTPUT);
    Lgpio_set_pinmode(PIN_DC,LGPIO_OUTPUT);
    Lgpio_set_pinmode(PIN_BUSY,LGPIO_INPUT); 
    LG_SPI_Init(0, 0, 8000000);
}


/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_7IN3F_Init(void)
{
    Lgpio_write_pin(PIN_RST,LGPIO_HIGH);   
	delay_xms(10);
    Lgpio_write_pin(PIN_RST,LGPIO_LOW);    
	delay_xms(10);//At least 10ms delay 
	Lgpio_write_pin(PIN_RST,LGPIO_HIGH); 
	delay_xms(10); //At least 10ms delay 
 
    Epaper_READBUSY();
    delay_xms(30);
  
    Epaper_Write_Command(0xAA);    // CMDH
    Epaper_Write_Data(0x49);
    Epaper_Write_Data(0x55);
    Epaper_Write_Data(0x20);
    Epaper_Write_Data(0x08);
    Epaper_Write_Data(0x09);
    Epaper_Write_Data(0x18);
 
    Epaper_Write_Command(0x01);
    Epaper_Write_Data(0x3F);
    Epaper_Write_Data(0x00);
    Epaper_Write_Data(0x32);
    Epaper_Write_Data(0x2A);
    Epaper_Write_Data(0x0E);
    Epaper_Write_Data(0x2A);
  
    Epaper_Write_Command(0x00);
    Epaper_Write_Data(0x5F);
    Epaper_Write_Data(0x69);

    Epaper_Write_Command(0x03);
    Epaper_Write_Data(0x00);
    Epaper_Write_Data(0x54);
    Epaper_Write_Data(0x00);
    Epaper_Write_Data(0x44); 

    Epaper_Write_Command(0x05);
    Epaper_Write_Data(0x40);
    Epaper_Write_Data(0x1F);
    Epaper_Write_Data(0x1F);
    Epaper_Write_Data(0x2C);

    Epaper_Write_Command(0x06);
    Epaper_Write_Data(0x6F);
    Epaper_Write_Data(0x1F);
    Epaper_Write_Data(0x1F);
    Epaper_Write_Data(0x22);

    Epaper_Write_Command(0x08);
    Epaper_Write_Data(0x6F);
    Epaper_Write_Data(0x1F);
    Epaper_Write_Data(0x1F);
    Epaper_Write_Data(0x22);
 
    Epaper_Write_Command(0x13);    // IPC
    Epaper_Write_Data(0x00);
    Epaper_Write_Data(0x04);

    Epaper_Write_Command(0x30);
    Epaper_Write_Data(0x3C);
 
    Epaper_Write_Command(0x41);     // TSE
    Epaper_Write_Data(0x00);

    Epaper_Write_Command(0x50);
    Epaper_Write_Data(0x3F);

    Epaper_Write_Command(0x60);
    Epaper_Write_Data(0x02);
    Epaper_Write_Data(0x00);

    Epaper_Write_Command(0x61);
    Epaper_Write_Data(0x03);
    Epaper_Write_Data(0x20);
    Epaper_Write_Data(0x01); 
    Epaper_Write_Data(0xE0);

    Epaper_Write_Command(0x82);
    Epaper_Write_Data(0x1E); 

    Epaper_Write_Command(0x84);
    Epaper_Write_Data(0x00);

    Epaper_Write_Command(0x86);    // AGID
    Epaper_Write_Data(0x00);

    Epaper_Write_Command(0xE3);
    Epaper_Write_Data(0x2F);

    Epaper_Write_Command(0xE0);   // CCSET
    Epaper_Write_Data(0x00); 

    Epaper_Write_Command(0xE6);   // TSSET
    Epaper_Write_Data(0x00);
}

/******************************************************************************
function :  Clear screen
parameter:
******************************************************************************/
void EPD_7IN3F_Clear(uint8_t color)
{
    uint16_t width, height;
    width = (EPD_7IN3F_WIDTH % 2 == 0)? (EPD_7IN3F_WIDTH / 2 ): (EPD_7IN3F_WIDTH / 2 + 1);
    height = EPD_7IN3F_HEIGHT;
    Epaper_Write_Command(0x10);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            Epaper_Write_Data((color<<4)|color);
        }
   
    }
 
    EPD_7IN3F_TurnOnDisplay();
 
}


/******************************************************************************
function :  Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN3F_Display(uint8_t *Image)
{
    uint16_t width, height;
    width = (EPD_7IN3F_WIDTH % 2 == 0)? (EPD_7IN3F_WIDTH / 2 ): (EPD_7IN3F_WIDTH / 2 + 1);
    height = EPD_7IN3F_HEIGHT;

    Epaper_Write_Command(0x10);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            Epaper_Write_Data(Image[i + j * width]);
        }
    }
    EPD_7IN3F_TurnOnDisplay();
}

/******************************************************************************
function :  Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN3F_Sleep(void)
{
    Epaper_Write_Command(0x07); // DEEP_SLEEP
    Epaper_Write_Data(0XA5);
}


/******************************************************************************
function :  Turn On Display
parameter:
******************************************************************************/
static void EPD_7IN3F_TurnOnDisplay(void)
{
  
    Epaper_Write_Command(0x04); // POWER_ON
    Epaper_READBUSY();
  
    Epaper_Write_Command(0x12); // DISPLAY_REFRESH
    Epaper_Write_Data(0x00);
    Epaper_READBUSY();
 
    Epaper_Write_Command(0x02); // POWER_OFF
    Epaper_Write_Data(0X00);
    Epaper_READBUSY();
   
}


/******************************************************************************
function :  Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void Epaper_READBUSY(void)
{ 
    //printf("e-Paper busy H\r\n");
	while(Lgpio_read_pin(PIN_BUSY)==LGPIO_LOW) {      //LOW: busy, HIGH: idle
        delay_xms(2);
    } 
    //printf("e-Paper busy H  r\r\n");
}

void delay_xms(unsigned int xms)
{
	unsigned int i;
	while(xms--)
	{
		i=12000;
		while(i--);
	}
}


void Epaper_Write_Command(unsigned char cmd)
{  
    Lgpio_write_pin(PIN_DC,LGPIO_LOW); // D/C#   0:command  1:data
    Lgpio_write_pin(PIN_CS,LGPIO_HIGH); 
    Lgpio_write_pin(PIN_CS,LGPIO_LOW); 
 
	LG_SPI_writebyte(cmd);
    
    Lgpio_write_pin(PIN_CS,LGPIO_HIGH); 
  
}

void Epaper_Write_Data(unsigned char data)
{
    Lgpio_write_pin(PIN_DC,LGPIO_HIGH); // D/C#   0:command  1:data
    Lgpio_write_pin(PIN_CS,LGPIO_HIGH); 
    Lgpio_write_pin(PIN_CS,LGPIO_LOW); 
   
	LG_SPI_writebyte(data);
     
    Lgpio_write_pin(PIN_CS,LGPIO_HIGH);
}

/******************************************************************************
function :  Read bmp files directly
parameter:
    path: bmp file path
    x:  Starting Xpoint point coordinates
    y:  Starting Xpoint point coordinates
******************************************************************************/
uint8_t GUI_ReadBmp_RGB_7Color(const char *path, uint16_t x, uint16_t y)
{
    FILE *fp;                     //Define a file pointer
    BMPFILEHEADER bmpfileheader;  //Define a bmp file header structure
    BMPINFOHEADER bmpinfoheader;  //Define a bmp info header structure
    
    // Binary file open
    if((fp = fopen(path, "rb")) == NULL) {
       // Debug("Cann't open the file!\n");
        exit(0);
    }

    // Set the file pointer from the beginning
    fseek(fp, 0, SEEK_SET);
    fread(&bmpfileheader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
    fread(&bmpinfoheader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 50
    printf("pixel = %d * %d\r\n", bmpinfoheader.biwidth, bmpinfoheader.biheight);
	
    uint32_t image_byte = bmpinfoheader.biwidth * bmpinfoheader.biheight * 3;
    uint8_t image[image_byte];
    memset(image, 0xFF, image_byte);

    // Determine if it is a monochrome bitmap
    int readbyte = bmpinfoheader.bibitcount;
    if(readbyte != 24){
       // Debug("Bmp image is not 24 bitmap!\n");
        exit(0);
    }
    // Read image data into the cache
    uint16_t dx, dy;
    uint8_t rdata[3];
    fseek(fp, bmpfileheader.boffset, SEEK_SET);
    
    for(dy = 0; dy < bmpinfoheader.biheight; dy++) {//Total display column
        for(dx = 0; dx < bmpinfoheader.biwidth ; dx++) {//Show a line in the line
            if(fread((char *)rdata, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
			if(fread((char *)rdata+1, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
			if(fread((char *)rdata+2, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }

			if(rdata[0] == 0 && rdata[1] == 0 && rdata[2] == 0){
				image[dx+(dy* bmpinfoheader.biwidth )] =  0;//Black
			}else if(rdata[0] == 255 && rdata[1] == 255 && rdata[2] == 255){
				image[dx+(dy* bmpinfoheader.biwidth )] =  1;//White
			}else if(rdata[0] == 0 && rdata[1] == 255 && rdata[2] == 0){
				image[dx+(dy* bmpinfoheader.biwidth )] =  2;//Green
			}else if(rdata[0] == 255 && rdata[1] == 0 && rdata[2] == 0){
				image[dx+(dy* bmpinfoheader.biwidth )] =  3;//Blue
			}else if(rdata[0] == 0 && rdata[1] == 0 && rdata[2] == 255){
				image[dx+(dy* bmpinfoheader.biwidth )] =  4;//Red
			}else if(rdata[0] == 0 && rdata[1] == 255 && rdata[2] == 255){
				image[dx+(dy* bmpinfoheader.biwidth )] =  5;//Yellow
			}else if(rdata[0] == 0 && rdata[1] == 128 && rdata[2] == 255){
				image[dx+(dy* bmpinfoheader.biwidth )] =  6;//Orange
			}
        }
    }
    fclose(fp);
   
    // Refresh the image to the display buffer based on the displayed orientation
    for(dy = 0; dy < bmpinfoheader.biheight; dy++) {
        for(dx = 0; dx < bmpinfoheader.biwidth; dx++) {
            if(dx > Image.w || dy > Image.h) {
                break;
            }
            Gui_SetPixel(x + dx, y + dy, image[bmpinfoheader.biheight *  bmpinfoheader.biwidth - 1 -(bmpinfoheader.biwidth-dx-1+(dy* bmpinfoheader.biwidth))]);
		}
    }
    return 0;
}
