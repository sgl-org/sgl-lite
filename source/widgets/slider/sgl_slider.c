/* source/widgets/sgl_slider.c
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

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_theme.h>
#include <sgl_cfgfix.h>
#include <string.h>
#include "sgl_slider.h"


static void sgl_slider_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_slider_t *slider = (sgl_slider_t*)obj;
    sgl_area_t knob = {
        .x1 = obj->coords.x1 + slider->body.border,
        .x2 = obj->coords.x2 - slider->body.border,
        .y1 = obj->coords.y1 + slider->body.border,
        .y2 = obj->coords.y2 - slider->body.border,
    };

    if(slider->direct == SGL_DIRECT_HORIZONTAL) {
        knob.x2 = obj->coords.x1 + (obj->coords.x2 - obj->coords.x1) * slider->value / 100 - slider->body.border;
    }
    else {
        knob.y1 = obj->coords.y2 - (obj->coords.y2 - obj->coords.y1) * slider->value / 100 + slider->body.border;
    }
    sgl_draw_rect(surf, area, &obj->coords, &slider->body);
    sgl_draw_fill_round_rect_with_border(surf, &knob, &obj->coords, obj->radius, slider->color, slider->body.border_color, slider->body.border, slider->alpha);
}


/**
 * @brief create a slider object
 * @param parent parent object of the slider
 * @return slider object
 */
sgl_obj_t* sgl_slider_create(sgl_obj_t* parent)
{
    sgl_slider_t *slider = sgl_malloc(sizeof(sgl_slider_t));
    if(slider == NULL) {
        SGL_LOG_ERROR("sgl_slider_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(slider, 0, sizeof(sgl_slider_t));

    sgl_obj_t *obj = &slider->obj;
    sgl_obj_init(&slider->obj, parent);
    obj->construct_fn = sgl_slider_construct_cb;

    slider->direct = SGL_DIRECT_HORIZONTAL;
    slider->body.alpha = SGL_THEME_ALPHA;
    slider->body.color = SGL_THEME_BG_COLOR;
    slider->body.border = SGL_THEME_BORDER_WIDTH;
    slider->body.border_color = SGL_THEME_BORDER_COLOR;

    slider->color = SGL_THEME_COLOR;
    slider->alpha = SGL_THEME_ALPHA;

    return obj;
}
