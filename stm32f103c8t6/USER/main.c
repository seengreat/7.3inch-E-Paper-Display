/***************************************************************************************
 * Project  :
 * Experimental Platform :STM32F103C8T6 + 7.3inch E-Paper Display
 * Hardware Connection :STM32F103 -> 7.3inch E-Paper
 *			PB12 --> CS 
 *			PB13 --> CLK
 *			PB15 --> MOSI
 *			PA8  --> DC
 *      PA12 --> BUSY
 *      PA11 --> RST
 *      GND  --> GND
 *      3.3V --> VCC
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


#include "sys.h"
#include "usart.h"
#include "gpio.h"
#include "delay.h"
#include "7in3_epd.h"
#include "timer.h"
#include "epd_gui.h"
#include "fonts.h"
#include "image.h"
#include <stdlib.h> // malloc() free()
unsigned char BlackImage[19500];//Define canvas space 


void delayms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;
      while(i--) ;    
   }
}

int main()
{	
	delay_init();	    
	SystemInit();
	Uart1Init(115200,0,0);
	delayms(100);
	EPD_GPIO_Init();//gpio port initialisation, using spi communication
	while(1)
	{

		delay_ms(100);
		EPD_init(); //Initialisation of EPD 
		printf("e-Paper  Init successful...\r\n");
		delayms(100);
		
		//EPD_7IN3F_Clear(EPD_7IN3F_YELLOW);
		
		EPD_7IN3F_Clear(EPD_7IN3F_WHITE);//Clear the screen
		delayms(400);
		printf("e-Paper clear successful...\r\n");
	
    Image_Init(BlackImage, 250, 150, 0, EPD_7IN3F_WHITE);//Create Image
    Gui_SetScale(7);
		
		    // Drawing on the image
    // 1.Select Image
    Gui_SelectImage(BlackImage);
    Gui_Clear(EPD_7IN3F_WHITE);//Clear the color of the picture
		printf("Gui_Clear\r\n");
		
		
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Gui_Draw_Point(10, 10, EPD_7IN3F_RED, PIXEL_1X1, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 20, EPD_7IN3F_BLUE, PIXEL_2X2, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 30, EPD_7IN3F_GREEN, PIXEL_3X3, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 40, EPD_7IN3F_YELLOW, PIXEL_4X4, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 50, EPD_7IN3F_ORANGE, PIXEL_5X5, DOT_STYLE_DFT);
		Gui_Draw_Rectangle(20, 20, 50, 50, EPD_7IN3F_BLACK, EMPTY, PIXEL_1X1);
    //Gui_Draw_Rectangle(80, 20, 110, 50, EPD_7IN3F_BLACK, FULL, PIXEL_1X1);
    Gui_Draw_Circle(35, 35, 20, EPD_7IN3F_BLACK, EMPTY, PIXEL_1X1);
   // Gui_Draw_Circle(95, 35, 20, EPD_7IN3F_WHITE, FULL, PIXEL_1X1);
	//Gui_Draw_Num(20, 20, 123456789, &Font8, EPD_7IN3F_WHITE, EPD_7IN3F_BLACK);
   // Gui_Draw_Num(20, 40, 987654321, &Font8,EPD_7IN3F_BLACK, EPD_7IN3F_WHITE );
    Gui_Draw_Str_EN(20, 60, "Seengerat", &Font16, EPD_7IN3F_WHITE, EPD_7IN3F_BLACK);
    Gui_Draw_Str_EN(20, 90, "Seengerat" ,&Font20, EPD_7IN3F_WHITE,EPD_7IN3F_GREEN);
    Gui_Draw_Str_EN(20, 120, "Seengerat", &Font24, EPD_7IN3F_WHITE, EPD_7IN3F_BLUE);
		Gui_Draw_Str_EN(100, 0, "Welcome", &Font24, EPD_7IN3F_WHITE, EPD_7IN3F_YELLOW);
    Gui_Draw_Str_EN(100, 30, "Welcome", &Font24, EPD_7IN3F_RED, EPD_7IN3F_YELLOW);
    Gui_Draw_Str_EN(100, 60, "Welcome", &Font24,  EPD_7IN3F_ORANGE, EPD_7IN3F_YELLOW);
		EPD_7IN3F_DisplayPart(BlackImage,0 , 0, 250, 150);//Displaying images on an ink screen
		printf("display successful...\r\n");
		delayms(2000); 
		delayms(2000); 
	//EPD_vertical();
	//EPD_horizontal();
	//Display_color();
	//EPD_7IN3F_Clear(EPD_7IN3F_RED );
	EPD_7IN3F_Clear(EPD_7IN3F_WHITE);//Clear the screen
	
  delayms(400);	
		
	EPD_7IN3F_Display(gImage_7in3f, 250, 150, 300, 160);////Displaying images on an ink screen
	printf("display successful...\r\n");
	delayms(2000); 
	delayms(2000); 
		
	
//	EPD_7IN3F_Clear(EPD_7IN3F_WHITE);//Clear the screen
	printf("e-Paper clear successful...\r\n");
	delayms(400);
	EPD_sleep();//
	printf("epd Sleep...\r\n");
	delayms(2000); 
	while(1);
		
			//printf("while 1...\r\n");
		// The program stops here							
	}

}			

