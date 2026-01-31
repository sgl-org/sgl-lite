/* source/widgets/sgl_circle.h
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

#ifndef __SGL_CIRCLE_H__
#define __SGL_CIRCLE_H__


#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief draw a circle
 * @obj:  sgl general object
 * @desc: circle description
 */
typedef struct sgl_circle {
    sgl_obj_t         obj;
    sgl_draw_circle_t desc;
}sgl_circle_t;


/**
 * @brief create a circle object
 * @param parent parent of the object
 * @return pointer to the object
 */
sgl_obj_t* sgl_circle_create(sgl_obj_t* parent);


/**
 * @brief set the color of the circle
 * @param obj pointer to the object
 * @param color color of the circle
 * @return none
 */
static inline void sgl_circle_set_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_circle_t *circle = (sgl_circle_t *)obj;
    circle->desc.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the radius of the circle
 * @param obj pointer to the object
 * @param radius radius of the circle
 * @return none
 */
static inline void sgl_circle_set_radius(sgl_obj_t *obj, uint16_t radius)
{
    sgl_circle_t *circle = (sgl_circle_t *)obj;
    circle->desc.radius = radius;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the alpha of the circle
 * @param obj pointer to the object
 * @param alpha alpha of the circle
 * @return none
 */
static inline void sgl_circle_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_circle_t *circle = (sgl_circle_t *)obj;
    circle->desc.alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the pixmap of the circle
 * @param obj pointer to the object
 * @param pixmap pixmap of the circle
 * @return none
 */
static inline void sgl_circle_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_circle_t *circle = (sgl_circle_t *)obj;
    circle->desc.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the border color of the circle
 * @param obj pointer to the object
 * @param color border color of the circle
 * @return none
 */
static inline void sgl_circle_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_circle_t *circle = (sgl_circle_t *)obj;
    circle->desc.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the border width of the circle
 * @param obj pointer to the object
 * @param width border width of the circle
 * @return none
 */
static inline void sgl_circle_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_circle_t *circle = (sgl_circle_t *)obj;
    circle->desc.border = width;
    sgl_obj_set_dirty(obj);
}


#endif // !__SGL_CIRCLE_H__
