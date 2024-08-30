/***************************************************************************************
 * Project  :
 * Experimental Platform :Raspberry Pi 5 + 7.3inch E-Paper Display
 * Hardware Connection : 7.3inch E-Paper ->  Raspberry Pi 5  in BCM number
 * define pin conection in BCM number
7.3inch_EPD    Raspberry Pi
	CS   -->  CE0
	CLK  -->  SCK
	MOSI -->  MOSI
	DC   -->  D25
    BUSY -->  D24
    RST  -->  D17
    GND  -->  GND
    VCC  -->  3.3V 
    Web Site : www.seengreat.com
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "sg_lgpio.h"	
#include "7in3_epd.h"	
#include "epd_gui.h"
#include "fonts.h"
#include "color_conversion.h"
#include "led_key.h"
/******************************
 * Definition of screen coordinates:
 * 0------------> x (1~800) 
 * |
 * |
 * |
 * |
 * y (1~480)
 * 
********************************/
int	main(void)
{
	printf("7.3inch epaper \r\n");
	EPD_GPIO_Init(); //gpio port initialisation, using spi communication
    LED_KEY_Init();
    delay(2000);	
    EPD_7IN3F_Init();//Initialisation of EPD 
    delay(2000);
    printf("e-Paper Init successful...\r\n");	
    EPD_7IN3F_Clear(EPD_7IN3F_WHITE); //Clear the screen
    delay(2000);
    printf("e-Paper clear successful...\r\n");	

    uint8_t *BlackImage;

//Create a new image cache

    uint32_t Imagesize = ((EPD_7IN3F_WIDTH % 2 == 0)? (EPD_7IN3F_WIDTH / 2 ): (EPD_7IN3F_WIDTH / 2 + 1)) * EPD_7IN3F_HEIGHT;
    if((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL) {
        //Dynamically allocating memory space for BlackImage
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Image_Init(BlackImage, EPD_7IN3F_WIDTH, EPD_7IN3F_HEIGHT, 0, EPD_7IN3F_WHITE);//Create Image
    printf("Image_init successful\r\n");
    Gui_SetScale(7);//Set to 7-colour mode

#if 1   // Drawing on the image
    // 1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Gui_SelectImage(BlackImage);
    printf("Gui_SelectImage successful\r\n");
    Gui_Clear(EPD_7IN3F_WHITE);
    printf("Gui_Clear\r\n");
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Gui_Draw_Point(10, 80, EPD_7IN3F_RED, PIXEL_1X1, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 110, EPD_7IN3F_BLUE, PIXEL_2X2, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 140, EPD_7IN3F_GREEN, PIXEL_3X3, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 170, EPD_7IN3F_YELLOW, PIXEL_4X4, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 200, EPD_7IN3F_ORANGE, PIXEL_5X5, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 230, EPD_7IN3F_RED, PIXEL_6X6, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 260, EPD_7IN3F_BLUE, PIXEL_7X7, DOT_STYLE_DFT);
    Gui_Draw_Point(10, 290, EPD_7IN3F_GREEN, PIXEL_8X8, DOT_STYLE_DFT);
    
    Gui_Draw_Line(40, 170, 90, 220, EPD_7IN3F_ORANGE, PIXEL_1X1, SOLID);
    Gui_Draw_Line(90, 170, 40, 220, EPD_7IN3F_ORANGE, PIXEL_1X1, SOLID);
    
    Gui_Draw_Rectangle(40, 170, 90, 220, EPD_7IN3F_BLACK, EMPTY, PIXEL_3X3);
    Gui_Draw_Rectangle(40, 250, 90, 300, EPD_7IN3F_BLACK, FULL, PIXEL_1X1);
    Gui_Draw_Circle(45, 95, 20, EPD_7IN3F_BLACK, EMPTY, PIXEL_1X1);
    Gui_Draw_Circle(105, 95, 20, EPD_7IN3F_RED, FULL, PIXEL_1X1);
    
    Gui_Draw_Num(200, 150, 123456789, &Font8, EPD_7IN3F_WHITE, EPD_7IN3F_BLACK);
    Gui_Draw_Num(200, 180, 987654321, &Font12,EPD_7IN3F_BLACK, EPD_7IN3F_WHITE );
    Gui_Draw_Num(200, 210, 123456789, &Font16, EPD_7IN3F_WHITE, EPD_7IN3F_BLACK);
    Gui_Draw_Num(200, 240, 987654321, &Font20,EPD_7IN3F_BLACK, EPD_7IN3F_WHITE );
    Gui_Draw_Str_EN(200, 0, "Seengerat", &Font16, EPD_7IN3F_WHITE, EPD_7IN3F_BLACK);
    Gui_Draw_Str_EN(220, 20, "Seengerat" ,&Font16, EPD_7IN3F_WHITE,EPD_7IN3F_GREEN);
    Gui_Draw_Str_EN(240, 40, "Seengerat", &Font16, EPD_7IN3F_WHITE, EPD_7IN3F_BLUE);
    Gui_Draw_Str_EN(260, 60, "Seengerat", &Font16, EPD_7IN3F_WHITE, EPD_7IN3F_RED);
    Gui_Draw_Str_EN(280, 80, "Seengerat", &Font16, EPD_7IN3F_WHITE, EPD_7IN3F_YELLOW);
    Gui_Draw_Str_EN(300, 100, "Seengerat", &Font16, EPD_7IN3F_WHITE, EPD_7IN3F_ORANGE);
    Gui_Draw_Str_EN(400, 0, "Welcome to EPD", &Font20, EPD_7IN3F_WHITE, EPD_7IN3F_BLACK);
    Gui_Draw_Str_EN(415, 30, "Welcome to EPD", &Font20, EPD_7IN3F_GREEN, EPD_7IN3F_BLACK);
    Gui_Draw_Str_EN(430, 60, "Welcome to EPD", &Font20, EPD_7IN3F_BLUE, EPD_7IN3F_BLACK);
    Gui_Draw_Str_EN(445, 90, "Welcome to EPD", &Font24, EPD_7IN3F_RED, EPD_7IN3F_BLACK);
    Gui_Draw_Str_EN(460, 120, "Welcome to EPD", &Font24, EPD_7IN3F_YELLOW, EPD_7IN3F_BLACK);
    Gui_Draw_Str_EN(475, 150, "Welcome to EPD", &Font24, EPD_7IN3F_ORANGE, EPD_7IN3F_BLACK);
    Gui_Draw_Str_EN(490, 180, "Welcome to EPD", &Font24, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
    printf("EPD_Display\r\n");
    EPD_7IN3F_Display(BlackImage);
    delay(2000);
#endif

#if 1   // show image
    printf("show image-----------------\r\n");
    Gui_SelectImage(BlackImage);//Select Image
    printf("Gui_SelectImage successful\r\n");
    Gui_Clear(EPD_7IN3F_WHITE);//Clear the color of the picture

/*******************************
* Conversion( ) is an image conversion function.
If your image has been colour converted and has a pixel size of 800*480,
Please comment out the following five lines of code.
Output your image directly using GUI_ReadBmp_RGB_7Color()
****************************************************/

    const char* file_path = "./pic/b3.jpg";  //Path of the image to be imported
    const char* outputFilePath = "./pic/out1.bmp";//The path where the converted image is stored
    conversion(file_path, outputFilePath, EPD_7IN3F_WIDTH, EPD_7IN3F_HEIGHT, 0);//Convert image to 800*480 bmp format    
    delay(1000);
    GUI_ReadBmp_RGB_7Color("./pic/out1.bmp", 0, 0);//Read bmp files directly


    //GUI_ReadBmp_RGB_7Color("./pic/a8.bmp", 0, 0);

    EPD_7IN3F_Display(BlackImage);//Sends the image buffer in RAM to e-Paper and displays
    delay(3000);

#endif

    printf("Goto Sleep...\r\n");
    //EPD_7IN3F_Clear(EPD_7IN3F_WHITE); //Clear the screen
    EPD_7IN3F_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    delay(2000); // important, at least 2s
    //EPD_GPIO_Exit();
    printf("EPD Sleep!\r\n");
    Led_key_task();

}
