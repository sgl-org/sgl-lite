/* source/widgets/sgl_progress.h
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

#ifndef __SGL_PROGRESS_H__
#define __SGL_PROGRESS_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl progress struct
 * @obj: sgl general object
 * @value: progress value
 * @desc: progress description
 */
typedef struct sgl_progress {
    sgl_obj_t          obj;
    sgl_draw_rect_t    body;
    sgl_color_t        color;
    uint8_t            value;
    uint8_t            alpha;
    uint8_t            interval;
    uint8_t            shift;
    uint8_t            knob_radius;
    uint8_t            knob_width;
}sgl_progress_t;


/**
 * @brief create a progress object
 * @param parent parent object of the progress
 * @return progress object
 */
sgl_obj_t* sgl_progress_create(sgl_obj_t* parent);

/**
 * @brief set progress track color
 * @param obj progress object
 * @param color track color
 * @return none
 */
static inline void sgl_progress_set_track_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->body.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress track alpha
 * @param obj progress object
 * @param alpha track alpha
 * @return none
 */
static inline void sgl_progress_set_track_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->body.alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress fill color
 * @param obj progress object
 * @param color fill color
 * @return none
 */
static inline void sgl_progress_set_fill_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress fill alpha
 * @param obj progress object
 * @param alpha fill alpha
 * @return none
 */
static inline void sgl_progress_set_fill_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress radius
 * @param obj progress object
 * @param radius progress radius
 * @return none
 */
static inline void sgl_progress_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->body.radius = sgl_obj_fix_radius(obj, radius);
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress border width
 * @param obj progress object
 * @param width progress border width
 * @return none
 */
static inline void sgl_progress_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->body.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress border color
 * @param obj progress object
 * @param color progress border color
 * @return none
 */
static inline void sgl_progress_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->body.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress pixmap
 * @param obj progress object
 * @param pixmap progress pixmap
 * @return none
 */
static inline void sgl_progress_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->body.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress fill interval
 * @param obj progress object
 * @param interval progress fill interval
 * @return none
 */
static inline void sgl_progress_set_fill_interval(sgl_obj_t *obj, uint8_t interval)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->interval = interval;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress fill radius
 * @param obj progress object
 * @param radius progress fill radius
 * @return none
 */
static inline void sgl_progress_set_fill_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->knob_radius = radius;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress fill width
 * @param obj progress object
 * @param width progress fill width
 * @return none
 */
static inline void sgl_progress_set_fill_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->knob_width = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set progress value
 * @param obj progress object
 * @param value progress value
 * @return none
 */
static inline void sgl_progress_set_value(sgl_obj_t *obj, uint8_t value)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    progress->value = value;
    progress->shift ++;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief get progress value
 * @param obj progress object
 * @return progress value
 */
static inline uint8_t sgl_progress_get_value(sgl_obj_t *obj)
{
    sgl_progress_t *progress = (sgl_progress_t *)obj;
    return progress->value;
}


#endif // !__SGL_PROGRESS_H__
