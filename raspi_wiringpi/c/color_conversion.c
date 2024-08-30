#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <FreeImage.h>
#include <string.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include "color_conversion.h"

/***********************************************************************
We have prepared two dithering algorithms that are very versatile, 
You can choose to use one of them according to your needs,
Or you can use a more complex dithering algorithm.
One of them is ditherImage(), which is suitable for images with low colour contrast and even transitions.
ditherImage2() is for images with high colour contrast.
You can use each of these functions to convert an image for comparison and choose the one that suits your needs.
Of course, you can adjust ditherImage() and ditherImage2() ) to suit your needs.
*******************************************************************************************/




RGB getNearestColor(RGB color) {
    RGB paletteColors[] = {
        {0, 0, 0},       // Black
        {255, 0, 0},     // Red
        {0, 255, 0},     // Green
        {0, 0, 255},     // Blue
        {255, 255, 255}, // White
        {255, 255, 0},   // Yellow
        {255, 128, 0}    // Orange
    };

    RGB nearestColor = paletteColors[0];
    double minDistance = INFINITY;

    for (int i = 0; i < sizeof(paletteColors) / sizeof(paletteColors[0]); i++) {
        double distance = sqrt(pow(color.red - paletteColors[i].red, 2) +
                               pow(color.green - paletteColors[i].green, 2) +
                               pow(color.blue - paletteColors[i].blue, 2));
        if (distance < minDistance) {
            minDistance = distance;
            nearestColor = paletteColors[i];
        }
    }

    return nearestColor;
}

/******************************************************************************
function:	Image colour quantisation through dithering algorithms
parameter:

******************************************************************************/
void ditherImage(RGB* image, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGB oldPixel = image[y * width + x];
            RGB newPixel = getNearestColor(oldPixel);
            image[y * width + x] = newPixel;

            int errorRed = oldPixel.red - newPixel.red;
            int errorGreen = oldPixel.green - newPixel.green;
            int errorBlue = oldPixel.blue - newPixel.blue;
            if (x + 1 < width) {
                image[y * width + (x + 1)].red += (errorRed * 7) / 48;
                image[y * width + (x + 1)].green += (errorGreen * 7) / 48;
                image[y * width + (x + 1)].blue += (errorBlue * 7) / 48;
            }
            if (x + 2 < width) {
                image[y * width + (x + 2)].red += (errorRed * 5) / 48;
                image[y * width + (x + 2)].green += (errorGreen * 5) / 48;
                image[y * width + (x + 2)].blue += (errorBlue * 5) / 48;
            }
            if (x - 2 >= 0 && y + 1 < height) {
                image[(y + 1) * width + (x - 2)].red += (errorRed * 3) / 48;
                image[(y + 1) * width + (x - 2)].green += (errorGreen * 3) / 48;
                image[(y + 1) * width + (x - 2)].blue += (errorBlue * 3) / 48;
            }
            if (x - 1 >= 0 && y + 1 < height) {
                image[(y + 1) * width + (x - 1)].red += (errorRed * 5) / 48;
                image[(y + 1) * width + (x - 1)].green += (errorGreen * 5) / 48;
                image[(y + 1) * width + (x - 1)].blue += (errorBlue * 5) / 48;
            }
            if (y + 1 < height) {
                image[(y + 1) * width + x].red += (errorRed * 7) / 48;
                image[(y + 1) * width + x].green += (errorGreen * 7) / 48;
                image[(y + 1) * width + x].blue += (errorBlue * 7) / 48;
            }
            if (x + 1 < width && y + 1 < height) {
                image[(y + 1) * width + (x + 1)].red += (errorRed * 5) / 48;
                image[(y + 1) * width + (x + 1)].green += (errorGreen * 5) / 48;
                image[(y + 1) * width + (x + 1)].blue += (errorBlue * 5) / 48;
            }
            if (x + 2 < width && y + 1 < height) {
                image[(y + 1) * width + (x + 2)].red += (errorRed * 3) / 48;
                image[(y + 1) * width + (x + 2)].green += (errorGreen * 3) / 48;
                image[(y + 1) * width + (x + 2)].blue += (errorBlue * 3) / 48;
            }
            if (x - 2 >= 0 && y + 2 < height) {
                image[(y + 2) * width + (x - 2)].red += (errorRed * 1) / 48;
                image[(y + 2) * width + (x - 2)].green += (errorGreen * 1) / 48;
                image[(y + 2) * width + (x - 2)].blue += (errorBlue * 1) / 48;
            }
            if (x - 1 >= 0 && y + 2 < height) {
                image[(y + 2) * width + (x - 1)].red += (errorRed * 3) / 48;
                image[(y + 2) * width + (x - 1)].green += (errorGreen * 3) / 48;
                image[(y + 2) * width + (x - 1)].blue += (errorBlue * 3) / 48;
            }
            if (y + 2 < height) {
                image[(y + 2) * width + x].red += (errorRed * 5) / 48;
                image[(y + 2) * width + x].green += (errorGreen * 5) / 48;
                image[(y + 2) * width + x].blue += (errorBlue * 5) / 48;
            }
            if (x + 1 < width && y + 2 < height) {
                image[(y + 2) * width + (x + 1)].red += (errorRed * 3) / 48;
                image[(y + 2) * width + (x + 1)].green += (errorGreen * 3) / 48;
                image[(y + 2) * width + (x + 1)].blue += (errorBlue * 3) / 48;
            }
            if (x + 2 < width && y + 2 < height) {
                image[(y + 2) * width + (x + 2)].red += (errorRed * 1) / 48;
                image[(y + 2) * width + (x + 2)].green += (errorGreen * 1) / 48;
                image[(y + 2) * width + (x + 2)].blue += (errorBlue * 1) / 48;
            }
        }
    }
}

/******************************************************************************
function:	Image colour quantisation through dithering algorithms
parameter:


******************************************************************************/
void ditherImage2(RGB* image, int width, int height) {
    int neighborhoodSize = 6;
    int threshold = 200; // Adjust this threshold as needed

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGB oldPixel = image[y * width + x];
            RGB newPixel = getNearestColor(oldPixel);
            image[y * width + x] = newPixel;

            int errorRed = oldPixel.red - newPixel.red;
            int errorGreen = oldPixel.green - newPixel.green;
            int errorBlue = oldPixel.blue - newPixel.blue;

            // Calculate average color difference in a 5x5 neighborhood
            int totalDiff = 0;
            int numPixels = 0;

            for (int offsetY = -neighborhoodSize / 2; offsetY <= neighborhoodSize / 2; offsetY++) {
                for (int offsetX = -neighborhoodSize / 2; offsetX <= neighborhoodSize / 2; offsetX++) {
                    int neighborX = x + offsetX;
                    int neighborY = y + offsetY;

                    if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                        RGB neighborPixel = image[neighborY * width + neighborX];
                        int diff = abs(neighborPixel.red - newPixel.red) +
                                   abs(neighborPixel.green - newPixel.green) +
                                   abs(neighborPixel.blue - newPixel.blue);

                        totalDiff += diff;
                        numPixels++;
                    }
                }
            }

            if (numPixels > 0) {
                int averageDiff = totalDiff / numPixels;

                if (averageDiff > threshold) {
                    // Perform new dithering algorithm
                    // ... implement your new dithering algorithm here ...


                    if (x + 1 < width) {
                        image[y * width + (x + 1)].red += (errorRed * 5) / 100;
                        image[y * width + (x + 1)].green += (errorGreen * 5) / 100;
                        image[y * width + (x + 1)].blue += (errorBlue * 5) / 100;
                    }
                    if (x + 2 < width) {
                        image[y * width + (x + 2)].red += (errorRed * 5) / 100;
                        image[y * width + (x + 2)].green += (errorGreen * 5) / 100;
                        image[y * width + (x + 2)].blue += (errorBlue * 5) / 100;
                    }
                    if (x - 2 >= 0 && y + 1 < height) {
                        image[(y + 1) * width + (x - 2)].red += (errorRed * 3) / 100;
                        image[(y + 1) * width + (x - 2)].green += (errorGreen * 3) / 100;
                        image[(y + 1) * width + (x - 2)].blue += (errorBlue * 3) / 100;
                    }
                    if (x - 1 >= 0 && y + 1 < height) {
                        image[(y + 1) * width + (x - 1)].red += (errorRed * 5) / 100;
                        image[(y + 1) * width + (x - 1)].green += (errorGreen * 5) / 100;
                        image[(y + 1) * width + (x - 1)].blue += (errorBlue * 5) / 100;
                    }
                    if (y + 1 < height) {
                        image[(y + 1) * width + x].red += (errorRed * 5) / 100;
                        image[(y + 1) * width + x].green += (errorGreen * 5) / 100;
                        image[(y + 1) * width + x].blue += (errorBlue * 5) / 100;
                    }
                    if (x + 1 < width && y + 1 < height) {
                        image[(y + 1) * width + (x + 1)].red += (errorRed * 5) / 100;
                        image[(y + 1) * width + (x + 1)].green += (errorGreen * 5) / 100;
                        image[(y + 1) * width + (x + 1)].blue += (errorBlue * 5) / 100;
                    }
                    if (x + 2 < width && y + 1 < height) {
                        image[(y + 1) * width + (x + 2)].red += (errorRed * 3) / 100;
                        image[(y + 1) * width + (x + 2)].green += (errorGreen * 3) / 100;
                        image[(y + 1) * width + (x + 2)].blue += (errorBlue * 3) / 100;
                    }
                }
                else {
                    // Perform original error diffusion
                    // Jarvis-Judice-Ninke error diffusion
                    // ... original error diffusion code ...
                    if (x + 1 < width) {
                        image[y * width + (x + 1)].red += (errorRed * 7) / 48;
                        image[y * width + (x + 1)].green += (errorGreen * 7) / 48;
                        image[y * width + (x + 1)].blue += (errorBlue * 7) / 48;
                    }
                    if (x + 2 < width) {
                        image[y * width + (x + 2)].red += (errorRed * 5) / 48;
                        image[y * width + (x + 2)].green += (errorGreen * 5) / 48;
                        image[y * width + (x + 2)].blue += (errorBlue * 5) / 48;
                    }
                    if (x - 2 >= 0 && y + 1 < height) {
                        image[(y + 1) * width + (x - 2)].red += (errorRed * 3) / 48;
                        image[(y + 1) * width + (x - 2)].green += (errorGreen * 3) / 48;
                        image[(y + 1) * width + (x - 2)].blue += (errorBlue * 3) / 48;
                    }
                    if (x - 1 >= 0 && y + 1 < height) {
                        image[(y + 1) * width + (x - 1)].red += (errorRed * 5) / 48;
                        image[(y + 1) * width + (x - 1)].green += (errorGreen * 5) / 48;
                        image[(y + 1) * width + (x - 1)].blue += (errorBlue * 5) / 48;
                    }
                    if (y + 1 < height) {
                        image[(y + 1) * width + x].red += (errorRed * 7) / 48;
                        image[(y + 1) * width + x].green += (errorGreen * 7) / 48;
                     image[(y + 1) * width + x].blue += (errorBlue * 7) / 48;
                    }
                    if (x + 1 < width && y + 1 < height) {
                        image[(y + 1) * width + (x + 1)].red += (errorRed * 5) / 48;
                        image[(y + 1) * width + (x + 1)].green += (errorGreen * 5) / 48;
                        image[(y + 1) * width + (x + 1)].blue += (errorBlue * 5) / 48;
                    }
                    if (x + 2 < width && y + 1 < height) {
                        image[(y + 1) * width + (x + 2)].red += (errorRed * 3) / 48;
                        image[(y + 1) * width + (x + 2)].green += (errorGreen * 3) / 48;
                        image[(y + 1) * width + (x + 2)].blue += (errorBlue * 3) / 48;
                    }
                    if (x - 2 >= 0 && y + 2 < height) {
                        image[(y + 2) * width + (x - 2)].red += (errorRed * 1) / 48;
                        image[(y + 2) * width + (x - 2)].green += (errorGreen * 1) / 48;
                        image[(y + 2) * width + (x - 2)].blue += (errorBlue * 1) / 48;
                    }
                    if (x - 1 >= 0 && y + 2 < height) {
                        image[(y + 2) * width + (x - 1)].red += (errorRed * 3) / 48;
                        image[(y + 2) * width + (x - 1)].green += (errorGreen * 3) / 48;
                        image[(y + 2) * width + (x - 1)].blue += (errorBlue * 3) / 48;
                    }
                    if (y + 2 < height) {
                        image[(y + 2) * width + x].red += (errorRed * 5) / 48;
                        image[(y + 2) * width + x].green += (errorGreen * 5) / 48;
                        image[(y + 2) * width + x].blue += (errorBlue * 5) / 48;
                    }
                    if (x + 1 < width && y + 2 < height) {
                        image[(y + 2) * width + (x + 1)].red += (errorRed * 3) / 48;
                        image[(y + 2) * width + (x + 1)].green += (errorGreen * 3) / 48;
                        image[(y + 2) * width + (x + 1)].blue += (errorBlue * 3) / 48;
                    }
                    if (x + 2 < width && y + 2 < height) {
                        image[(y + 2) * width + (x + 2)].red += (errorRed * 1) / 48;
                        image[(y + 2) * width + (x + 2)].green += (errorGreen * 1) / 48;
                        image[(y + 2) * width + (x + 2)].blue += (errorBlue * 1) / 48;
                    }
                }
            }
        }
    }
}


FIBITMAP* image_conversion(const char* file_path, int target_width, int target_height, int rotation_angle) {
    // Initialising the FreeImage library
    FreeImage_Initialise(FALSE);

    // Getting the image format
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(file_path, 0);
    if (format == FIF_UNKNOWN) {
        printf("Unknown Image Format:%s\n", file_path);
        FreeImage_DeInitialise();
        return NULL;
    }

    // Loading an Image
    FIBITMAP* image = FreeImage_Load(format, file_path, 0);
    if (image == NULL) {
        printf("Unable to load image file:%s\n", file_path);
        FreeImage_DeInitialise();
        return NULL;
    }

    // Rotate image
    if (rotation_angle == 90) {
        image = FreeImage_Rotate(image, 90, NULL);  //
    } else if (rotation_angle == 180) {
        image = FreeImage_Rotate(image, 180, NULL);  // 
    } else if (rotation_angle == 270) {
        image = FreeImage_Rotate(image, 270, NULL);  //
    }

    // Get the width and height of the image
    int width = FreeImage_GetWidth(image);
    int height = FreeImage_GetHeight(image);

    // Proportion of width and height
    float width_ratio = (float)target_width / width;
    float height_ratio = (float)target_height / height;

    // Calculate width and height after equal scaling
    int new_width, new_height;
    if (width_ratio < height_ratio) {
        new_width = target_width;
        new_height = (int)(height * width_ratio);
    } else {
        new_width = (int)(width * height_ratio);
        new_height = target_height;
    }

    // Creating a new image
    FIBITMAP* new_image = FreeImage_Rescale(image, new_width, new_height, FILTER_BICUBIC);

    // Release image resources (if not needed to return original image data)
    FreeImage_Unload(image);
    FreeImage_DeInitialise();

    // Returning processed image data
    return new_image;
}



void saveImageAsBMP(const char* filePath, RGB* imagePixels, int width, int height) {
    // Creates a new 24-bit BMP image
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24, 0, 0, 0);
    if (!bitmap) {
        printf("Unable to create a BMP image\n");
        return;
    }

    // Set image pixel data
    int pixelIndex = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBQUAD color;
            color.rgbRed = imagePixels[pixelIndex].red;
            color.rgbGreen = imagePixels[pixelIndex].green;
            color.rgbBlue = imagePixels[pixelIndex].blue;
            FreeImage_SetPixelColor(bitmap, x, y, &color);
            pixelIndex++;
        }
    }

    // Save BMP image to file
    if (!FreeImage_Save(FIF_BMP, bitmap, filePath, BMP_DEFAULT)) {
        printf("Cannot save BMP image\n");
    }

    // Releasing BMP image resources
    FreeImage_Unload(bitmap);
}

/******************************************************************************
function:	Conversion function to complete the image format 
            and colour conversion to a 7-colour 24-bit bmp format image.
parameter:
    file_path		    :   Path to input file
    outputFilePath		:   Path to output file
    target_width		:   Width of the converted image
    target_height	    :	Height of the converted image
    rotation_angle	    :   Rotation angle of the image
******************************************************************************/

void conversion(const char* file_path, const char* outputFilePath, int target_width, int target_height, int rotation_angle) {
    // Call image_conversion function
    FIBITMAP* image = image_conversion(file_path, target_width, target_height, rotation_angle);
    if (image == NULL) {
        printf("Image Conversion Failed\n");
        return;
    }

    // Getting the width and height of an image
    int width = FreeImage_GetWidth(image);
    int height = FreeImage_GetHeight(image);

    // Convert image pixel data to an array of RGB structures
    RGB* imagePixels = (RGB*)malloc(sizeof(RGB) * width * height);
    int pixelIndex = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBQUAD color;
            FreeImage_GetPixelColor(image, x, y, &color);
            imagePixels[pixelIndex].blue = color.rgbBlue;
            imagePixels[pixelIndex].green = color.rgbGreen;
            imagePixels[pixelIndex].red = color.rgbRed;
            pixelIndex++;
        }
    }



    // Perform colour quantisation and dithering
    //ditherImage(imagePixels, width, height);
    ditherImage2(imagePixels, width, height);



    // Create a new target-sized image
    RGB* targetImagePixels = (RGB*)malloc(sizeof(RGB) * target_width * target_height);

    // Determine if the image size matches the target size
    if (width == target_width && height == target_height) {
        // If the size matches, copy the pixel data directly
        memcpy(targetImagePixels, imagePixels, sizeof(RGB) * width * height);
    } else {
        // If the size does not match, fill the target image with white colour
        int offsetX = (target_width - width) / 2;
        int offsetY = (target_height - height) / 2;
        for (int y = 0; y < target_height; y++) {
            for (int x = 0; x < target_width; x++) {
                if (x >= offsetX && x < offsetX + width && y >= offsetY && y < offsetY + height) {
                    // Copy the quantised pixel data
                    int sourceX = x - offsetX;
                    int sourceY = y - offsetY;
                    targetImagePixels[y * target_width + x] = imagePixels[sourceY * width + sourceX];
                } else {
                    // Fill with white
                    targetImagePixels[y * target_width + x].red = 255;
                    targetImagePixels[y * target_width + x].green = 255;
                    targetImagePixels[y * target_width + x].blue = 255;
                }
            }
        }
    }

    // Save the processed image as a 24-bit BMP file
    saveImageAsBMP(outputFilePath, targetImagePixels, target_width, target_height);

    // Release memory
    free(imagePixels);
    free(targetImagePixels);
    printf("Colour quantisation is complete\n");
}

