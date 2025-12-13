/* source/include/sgl_cfgfix.h
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

#ifndef __SGL_CFGFIX_H__
#define __SGL_CFGFIX_H__

#include <sgl_config.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief sgl config fix file, it will fix the macro if not defined
 * @note please add the macro in this file if you want to add new macro into source code
 * 
 * @description:
 * 
 * CONFIG_SGL_PANEL_PIXEL_DEPTH:
 *      The pixel depth of panel, it will be used to define the color type
 *
 * CONFIG_SGL_USE_FULL_FB:
 *      If you want to use full framebuffer, please define this macro to 1
 *
 * CONFIG_SGL_SYSTICK_MS:
 *      The macro should be defined to the system tick ms, default: 10
 * 
 * CONFIG_SGL_COLOR16_SWAP:
 *      Its for 16 bit color, the color will be swapped
 * 
 * CONFIG_SGL_EVENT_QUEUE_SIZE:
 *      the size of event queue, default: 32
 * 
 * CONFIG_SGL_OBJ_SLOT_DYNAMIC
 *      If the object slot is dynamic, the object slot size will be dynamic allocated, otherwise, the object 
 *      slot size will be static allocated that you should define CONFIG_SGL_OBJ_NUM_MAX macro
 * 
 * CONFIG_SGL_OBJ_NUM_MAX:
 *      If CONFIG_SGL_OBJ_SLOT_DYNAMIC is 0 or not defined, you should define CONFIG_SGL_OBJ_NUM_MAX macro
 * 
 * CONFIG_SGL_ANIMATION:
 *      If you want to use animation, please define this macro to 1
 * 
 * CONFIG_SGL_DEBUG:
 *      If you want to use debug, please define this macro to 1
 * 
 * CONFIG_SGL_EXTERNAL_PIXMAP:
 *      If you want to use external pixmap, please define this macro to 1
 * 
 * CONFIG_SGL_USE_OBJ_ID:
 *      If you want to use obj id, please define this macro to 1, at mostly, the CONFIG_SGL_USE_OBJ_ID should be 0
 * 
 * CONFIG_SGL_HEAP_ALGO:
 *      The heap algorithm, default: lwmem
 * 
 * CONFIG_SGL_HEAP_MEMORY_SIZE:
 *      The heap memory size, default: 10240
 * 
 * CONFIG_SGL_FONT_SONG23:
 *      If you want to use font song23, please define this macro to 1
 * 
 * CONFIG_SGL_FONT_CONSOLAS23:
 *      If you want to use font consolas23, please define this macro to 1
 * 
 */

#ifndef CONFIG_SGL_PANEL_PIXEL_DEPTH
#define CONFIG_SGL_PANEL_PIXEL_DEPTH                               (16)
#endif

#ifndef CONFIG_SGL_USE_FULL_FB
#define CONFIG_SGL_USE_FULL_FB                                     (0)
#endif

#ifndef CONFIG_SGL_SYSTICK_MS
#define CONFIG_SGL_SYSTICK_MS                                      (10)
#endif

#ifndef CONFIG_SGL_COLOR16_SWAP
#define CONFIG_SGL_COLOR16_SWAP                                    (0)
#endif

#ifndef CONFIG_SGL_EVENT_QUEUE_SIZE
#define CONFIG_SGL_EVENT_QUEUE_SIZE                                (32)
#endif

#ifndef CONFIG_SGL_DIRTY_AREA_THRESHOLD
#define CONFIG_SGL_DIRTY_AREA_THRESHOLD                            (64)
#endif

#ifndef CONFIG_SGL_ANIMATION
#define CONFIG_SGL_ANIMATION                                       (0)
#endif

#ifndef CONFIG_SGL_DEBUG
#   define CONFIG_SGL_DEBUG                                        (0)
#elif (CONFIG_SGL_DEBUG == 1)
#   ifndef CONFIG_SGL_LOG_COLOR
#       define CONFIG_SGL_LOG_COLOR                                (0)
#   endif
#   ifndef CONFIG_SGL_LOG_LEVEL
#       define CONFIG_SGL_LOG_LEVEL                                (1)
#   endif
#endif

#ifndef CONFIG_SGL_EXTERNAL_PIXMAP
#define CONFIG_SGL_EXTERNAL_PIXMAP                                 (0)
#endif

#ifndef CONFIG_SGL_OBJ_USE_NAME
#define CONFIG_SGL_OBJ_USE_NAME                                    (0)
#endif

#ifndef CONFIG_SGL_HEAP_ALGO
#define CONFIG_SGL_HEAP_ALGO                                       (lwmem)
#endif

#ifndef CONFIG_SGL_HEAP_MEMORY_SIZE
#   define CONFIG_SGL_HEAP_MEMORY_SIZE                             (10240)
#   ifndef CONFIG_SGL_FL_INDEX_MAX
#       define CONFIG_SGL_FL_INDEX_MAX                             (20)
#   endif
#endif

#ifndef CONFIG_SGL_FONT_SONG23
#define CONFIG_SGL_FONT_SONG23                                     (0)
#endif

#ifndef CONFIG_SGL_FONT_CONSOLAS14
#define CONFIG_SGL_FONT_CONSOLAS14                                 (0)
#endif

#ifndef CONFIG_SGL_FONT_CONSOLAS23
#define CONFIG_SGL_FONT_CONSOLAS23                                 (0)
#endif

#ifndef CONFIG_SGL_FONT_CONSOLAS24
#define CONFIG_SGL_FONT_CONSOLAS24                                 (0)
#endif

#ifndef CONFIG_SGL_FONT_CONSOLAS32
#define CONFIG_SGL_FONT_CONSOLAS32                                 (0)
#endif

#if !(defined(CONFIG_SGL_THEME_DARK) || defined(CONFIG_SGL_THEME_LIGHT))
#   ifndef CONFIG_SGL_THEME_DEFAULT
#   define CONFIG_SGL_THEME_DEFAULT                                (1)
#   endif
#endif

#ifndef CONFIG_SGL_BOOT_LOGO
#define CONFIG_SGL_BOOT_LOGO                                       (1)
#endif

#ifndef CONFIG_SGL_BOOT_ANIMATION
#   define CONFIG_SGL_BOOT_ANIMATION                               (0)
#elif (CONFIG_SGL_ANIMATION == 0)
#   undef CONFIG_SGL_BOOT_ANIMATION
#   define CONFIG_SGL_BOOT_ANIMATION                               (0)
#endif

#ifdef __cplusplus
}
#endif

#endif // !__SGL_CFGFIX_H__
