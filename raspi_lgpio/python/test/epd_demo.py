import os
from PIL import ImageDraw, Image, ImageFont
import time
from epd_7inch3 import *
from epd_gui import *
'''define pin conection in BCM number
7.3inch_EPD    Raspberry Pi
    CS   -->  CE0/D8
    CLK  -->  SCK/D11
    MOSI -->  MOSI/D10
    DC   -->  D25
    BUSY -->  D24
    RST  -->  D17
    GND  -->  GND
    VCC  -->  3.3V 
    Web Site : www.seengreat.com
'''
'''
 * Definition of screen coordinates:
 * 0------------> x (1~800) 
 * |
 * |
 * |
 * |
 * y (1~480)

'''

if __name__ == '__main__':
    
    picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')#Folders for storing pictures
    gui = EPD_GUI()

    gui.epd.hw_init() # EPD init
    print("epd init y")
    gui.epd.clear() # EPD Clear
    print("epd clear y")
    time.sleep(2)  # delay 2s
    # Drawing
    
    gui.clear(EPD_7IN3F_WHITE)#Clear image
    print("gui clear y")
    
    font_12 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_12)  # read chinese font file
    font_18 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_18)  # read chinese font file
    font_24 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_24)  # read chinese font file
    font_36 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_36)  # read chinese font file
    font_48 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_48)  # read chinese font file
    font_60 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_60)  # read chinese font file
    font_72 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_72)  # read chinese font file
    # Point

    gui.draw_point(10, 20, EPD_7IN3F_BLACK, PIXEL_1X1, DOT_STYLE_DFT)
    gui.draw_point(10, 40, EPD_7IN3F_RED, PIXEL_2X2, DOT_STYLE_DFT)
    gui.draw_point(10, 70, EPD_7IN3F_GREEN, PIXEL_3X3, DOT_STYLE_DFT)
    gui.draw_point(10, 100, EPD_7IN3F_BLUE, PIXEL_4X4, DOT_STYLE_DFT)
    gui.draw_point(10, 130, EPD_7IN3F_YELLOW, PIXEL_5X5, DOT_STYLE_DFT)
    gui.draw_point(10, 160, EPD_7IN3F_ORANGE, PIXEL_6X6, DOT_STYLE_DFT)
    gui.draw_point(10, 190, EPD_7IN3F_RED, PIXEL_7X7, DOT_STYLE_DFT)
    gui.draw_point(10, 230, EPD_7IN3F_GREEN, PIXEL_8X8, DOT_STYLE_DFT)
    # Line
    gui.draw_line(100, 10, 140, 50, EPD_7IN3F_GREEN, PIXEL_2X2, LINE_SOLID)
    gui.draw_line(100, 50, 140, 10, EPD_7IN3F_GREEN, PIXEL_1X1, LINE_DOTTED)
    # Rectangle
    gui.draw_rectangle(100, 10, 140, 50, EPD_7IN3F_ORANGE, FILL_EMPTY, PIXEL_2X2)
    gui.draw_rectangle(170, 10, 210, 50, EPD_7IN3F_BLUE, FILL_FULL, PIXEL_1X1)
    # Circle
    gui.draw_circle(600, 100, 50, EPD_7IN3F_RED, FILL_EMPTY, PIXEL_2X2)
    gui.draw_circle(600, 300, 50, EPD_7IN3F_RED, FILL_FULL, PIXEL_2X2)
    #char
    gui.draw_str(100, 60, "abcdefg", EPD_7IN3F_WHITE, EPD_7IN3F_YELLOW,FONT_SIZE_12, font_12)
    gui.draw_str(100, 80, "ABCDEFG", EPD_7IN3F_WHITE, EPD_7IN3F_RED,FONT_SIZE_18, font_18)
    gui.draw_str(100, 120, "1234567890.", EPD_7IN3F_WHITE, EPD_7IN3F_BLACK, FONT_SIZE_24, font_24)
    gui.draw_str(100, 170, "seengreat", EPD_7IN3F_WHITE, EPD_7IN3F_ORANGE,FONT_SIZE_36, font_36)
    gui.draw_str(100, 230, "seengreat", EPD_7IN3F_WHITE, EPD_7IN3F_BLUE,FONT_SIZE_48, font_48)
    gui.draw_str(100, 300, "SEENGREAT", EPD_7IN3F_GREEN,EPD_7IN3F_WHITE,FONT_SIZE_60, font_60)
    gui.draw_str(100, 400, "SEENGREAT", EPD_7IN3F_RED,EPD_7IN3F_WHITE,FONT_SIZE_72, font_72)

    gui.epd.display(gui.img)  # Sends the image buffer in RAM to e-Paper and displays
    print("display y")
    time.sleep(3)  # 3s
    
    gui.clear(EPD_7IN3F_WHITE)
    print("gui clear y")
    Himage = Image.open(os.path.join(picdir, 'b3.jpg'))#Open image path
    gui.epd.display(gui.epd.getbuffer(Himage,0))#display
    time.sleep(3)  # 3s

#     gui.epd.clear() # EPD Clear
    gui.epd.sleep()  # EPD_DeepSleep, Sleep instruction is necessary, please do not delete!!!
    gui.epd.gpio_exit()
    print("epd sleep")
    time.sleep(3)  # 3s


