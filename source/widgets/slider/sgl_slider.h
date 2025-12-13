/* source/widgets/sgl_slider.h
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

#ifndef __SGL_SLIDER_H__
#define __SGL_SLIDER_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl slider struct
 * @obj: sgl general object
 * @value: slider value
 * @desc: slider description
 */
typedef struct sgl_slider {
    sgl_obj_t          obj;
    sgl_draw_rect_t    body;
    sgl_color_t        color;          
    /* 0: horizontal, 1: vertical */
    uint8_t            direct;
    uint8_t            value;
    uint8_t            alpha;
}sgl_slider_t;


/**
 * @brief create a slider object
 * @param parent parent object of the slider
 * @return slider object
 */
sgl_obj_t* sgl_slider_create(sgl_obj_t* parent);

/**
 * @brief set the fill color of the slider
 * @param obj slider object
 * @param color fill color
 * @return none
 */
static inline void sgl_slider_set_fill_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the fill alpha of the slider
 * @param obj slider object
 * @param alpha fill alpha
 * @return none
 */
static inline void sgl_slider_set_fill_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the track color of the slider
 * @param obj slider object
 * @param color track color
 * @return none
 */
static inline void sgl_slider_set_track_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->body.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the track alpha of the slider
 * @param obj slider object
 * @param alpha track alpha
 * @return none
 */
static inline void sgl_slider_set_track_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->body.alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the slider direction
 * @param obj slider object
 * @param direct slider direction
 * @return none
 * @note direct : SGL_DIRECT_HORIZONTAL or SGL_DIRECT_VERTICAL
 */
static inline void sgl_slider_set_direct(sgl_obj_t *obj, uint8_t direct)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->direct = direct;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the slider radius
 * @param obj slider object
 * @param radius slider radius
 * @return none
 */
static inline void sgl_slider_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->body.radius = sgl_obj_fix_radius(obj, radius);
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the slider value
 * @param obj slider object
 * @param value slider value
 * @return none
 */
static inline void sgl_slider_set_value(sgl_obj_t *obj, uint8_t value)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->value = value;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief get the slider value
 * @param obj slider object
 * @return slider value
 */
static inline uint8_t sgl_slider_get_value(sgl_obj_t *obj)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    return slider->value;
}

/**
 * @brief set the slider border width
 * @param obj slider object
 * @param width border width
 */
static inline void sgl_slider_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->body.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the slider border color
 * @param obj slider object
 * @param color border color
 */
static inline void sgl_slider_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->body.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set the slider pixmap
 * @param obj slider object
 * @param pixmap pixmap
 */
static inline void sgl_slider_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_slider_t *slider = (sgl_slider_t *)obj;
    slider->body.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

#endif // !__SGL_SLIDER_H__
