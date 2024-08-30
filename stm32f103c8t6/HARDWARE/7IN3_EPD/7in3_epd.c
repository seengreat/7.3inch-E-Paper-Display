#include "7in3_epd.h"
#include "gpio.h"
#include "stdio.h"
void delay_xms(unsigned int xms)
{
	unsigned int i;
	while(xms--)
	{
		i=12000;
		while(i--);
	}
}

void EPD_W21_Init(void)
{
	EPD_W21_RST_1;
	delay_xms(10);//At least 10ms delay 
	EPD_W21_RST_0;		// Module reset
	delay_xms(10);//At least 10ms delay 
	EPD_W21_RST_1;
	delay_xms(10);//At least 10ms delay 
	
}

/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/

void EPD_init(void)
{	

	EPD_W21_Init();	//Electronic paper IC reset		
	
	//lcd_chkstatus();
	delay_xms(30);
  	EPD_W21_WriteCMD(0xAA);    // CMDH
	EPD_W21_WriteDATA(0x49);
	EPD_W21_WriteDATA(0x55);
	EPD_W21_WriteDATA(0x20);
	EPD_W21_WriteDATA(0x08);
	EPD_W21_WriteDATA(0x09);
	EPD_W21_WriteDATA(0x18);

	EPD_W21_WriteCMD(PWRR);
	EPD_W21_WriteDATA(0x3F);
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x32);
	EPD_W21_WriteDATA(0x2A);
	EPD_W21_WriteDATA(0x0E);
	EPD_W21_WriteDATA(0x2A);

	EPD_W21_WriteCMD(PSR);  
	EPD_W21_WriteDATA(0x5F);
	EPD_W21_WriteDATA(0x69);
  
	EPD_W21_WriteCMD(POFS);
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x54);
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x44); 

	EPD_W21_WriteCMD(BTST1);
	EPD_W21_WriteDATA(0x40);
	EPD_W21_WriteDATA(0x1F);
	EPD_W21_WriteDATA(0x1F);
	EPD_W21_WriteDATA(0x2C);
  
	EPD_W21_WriteCMD(BTST2);
	EPD_W21_WriteDATA(0x6F);
	EPD_W21_WriteDATA(0x1F);
	EPD_W21_WriteDATA(0x16);
	EPD_W21_WriteDATA(0x25);

	EPD_W21_WriteCMD(BTST3);
	EPD_W21_WriteDATA(0x6F);
	EPD_W21_WriteDATA(0x1F);
	EPD_W21_WriteDATA(0x1F);
	EPD_W21_WriteDATA(0x22);
 
	EPD_W21_WriteCMD(0x13);    // IPC
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x04);
  
	EPD_W21_WriteCMD(PLL);
	EPD_W21_WriteDATA(0x02);
  
	EPD_W21_WriteCMD(0x41);     // TSE
	EPD_W21_WriteDATA(0x00);
  
	EPD_W21_WriteCMD(CDI);
	EPD_W21_WriteDATA(0x3F);
  
	EPD_W21_WriteCMD(TCON);
	EPD_W21_WriteDATA(0x02);
	EPD_W21_WriteDATA(0x00);
  
	EPD_W21_WriteCMD(TRES);
	EPD_W21_WriteDATA(0x03);
	EPD_W21_WriteDATA(0x20);
	EPD_W21_WriteDATA(0x01); 
	EPD_W21_WriteDATA(0xE0);
  
	EPD_W21_WriteCMD(VDCS);
	EPD_W21_WriteDATA(0x1E); 

	EPD_W21_WriteCMD(T_VDCS);
	EPD_W21_WriteDATA(0x00);

	EPD_W21_WriteCMD(0x86);    // AGID
	EPD_W21_WriteDATA(0x00);
  
	EPD_W21_WriteCMD(PWS);
	EPD_W21_WriteDATA(0x2F);
 
	EPD_W21_WriteCMD(0xE0);   // CCSET
	EPD_W21_WriteDATA(0x00); 
  
	EPD_W21_WriteCMD(0xE6);   // TSSET
	EPD_W21_WriteDATA(0x00);	

	EPD_W21_WriteCMD(0x04);			//PWR on 	
	//lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal
}

/******************************************************************************
function :  Clear screen
parameter:
******************************************************************************/

void EPD_sleep(void)
{
   // EPD_W21_WriteCMD(0X02);  	//power off
	 // EPD_W21_WriteDATA(0x00);
	 // lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal
		EPD_W21_WriteCMD(0X07);  	//deep sleep
		EPD_W21_WriteDATA(0xA5);
}


/******************************************************************************
function :  Sends the image buffer in SRAM to e-Paper and displays
parameter:
		image    : Select the image to be displayed
		xstart   : Starting Xpoint point coordinates
    ystart   : Starting Ypoint point coordinates
    image_width   :Setting the image width
    image_heigh   :Setting the image height

    
******************************************************************************/
void EPD_7IN3F_Display( const unsigned char *Image, uint16_t xstart, uint16_t ystart, uint16_t image_width, uint16_t image_heigh)
{
	unsigned long i, j;
	printf("Please be patient ....  \r\n");
	EPD_W21_WriteCMD(0x10);
	for(i=0; i<480; i++) {
		for(j=0; j<400; j++) {
			if(i<image_heigh+ystart && i>=ystart && j<(image_width+xstart)/2 && j>=xstart/2) {
				EPD_W21_WriteDATA(Image[(j-xstart/2) + (image_width/2*(i-ystart))]);
			}
			else {
				EPD_W21_WriteDATA(0x11);
			}
		}
	}
	EPD_W21_WriteCMD(PON);
		delay_xms(5000);     
		EPD_W21_WriteCMD(DRF);
		EPD_W21_WriteDATA(0x00);
		lcd_chkstatus();   
		EPD_W21_WriteCMD(POF);  
		EPD_W21_WriteDATA(0x00);
		delay_xms(5000);  
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

void EPD_7IN3F_DisplayPart(unsigned char *Image, uint16_t xstart, uint16_t ystart, uint16_t image_width, uint16_t image_heigh)
{
	unsigned long i, j;
	printf("Please be patient ....  \r\n");
	EPD_W21_WriteCMD(0x10);
	for(i=0; i<480; i++) {
		for(j=0; j<400; j++) {
			if(i<image_heigh+ystart && i>=ystart && j<(image_width+xstart)/2 && j>=xstart/2) {
				EPD_W21_WriteDATA(Image[(j-xstart/2) + (image_width/2*(i-ystart))]);
			}
			else {
				EPD_W21_WriteDATA(0x11);
			}
		}
	}
		EPD_W21_WriteCMD(PON);
		
		delay_xms(5000);    
		EPD_W21_WriteCMD(DRF);
		EPD_W21_WriteDATA(0x00);
		lcd_chkstatus();   
		EPD_W21_WriteCMD(POF);  
		EPD_W21_WriteDATA(0x00);
		delay_xms(5000);  
}
/******************************************************************************
function :  A horizontal colour display routine
parameter:
******************************************************************************/
void EPD_horizontal(void)
{
  unsigned int i,j;
  unsigned char index = 0x00;
  unsigned char const Color[8] = {EPD_7IN3F_BLACK,EPD_7IN3F_WHITE,EPD_7IN3F_GREEN,EPD_7IN3F_BLUE,EPD_7IN3F_RED,EPD_7IN3F_YELLOW,EPD_7IN3F_ORANGE,EPD_7IN3F_WHITE};

	EPD_W21_WriteCMD(0x10);	     //start to transport picture
  for(i=0;i<480;i++){
    if((i%30 == 0) && (i != 0))
      index ++;
    for(j =0;j<800/2;j++){
      EPD_W21_WriteDATA((Color[index]<<4)|Color[index]); 
    }
  }
		EPD_W21_WriteCMD(PON);
		delay_xms(5000);     
		EPD_W21_WriteCMD(DRF);
		EPD_W21_WriteDATA(0x00);
		lcd_chkstatus();   
		EPD_W21_WriteCMD(POF);  
		EPD_W21_WriteDATA(0x00);
		delay_xms(5000);        //waiting for the electronic paper IC to release the idle signal

}

/******************************************************************************
function :  A vertical colour display routine
parameter:
******************************************************************************/
void EPD_vertical(void)
{
  unsigned int i,j,k;
  unsigned char const Color[8] = {EPD_7IN3F_BLACK,EPD_7IN3F_WHITE,EPD_7IN3F_GREEN,EPD_7IN3F_BLUE,EPD_7IN3F_RED,EPD_7IN3F_YELLOW,EPD_7IN3F_ORANGE,EPD_7IN3F_WHITE};
	
  // Acep_color(White); //Each refresh must be cleaned first	 
	EPD_W21_WriteCMD(0x10);	     //start to transport pictu
  for(i=0;i<480;i++)
	{
    for(k = 0 ; k < 7; k ++)  //7 color      400/8=50  
		{
      for(j = 0 ; j < 50; j ++)
			{
        EPD_W21_WriteDATA(Color[k]);
      }
    }
    for(j = 0; j <50 ; j++) 
		{
      EPD_W21_WriteDATA(EPD_7IN3F_WHITE);
    }
  }
		EPD_W21_WriteCMD(PON);
		delay_xms(5000);    
		EPD_W21_WriteCMD(DRF);
		EPD_W21_WriteDATA(0x00);
		lcd_chkstatus();   
		EPD_W21_WriteCMD(POF);  
		EPD_W21_WriteDATA(0x00);
		delay_xms(5000);        //waiting for the electronic paper IC to release the idle signal

}





/******************************************************************************
function :  A seven colour display routine
parameter:
******************************************************************************/
void Display_color(void)  
{
  unsigned long i,j;
  
  EPD_W21_WriteCMD(0x04);
	printf("Please be patient .... \r\n");
  lcd_chkstatus();
  EPD_W21_WriteCMD(0x10);
  for(i=0;i<240;i++)
  {

	for(j=0;j<100;j++)
	{
		EPD_W21_WriteDATA(0x00);					
	}
	for(j=0;j<100;j++)
	{
		EPD_W21_WriteDATA(0x33);				
	}
	for(j=0;j<100;j++)
	{
		EPD_W21_WriteDATA(0x22);	
	}
	for(j=0;j<100;j++)
	{
		EPD_W21_WriteDATA(0x66);			
	}	

	
  }
	
  for(i=0;i<240;i++)
  {
	for(j=0;j<100;j++)
	{
		EPD_W21_WriteDATA(0x44);			
	}
	for(j=0;j<100;j++)
	{
		EPD_W21_WriteDATA(0x55);				
	}
	for(j=0;j<100;j++)
	{
		EPD_W21_WriteDATA(0x11);	
	}
	for(j=0;j<100;j++)
	{
		EPD_W21_WriteDATA(0x11);					
	}						
  }	
  EPD_W21_WriteCMD(0x06);
  EPD_W21_WriteDATA(0x6F);
  EPD_W21_WriteDATA(0x26);
  EPD_W21_WriteDATA(0x36);
  EPD_W21_WriteDATA(0x44);
  
  EPD_W21_WriteCMD(0x12);
  EPD_W21_WriteDATA(0x00);
	printf("Please be patient .... \r\n");
  lcd_chkstatus();
  //EPD_W21_WriteCMD(0x02);
  //EPD_W21_WriteDATA(0x00);
  
   for(i=0;i<8;i++)
  {
        delay_xms(1000);
  }
  EPD_W21_Init();
  //=============================================
  EPD_W21_WriteCMD(0x06);
  EPD_W21_WriteDATA(0x6F);
  EPD_W21_WriteDATA(0x1F);
  EPD_W21_WriteDATA(0x15);
  EPD_W21_WriteDATA(0x15);
}






void EPD_7IN3F_Clear(uint8_t color)
{
	
    uint16_t width, height;
		uint16_t i,j;
		printf("Please be patient .... \r\n");
    width = (800 % 2 == 0)? (800 / 2 ): (800 / 2 + 1);
    height = 480;
    EPD_W21_WriteCMD(0x10);
    for ( j = 0; j < height; j++) {
        for ( i = 0; i < width; i++) {
            EPD_W21_WriteDATA((color<<4)|color);
        }
   
    }
 
		EPD_W21_WriteCMD(PON);
		delay_xms(5000);   
		EPD_W21_WriteCMD(DRF);
		EPD_W21_WriteDATA(0x00);
		lcd_chkstatus();   
		EPD_W21_WriteCMD(POF);  
		EPD_W21_WriteDATA(0x00);
		delay_xms(40);
}


/******************************************************************************
function :  Wait until the busy_pin goes LOW
parameter:
******************************************************************************/

void lcd_chkstatus(void)
{/*
	printf("busy h\r\n");
	while(!isEPD_W21_BUSY)
	{printf("busy=low\r\n");
		delay_xms(2);//At least 10ms delay 
	}*/
	printf("Please be patient .... \r\n");
	printf("busy h ....\r\n");
	delay_xms(50000);
}














/***********************************************************
						end file
***********************************************************/
