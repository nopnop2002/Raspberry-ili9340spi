#pragma once
#include <stdint.h>

#define 	JPG_OK   0
#define 	JPG_FAIL   -1
#define 	JPG_ERR_NO_MEM   0x101
#define 	JPG_ERR_INVALID_ARG   0x102
#define 	JPG_ERR_INVALID_STATE   0x103
#define 	JPG_ERR_INVALID_SIZE   0x104
#define 	JPG_ERR_NOT_FOUND   0x105
#define 	JPG_ERR_NOT_SUPPORTED   0x106
#define 	JPG_ERR_TIMEOUT   0x107
#define 	JPG_ERR_INVALID_RESPONSE   0x108
#define 	JPG_ERR_INVALID_CRC   0x109
#define 	JPG_ERR_INVALID_VERSION   0x10A
#define 	JPG_ERR_INVALID_MAC   0x10B
#define 	JPG_ERR_WIFI_BASE   0x3000
#define 	JPG_ERR_MESH_BASE   0x4000

#define rgb565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

//rgb565 format
typedef uint16_t pixel_jpeg;

/**
 * @brief Decode the jpeg ``image.jpg`` embedded into the program file into pixel data.
 *
 * @param pixels A pointer to a pointer for an array of rows, which themselves are an array of pixels.
 *        Effectively, you can get the pixel data by doing ``decode_image(&myPixels); pixelval=myPixels[ypos][xpos];``
 * @return - ESP_ERR_NOT_SUPPORTED if image is malformed or a progressive jpeg file
 *         - ESP_ERR_NO_MEM if out of memory
 *         - ESP_OK on succesful decode
 */

int16_t decode_jpeg(pixel_jpeg ***pixels, char * file, uint16_t width, uint16_t height, uint16_t * imageWidth, uint16_t * imageHeight);

/**
 * @brief Release image memory.
 *
 */

int16_t release_jpeg(pixel_jpeg ***pixels, uint16_t width, uint16_t height);

