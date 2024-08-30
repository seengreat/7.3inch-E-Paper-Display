# Seengreat 7.3 Inch E-Paper Display demo
# Author(s):Andy Li from Seengreat

import os
import sys
import spidev
import time
import wiringpi as wpi
import numpy as np
import PIL
from PIL import Image
"""the following pin definiting use wiringpi"""
PIN_CS     = 10
PIN_DC     = 6
PIN_BUSY   = 5
PIN_RST    = 0

EPD_WIDTH  = 800
EPD_HEIGHT = 480

'''
Color Index
'''
EPD_7IN3F_BLACK   = 0x0  # 000
EPD_7IN3F_WHITE   = 0x1  # 001
EPD_7IN3F_GREEN   = 0x2  # 010
EPD_7IN3F_BLUE    = 0x3  # 011
EPD_7IN3F_RED     = 0x4  # 100
EPD_7IN3F_YELLOW  = 0x5  # 101
EPD_7IN3F_ORANGE  = 0x6  # 110
EPD_7IN3F_CLEAN   = 0x7  # 111   unavailable  Afterimage

class EPD_7Inch3():
    def __init__(self):
        # gpio init
        wpi.wiringPiSetup()
        wpi.pinMode(PIN_DC, wpi.OUTPUT)  # D/C pin
        wpi.pinMode(PIN_RST, wpi.OUTPUT)  # reset pin
        wpi.pinMode(PIN_CS, wpi.OUTPUT)  # cs pin
        wpi.pinMode(PIN_BUSY, wpi.INPUT)  # BUSY pin
        wpi.pullUpDnControl(PIN_BUSY, wpi.PUD_DOWN)
        # spi init
        self.bus = 0
        self.dev = 0
        self.spi_speed = 8000000
        self.spi = spidev.SpiDev()
        self.spi.open(self.bus, self.dev)
        self.spi.max_speed_hz = self.spi_speed
        self.spi.mode = 0b00

        self.w = 800
        self.h = 480
        self.BLACK  = 0x000000   #   0000  BGR
        self.WHITE  = 0xffffff   #   0001
        self.GREEN  = 0x00ff00   #   0010
        self.BLUE   = 0xff0000   #   0011
        self.RED    = 0x0000ff   #   0100
        self.YELLOW = 0x00ffff   #   0101
        self.ORANGE = 0x0080ff   #   0110
    def write_cmd(self, cmd):
        """write command"""
        wpi.digitalWrite(PIN_DC,wpi.LOW)
        wpi.digitalWrite(PIN_CS,wpi.LOW)
        self.spi.writebytes([cmd])
        wpi.digitalWrite(PIN_CS,wpi.HIGH)
        
    def write_data(self, value):
        """write data"""
        wpi.digitalWrite(PIN_DC,wpi.HIGH)
        wpi.digitalWrite(PIN_CS,wpi.LOW)
        self.spi.writebytes([value])
        wpi.digitalWrite(PIN_CS,wpi.HIGH)

    def write_data2(self, value):
        """write data"""
        wpi.digitalWrite(PIN_DC,wpi.HIGH)
        wpi.digitalWrite(PIN_CS,wpi.LOW)
        self.spi.writebytes2(value)
        wpi.digitalWrite(PIN_CS,wpi.HIGH)

    def chkstatus(self):
        while (wpi.digitalRead(PIN_BUSY)==wpi.LOW):
            time.sleep(0.01)
        
    def reset(self):
        """reset the epd"""
        wpi.digitalWrite(PIN_RST,wpi.LOW)
        time.sleep(0.1)
        wpi.digitalWrite(PIN_RST,wpi.HIGH)
        time.sleep(0.1)
    def gpio_exit(self):
        wpi.digitalWrite(PIN_CS,wpi.LOW)
        wpi.digitalWrite(PIN_DC,wpi.LOW)
        wpi.digitalWrite(PIN_RST,wpi.LOW)   
    
    def hw_init(self):
        """epd init..."""
        self.reset()
        self.chkstatus()
        
        self.write_cmd(0xAA)    # CMDH
        self.write_data(0x49)
        self.write_data(0x55)
        self.write_data(0x20)
        self.write_data(0x08)
        self.write_data(0x09)
        self.write_data(0x18)

        self.write_cmd(0x01)
        self.write_data(0x3F)
        self.write_data(0x00)
        self.write_data(0x32)
        self.write_data(0x2A)
        self.write_data(0x0E)
        self.write_data(0x2A)

        self.write_cmd(0x00)
        self.write_data(0x5F)
        self.write_data(0x69)

        self.write_cmd(0x03)
        self.write_data(0x00)
        self.write_data(0x54)
        self.write_data(0x00)
        self.write_data(0x44) 

        self.write_cmd(0x05)
        self.write_data(0x40)
        self.write_data(0x1F)
        self.write_data(0x1F)
        self.write_data(0x2C)

        self.write_cmd(0x06)
        self.write_data(0x6F)
        self.write_data(0x1F)
        self.write_data(0x1F)
        self.write_data(0x22)

        self.write_cmd(0x08)
        self.write_data(0x6F)
        self.write_data(0x1F)
        self.write_data(0x1F)
        self.write_data(0x22)

        self.write_cmd(0x13)    # IPC
        self.write_data(0x00)
        self.write_data(0x04)

        self.write_cmd(0x30)
        self.write_data(0x3C)

        self.write_cmd(0x41)     # TSE
        self.write_data(0x00)

        self.write_cmd(0x50)
        self.write_data(0x3F)

        self.write_cmd(0x60)
        self.write_data(0x02)
        self.write_data(0x00)

        self.write_cmd(0x61)
        self.write_data(0x03)
        self.write_data(0x20)
        self.write_data(0x01) 
        self.write_data(0xE0)

        self.write_cmd(0x82)
        self.write_data(0x1E) 

        self.write_cmd(0x84)
        self.write_data(0x00)

        self.write_cmd(0x86)    # AGID
        self.write_data(0x00)

        self.write_cmd(0xE3)
        self.write_data(0x2F)

        self.write_cmd(0xE0)   # CCSET
        self.write_data(0x00) 

        self.write_cmd(0xE6)   # TSSET
        self.write_data(0x00)
        return 0

    def TurnOnDisplay(self):
        self.write_cmd(0x04) # POWER_ON
        self.chkstatus()

        self.write_cmd(0x12) # DISPLAY_REFRESH
        self.write_data(0X00)
        self.chkstatus()
        
        self.write_cmd(0x02) # POWER_OFF
        self.write_data(0X00)
        self.chkstatus()

    # display
    def clear(self, color=0x11):
        self.write_cmd(0x10)
        self.write_data2([color] * int(self.h) * int(self.w/2))

        self.TurnOnDisplay()

    def sleep(self):
        self.write_cmd(0x07)
        self.write_data(0xA5)
        time.sleep(2)

    def display(self, image):
        if self.w%2 == 0:
            width = self.w//2
        else:
            width = self.w//2+1
#         width = (self.w % 8 == 0)?(self.w/8):(self.w / 8 + 1)
        height = self.h

        self.write_cmd(0x10)
        for j in range(height):
            for i in range(width):
               self.write_data(image[i + j * width])
        self.TurnOnDisplay()
        
    
    def getbuffer(self,image,rotate):
        # Set the target pixel size directly
        target_width = self.w
        target_height = self.h

        # Create a palette with the 7 colors supported by the panel
        pal_image = Image.new("P", (1,1))
        pal_image.putpalette((0,0,0,  255,255,255,  0,255,0,   0,0,255,  255,0,0,  255,255,0, 255,128,0) + (0,0,0)*249)

        # Rotate the image based on the input value
        if rotate == 90:
            image = image.rotate(90, expand=True)
        elif rotate == 180:
            image = image.rotate(180, expand=True)
        elif rotate == 270:
            image = image.rotate(270, expand=True)
        # Compare image dimensions with target dimensions
        imwidth, imheight = image.size
        if imwidth == target_width and imheight == target_height:
            resized_image = image
        else:
            # Calculate the aspect ratio of the image
            aspect_ratio = imwidth / imheight

            # Calculate the new dimensions while maintaining the aspect ratio
            if imwidth > imheight:
                new_width = target_width
                new_height = int(target_width / aspect_ratio)
            else:
                new_width = int(target_height * aspect_ratio)
                new_height = target_height

            # Resize the image while maintaining the aspect ratio
            resized_image = image.resize((new_width, new_height))

            # Create a new blank image with the target dimensions
            blank_image = Image.new("RGB", (target_width, target_height), color=(255, 255, 255))

            # Calculate the position to paste the resized image in the center
            paste_x = (target_width - new_width) // 2
            paste_y = (target_height - new_height) // 2

            # Paste the resized image onto the blank image
            blank_image.paste(resized_image, (paste_x, paste_y))

            resized_image = blank_image

        # Convert the resized image to the 7 colors, dithering if needed

        
        image_7color = resized_image.convert("RGB").quantize(palette=pal_image)
        buf_7color = bytearray(image_7color.tobytes('raw'))
        
        # PIL does not support 4-bit color, so pack the 4 bits of color
        # into a single byte to transfer to the panel
        buf = [0x00] * int(target_width * target_height / 2)
        idx = 0
        for i in range(0, len(buf_7color), 2):
            buf[idx] = (buf_7color[i] << 4) + buf_7color[i+1]
            idx += 1

        return buf