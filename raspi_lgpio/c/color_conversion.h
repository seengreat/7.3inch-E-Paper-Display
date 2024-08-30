#ifndef __COLOR_CONVERSION_H_
#define __COLOR_CONVERSION_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <FreeImage.h>
#include <string.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} RGB;


RGB getNearestColor(RGB color);

void ditherImage(RGB* image, int width, int height);
void ditherImage2(RGB* image, int width, int height);
FIBITMAP* image_conversion(const char* file_path, int target_width, int target_height, int rotation_angle);
void saveImageAsBMP(const char* filePath, RGB* imagePixels, int width, int height);
void conversion(const char* file_path, const char* outputFilePath, int target_width, int target_height, int rotation_angle);



#endif