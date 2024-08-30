/********************************************
This is a routine to convert a bmp format image to a hexadecimal array, t
he exported image data can be used in stm32 and arduino routines
to enable displaying the image on a seven-colour ink screen.
Please note, please make sure the imported image has been processed 
by the seven-colour colour conversion, 
otherwise the exported data will not work properly.
Please refer to the user manual for the specific steps of image colour conversion.


Please change the path to your file in the main function section.

*********************************************/
#include <stdint.h>


/**********************************
Color Index
**********************************/
#define EPD_7IN3F_BLACK   0x0   /// 000
#define EPD_7IN3F_WHITE   0x1   /// 001
#define EPD_7IN3F_GREEN   0x2   /// 010
#define EPD_7IN3F_BLUE    0x3   /// 011
#define EPD_7IN3F_RED     0x4   /// 100
#define EPD_7IN3F_YELLOW  0x5   /// 101
#define EPD_7IN3F_ORANGE  0x6   /// 110
#define EPD_7IN3F_CLEAN   0x7   /// 111   unavailable  Afterimage

/*Bitmap file header   14bit*/
typedef struct BMP_FILE_HEADER {
    uint16_t btype;        //File identifier
    uint32_t bsize;      //The size of the file
    uint16_t breserved1;   //Reserved value, must be set to 0
    uint16_t breserved2;   //Reserved value, must be set to 0
    uint32_t boffset;    //The offset from the beginning of the file header to the beginning of the image data bit
} __attribute__ ((packed)) BMPFILEHEADER;    // 14bit

/*Bitmap information header  40bit*/
typedef struct BMP_INFO {
    uint32_t biinfoSize;      //The size of the header
    uint32_t biwidth;         //The width of the image
    uint32_t biheight;        //The height of the image
    uint16_t biplanes;          //The number of planes in the image
    uint16_t bibitcount;        //The number of bits per pixel
    uint32_t bicompression;   //Compression type
    uint32_t bimpimagesize;   //The size of the image, in bytes
    uint32_t bixpelspermeter; //Horizontal resolution
    uint32_t biypelspermeter; //Vertical resolution
    uint32_t biclrused;       //The number of colors used
    uint32_t biclrimportant;  //The number of important colors
} __attribute__ ((packed)) BMPINFOHEADER;

/*Color table: palette */
typedef struct RGB_QUAD {
    uint8_t rgbblue;               //Blue intensity
    uint8_t rgbgreen;              //Green strength
    uint8_t rgbred;                //Red intensity
    uint8_t rgbreversed;           //Reserved value
} __attribute__ ((packed)) BMPRGBQUAD;

uint8_t GUI_ReadBmp_RGB_7Color(const char *path,  const char *outpath);

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


int main()
{


        
    //Enter the absolute path to the bmp image file to be imported.


    //C:\Users\Administrator\Desktop\7.3inch_e-paper_code\Image_Data_Conversion
    const char* bmpPath = "E:/lyh/products/E-Paper/7.3inch/7.3inch_e-paper_code/Image_Data_Conversion/b4.bmp";
    
    //Please select the absolute path to the storage location of the exported txt file.
    const char *output_path = "E:/lyh/products/E-Paper/7.3inch/7.3inch_e-paper_code/Image_Data_Conversion/output.txt";
    uint8_t result = GUI_ReadBmp_RGB_7Color(bmpPath,output_path);
    
    if (result) {
        printf("BMP file read successfully.\n");
    } else {
        printf("Failed to read BMP file.\n");
    }
    
    return 0;
}

/******************************************************************************
function :  Read bmp files directly
parameter:
    path: bmp file path
******************************************************************************/
uint8_t GUI_ReadBmp_RGB_7Color(const char *path,  const char *outpath)
{
    FILE *fp;                     // Define a file pointer
    BMPFILEHEADER bmpfileheader;  // Define a bmp file header structure
    BMPINFOHEADER bmpinfoheader;  // Define a bmp info header structure

    // Binary file open
    if ((fp = fopen(path, "rb")) == NULL) {
        printf("Cannot open the file!\n");
        exit(0);
    }

    // Set the file pointer from the beginning
    fseek(fp, 0, SEEK_SET);
    fread(&bmpfileheader, sizeof(BMPFILEHEADER), 1, fp);    // sizeof(BMPFILEHEADER) must be 14
    fread(&bmpinfoheader, sizeof(BMPINFOHEADER), 1, fp);    // sizeof(BMPFILEHEADER) must be 50
    printf("pixel = %d * %d\n", bmpinfoheader.biwidth, bmpinfoheader.biheight);

    uint32_t image_byte = bmpinfoheader.biwidth * bmpinfoheader.biheight *3;
   // uint8_t image[image_byte];
    uint8_t* image = (uint8_t*)malloc(image_byte);
    memset(image, 0xFF, image_byte);

    // Determine if it is a monochrome bitmap
    int readbyte = bmpinfoheader.bibitcount;
    if (readbyte != 24) {
        printf("Bmp image is not 24 bitmap!\n");
        fclose(fp);
        return 0;
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


    uint8_t *BlackImage;
    uint32_t Imagesize = ((bmpinfoheader.biwidth % 2 == 0)? (bmpinfoheader.biwidth / 2 ): (bmpinfoheader.biwidth / 2 + 1)) * bmpinfoheader.biheight;
    if((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    uint16_t byte_w = ( bmpinfoheader.biwidth% 2 == 0)? (bmpinfoheader.biwidth / 2 ): (bmpinfoheader.biwidth / 2 + 1);
    for(dy = 0; dy < bmpinfoheader.biheight; dy++) {
        for(dx = 0; dx < bmpinfoheader.biwidth; dx++) {
            if(dx > bmpinfoheader.biwidth || dy > bmpinfoheader.biheight) {
                break;
            }
	            uint32_t Addr = dx / 2  + dy * byte_w;
	            uint8_t Rdata = BlackImage[Addr];
		        Rdata = Rdata & (~(0xF0 >> ((dx % 2)*4)));//Clear first, then set value
		        BlackImage[Addr] = Rdata | ((image[bmpinfoheader.biheight *  bmpinfoheader.biwidth - 1 -(bmpinfoheader.biwidth-dx-1+(dy* bmpinfoheader.biwidth))] << 4) >> ((dx % 2)*4));
		}
    }


    // Save image data to a text file
    //const char *output_path = "C:/Users/Administrator/Desktop/dd/output.txt";
    FILE *output = fopen(outpath, "w");
    if (output == NULL) {
        printf("Failed to create output file!\n");
        free(image);
        return 0;
    }

    // Write image data to the text file
    for (dy = 0; dy < bmpinfoheader.biheight; dy++) {
        for (dx = 0; dx < byte_w; dx++) {
            fprintf(output, "0x%02X", BlackImage[dx + (dy * byte_w)]);
             if (dx < byte_w) {
                fprintf(output, ","); 
             }
              if ((dx + (dy * byte_w)+ 1) % 16 == 0) {
                fprintf(output, "\n"); // 每16个数据换行
            }
        }
        fprintf(output, "\n");
    }

    fclose(output);
    free(image);

    printf("Image data has been saved to output.txt\n");
    return 1;
}

