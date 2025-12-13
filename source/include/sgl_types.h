/* source/include/sgl_types.h
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Document reference link: docs directory
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __SGL_TYPES_H__
#define __SGL_TYPES_H__


#include <stdint.h>
#include <stdbool.h>
#include <sgl_cfgfix.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Get the offset of internal members of the structure
 * 
 * @struct_t: structure typedef 
 * @member: member in structure
 * 
 * @return offset of member in the structure
*/
#define sgl_offsetof(struct_t, member)          ((size_t)&((struct_t*)0)->member)


/**
 * @brief Get the address of the structure instance.
 *
 * @ptr: address of the structure member.
 * @type: type of the structure.
 * @member: member name of the ptr in structure.
 * @return pointer to address of structure 
 */
#define sgl_container_of(ptr, type, member)     ((type *)((char *)ptr - sgl_offsetof(type, member)))


#ifndef SGL_ARRAY_SIZE
#define SGL_ARRAY_SIZE(array)                   (sizeof(array) / sizeof(array[0]))
#endif


#define SGL_ALIGN_UP(x, align)                  (((x) + ((align) - 1)) & ~((align) - 1))
#define SGL_ROUND_UP(x, round)                  ((((x) + ((round) - 1)) / (round)) * (round))

#define SGL_UNUSED(x)                           (void)(x)


#define SGL_POS_INVALID                         (0xefff)
#define SGL_POS_MAX                             (8192)
#define SGL_POS_MIN                             (-8192)
#define SGL_RADIUS_INVALID                      (0xFFF)

#define SGL_AREA_MAX                            {.x1 = SGL_POS_MIN, .y1 = SGL_POS_MIN, .x2 = SGL_POS_MAX, .y2 = SGL_POS_MAX}
#define SGL_AREA_INVALID                        {.x1 = SGL_POS_MAX, .y1 = SGL_POS_MAX, .x2 = SGL_POS_MIN, .y2 = SGL_POS_MIN}

#define SGL_WIDTH_INVALID                       (-1)
#define SGL_WIDTH_MAX                           (8192)
#define SGL_WIDTH_MIN                           (0)
#define SGL_HEIGHT_INVALID                      (-1)
#define SGL_HEIGHT_MAX                          (8192)
#define SGL_HEIGHT_MIN                          (0)


#define SGL_DIRECT_HORIZONTAL                   (0)
#define SGL_DIRECT_VERTICAL                     (1)


#define SGL_ALPHA_MAX                           (255)
#define SGL_ALPHA_MIN                           (0)
#define SGL_ALPHA_NUM                           (256)


#define SGL_STYLE_OK                            (0)
#define SGL_STYLE_FAILED                        (UINT32_MAX)
#define SGL_STYLE_INVALID                       (UINT32_MAX)


#define SGL_COLOR_RGB233                        (8)
#define SGL_COLOR_RGB565                        (16)
#define SGL_COLOR_RGB888                        (24)
#define SGL_COLOR_ARGB8888                      (32)


#ifdef __GNUC__            /* gcc compiler   */
#ifndef likely
#  define likely(x)                             __builtin_expect(!!(x), 1)
#  define unlikely(x)                           __builtin_expect(!!(x), 0)
#endif
#elif defined(__clang__)   /* clang compiler */
#ifndef likely
#  define likely(x)                             __builtin_expect(!!(x), 1)
#  define unlikely(x)                           __builtin_expect(!!(x), 0)
#endif
#elif defined(__CC_ARM)    /* RealView compiler (Keil ARMCC) */
#ifndef likely
#  define likely(x)                             __builtin_expect(!!(x), 1)
#  define unlikely(x)                           __builtin_expect(!!(x), 0)
#endif
#elif defined(__ICCARM__)  /* IAR compiler    */
#ifndef likely
#  define likely(x)                             __iar_builtin_expect(!!(x), 1)
#  define unlikely(x)                           __iar_builtin_expect(!!(x), 0)
#endif
#elif defined(_MSC_VER)    /* MSVC compiler   */
#ifndef likely
#  define likely(x)                             (x)
#  define unlikely(x)                           (x)
#endif
#elif defined(__MINGW32__) /* MinGW compiler  */
#ifndef likely
#define likely(x)                               __builtin_expect(!!(x), 1)
#define unlikely(x)                             __builtin_expect(!!(x), 0)
#endif
#else                      /* others compiler */
#ifndef likely
#  define likely(x)                             (x)
#  define unlikely(x)                           (x)
#endif
#endif


#define  sgl_check_ptr_break(ptr)               if (unlikely((ptr) == NULL)) { SGL_LOG_ERROR("Function: %s, Line: %d, "#ptr" is NULL", __func__, __LINE__); return;}
#define  sgl_check_ptr_return(ptr, r)           if (unlikely((ptr) == NULL)) { SGL_LOG_ERROR("Function: %s, Line: %d, "#ptr" is NULL", __func__, __LINE__); return (r);}


/**
 * @brief Run once function
 * @note you must sure the statement is thread safe
 */
#define  SGL_RUN_ONCE(statement)                do { \
                                                    static bool _run_once_flag = false;     \
                                                    if (unlikely(_run_once_flag == false)){ \
                                                        statement;                          \
                                                        _run_once_flag = true;              \
                                                    }                                       \
                                                } while(0)


// prototype: sgl_rgb(uint8_t r, uint8_t g, uint8_t b)
#if (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_ARGB8888 || CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_RGB888)
#define sgl_rgb(r,g,b)                          (sgl_color_t){ .ch.blue    = (b),              \
                                                               .ch.green   = (g),              \
                                                               .ch.red     = (r),}
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_RGB565)
#define sgl_rgb(r,g,b)                          (sgl_color_t){ .ch.blue    = (b) >> 3,         \
                                                               .ch.green   = (g) >> 2,         \
                                                               .ch.red     = (r) >> 3,}
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_RGB233)
#define sgl_rgb(r,g,b)                          (sgl_color_t){ .ch.blue    = (b >> 6),         \
                                                               .ch.green   = (g >> 5),         \
                                                               .ch.red     = (r >> 5),}
#endif


/**
 * Define some basic colors
*/
#define SGL_COLOR_RED                           sgl_rgb(0xFF, 0, 0)
#define SGL_COLOR_GREEN                         sgl_rgb(0, 0xFF, 0)
#define SGL_COLOR_BLUE                          sgl_rgb(0, 0, 0xFF)
#define SGL_COLOR_WHITE                         sgl_rgb(0xFF, 0xFF, 0xFF)
#define SGL_COLOR_BLACK                         sgl_rgb(0, 0, 0)
#define SGL_COLOR_CYAN                          sgl_rgb(0, 0xFF, 0xFF)
#define SGL_COLOR_MAGENTA                       sgl_rgb(0xFF, 0, 0xFF)
#define SGL_COLOR_YELLOW                        sgl_rgb(0xFF, 0xFF, 0)
#define SGL_COLOR_GRAY                          sgl_rgb(0x80, 0x80, 0x80)
#define SGL_COLOR_DARK_GRAY                     sgl_rgb(0x40, 0x40, 0x40)
#define SGL_COLOR_LIGHT_GRAY                    sgl_rgb(0xC0, 0xC0, 0xC0)
#define SGL_COLOR_BRIGHT_PURPLE                 sgl_rgb(0xFF, 0x00, 0xFF)
#define SGL_COLOR_BRIGHT_BLUE                   sgl_rgb(0x00, 0xFF, 0xFF)
#define SGL_COLOR_ORANGE                        sgl_rgb(0xFF, 0xA5, 0x00)
#define SGL_COLOR_DARK_ORANGE                   sgl_rgb(0xFF, 0x8C, 0x00)
#define SGL_COLOR_RED_ORANGE                    sgl_rgb(0xFF, 0x45, 0x00)
#define SGL_COLOR_GOLD                          sgl_rgb(0xFF, 0xD7, 0x00)
#define SGL_COLOR_GOLDENROD                     sgl_rgb(0xDA, 0xA5, 0x20)
#define SGL_COLOR_BROWN                         sgl_rgb(0xA5, 0x2A, 0x2A)
#define SGL_COLOR_SADDLE_BROWN                  sgl_rgb(0x8B, 0x45, 0x13)
#define SGL_COLOR_TOMATO                        sgl_rgb(0xFF, 0x63, 0x47)
#define SGL_COLOR_CORAL                         sgl_rgb(0xFF, 0x7F, 0x50)
#define SGL_COLOR_SALMON                        sgl_rgb(0xFA, 0x80, 0x72)
#define SGL_COLOR_LIME                          sgl_rgb(0x32, 0xCD, 0x32)
#define SGL_COLOR_OLIVE                         sgl_rgb(0x80, 0x80, 0x00)
#define SGL_COLOR_DARK_OLIVE_GREEN              sgl_rgb(0x55, 0x6B, 0x2F)
#define SGL_COLOR_FOREST_GREEN                  sgl_rgb(0x22, 0x8B, 0x2F)
#define SGL_COLOR_DARK_GREEN                    sgl_rgb(0x00, 0x64, 0x00)
#define SGL_COLOR_LAWN_GREEN                    sgl_rgb(0x7C, 0xFC, 0x00)
#define SGL_COLOR_CHARTREUSE                    sgl_rgb(0x7F, 0xFF, 0x00)
#define SGL_COLOR_YELLOW_GREEN                  sgl_rgb(0x9A, 0xCD, 0x32)
#define SGL_COLOR_SPRING_GREEN                  sgl_rgb(0x00, 0xFF, 0x7F)
#define SGL_COLOR_DEEP_SKY_BLUE                 sgl_rgb(0x00, 0xBF, 0xFF)
#define SGL_COLOR_DODGER_BLUE                   sgl_rgb(0x1E, 0x90, 0xFF)
#define SGL_COLOR_ROYAL_BLUE                    sgl_rgb(0x41, 0x69, 0xE1)
#define SGL_COLOR_MIDNIGHT_BLUE                 sgl_rgb(0x19, 0x19, 0x70)
#define SGL_COLOR_NAVY                          sgl_rgb(0x00, 0x00, 0x80)
#define SGL_COLOR_TEAL                          sgl_rgb(0x00, 0x80, 0x80)
#define SGL_COLOR_DARK_CYAN                     sgl_rgb(0x00, 0x8B, 0x8B)
#define SGL_COLOR_LIGHT_SEA_GREEN               sgl_rgb(0x20, 0xB2, 0xAA)
#define SGL_COLOR_CADET_BLUE                    sgl_rgb(0x5F, 0x9E, 0xA0)
#define SGL_COLOR_INDIGO                        sgl_rgb(0x4B, 0x00, 0x82)
#define SGL_COLOR_VIOLET                        sgl_rgb(0xEE, 0x82, 0xEE)
#define SGL_COLOR_DARK_VIOLET                   sgl_rgb(0x94, 0x00, 0xD3)
#define SGL_COLOR_MEDIUM_VIOLET_RED             sgl_rgb(0xC7, 0x15, 0x85)
#define SGL_COLOR_PINK                          sgl_rgb(0xFF, 0xC0, 0xCB)
#define SGL_COLOR_HOT_PINK                      sgl_rgb(0xFF, 0x69, 0xB4)
#define SGL_COLOR_DEEP_PINK                     sgl_rgb(0xFF, 0x14, 0x93)
#define SGL_COLOR_PLUM                          sgl_rgb(0xDD, 0xA0, 0xDD)
#define SGL_COLOR_ORCHID                        sgl_rgb(0xDA, 0x70, 0xD6)
#define SGL_COLOR_TAN                           sgl_rgb(0xD2, 0xB4, 0x8C)
#define SGL_COLOR_WHEAT                         sgl_rgb(0xF5, 0xDE, 0xB3)
#define SGL_COLOR_SAND                          sgl_rgb(0xC2, 0xB2, 0x90)
#define SGL_COLOR_BEIGE                         sgl_rgb(0xF5, 0xF5, 0xDC)
#define SGL_COLOR_IVORY                         sgl_rgb(0xFF, 0xFF, 0xF0)
#define SGL_COLOR_LAVENDER                      sgl_rgb(0xE6, 0xE6, 0xFA)
#define SGL_COLOR_MISTY_ROSE                    sgl_rgb(0xFF, 0xE4, 0xE1)
#define SGL_COLOR_SEASHELL                      sgl_rgb(0xFF, 0xF5, 0xEE)
#define SGL_COLOR_FLORAL_WHITE                  sgl_rgb(0xFF, 0xFA, 0xF0)


#ifdef __cplusplus
}
#endif

#endif //__SGL_TYPES_H__
