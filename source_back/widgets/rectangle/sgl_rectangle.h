/* source/widgets/sgl_rectangle.h
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

#ifndef __SGL_RECTANGLE_H__
#define __SGL_RECTANGLE_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl rectangle
 * @obj: sgl general object
 * @desc: rectangle draw description
 */
typedef struct sgl_rectangle {  
    sgl_obj_t       obj;
    sgl_draw_rect_t desc;
}sgl_rectangle_t;


/**
 * @brief  create a rectangle
 * @param  parent: parent object
 * @retval rectangle object
 */
sgl_obj_t* sgl_rect_create(sgl_obj_t* parent);


/**
 * @brief  set rectangle color
 * @param  obj: rectangle object
 * @param  color: rectangle color
 * @retval none
 */
static inline void sgl_rect_set_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief  set rectangle alpha
 * @param  obj: rectangle object
 * @param  alpha: rectangle alpha
 * @retval none
 */
static inline void sgl_rect_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.alpha = alpha;
    sgl_obj_set_dirty(obj);
}


/**
 * @brief  set rectangle radius
 * @param  obj: rectangle object
 * @param  radius: rectangle radius
 * @retval none
 */
static inline void sgl_rect_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.radius = radius;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief  set rectangle border width
 * @param  obj: rectangle object
 * @param  width: rectangle border width
 * @retval none
 */
static inline void sgl_rect_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief  set rectangle border color
 * @param  obj: rectangle object
 * @param  color: rectangle border color
 * @retval none
 */
static inline void sgl_rect_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief  set rectangle pixmap
 * @param  obj: rectangle object
 * @param  pixmap: rectangle pixmap
 * @retval none
 */
static inline void sgl_rect_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

#endif // !__SGL_RECTANGLE_H__
