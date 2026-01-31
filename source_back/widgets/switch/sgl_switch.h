/* source/widgets/sgl_switch.h
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

#ifndef __SGL_SWITCH_H__
#define __SGL_SWITCH_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl switch struct
 * @obj: sgl general object
 * @bg_desc: pointer to sgl_draw_rect_t descriptor
 * @knob_desc: pointer to sgl_draw_circle_t descriptor
 * @statue: switch status, false: off, true: on
 */
typedef struct sgl_switch {
    sgl_obj_t         obj;
    sgl_draw_rect_t   bg_desc;
    sgl_color_t       color;
    sgl_color_t       bg_color;
    sgl_color_t       knob_color;
    bool              status;
}sgl_switch_t;


/**
 * @brief create a switch object
 * @param parent parent of the switch
 * @return switch object
 */
sgl_obj_t* sgl_switch_create(sgl_obj_t* parent);


/**
 * @brief set switch color
 * @param obj switch object
 * @param color switch color
 * @return none
 */
static inline void sgl_switch_set_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_switch_t *switch_obj = (sgl_switch_t *)obj;
    switch_obj->color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set switch background color
 * @param obj switch object
 * @param color switch background color
 * @return none
 */
static inline void sgl_switch_set_bg_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_switch_t *switch_obj = (sgl_switch_t *)obj;
    switch_obj->bg_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief Set the knob color of the switch object
 * @param obj  Pointer to the switch object
 * @param color  The color of the knob
 * @return none
 */
static inline void sgl_switch_set_knob_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_switch_t *switch_obj = (sgl_switch_t *)obj;
    switch_obj->knob_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set switch alpha
 * @param obj switch object
 * @param alpha switch alpha
 * @return none
 */
static inline void sgl_switch_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_switch_t *switch_obj = (sgl_switch_t *)obj;
    switch_obj->bg_desc.alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set switch radius
 * @param obj switch object
 * @param radius switch radius
 * @return none
 */
static inline void sgl_switch_set_radius(sgl_obj_t *obj, uint16_t radius)
{
    sgl_switch_t *switch_obj = (sgl_switch_t *)obj;
    switch_obj->bg_desc.radius = sgl_obj_fix_radius(obj, radius);
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set switch border color
 * @param obj switch object
 * @param color switch border color
 * @return none
 */
static inline void sgl_switch_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_switch_t *switch_obj = (sgl_switch_t *)obj;
    switch_obj->bg_desc.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set switch border width
 * @param obj switch object
 * @param width switch border width
 * @return none
 */
static inline void sgl_switch_set_border_width(sgl_obj_t *obj, int16_t width)
{
    sgl_switch_t *switch_obj = (sgl_switch_t *)obj;
    switch_obj->bg_desc.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set status of switch
 * @param obj switch object
 * @param status switch status
 * @return none 
 */
static inline void sgl_switch_set_status(sgl_obj_t *obj, bool status)
{
    sgl_switch_t *switch_obj = (sgl_switch_t *)obj;
    switch_obj->status = status;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief get status of switch
 * @param obj switch object
 * @return switch status
 */
static inline bool sgl_switch_get_status(sgl_obj_t *obj)
{
    sgl_switch_t *switch_obj = (sgl_switch_t *)obj;
    return switch_obj->status;
}


#endif // !__SGL_SWITCH_H__
