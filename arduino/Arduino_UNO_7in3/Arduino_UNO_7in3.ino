/***************************************************************************************
 * Project  :
 * Experimental Platform :Arduino UNO + 7.3inch E-Paper Display
 * Hardware Connection :Arduino UNO-> 7.3inch E-Paper
 *      D11 --> CS
 *      D12 --> CLK
 *      D13 --> MOSI
 *      D10 --> DC
 *      D8  --> BUSY
 *      D9  --> RST
 *      GND --> GND
 *      5V  --> VCC
 * Web Site    :www.seengreat.com
***************************************************************************************/

/******************************
 * Definition of screen coordinates:
 * 0------------> x (1~800) 
 * |
 * |
 * |
 * |
 * y (1~480)
******************************/


#include "image.h"
#include <avr/pgmspace.h>

//IO settings
int BUSY_Pin = 8; //
int RES_Pin = 9; //
int DC_Pin = 10; //
int CS_Pin = 11; //
int SCK_Pin = 12; 
int SDI_Pin = 13; 

//#define TEST_0  digitalWrite(TEST_Pin,LOW)
//#define TEST_1  digitalWrite(TEST_Pin,HIGH) 

#define EPD_W21_MOSI_0  digitalWrite(SDI_Pin,LOW)
#define EPD_W21_MOSI_1  digitalWrite(SDI_Pin,HIGH) 
#define EPD_W21_CLK_0 digitalWrite(SCK_Pin,LOW)
#define EPD_W21_CLK_1 digitalWrite(SCK_Pin,HIGH)
#define EPD_W21_CS_0 digitalWrite(CS_Pin,LOW)
#define EPD_W21_CS_1 digitalWrite(CS_Pin,HIGH)
#define EPD_W21_DC_0  digitalWrite(DC_Pin,LOW)
#define EPD_W21_DC_1  digitalWrite(DC_Pin,HIGH)
#define EPD_W21_RST_0 digitalWrite(RES_Pin,LOW)
#define EPD_W21_RST_1 digitalWrite(RES_Pin,HIGH)
#define isEPD_W21_BUSY digitalRead(BUSY_Pin)
#define EPD_WIDTH       800
#define EPD_HEIGHT      480

/**********************************
Color Index
**********************************/
#define EPD_7IN3F_BLACK   0x0	/// 000
#define EPD_7IN3F_WHITE   0x1	///	001
#define EPD_7IN3F_GREEN   0x2	///	010
#define EPD_7IN3F_BLUE    0x3	///	011
#define EPD_7IN3F_RED     0x4	///	100
#define EPD_7IN3F_YELLOW  0x5	///	101
#define EPD_7IN3F_ORANGE  0x6	///	110
#define EPD_7IN3F_CLEAN   0x7	///	111   unavailable  Afterimage

////////FUNCTION//////
void driver_delay_us(unsigned int xus);
void driver_delay_xms(unsigned long xms);
void DELAY_S(unsigned int delaytime);     
void SPI_Delay(unsigned char xrate);
void SPI_Write(unsigned char value);
void Epaper_Write_Command(unsigned char command);
void Epaper_Write_Data(unsigned char command);
void Epaper_READBUSY(void);
//EPD
void EPD_Init(void); //Electronic paper initialization
void EPD_Clear(unsigned char color);
void EPD_DeepSleep(void);
//Display 
void TurnOnDisplay(void);
void EPD_7IN3F_Display(const unsigned char *image, unsigned int x, unsigned int y, 
                                        unsigned int image_w, unsigned int image_h);


void setup() {
   pinMode(BUSY_Pin, INPUT); 
   pinMode(RES_Pin, OUTPUT);  
   pinMode(DC_Pin, OUTPUT);    
   pinMode(CS_Pin, OUTPUT);    
   pinMode(SCK_Pin, OUTPUT);    
   pinMode(SDI_Pin, OUTPUT); 
}
//Tips//
/*
1.When the e-paper is refreshed in full screen, the picture flicker is a normal phenomenon, and the main function is to clear the display afterimage in the previous picture.
2.After the e-paper is refreshed, you need to put it into sleep mode, please do not delete the sleep command.
3.Please do not take out the electronic paper when power is on.
4.Wake up from sleep, need to re-initialize the e-paper.
5.When you need to transplant the driver, you only need to change the corresponding IO. The BUSY pin is the input mode and the others are the output mode.
*/
void loop() {
    Serial.begin(115200);
    Serial.print("EPD init\r\n ");
    EPD_Init(); //Electronic paper initialization
    Serial.print("EPD Clear\r\n ");
    Serial.print("Please be patient for 25 seconds.\r\n ");
    EPD_Clear(EPD_7IN3F_WHITE); //Refresh the picture in full screen need 25s+30s
    delay(200);
    Serial.print("Show pic\r\n ");
    Serial.print("Please be patient for 25 seconds.\r\n "); 
    EPD_7IN3F_Display(gImage_7in3f, 250, 150, 300, 180);//need 25s+30s
    delay(2000);
    Serial.print("EPD Clear\r\n ");
    Serial.print("Please be patient for 25 seconds.\r\n ");
    EPD_Clear(EPD_7IN3F_WHITE); 
    delay(200);
    Serial.print("EPD sleep\r\n "); 
    EPD_DeepSleep();  //Sleep
    while(1); 
}

///////////////////EXTERNAL FUNCTION////////////////////////////////////////////////////////////////////////
/////////////////////delay//////////////////////////////////////
void driver_delay_us(unsigned int xus)  //1us
{
  for(;xus>1;xus--);
}
void driver_delay_xms(unsigned long xms) //1ms
{  
    unsigned long i = 0 , j=0;

    for(j=0;j<xms;j++)
  {
        for(i=0; i<256; i++);
    }
}
void DELAY_S(unsigned int delaytime)     
{
  int i,j,k;
  for(i=0;i<delaytime;i++)
  {
    for(j=0;j<4000;j++)           
    {
      for(k=0;k<222;k++);
                
    }
  }
}
//////////////////////SPI///////////////////////////////////
void SPI_Delay(unsigned char xrate)
{
  unsigned char i;
  while(xrate)
  {
    for(i=0;i<2;i++);
    xrate--;
  }
}

void SPI_Write(unsigned char value)                                    
{                                                           
    unsigned char i;  
   SPI_Delay(1);
    for(i=0; i<8; i++)   
    {
        EPD_W21_CLK_0;
       SPI_Delay(1);
       if(value & 0x80)
          EPD_W21_MOSI_1;
        else
          EPD_W21_MOSI_0;   
        value = (value << 1); 
       SPI_Delay(1);
       driver_delay_us(1);
        EPD_W21_CLK_1; 
        SPI_Delay(1);
    }
}

void Epaper_Write_Command(unsigned char command)
{
  SPI_Delay(1);
  EPD_W21_CS_0;                   
  EPD_W21_DC_0;   // command write
  SPI_Write(command);
  EPD_W21_CS_1;
}
void Epaper_Write_Data(unsigned char command)
{
  SPI_Delay(1);
  EPD_W21_CS_0;                   
  EPD_W21_DC_1;   // command write
  SPI_Write(command);
  EPD_W21_CS_1;
}

/////////////////EPD settings Functions/////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
void EPD_Init(void)
{
  EPD_W21_RST_1; //hard reset  
  delay(10); 
  EPD_W21_RST_0;     
  delay(10); 
  EPD_W21_RST_1; //hard reset  
  delay(10); 

  Epaper_READBUSY();


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
//////////////////////////////All screen update////////////////////////////////////////////
void EPD_Clear(unsigned char color) {
    Epaper_Write_Command(0x10);
    for(int i=0; i<EPD_WIDTH/2; i++) {
        for(int j=0; j<EPD_HEIGHT; j++) {
          Epaper_Write_Data((color<<4)|color);
		}
	}
    
    TurnOnDisplay();
}

/******************************************************************************
function :  Sends the image buffer in Flash to e-Paper and displays
parameter:
	image    : Select the image to be displayed
	xstart   : Starting Xpoint point coordinates
    ystart   : Starting Ypoint point coordinates
    image_width   :Setting the image width
    image_heigh   :Setting the image height

    
******************************************************************************/
void EPD_7IN3F_Display(const unsigned char *image, unsigned int x, unsigned int y,
unsigned int image_w, unsigned int image_h)
{
unsigned long di,dj;

Epaper_Write_Command(0x10);
for(di=0; di<EPD_HEIGHT; di++) {
    for(dj=0; dj< EPD_WIDTH/2; dj++) {
        if(di<image_h+y && di>=y && dj<(image_w+x)/2 && dj>=x/2) {
          Epaper_Write_Data(pgm_read_byte(&image[(dj-x/2) + (image_w/2*(di-y))]));
        }
    else {
      Epaper_Write_Data(0x11);
    }
  }
}

TurnOnDisplay();
}


void TurnOnDisplay(void) {
    Epaper_Write_Command(0x04);  // POWER_ON
    Epaper_READBUSY();
    
    Epaper_Write_Command(0x12);  // DISPLAY_REFRESH
    Epaper_Write_Data(0x00);
    Epaper_READBUSY();
    
    Epaper_Write_Command(0x02);  // POWER_OFF
    Epaper_Write_Data(0x00);
    Epaper_READBUSY();
}

void EPD_DeepSleep(void)
{  
  Epaper_Write_Command(0x07);
  Epaper_Write_Data(0xA5);
  delay(10);
	EPD_W21_RST_0; // Reset
  delay(10);
}
void Epaper_READBUSY(void)
{ 
  while(!isEPD_W21_BUSY)
  {   
    delay(2);
  }  
}



//////////////////////////////////END//////////////////////////////////////////////////
