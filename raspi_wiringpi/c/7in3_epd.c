#include <string.h>
#include <stdlib.h>	
#include <stdio.h>
#include <stdio.h>	
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include "7in3_epd.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "epd_gui.h"
//define pin conection in wiringpi number
/****************************
* PIN_CS      10 
* PIN_RST     0 
* PIN_DC      6
* PIN_BUSY    5
***************************/

void EPD_GPIO_Init(void)
{
	printf("Init gpio for wiringpi\r\n");
    if(wiringPiSetup() < 0)//use wiringpi Pin number
    { 
        printf("wiringPi lib init failed! \r\n");
    } 
    pinMode(PIN_CS, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    pinMode(PIN_DC, OUTPUT);
    pinMode(PIN_BUSY, INPUT);
    pullUpDnControl(PIN_BUSY, PUD_UP);
    
    digitalWrite(PIN_CS, 1);
    wiringPiSPISetup(0,8000000);  
}


/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_7IN3F_Init(void)
{
    digitalWrite(PIN_RST, 1);  
	delay_xms(10);
    digitalWrite(PIN_RST, 0);   
	delay_xms(10);//At least 10ms delay 
	digitalWrite(PIN_RST, 1);
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
    printf("e-Paper busy H\r\n");
	while(!digitalRead(PIN_BUSY)) {      //LOW: busy, HIGH: idle
        delay_xms(2);
    } 
    printf("e-Paper busy H  r\r\n");
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
    digitalWrite(PIN_DC, 0);   // D/C#   0:command  1:data
	digitalWrite(PIN_CS, 1); 
	digitalWrite(PIN_CS, 0); 
	
 
	wiringPiSPIDataRW(0,&cmd,1); 
    
//	Epaper_Spi_WriteByte(cmd);
	digitalWrite(PIN_CS, 1); 
  
}

void Epaper_Write_Data(unsigned char data)
{
    digitalWrite(PIN_DC, 1);   // D/C#   0:command  1:data
	digitalWrite(PIN_CS, 1); 
	digitalWrite(PIN_CS, 0); 
   
	
	wiringPiSPIDataRW(0,&data,1); 
     
//	Epaper_Spi_WriteByte(data);
	digitalWrite(PIN_CS, 1); 
   
}

void EPD_GPIO_Exit(void)
{

    digitalWrite(PIN_CS, 0);
    digitalWrite(PIN_DC, 0);
    digitalWrite(PIN_RST, 0);
    printf("Exit gpio \r\n");
    
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
