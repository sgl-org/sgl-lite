/* source: sgl_core.c
 * Copyright (c) 2026-2028, Lishanwen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sgl_core.h"
#include "sgl_mm.h"
#include "sgl_draw.h"

/* current sgl system variable */
sgl_system_t sgl_system;


#if CONFIG_SGL_DEBUG
/**
 * @brief sgl log printing function, used to print debugging information. Note that this function 
 *        should only be called in debugging mode, otherwise it may affect system real-time 
 *        performance due to long execution time
 * @param level:  log level, such as, INFO, USER...
 * @param format:  log content
 * @return none
*/
void sgl_log(const char *level, const char * format, ...)
{
    char buffer[200];
    int  tail = 0;
    int  pref_size = strlen(level);

    strcpy(buffer, level);

    va_list va;
    va_start(va, format);
    sgl_vsnprintf(buffer + pref_size, sizeof(buffer), format, va);
    va_end(va);

    tail = strlen(buffer);
    memcpy(&buffer[tail], &"\r\n"SGL_LOG_NONE, strlen("\r\n"SGL_LOG_NONE));
    buffer[tail + strlen("\r\n"SGL_LOG_NONE)] = 0;

    sgl_log_stdout(buffer);
}


/**
 * @brief sgl assert handler, used to handle assertions
 * @param file:  file name
 * @param func:  function name
 * @param line:  line number
 * @return none
*/
void sgl_assert_handler(const char *file, const char *func, int line)
{
    sgl_log(SGL_ASSERT_FLAG, "file: %s, function: %s, line: %d", file, func, line);
    while (1) {

    };
}
#endif // !CONFIG_SGL_DEBUG


/**
 * @brief sine table
 * @note the table is used to calculate the sine of an angle
 */
static const int16_t sin0_90_table[] = {
    0,     572,   1144,  1715,  2286,  2856,  3425,  3993,  4560,  5126,  5690,  6252,  6813,  7371,  7927,  8481,
    9032,  9580,  10126, 10668, 11207, 11743, 12275, 12803, 13328, 13848, 14364, 14876, 15383, 15886, 16383, 16876,
    17364, 17846, 18323, 18794, 19260, 19720, 20173, 20621, 21062, 21497, 21925, 22347, 22762, 23170, 23571, 23964,
    24351, 24730, 25101, 25465, 25821, 26169, 26509, 26841, 27165, 27481, 27788, 28087, 28377, 28659, 28932, 29196,
    29451, 29697, 29934, 30162, 30381, 30591, 30791, 30982, 31163, 31335, 31498, 31650, 31794, 31927, 32051, 32165,
    32269, 32364, 32448, 32523, 32587, 32642, 32687, 32722, 32747, 32762, 32767
};


/**
 * @brief sqrt table, if the number less than 1024, the sqrt root will get by the table
 */
static const uint8_t sqrt_error_table[] = {
    0x00,0x00,0x6a,0xbb,0x00,0x3c,0x73,0xa5,0xd4,0x00,0x29,0x51,0x76,0x9b,0xbd,0xdf,
    0x00,0x1f,0x3e,0x5b,0x78,0x95,0xb0,0xcb,0xe6,0x00,0x19,0x32,0x4a,0x62,0x7a,0x91,
    0xa8,0xbe,0xd4,0xea,0x00,0x15,0x2a,0x3e,0x53,0x67,0x7b,0x8e,0xa2,0xb5,0xc8,0xdb,
    0xed,0x00,0x12,0x24,0x36,0x47,0x59,0x6a,0x7b,0x8c,0x9d,0xae,0xbe,0xcf,0xdf,0xef,
    0x00,0x0f,0x1f,0x2f,0x3f,0x4e,0x5d,0x6d,0x7c,0x8b,0x9a,0xa9,0xb7,0xc6,0xd4,0xe3,
    0xf1,0x00,0x0e,0x1c,0x2a,0x38,0x46,0x53,0x61,0x6f,0x7c,0x8a,0x97,0xa4,0xb2,0xbf,
    0xcc,0xd9,0xe6,0xf3,0x00,0x0c,0x19,0x26,0x32,0x3f,0x4b,0x58,0x64,0x70,0x7c,0x89,
    0x95,0xa1,0xad,0xb9,0xc5,0xd1,0xdc,0xe8,0xf4,0x00,0x0b,0x17,0x22,0x2e,0x39,0x44,
    0x50,0x5b,0x66,0x72,0x7d,0x88,0x93,0x9e,0xa9,0xb4,0xbf,0xca,0xd5,0xdf,0xea,0xf5,
    0x00,0x0a,0x15,0x1f,0x2a,0x34,0x3f,0x49,0x54,0x5e,0x68,0x73,0x7d,0x87,0x91,0x9c,
    0xa6,0xb0,0xba,0xc4,0xce,0xd8,0xe2,0xec,0xf6,0x00,0x09,0x13,0x1d,0x27,0x30,0x3a,
    0x44,0x4d,0x57,0x61,0x6a,0x74,0x7d,0x87,0x90,0x99,0xa3,0xac,0xb6,0xbf,0xc8,0xd1,
    0xdb,0xe4,0xed,0xf6,0x00,0x09,0x12,0x1b,0x24,0x2d,0x36,0x3f,0x48,0x51,0x5a,0x63,
    0x6c,0x74,0x7d,0x86,0x8f,0x98,0xa0,0xa9,0xb2,0xbb,0xc3,0xcc,0xd5,0xdd,0xe6,0xee,
    0xf7,0x00,0x08,0x11,0x19,0x21,0x2a,0x32,0x3b,0x43,0x4c,0x54,0x5c,0x65,0x6d,0x75,
    0x7d,0x86,0x8e,0x96,0x9e,0xa7,0xaf,0xb7,0xbf,0xc7,0xcf,0xd7,0xdf,0xe7,0xef,0xf7,
    0x00,0x07,0x0f,0x17,0x1f,0x27,0x2f,0x37,0x3f,0x47,0x4f,0x57,0x5e,0x66,0x6e,0x76,
    0x7e,0x85,0x8d,0x95,0x9c,0xa4,0xac,0xb4,0xbb,0xc3,0xca,0xd2,0xda,0xe1,0xe9,0xf0,
    0xf8,0x00,0x07,0x0f,0x16,0x1e,0x25,0x2c,0x34,0x3b,0x43,0x4a,0x52,0x59,0x60,0x68,
    0x6f,0x76,0x7e,0x85,0x8c,0x94,0x9b,0xa2,0xa9,0xb1,0xb8,0xbf,0xc6,0xcd,0xd5,0xdc,
    0xe3,0xea,0xf1,0xf8,0x00,0x07,0x0e,0x15,0x1c,0x23,0x2a,0x31,0x38,0x3f,0x46,0x4d,
    0x54,0x5b,0x62,0x69,0x70,0x77,0x7e,0x85,0x8c,0x92,0x99,0xa0,0xa7,0xae,0xb5,0xbc,
    0xc2,0xc9,0xd0,0xd7,0xde,0xe4,0xeb,0xf2,0xf9,0x00,0x06,0x0d,0x14,0x1a,0x21,0x28,
    0x2e,0x35,0x3c,0x42,0x49,0x50,0x56,0x5d,0x64,0x6a,0x71,0x77,0x7e,0x84,0x8b,0x92,
    0x98,0x9f,0xa5,0xac,0xb2,0xb9,0xbf,0xc6,0xcc,0xd3,0xd9,0xdf,0xe6,0xec,0xf3,0xf9,
    0x00,0x06,0x0c,0x13,0x19,0x1f,0x26,0x2c,0x32,0x39,0x3f,0x45,0x4c,0x52,0x58,0x5f,
    0x65,0x6b,0x71,0x78,0x7e,0x84,0x8a,0x91,0x97,0x9d,0xa3,0xa9,0xb0,0xb6,0xbc,0xc2,
    0xc8,0xcf,0xd5,0xdb,0xe1,0xe7,0xed,0xf3,0xf9,0x00,0x06,0x0c,0x12,0x18,0x1e,0x24,
    0x2a,0x30,0x36,0x3c,0x42,0x48,0x4e,0x54,0x5a,0x60,0x66,0x6c,0x72,0x78,0x7e,0x84,
    0x8a,0x90,0x96,0x9c,0xa2,0xa8,0xad,0xb3,0xb9,0xbf,0xc5,0xcb,0xd1,0xd7,0xdc,0xe2,
    0xe8,0xee,0xf4,0xfa,0x00,0x05,0x0b,0x11,0x17,0x1d,0x22,0x28,0x2e,0x34,0x39,0x3f,
    0x45,0x4b,0x50,0x56,0x5c,0x62,0x67,0x6d,0x73,0x78,0x7e,0x84,0x89,0x8f,0x95,0x9a,
    0xa0,0xa6,0xab,0xb1,0xb7,0xbc,0xc2,0xc8,0xcd,0xd3,0xd8,0xde,0xe4,0xe9,0xef,0xf4,
    0xfa,0x00,0x05,0x0b,0x10,0x16,0x1b,0x21,0x26,0x2c,0x31,0x37,0x3c,0x42,0x47,0x4d,
    0x52,0x58,0x5d,0x63,0x68,0x6e,0x73,0x79,0x7e,0x84,0x89,0x8e,0x94,0x99,0x9f,0xa4,
    0xaa,0xaf,0xb4,0xba,0xbf,0xc5,0xca,0xcf,0xd5,0xda,0xdf,0xe5,0xea,0xef,0xf5,0xfa,
    0x00,0x05,0x0a,0x0f,0x15,0x1a,0x1f,0x25,0x2a,0x2f,0x35,0x3a,0x3f,0x44,0x4a,0x4f,
    0x54,0x5a,0x5f,0x64,0x69,0x6e,0x74,0x79,0x7e,0x83,0x89,0x8e,0x93,0x98,0x9d,0xa3,
    0xa8,0xad,0xb2,0xb7,0xbd,0xc2,0xc7,0xcc,0xd1,0xd6,0xdc,0xe1,0xe6,0xeb,0xf0,0xf5,
    0xfa,0x00,0x05,0x0a,0x0f,0x14,0x19,0x1e,0x23,0x28,0x2d,0x32,0x38,0x3d,0x42,0x47,
    0x4c,0x51,0x56,0x5b,0x60,0x65,0x6a,0x6f,0x74,0x79,0x7e,0x83,0x88,0x8d,0x92,0x97,
    0x9c,0xa1,0xa6,0xab,0xb0,0xb5,0xba,0xbf,0xc4,0xc9,0xce,0xd3,0xd8,0xdd,0xe2,0xe7,
    0xec,0xf1,0xf6,0xfb,0x00,0x04,0x09,0x0e,0x13,0x18,0x1d,0x22,0x27,0x2c,0x31,0x35,
    0x3a,0x3f,0x44,0x49,0x4e,0x53,0x58,0x5c,0x61,0x66,0x6b,0x70,0x75,0x79,0x7e,0x83,
    0x88,0x8d,0x92,0x96,0x9b,0xa0,0xa5,0xaa,0xae,0xb3,0xb8,0xbd,0xc2,0xc6,0xcb,0xd0,
    0xd5,0xd9,0xde,0xe3,0xe8,0xed,0xf1,0xf6,0xfb,0x00,0x04,0x09,0x0e,0x12,0x17,0x1c,
    0x21,0x25,0x2a,0x2f,0x33,0x38,0x3d,0x42,0x46,0x4b,0x50,0x54,0x59,0x5e,0x62,0x67,
    0x6c,0x70,0x75,0x7a,0x7e,0x83,0x88,0x8c,0x91,0x96,0x9a,0x9f,0xa3,0xa8,0xad,0xb1,
    0xb6,0xbb,0xbf,0xc4,0xc8,0xcd,0xd2,0xd6,0xdb,0xdf,0xe4,0xe9,0xed,0xf2,0xf6,0xfb,
    0x00,0x04,0x09,0x0d,0x12,0x16,0x1b,0x1f,0x24,0x29,0x2d,0x32,0x36,0x3b,0x3f,0x44,
    0x48,0x4d,0x51,0x56,0x5a,0x5f,0x63,0x68,0x6c,0x71,0x75,0x7a,0x7e,0x83,0x87,0x8c,
    0x90,0x95,0x99,0x9e,0xa2,0xa7,0xab,0xb0,0xb4,0xb9,0xbd,0xc1,0xc6,0xca,0xcf,0xd3,
    0xd8,0xdc,0xe1,0xe5,0xe9,0xee,0xf2,0xf7,0xfb,0x00,0x04,0x08,0x0d,0x11,0x16,0x1a,
    0x1e,0x23,0x27,0x2c,0x30,0x34,0x39,0x3d,0x41,0x46,0x4a,0x4f,0x53,0x57,0x5c,0x60,
    0x64,0x69,0x6d,0x71,0x76,0x7a,0x7e,0x83,0x87,0x8b,0x90,0x94,0x98,0x9d,0xa1,0xa5,
    0xaa,0xae,0xb2,0xb7,0xbb,0xbf,0xc4,0xc8,0xcc,0xd0,0xd5,0xd9,0xdd,0xe2,0xe6,0xea,
    0xee,0xf3,0xf7,0xfb,0x00,0x04,0x08,0x0c,0x11,0x15,0x19,0x1d,0x22,0x26,0x2a,0x2e,
    0x33,0x37,0x3b,0x3f,0x43,0x48,0x4c,0x50,0x54,0x59,0x5d,0x61,0x65,0x69,0x6e,0x72,
    0x76,0x7a,0x7e,0x83,0x87,0x8b,0x8f,0x93,0x98,0x9c,0xa0,0xa4,0xa8,0xac,0xb1,0xb5,
    0xb9,0xbd,0xc1,0xc5,0xca,0xce,0xd2,0xd6,0xda,0xde,0xe3,0xe7,0xeb,0xef,0xf3,0xf7,
    0xfb,0x00,0x04,0x08,0x0c,0x10,0x14,0x18,0x1c,0x20,0x25,0x29,0x2d,0x31,0x35,0x39,
    0x3d,0x41,0x45,0x49,0x4e,0x52,0x56,0x5a,0x5e,0x62,0x66,0x6a,0x6e,0x72,0x76,0x7a,
    0x7e,0x83,0x87,0x8b,0x8f,0x93,0x97,0x9b,0x9f,0xa3,0xa7,0xab,0xaf,0xb3,0xb7,0xbb,
    0xbf,0xc3,0xc7,0xcb,0xcf,0xd3,0xd7,0xdb,0xdf,0xe3,0xe7,0xeb,0xef,0xf3,0xf7,0xfb,
    0x00
};


/**
 * the table will used to calculate the square root of a number
 */
static const uint32_t sqrt_error_init_table[32] = {
    0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7
};


/**
 * @brief Calculate the sine of an angle
 * @param angle: Angle in degrees such 0-359
 * @return sine of the angle from sin0_90_table
 */
int32_t sgl_sin(int16_t angle)
{
    int16_t ret = 0;

    while (angle >= 360) {
        angle -= 360;
    }
    while (angle < 0) {
        angle += 360;
    }

    if (angle < 0) {
        angle = 360 + angle;
    }

    if (angle < 90) {
        ret = sin0_90_table[angle];
    }
    else if (angle >= 90 && angle < 180){
        angle = 180 - angle;
        ret   = sin0_90_table[angle];
    }
    else if (angle >= 180 && angle < 270){
        angle = angle - 180;
        ret   = -sin0_90_table[angle];
    }
    else {
        angle = 360 - angle;
        ret   = -sin0_90_table[angle];
    }

    return ret;
}


/**
 * @brief  Calculate x number square root
 * @param  x: x number
 * @retval x number square root
 */
uint16_t sgl_sqrt(uint32_t x)
{
    uint32_t rem = 0;
    uint32_t root = 0;
    uint32_t divisor = 0;

    for (int i=0; i<16; i++) {
        root <<= 1;
        rem = ((rem << 2) + (x >> 30));
        x <<= 2;
        divisor = (root<<1) + 1;
        
        if (divisor <= rem) {
            rem -= divisor;
            root++;
        }
    }
    return (uint16_t)(root);
}


/**
 * @brief Calculate error of the square root for a number.
 * @param x The number to calculate the error for.
 * @return The error of the square root. [0 ~ 255]
 */
uint8_t sgl_sqrt_error(uint32_t x)
{
    uint32_t bsh = (1 << 14);
    uint32_t fpr = 0;
    uint32_t osh = 0;
    uint32_t bod;

    if (x < 1025) {
        return sqrt_error_table[x];
    }
    else if (x > (1 << 30)) {
        return 0;
    }

    int32_t index = 0;
    uint32_t xp = x;

    if ((xp & 0xFFFF0000) == 0) { index += 16; xp <<= 16; }
    if ((xp & 0xFF000000) == 0) { index += 8; xp <<= 8; }
    if ((xp & 0xF0000000) == 0) { index += 4; xp <<= 4; }
    if ((xp & 0xC0000000) == 0) { index += 2; xp <<= 2; }
    if ((xp & 0x80000000) == 0) { index += 1; }

    osh = sqrt_error_init_table[31 - index];
    bsh = 1 << (2 * osh + 14);

    do {
        bod = bsh + fpr;
        if (x >= bod) {
            x -= bod;
            fpr = bsh + bod;
        }
        x <<= 1;
    }while (bsh >>= 1);

    return fpr>>osh;
}


/**
 * @brief Calculate the angle based on the x and y coordinates. This function is a fast algorithm
 *         implementation, with reference address: www.RomanBlack.com (Fast XY vector to integer degree algorithm)
 *
 * @param x:  The x coordinate on a circle
 * @param y:  The y coordinate on a circle
 * @return angle 
 * @note return angle [-32767 ~ 32767]
 */
int32_t sgl_atan2_raw(int x, int y)
{
    int32_t ret;

    if (y == 0) {
        if (x > 0) {
            ret = -32767;
        }
        else {
            ret = 32767;
        }
    }
    else {
        ret = (x << 8) / y;
    }
    if (y > 0) {
        ret += 32767;
    }
    else {
        ret -= 32767;
    }

    return ret;
}


/**
 * @brief Calculate the angle based on the x and y coordinates. This function is a fast algorithm 
 *        implementation, with reference address: www.RomanBlack.com (Fast XY vector to integer degree algorithm)
 * 
 * @param x:  The x coordinate on a circle
 * @param y:  The y coordinate on a circle
 * @return angle
 * @note return angle [0 ~ 359]
*/
uint16_t sgl_atan2_angle(int x, int y)
{
    unsigned char negflag;
    unsigned char tempdegree;
    unsigned char comp;
    unsigned int degree;
    unsigned int ux;
    unsigned int uy;

    negflag = 0;
    if (x < 0) {
        negflag += 0x01;
        x = (0 - x);
    }
    ux = x;
    if (y < 0) {
        negflag += 0x02;
        y = (0 - y);
    }
    uy = y;

    if (ux > uy) {
        degree = (uy * 45) / ux;
        negflag += 0x10;
    }
    else {
        degree = (ux * 45) / uy;
    }

    comp = 0;
    tempdegree = degree;
    if (tempdegree > 22) {
        if (tempdegree <= 44) comp++;
        if (tempdegree <= 41) comp++;
        if (tempdegree <= 37) comp++;
        if (tempdegree <= 32) comp++;
    }
    else {
        if (tempdegree >= 2) comp++;
        if (tempdegree >= 6) comp++;
        if (tempdegree >= 10) comp++;
        if (tempdegree >= 15) comp++;
    }
    degree += comp;

    if (negflag & 0x10) degree = (90 - degree);

    if (negflag & 0x02) {
        if (negflag & 0x01)
            degree = (180 + degree);
        else
            degree = (180 - degree);
    }
    else {
        if (negflag & 0x01)
            degree = (360 - degree);
    }
    return degree;
}


/**
 * @brief Split the length into n parts, with the weight of each part.
 * @param weight: The weight of each part.
 * @param count: The count of parts.
 * @param length: The length to split.
 * @param gap: The gap between each part.
 * @param out: The length of each part.
 * @note The error accumulation method of Bresenham's algorithm
 */
void sgl_split_len(const uint8_t *weight, int count, int16_t length, int16_t gap, int16_t *out)
{
    int16_t total_w = 0, span = 0, accumulated = 0, error = 0;
    for (int i = 0; i < count; i++) {
        total_w += weight[i];
    }

    span = length - gap * (count + 1);

    for (int i = 0; i < count; i++) {
        int numerator = weight[i] * span;
        out[i] = numerator / total_w;
        error += numerator % total_w;

        if (error >= total_w) {
            out[i] += 1;
            error -= total_w;
        }
        accumulated += out[i];
    }

    error = span - accumulated;

    for (int i = 0; i < count && error > 0; i++) {
        out[i] += 1;
        error --;
    }

    for (int i = 0; i < count && error < 0; i++) {
        out[i] -= 1;
        error ++;
    }
}


/**
 * @brief Split the length into n parts, with the weight of each part.
 * @param length: The length to split.
 * @param count: The count of parts.
 * @param gap: The gap between each part.
 * @param out: The length of each part.
 * @note The average method of Bresenham's algorithm
 */
void sgl_split_len_avg(int length, int count, int16_t gap, int16_t *out)
{
    int16_t available_length = length - (count + 1) * gap;
    int16_t base = available_length / count;
    int16_t remainder = available_length % count;

    int16_t error = 0;
    for (int i = 0; i < count; i++) {
        out[i] = base;
        error += remainder;

        if (error > count / 2) {
            out[i] += 1;
            error -= count;
        }
    }
}


/* Random number generator */
static size_t rand_next = 1;

/**
 * @brief Generate a random number.
 * @param none
 * @return Random number.
 * @note The random number generator is in the range of [0, 32767].
 */
int16_t sgl_rand(void)
{
    rand_next = rand_next * 1103515245 + 12345;
    return (unsigned int)(rand_next / 65536) % 32768;
}


/**
 * @brief Set the seed of the random number generator.
 * @param seed: The seed of the random number generator.
 * @return none
 */
void sgl_srand(unsigned int seed)
{
    rand_next = seed;
}


/**
 * @brief append a character to the buffer
 * @param buf buffer
 * @param size buffer size
 * @param pos current position
 * @param c character to append
 */
static inline void append_char(char *buf, size_t size, size_t *pos, char c)
{
    if (*pos + 1 < size) {
        buf[*pos] = c;
    }
    (*pos)++;
}

/**
 * @brief append a string to the buffer
 * @param buf buffer
 * @param size buffer size
 * @param pos current position
 * @param s string to append
 */
static inline void append_str(char *buf, size_t size, size_t *pos, const char* s)
{
    while (*s) append_char(buf, size, pos, *s++);
}

/**
 * @brief pad alignment characters (default: space) to the buffer
 * @param buf buffer
 * @param size buffer size
 * @param pos current position
 * @param pad_len number of padding characters to append
 * @param pad_char padding character (default: ' ')
 */
static inline void pad_align(char *buf, size_t size, size_t *pos, size_t pad_len, char pad_char)
{
    for (size_t i = 0; i < pad_len; i++) {
        append_char(buf, size, pos, pad_char);
    }
}

/**
 * @brief calculate the length of an integer when converted to string
 * @param val integer value
 * @return number of characters of the integer string
 */
static inline size_t int_str_len(int val)
{
    if (val == 0) return 1; // Special case: 0 has length 1

    size_t len = 0;
    bool neg = val < 0;
    
    if (neg) val = -val;

    do {
        len++;
        val /= 10;
    } while (val);

    return neg ? len + 1 : len; // Add 1 for negative sign
}

/**
 * @brief append an integer to the buffer with left/right alignment support
 * @param buf buffer
 * @param size buffer size
 * @param pos current position
 * @param val integer to append
 * @param width alignment width (0 means no alignment)
 * @param left_align whether to use left alignment (true: left, false: right)
 */
static inline void append_int(char *buf, size_t size, size_t *pos, int val, int width, bool left_align)
{
    char tmp[64];
    int i = 0;
    bool neg = val < 0;
    size_t num_len = int_str_len(val);
    size_t pad_len = 0;

    if (width > 0 && (size_t)width > num_len) {
        pad_len = (size_t)width - num_len;
    }

    if (!left_align && pad_len > 0) {
        pad_align(buf, size, pos, pad_len, ' ');
    }

    if (neg) val = -val;

    do {
        tmp[i++] = '0' + (val % 10);
        val /= 10;
    } while (val);

    if (neg) tmp[i++] = '-';

    while (i--) append_char(buf, size, pos, tmp[i]);

    if (left_align && pad_len > 0) {
        pad_align(buf, size, pos, pad_len, ' ');
    }
}

/**
 * @brief append a hexadecimal number to the buffer
 * @param buf buffer
 * @param size buffer size
 * @param pos current position
 * @param val hexadecimal number to append
 * @param upper whether to use uppercase letters
 */
static inline void append_hex(char *buf, size_t size, size_t *pos, unsigned int val, bool upper)
{
    char tmp[32];
    int i = 0;
    const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";

    do {
        tmp[i++] = digits[val & 0xF];
        val >>= 4;
    } while (val);

    while (i--) append_char(buf, size, pos, tmp[i]);
}

/**
 * @brief append a float to the buffer
 * @param buf buffer
 * @param size buffer size
 * @param pos current position
 * @param val float to append
 * @param precision number of decimal places (-1 for default of 6)
 */
static void append_float(char *buf, size_t size, size_t *pos, double val, int precision)
{
    int int_part = (int)val;
    double frac = val - int_part;
    
    if (val < 0) {
        append_char(buf, size, pos, '-');
        int_part = -int_part;
        frac = -frac;
    }

    append_int(buf, size, pos, int_part, 0, false);
    append_char(buf, size, pos, '.');

    int prec = (precision >= 0) ? precision : 6;
    for (int i = 0; i < prec; i++) {
        frac *= 10;
        int d = (int)frac;
        append_char(buf, size, pos, '0' + d);
        frac -= d;
    }
}

/**
 * @brief format a string, a simple version of vsnprintf (with width alignment support)
 * @param buf buffer
 * @param size buffer size
 * @param fmt format string
 * @param ap argument list
 * @return number of characters written
 */
int sgl_vsnprintf(char *buf, size_t size, const char *fmt, va_list ap)
{
    size_t pos = 0;

    while (*fmt) {
        char c = *fmt;

        if (c != '%') {
            append_char(buf, size, &pos, c);
            fmt++;
            continue;
        }

        fmt++;
        bool left_align = false;
        int width = 0;
        int precision = -1;

        if (*fmt == '-') {
            left_align = true;
            fmt++;
        }

        while (*fmt >= '0' && *fmt <= '9') {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }

        if (*fmt == '.') {
            fmt++;
            precision = 0;
            while (*fmt >= '0' && *fmt <= '9') {
                precision = precision * 10 + (*fmt - '0');
                fmt++;
            }
        }

        char spec = *fmt;
        switch (spec) {
        case 's': {
            const char *s = va_arg(ap, const char*);
            append_str(buf, size, &pos, s);
            break;
        }

        case 'd': {
            int d = va_arg(ap, int);
            append_int(buf, size, &pos, d, width, left_align);
            break;
        }

        case 'x': {
            unsigned int x = va_arg(ap, unsigned int);
            append_hex(buf, size, &pos, x, false);
            break;
        }

        case 'X': {
            unsigned int x = va_arg(ap, unsigned int);
            append_hex(buf, size, &pos, x, true);
            break;
        }

        case 'f': {
            double f = va_arg(ap, double);
            append_float(buf, size, &pos, f, precision);
            break;
        }

        case 'c': {
            char ch = (char)va_arg(ap, int);
            append_char(buf, size, &pos, ch);
            break;
        }

        case '%': {
            append_char(buf, size, &pos, '%');
            break;
        }

        default:
            append_char(buf, size, &pos, '%');
            append_char(buf, size, &pos, spec);
            break;
        }

        fmt++;
    }

    if (size > 0) {
        buf[(pos < size) ? pos : (size - 1)] = '\0';
    }

    return (int)pos;
}

/**
 * @brief format a string, a simple version of snprintf (with width alignment support)
 * @param buf buffer
 * @param size buffer size
 * @param fmt format string
 * @param ... arguments
 * @return number of characters written
 */
int sgl_snprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = sgl_vsnprintf(buf, size, fmt, ap);
    va_end(ap);
    return ret;
}


/**
 * @brief register the frame buffer device
 * @param fbinfo the frame buffer device information
 * @return int, 0 if success, -1 if failed
 * @note you must check the result of this function
 */
int sgl_fbdev_register(sgl_fbinfo_t *fbinfo)
{
    if (fbinfo == NULL) {
        SGL_LOG_ERROR("You haven't set up the frame buffer device.");
        SGL_ASSERT(0);
        return -1;
    }

    if (fbinfo->buffer[0] == NULL) {
        SGL_LOG_ERROR("You haven't set up the frame buffer.");
        SGL_ASSERT(0);
        return -1;
    }

    if (fbinfo->flush_area == NULL) {
        SGL_LOG_ERROR("You haven't set up the flush area.");
        SGL_ASSERT(0);
        return -1;
    }

    if (fbinfo->buffer_size == 0) {
        SGL_LOG_ERROR("You haven't set up the frame buffer size.");
        SGL_ASSERT(0);
        return -1;
    }

    sgl_system.fbdev.fbinfo = *fbinfo;

    sgl_system.fbdev.surf.buffer = (sgl_color_t*)fbinfo->buffer[0];
    sgl_system.fbdev.surf.x1 = 0;
    sgl_system.fbdev.surf.y1 = 0;
    sgl_system.fbdev.surf.x2 = fbinfo->xres - 1;
    sgl_system.fbdev.surf.y2 = fbinfo->yres - 1;
    sgl_system.fbdev.surf.size = fbinfo->buffer_size;
    sgl_system.fbdev.surf.w = fbinfo->xres;

    sgl_system.tick_ms = 0;
    sgl_system.fbdev.fb_status = 3;
    sgl_system.fbdev.fb_swap = 0;

    return 0;
}


/**
 * @brief get pixmap format bits
 * @param pixmap pointer to pixmap
 * @return pixmap bits of per pixel
 */
uint8_t sgl_pixmal_get_bits(const sgl_pixmap_t *pixmap)
{
    SGL_ASSERT(pixmap != NULL);
    uint8_t bits = 0;
    switch (pixmap->format)
    {
    case SGL_PIXMAP_FMT_NONE:
        bits = sizeof(sgl_color_t); break;
    case SGL_PIXMAP_FMT_RGB332:
    case SGL_PIXMAP_FMT_RLE_RGB332:
        bits = 1; break;
    case SGL_PIXMAP_FMT_RGB565:
    case SGL_PIXMAP_FMT_RLE_RGB565:
        bits = 2; break;
    case SGL_PIXMAP_FMT_RGB888:
    case SGL_PIXMAP_FMT_RLE_RGB888:
        bits = 3; break;
    case SGL_PIXMAP_FMT_RLE_RGBA8888:
        bits = 4; break;
    default:
        SGL_LOG_ERROR("pixmap format error");
        break;
    }
    return bits;
}


/**
 * @brief add object to parent
 * @param parent: pointer of parent object
 * @param obj: pointer of object
 * @return none
 */
void sgl_obj_add_child(sgl_obj_t *parent, sgl_obj_t *obj)
{
    SGL_ASSERT(parent != NULL && obj != NULL);
    sgl_obj_t *tail = parent->child;

    if (parent->child) {
        while (tail->sibling != NULL) {
            tail = tail->sibling;
        };
        tail->sibling = obj;
    }
    else {
        parent->child = obj;
    }

    obj->parent = parent;
}


/**
 * @brief remove an object from its parent
 * @param obj object to remove
 * @return none
 */
void sgl_obj_remove(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);

    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *pos = NULL;

    if (parent->child != obj) {
        pos = parent->child;
        while (pos->sibling != obj) {
            pos = pos->sibling;
        }
        pos->sibling = obj->sibling;
    }
    else {
        parent->child = obj->sibling;
    }

    obj->sibling = NULL;
}


/**
 * @brief move object child position
 * @param obj point to object
 * @param ofs_x: x offset position
 * @param ofs_y: y offset position
 * @return none
 */
void sgl_obj_move_child_pos(sgl_obj_t *obj, int16_t ofs_x, int16_t ofs_y)
{
    SGL_ASSERT(obj != NULL);
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;

    if (obj->child == NULL) {
        return;
    }
    stack[top++] = obj->child;

    while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        obj->dirty = 1;
        obj->coords.x1 += ofs_x;
        obj->coords.x2 += ofs_x;
        obj->coords.y1 += ofs_y;
        obj->coords.y2 += ofs_y;

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }
}


/**
 * @brief Set object absolute position
 * @param obj point to object
 * @param abs_x: x absolute position
 * @param abs_y: y absolute position
 * @return none
 */
void sgl_obj_set_abs_pos(sgl_obj_t *obj, int16_t abs_x, int16_t abs_y)
{
    SGL_ASSERT(obj != NULL);
    int16_t x_diff = abs_x - obj->coords.x1;
    int16_t y_diff = abs_y - obj->coords.y1;

    obj->dirty = 1;
    obj->coords.x1 += x_diff;
    obj->coords.x2 += x_diff;
    obj->coords.y1 += y_diff;
    obj->coords.y2 += y_diff;

    sgl_obj_move_child_pos(obj, x_diff, y_diff);
}


/**
 * @brief zoom object size
 * @param obj point to object
 * @param zoom zoom size
 * @return none
 * @note if you want to zoom out, the zoom should be positive, if you want to zoom in, the zoom should be negative
 */
void sgl_obj_size_zoom(sgl_obj_t *obj, int16_t zoom)
{
    SGL_ASSERT(obj != NULL);
    obj->coords.x1 -= zoom;
    obj->coords.x2 += zoom;
    obj->coords.y1 -= zoom;
    obj->coords.y2 += zoom;
}


/**
 * @brief move object up a level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_up(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL;
    sgl_obj_t *next = NULL;

    /* if the object is the last child, do not move it */
    if (obj->sibling == NULL) {
        return;
    }
    else if (parent->child == obj) {
        parent->child = obj->sibling;
        obj->sibling = obj->sibling->sibling;
        /* mark object as dirty */
        sgl_obj_set_dirty(obj);
        return;
    }

    /* move the object to its next sibling */
    sgl_obj_for_each_child(prev, parent) {
        if (prev->sibling == obj) {
            next = obj->sibling;
            obj->sibling = next->sibling;
            prev->sibling = next;
            next->sibling = obj;
            /* mark object as dirty */
            sgl_obj_set_dirty(obj);
            return;
        }
    }
}


/**
 * @brief move object down a level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_down(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL;
    sgl_obj_t *gprev = NULL;

    /* if the object is the first child, do not move it */
    if (parent->child == obj || obj->sibling == NULL) {
        return;
    }
    else if (parent->child->sibling == obj) {
        parent->child->sibling = obj->sibling;
        obj->sibling = parent->child;
        parent->child = obj;
        /* mark object as dirty */
        sgl_obj_set_dirty(obj);
        return;
    }

    /* move the object to its prev sibling */
    sgl_obj_for_each_child(gprev, parent) {
        prev = gprev->sibling;

        if (prev->sibling == obj) {
            prev->sibling = obj->sibling;
            gprev->sibling = obj;
            obj->sibling = prev;
            /* mark object as dirty */
            sgl_obj_set_dirty(obj);
            return;
        }
    }
}


/**
 * @brief move object top level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_top(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL && obj->parent != NULL);

    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL, *curr = parent->child, *last = NULL;

    /* if the object is the last child, do not move it */
    if (obj->sibling == NULL) {
        return;
    }

    while (curr != NULL && curr != obj) {
        prev = curr;
        curr = curr->sibling;
    }

    if (prev == NULL) {
        parent->child = obj->sibling;
    }
    else {
        prev->sibling = obj->sibling;
    }

    last = parent->child;
    if (last == NULL) {
        parent->child = obj;
        obj->sibling = NULL;
    }
    else {
        while (last->sibling != NULL) {
            last = last->sibling;
        }
        last->sibling = obj;
        obj->sibling = NULL;
    }

    sgl_obj_set_dirty(obj);
}


/**
 * @brief move object bottom level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_bottom(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL;

    /* if the object is the first child, do not move it */
    if (parent->child == obj) {
        return;
    }

    sgl_obj_for_each_child(prev, parent) {
        if (prev->sibling == obj) {
            break;
        }
    }

    prev->sibling = obj->sibling;
    obj->sibling = parent->child;
    parent->child = obj;
    /* mark object as dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get fix radius of object
 * @param obj object
 * @param radius: radius that you want to set
 * @return none
 * @note if radius is larger than object's width or height, fix radius will be returned
 */
void sgl_obj_set_radius(sgl_obj_t *obj, size_t radius)
{
    int16_t w = (obj->coords.x2 - obj->coords.x1 + 1);
    int16_t h = (obj->coords.y2 - obj->coords.y1 + 1);
    int16_t d_min = w > h ? h : w;

    if ((int16_t)radius >= (d_min / 2)) {
        radius = sgl_is_odd(d_min) ? d_min / 2 : (d_min - 1) / 2;
    }

    obj->radius = radius & 0xFFF;
}


#if (CONFIG_SGL_OBJ_USE_NAME && CONFIG_SGL_DEBUG)
/**
 * @brief print object name that include this all child
 * @param obj point to object
 * @return none
 */
void sgl_obj_print_name(sgl_obj_t *obj)
{
    int top = 0;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    stack[top++] = obj;

    while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        if (obj->name == NULL) {
            SGL_LOG_INFO("[OBJ NAME]: %s", "NULL");
        }
        else {
            SGL_LOG_INFO("[OBJ NAME]: %s", obj->name);
        }

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }
}

#endif


/**
 * @brief page construct callback function
 * @param surf surface pointer
 * @param obj page object
 * @param evt event
 * @return none
 * @note evt not used
 */
static void sgl_page_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_page_t *page = (sgl_page_t *)obj;
    const sgl_pixmap_t *pixmap = page->pixmap;

    if (pixmap == NULL) {
        sgl_draw_fill_rect(surf, area, &obj->coords, page->color, SGL_ALPHA_MAX);
    }
    else {
        sgl_draw_fill_rect_pixmap(surf, area, &obj->coords, pixmap, SGL_ALPHA_MAX);
    }
}


/**
 * @brief set page background color
 * @param obj point to object
 * @param color background color
 * @return none
 */
void sgl_page_set_color(sgl_obj_t* obj, sgl_color_t color)
{
    sgl_page_t* page = (sgl_page_t*)obj;
    page->color = color;
    sgl_obj_set_dirty(obj);
}


/**
 * @brief set page background pixmap
 * @param obj point to object
 * @param pixmap background pixmap
 * @return none
 */
void sgl_page_set_pixmap(sgl_obj_t* obj, const sgl_pixmap_t *pixmap)
{
    sgl_page_t* page = (sgl_page_t*)obj;
    page->pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}


/**
 * @brief create a page
 * @param none
 * @return sgl_page_t* the page pointer
 */
static sgl_page_t* sgl_page_create(void)
{
    sgl_page_t *page = sgl_malloc(sizeof(sgl_page_t));
    if (page == NULL) {
        SGL_LOG_ERROR("sgl_page_create: malloc failed");
        return NULL;
    }

    /* clear the page all fields */
    memset(page, 0, sizeof(sgl_page_t));

    sgl_obj_t *obj = &page->obj;

    if (sgl_system.fbdev.fbinfo.buffer[0] == NULL) {
        SGL_LOG_ERROR("sgl_page_create: framebuffer is NULL");
        sgl_free(page);
        return NULL;
    }

    page->color = SGL_COLOR_WHITE;

    obj->parent = obj;
    obj->construct_fn = sgl_page_construct_cb;
    obj->dirty = 1;
    obj->page = 1;
    obj->border = 0;
    obj->coords = (sgl_area_t) {
        .x1 = 0,
        .y1 = 0,
        .x2 = sgl_system.fbdev.fbinfo.xres - 1,
        .y2 = sgl_system.fbdev.fbinfo.yres - 1,
    };

    /* init child list */
    sgl_obj_node_init(&page->obj);

    if (sgl_system.fbdev.active == NULL) {
        sgl_system.fbdev.active = &page->obj;
    }

    return page;
}


/**
 * @brief Create an object
 * @param parent parent object
 * @return sgl_obj_t
 * @note if parent is NULL, the object will be as an new page
 */
sgl_obj_t* sgl_obj_create(sgl_obj_t *parent)
{
    sgl_obj_t *obj;

    /* create page object */
    if (parent == NULL) {
        sgl_page_t *page = sgl_page_create();
        if (page == NULL) {
            SGL_LOG_ERROR("sgl_obj_create: create page failed");
            return NULL;
        }
        obj = &page->obj;
        return obj;
    }
    else {
        obj = (sgl_obj_t*)sgl_malloc(sizeof(sgl_obj_t));
        if (obj == NULL) {
            SGL_LOG_ERROR("malloc failed");
            return NULL;
        }

        obj->coords = parent->coords;
        obj->parent = parent;
        obj->construct_fn = NULL;
        obj->dirty = 1;

        /* init node */
        sgl_obj_node_init(obj);
        /* add the child into parent's child list */
        sgl_obj_add_child(parent, obj);

        return obj;
    }
}


/**
 * @brief initialize global dirty area
 * @param none
 * @return none
 */
static inline void sgl_dirty_area_init(void)
{
    sgl_area_init(&sgl_system.fbdev.dirty);
}


/**
 * @brief sgl global initialization
 * @param none
 * @return int, 0 means success, others means failed
 * @note you should call this function before using sgl and you should call this function after register framebuffer device
 */
int sgl_init(void)
{
    sgl_obj_t *obj = NULL;

    /* init memory pool */
    sgl_mm_init(sgl_system.mem_pool, sizeof(sgl_system.mem_pool));

    /* initialize current context */
    sgl_system.fbdev.active = NULL;

    /* initialize dirty area */
    sgl_dirty_area_init();

    /* create a screen object for drawing */
    obj = sgl_obj_create(NULL);
    if (obj == NULL) {
        SGL_LOG_ERROR("sgl_init: create screen object failed");
        return -1;
    }

    /* if the rotation is not 0 or 180, we need to alloc a buffer for rotation */
#if (CONFIG_SGL_FBDEV_ROTATION != 0)
    sgl_system.rotation = (sgl_color_t*)sgl_malloc(sgl_system.fbdev.fbinfo.buffer_size * sizeof(sgl_color_t));
    if (sgl_system.rotation == NULL) {
        SGL_LOG_ERROR("sgl_init: alloc rotation buffer failed");
        return -1;
    }
#endif
    return 0;
}


/**
 * @brief set current object as screen object
 * @param obj object, that you want to set an object as active page
 * @return none
 */
void sgl_screen_load(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_system.fbdev.active = obj;

    /* initilize framebuffer swap */
    sgl_system.fbdev.fb_swap = 0;

    /* initialize dirty area */
    sgl_dirty_area_init();
    sgl_obj_set_dirty(obj);
}


/**
 * @brief  Get area intersection between two areas
 * @param area_a: area a
 * @param area_b: area b
 * @param clip: intersection area
 * @return true: intersect, otherwise false
 * @note: this function is unsafe, you should check the area_a and area_b and clip is not NULL by yourself
 */
bool sgl_area_clip(sgl_area_t *area_a, sgl_area_t *area_b, sgl_area_t *clip)
{
    SGL_ASSERT(area_a != NULL && area_b != NULL && clip != NULL);
    if (area_b->y1 > area_a->y2 || area_b->y2 < area_a->y1 || area_b->x1 > area_a->x2 || area_b->x2 < area_a->x1) {
        return false;
    }

    clip->x1 = sgl_max(area_a->x1, area_b->x1);
    clip->x2 = sgl_min(area_a->x2, area_b->x2);
    clip->y1 = sgl_max(area_a->y1, area_b->y1);
    clip->y2 = sgl_min(area_a->y2, area_b->y2);

    return true;
}


/**
 * @brief clip area with another area
 * @param clip [in][out] clip area
 * @param area [in] area
 * @return true if clip area is valid, otherwise two area is not overlapped
 * @note: this function is unsafe, you should check the clip and area is not NULL by yourself
 */
bool sgl_area_selfclip(sgl_area_t *clip, sgl_area_t *area)
{
    SGL_ASSERT(clip != NULL && area != NULL);
    if (area->y1 > clip->y2 || area->y2 < clip->y1 || area->x1 > clip->x2 || area->x2 < clip->x1) {
        return false;
    }

    clip->x1 = sgl_max(clip->x1, area->x1);
    clip->x2 = sgl_min(clip->x2, area->x2);
    clip->y1 = sgl_max(clip->y1, area->y1);
    clip->y2 = sgl_min(clip->y2, area->y2);

    return true;
}


/**
 * @brief Computes the total boundary expansion (in Manhattan distance) required to merge rectangle b into rectangle a.
 *
 * @param a[in]    Pointer to the b rectangle
 * @param b[in]    Pointer to the a rectangle
 * @return int32_t Total expansion amount (always non-negative)
 */
static inline int32_t sgl_area_growth(sgl_area_t *a, sgl_area_t *b)
{
    return (a->x1 - sgl_min(a->x1, b->x1)) + (sgl_max(a->x2, b->x2) - a->x2)
           + (a->y1 - sgl_min(a->y1, b->y1)) + (sgl_max(a->y2, b->y2) - a->y2);
}


/**
 * @brief Quickly determines if two rectangles are close enough to be merged.
 *
 * This fast heuristic is useful in performance-critical contexts (e.g., real-time segmentation or region merging)
 * to avoid excessive fragmentation while preventing merges between distant regions.
 *
 * @param a[in] Pointer to the first rectangle
 * @param b[in] Pointer to the second rectangle
 * @return bool true if the rectangles are sufficiently close for merging; false otherwise
 */
static inline bool sgl_merge_determines(sgl_area_t* a, sgl_area_t* b)
{
    int16_t gap_x = (a->x1 > b->x2) ? (a->x1 - b->x2) : (b->x1 > a->x2) ? (b->x1 - a->x2) : 0;
    int16_t gap_y = (a->y1 > b->y2) ? (a->y1 - b->y2) : (b->y1 > a->y2) ? (b->y1 - a->y2) : 0;    
    int16_t threshold = (sgl_min4(a->x2 - a->x1 + 1, a->y2 - a->y1 + 1, b->x2 - b->x1 + 1, b->y2 - b->y1 + 1) >> 2);

    return (gap_x <= threshold) && (gap_y <= threshold);
}


/**
 * @brief merge an area into global dirty area
 * 
 * This function calculates how much rectangle 'a' would need to grow in each direction (left, right, top, bottom)
 * to fully enclose both 'a' and 'b'. The result is the sum of the expansions along all four sides.
 * Note: This is not the increase in area, th is a lightweight heuristic for merge cost in bounding-box algorithms.
 * 
 * @param area [in] Pointer to the area
 * @return none
 */
void sgl_dirty_area_push(sgl_area_t *area)
{
    SGL_ASSERT(area != NULL);
    /* merge object area into best_idx dirty area */
    sgl_area_selfmerge(&sgl_system.fbdev.dirty, area);
}


/**
 * @brief initialize object
 * @param obj object
 * @param parent parent object
 * @return int, 0 means successful, -1 means failed
 */
int sgl_obj_init(sgl_obj_t *obj, sgl_obj_t *parent)
{
    SGL_ASSERT(obj != NULL);

    if (parent == NULL) {
        parent = sgl_screen_act();
        if (parent == NULL) {
            SGL_LOG_ERROR("sgl_obj_init: have no active page");
            return -1;
        }
    }

    /* set essential member */
    obj->coords = parent->coords;
    obj->parent = parent;
    obj->construct_fn = NULL;
    obj->dirty = 1;

    /* add the child into parent's child list */
    sgl_obj_add_child(parent, obj);

    return 0;
}


/**
 * @brief  free an object
 * @param  obj: object to free
 * @retval none
 * @note this function will free all the children of the object
 */
void sgl_obj_free(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    stack[top++] = obj;

    while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}

        sgl_free(obj);
    }
}


/**
 * @brief delete object
 * @param obj point to object
 * @return none
 * @note this function will set object and his childs to be destroyed, then next draw cycle, the object will be removed.
 *       if object is NULL, the all objects of active page will be delete, but the page object will not be deleted.
 *       if object is a page, the page object will be deleted and all its children will be deleted.
 */
void sgl_obj_delete(sgl_obj_t *obj)
{
    if (obj == NULL || obj == sgl_screen_act()) {
        obj = sgl_screen_act();
        sgl_dirty_area_push(&obj->coords);
        if (obj->child) {
            sgl_obj_free(obj->child);
        }
        sgl_obj_node_init(obj);
        return;
    }
    else if (obj->page == 1) {
        sgl_obj_free(obj);
        return;
    }

    sgl_obj_set_destroyed(obj);
    sgl_obj_set_dirty(obj);
}


/**
 * @brief Convert UTF-8 string to Unicode
 * @param utf8_str Pointer to the UTF-8 string to be converted
 * @param p_unicode_buffer Pointer to the buffer where the converted Unicode will be stored
 * @return The number of bytes in the UTF-8 string
 */
uint32_t sgl_utf8_to_unicode(const char *utf8_str, uint32_t *p_unicode_buffer)
{
    int bytes = 0;
    if (((uint8_t)(*utf8_str)) < 0x80) { // 1-byte/7-bit ASCII
        bytes = 1;
        *p_unicode_buffer = utf8_str[0];
    }
    else if ((((uint8_t)(*utf8_str)) & 0xE0) == 0xC0) { // 2-byte
        bytes = 2;
        *p_unicode_buffer = (utf8_str[0] & 0x1F) << 6;
        *p_unicode_buffer |= (utf8_str[1] & 0x3F);
    }
    else if ((((uint8_t)(*utf8_str)) & 0xF0) == 0xE0) { // 3-byte
        bytes = 3;
        *p_unicode_buffer = (utf8_str[0] & 0x0F) << 12;
        *p_unicode_buffer |= (utf8_str[1] & 0x3F) << 6;
        *p_unicode_buffer |= (utf8_str[2] & 0x3F);
    }
    else if ((((uint8_t)(*utf8_str)) & 0xF8) == 0xF0) { // 4-byte
        bytes = 4;
        *p_unicode_buffer = (utf8_str[0] & 0x07) << 18;
        *p_unicode_buffer |= (utf8_str[2] & 0x3F) << 6;
        *p_unicode_buffer |= (utf8_str[1] & 0x3F) << 12;
        *p_unicode_buffer |= (utf8_str[3] & 0x3F);
    }
    return bytes;
}


/**
 * @brief Search for the index of a Unicode character in the font table
 * @param font Pointer to the font structure containing character data
 * @param unicode Unicode of the character to be searched
 * @return Index of the character in the font table
 */
uint32_t sgl_search_unicode_ch_index(const sgl_font_t *font, uint32_t unicode)
{
    uint32_t left = 0, right = 0, mid = 0;
    uint32_t target = unicode;
    const sgl_font_unicode_t *code = font->unicode;

    for (uint32_t i = 1; i < font->unicode_num; i ++) {
        if (target < (code->offset + code->len)) {
            break;
        }
        code ++;
    }

    target -= code->offset;

    if (code->list == NULL) {
        if (target >= code->len) {
            SGL_LOG_WARN("sgl_search_unicode_ch_index: [0x%x]unicode not found in font table", unicode);
            return 0;
        }
        return target + code->tab_offset;
    }

    right = code->len - 1;
    while (left <= right) {
        mid = left + (right - left) / 2;

        if (code->list[mid] == target) {
            return mid + code->tab_offset;
        }
        else if (code->list[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    SGL_LOG_WARN("sgl_search_unicode_ch_index: [0x%x]unicode not found in font table", unicode);
    return 0;
}


/**
 * @brief get the width of a string
 * @param str string
 * @param font sgl font
 * @return width of string
 */
int32_t sgl_font_get_string_width(const char *str, const sgl_font_t *font)
{
    SGL_ASSERT(font != NULL);
    int32_t len = 0;
    uint32_t unicode = 0;
    uint32_t ch_index = 0;
    while (*str) {
        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);
        len += (font->table[ch_index].adv_w >> 4);
    }
    return len;
}


/**
 * @brief get the height of a string, which is in a rect area
 * @param width width of the rect area
 * @param str string
 * @param font sgl font of the string
 * @param line_space peer line space
 * @return height size of string
 */
int32_t sgl_font_get_string_height(int16_t width, const char *str, const sgl_font_t *font, uint8_t line_space)
{
    int16_t offset_x = 0;
    int16_t ch_index;
    int16_t ch_width;
    int16_t lines = 1;
    uint32_t unicode = 0;

    while (*str) {
        if (*str == '\n') {
            lines ++;
            offset_x = 0;
            str ++;
            continue;
        }

        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);

        ch_width = (font->table[ch_index].adv_w >> 4);

        if ((offset_x + ch_width) >= width) {
            offset_x = 0;
            lines ++;
        }

        offset_x += ch_width;
    }

    return lines * (font->font_height + line_space);
}


/**
 * @brief get the alignment position
 * @param parent_size parent size
 * @param size object size
 * @param type alignment type
 * @return alignment position offset
 */
sgl_pos_t sgl_get_align_pos(sgl_size_t *parent_size, sgl_size_t *size, sgl_align_type_t type)
{
    SGL_ASSERT(parent_size != NULL && size != NULL);
    sgl_pos_t ret = {.x = 0, .y = 0};

    switch (type) {
    case SGL_ALIGN_CENTER:
        ret.x = (parent_size->w - size->w) / 2;
        ret.y = (parent_size->h - size->h) / 2;
    break;

    case SGL_ALIGN_TOP_MID:
        ret.x = (parent_size->w - size->w) / 2;
        ret.y = 0;
    break;

    case SGL_ALIGN_TOP_LEFT:
        ret.x = 0;
        ret.y = 0;
    break;

    case SGL_ALIGN_TOP_RIGHT:
        ret.x = parent_size->w - size->w;
        ret.y = 0;
    break;

    case SGL_ALIGN_BOT_MID:
        ret.x = (parent_size->w - size->w) / 2;
        ret.y = parent_size->h - size->h;
    break;

    case SGL_ALIGN_BOT_LEFT:
        ret.x = 0;
        ret.y = parent_size->h - size->h;
    break;

    case SGL_ALIGN_BOT_RIGHT:
        ret.x = parent_size->w - size->w;
        ret.y = parent_size->h - size->h;
    break;

    case SGL_ALIGN_LEFT_MID:
        ret.x = 0;
        ret.y = (parent_size->h - size->h) / 2;
    break;

    case SGL_ALIGN_RIGHT_MID:
        ret.x = parent_size->w - size->w;
        ret.y = (parent_size->h - size->h) / 2;
    break;

    default: break;
    }
    return ret;
}


/**
 * @brief get the text position in the area
 * @param area point to area
 * @param font point to font
 * @param text text string
 * @param offset text offset
 * @param type alignment type
 * @return sgl_pos_t position of text
 */
sgl_pos_t sgl_get_text_pos(sgl_area_t *area, const sgl_font_t *font, const char *text, int16_t offset, sgl_align_type_t type)
{
    SGL_ASSERT(area != NULL && font != NULL);
    sgl_pos_t ret = {.x = 0, .y = 0};
    sgl_size_t parent_size = {
        .w = area->x2 - area->x1 + 1,
        .h = area->y2 - area->y1 + 1,
    };

    sgl_size_t text_size = {
        .w = sgl_font_get_string_width(text, font) + offset,
        .h = sgl_font_get_height(font),
    };

    ret = sgl_get_align_pos(&parent_size, &text_size, type);
    ret.x += area->x1;
    ret.y += area->y1;

    return ret;
}


/**
 * @brief get the icon position of area
 * @param area point to area
 * @param icon point to icon
 * @param offset offset
 * @param type align type
 */
sgl_pos_t sgl_get_icon_pos(sgl_area_t *area, const sgl_icon_pixmap_t *icon, int16_t offset, sgl_align_type_t type)
{
    SGL_ASSERT(area != NULL && icon != NULL);
    sgl_pos_t ret = {.x = 0, .y = 0};
    sgl_size_t parent_size = {
        .w = area->x2 - area->x1 + 1,
        .h = area->y2 - area->y1 + 1,
    };

    sgl_size_t text_size = {
        .w = icon->width + offset,
        .h = icon->height,
    };

    ret = sgl_get_align_pos(&parent_size, &text_size, type);
    ret.x += area->x1;
    ret.y += area->y1;

    return ret;
}


/**
 * @brief Set the alignment position of the object relative to its parent object.
 * @param obj The object to set the alignment position.
 * @param type The alignment type.
 * @return none
 * @note type should be one of the sgl_align_type_t values:
 *       - SGL_ALIGN_CENTER    : Center the object in the parent object.
 *       - SGL_ALIGN_TOP_MID   : Align the object at the top middle of the parent object.
 *       - SGL_ALIGN_TOP_LEFT  : Align the object at the top left of the parent object.
 *       - SGL_ALIGN_TOP_RIGHT : Align the object at the top right of the parent object.
 *       - SGL_ALIGN_BOT_MID   : Align the object at the bottom middle of the parent object.
 *       - SGL_ALIGN_BOT_LEFT  : Align the object at the bottom left of the parent object.
 *       - SGL_ALIGN_BOT_RIGHT : Align the object at the bottom right of the parent object.
 *       - SGL_ALIGN_LEFT_MID  : Align the object at the left middle of the parent object.
 *       - SGL_ALIGN_RIGHT_MID : Align the object at the right middle of the parent object.
 */
void sgl_obj_set_pos_align(sgl_obj_t *obj, sgl_align_type_t type)
{
    SGL_ASSERT(obj != NULL);

    sgl_size_t p_size   = {0};
    sgl_pos_t  p_pos    = {0};
    sgl_pos_t  obj_pos  = {0};
    sgl_size_t obj_size = {
        .w = obj->coords.x2 - obj->coords.x1 + 1,
        .h = obj->coords.y2 - obj->coords.y1 + 1,
    };

    p_size = (sgl_size_t){
        .w = obj->parent->coords.x2 - obj->parent->coords.x1 + 1,
        .h = obj->parent->coords.y2 - obj->parent->coords.y1 + 1,
    };
    p_pos = (sgl_pos_t){
        .x = obj->parent->coords.x1,
        .y = obj->parent->coords.y1,
    };

    obj_pos = sgl_get_align_pos(&p_size, &obj_size, type);

    sgl_obj_set_abs_pos(obj, p_pos.x + obj_pos.x, p_pos.y + obj_pos.y);
}


/**
 * @brief Set the alignment position of the object relative to sibling object.
 * @param ref The reference object, it should be the sibling object.
 * @param obj The object to set the alignment position.
 * @param type The alignment type.
 * @return none
 * @note type should be one of the sgl_align_type_t values:
 *       - SGL_ALIGN_VERT_LEFT  : Align the object at the left side of the reference object.
 *       - SGL_ALIGN_VERT_RIGHT : Align the object at the right side of the reference object.
 *       - SGL_ALIGN_VERT_MID   : Align the object at the middle of the reference object.
 *       - SGL_ALIGN_HORIZ_TOP  : Align the object at the top side of the reference object.
 *       - SGL_ALIGN_HORIZ_BOT  : Align the object at the bottom side of the reference object.
 *       - SGL_ALIGN_HORIZ_MID  : Align the object at the middle of the reference object.
 */
void sgl_obj_set_pos_align_ref(sgl_obj_t *ref, sgl_obj_t *obj, sgl_align_type_t type)
{
    SGL_ASSERT(ref != NULL && obj != NULL);

    if (unlikely(ref == obj->parent)) {
        sgl_obj_set_pos_align(obj, type);
        return;
    }

    int16_t ref_w = ref->coords.x2 - ref->coords.x1 + 1;
    int16_t obj_w = obj->coords.x2 - obj->coords.x1 + 1;
    int16_t ref_h = ref->coords.y2 - ref->coords.y1 + 1;
    int16_t obj_h = obj->coords.y2 - obj->coords.y1 + 1;

    switch (type) {
    case SGL_ALIGN_VERT_MID:
        obj->coords.x1 = ref->coords.x1 + (ref_w - obj_w) / 2;
        obj->coords.x2 = obj->coords.x1 + obj_w - 1;
    break;

    case SGL_ALIGN_VERT_LEFT:
        obj->coords.x1 = ref->coords.x1;
        obj->coords.x2 = obj->coords.x1 + obj_w - 1;
    break;

    case SGL_ALIGN_VERT_RIGHT:
        obj->coords.x1 = ref->coords.x2 - obj_w;
        obj->coords.x2 = obj->coords.x1 + obj_w - 1;
    break;

    case SGL_ALIGN_HORIZ_MID:
        obj->coords.y1 = ref->coords.y1 + (ref_h - obj_h) / 2;
        obj->coords.y2 = obj->coords.y1 + obj_h - 1;
    break;

    case SGL_ALIGN_HORIZ_TOP:
        obj->coords.y1 = ref->coords.y1;
        obj->coords.y2 = obj->coords.y1 + obj_h - 1;
    break;

    case SGL_ALIGN_HORIZ_BOT:
        obj->coords.y1 = ref->coords.y2 - obj_h;
        obj->coords.y2 = obj->coords.y1 + obj_h - 1;
    break;

    default:
        SGL_LOG_WARN("invalid align type");
    break;
    }
}


/**
 * @brief draw object slice completely
 * @param obj it should point to active root object
 * @param surf surface that draw to
 * @param dirty_h dirty height
 * @return none
 */
static inline void draw_obj_slice(sgl_obj_t *obj, sgl_surf_t *surf, sgl_area_t *area)
{
    int top = 0;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];

	SGL_ASSERT(obj != NULL);
	stack[top++] = obj;

	while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

        if (sgl_obj_is_hidden(obj)) {
            continue;
        }

		if (sgl_surf_area_is_overlap(surf, &obj->coords)) {
			SGL_ASSERT(obj->construct_fn != NULL);
			obj->construct_fn(surf, obj, area);

            if (obj->child != NULL) {
                stack[top++] = obj->child;
            }
		}
	}

    /* flush dirty area into screen */
    sgl_fbdev_flush_area((sgl_area_t*)surf, surf->buffer);
}


/**
 * @brief calculate dirty area by for each all object that is dirty and visible
 * @param obj it should point to active root object
 * @return bool true if dirty area is changed
 * @note if there is no dirty area, the dirty area will remain unchanged
 */
static inline bool sgl_dirty_area_calculate(sgl_obj_t *obj)
{
    bool changed = false;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    stack[top++] = obj;

    /* for each all object from the first task of page */
	while (top > 0) {
        SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        /* if sibling exists, push it to stack, it will be pop in next loop */
		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

        /* if object is hidden, skip it */
        if (unlikely(sgl_obj_is_hidden(obj))) {
            continue;
        }

        /* check if obj is destroyed */
        if (unlikely(sgl_obj_is_destroyed(obj))) {
            /* merge destroy area */
            sgl_dirty_area_push(&obj->coords);

            /* remove obj from parent */
            sgl_obj_remove(obj);

            /* free obj resource */
            sgl_obj_free(obj);

            changed = true;
            /* object is destroyed, skip */
            continue;
        }

        /* check child dirty and merge all dirty area */
        if (sgl_obj_is_dirty(obj)) {
            /* merge dirty area */
            sgl_dirty_area_push(&obj->coords);

            changed = true;
            /* clear dirty flag */
            sgl_obj_clear_dirty(obj);
        }

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }

    return changed;
}


/**
 * @brief sgl to draw complete frame
 * @param fbdev point to  frame buffer device
 * @return none
 * @note this function should be called in deamon thread or cyclic thread
 */
static inline void sgl_draw_task(sgl_fbdev_t *fbdev)
{
    sgl_surf_t *surf = &fbdev->surf;
    sgl_obj_t  *head = fbdev->active;
    sgl_area_t *dirty = NULL;

    /* dirty area number must less than SGL_DIRTY_AREA_MAX */
    dirty = &fbdev->dirty;
    sgl_area_selfclip(dirty, &head->coords);
    surf->dirty = dirty;

    /* check dirty area, ensure it is valid */
    SGL_ASSERT(dirty != NULL && dirty->x1 >= 0 && dirty->y1 >= 0 && dirty->x2 < SGL_SCREEN_WIDTH && dirty->y2 < SGL_SCREEN_HEIGHT);

    uint16_t draw_h = 0;
    surf->h = dirty->y2 - dirty->y1 + 1;

    surf->x1 = dirty->x1;
    surf->y1 = dirty->y1;
    surf->x2 = dirty->x2;
    surf->w  = surf->x2 - surf->x1 + 1;
    surf->h  = sgl_min(surf->size / surf->w, (uint32_t)(dirty->y2 - dirty->y1 + 1));

    SGL_LOG_TRACE("[fb:%d]sgl_draw_task: dirty area  x1:%d y1:%d x2:%d y2:%d", fbdev->fb_swap, dirty->x1, dirty->y1, dirty->x2, dirty->y2);

    while (surf->y1 <= dirty->y2) {
        draw_h = sgl_min(dirty->y2 - surf->y1 + 1, surf->h);
        surf->y2 = surf->y1 + draw_h - 1;

        /* wait current framebuffer for ready */
        while (sgl_fbdev_flush_wait_ready(fbdev));

        /* reset current framebuffer ready flag */
        fbdev->fb_status = (fbdev->fb_status & (2 - fbdev->fb_swap));

        /* draw object slice until the dirty area is finished */
        draw_obj_slice(head, surf, dirty);
        surf->y1 += draw_h;
    }

    /* clear dirty area */
    sgl_dirty_area_init();
}


/**
 * @brief sgl task handle function with sync mode
 * @param none
 * @return none
 * @note you can call this function for force update screen
 */
void sgl_task_handle_sync(void)
{
#if (CONFIG_SGL_ANIMATION)
    sgl_anim_task();
#endif // !CONFIG_SGL_ANIMATION
    sgl_tick_reset();

    /* foreach all object tree and calculate dirty area */
    if (sgl_dirty_area_calculate(sgl_system.fbdev.active)) {
        /* draw all object into screen */
        sgl_draw_task(&sgl_system.fbdev);
    }
}
