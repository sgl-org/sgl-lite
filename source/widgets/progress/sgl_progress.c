/* source/widgets/sgl_progress.c
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
#include "sgl_progress.h"


static void sgl_progress_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_progress_t *progress = (sgl_progress_t*)obj;
    sgl_area_t knob = obj->coords;
    knob.x1 = obj->coords.x1 + obj->radius / 2 + 2;
    sgl_area_t rect = {
        .x1 = obj->coords.x1 - progress->interval * 2 + progress->shift,
        .y1 = obj->coords.y1 + 1,
        .x2 = 0,
        .y2 = obj->coords.y2 - 1,
    };

    if (progress->shift > (progress->interval + progress->knob_width)) {
        progress->shift = 0;
    }

    knob.x2 = obj->coords.x1 - obj->radius / 2 - 2 + (obj->coords.x2 - obj->coords.x1) * progress->value / 100;

    sgl_draw_rect(surf, area, &obj->coords, &progress->body);

    while (rect.x2 <= knob.x2) {
        rect.x2 = rect.x1 + progress->knob_width;
        sgl_draw_fill_round_rect(surf, &knob, &rect, progress->knob_radius, progress->color, progress->alpha);
        rect.x1 = rect.x2 + progress->interval;
    }
}


/**
 * @brief create a progress object
 * @param parent parent object of the progress
 * @return progress object
 */
sgl_obj_t* sgl_progress_create(sgl_obj_t* parent)
{
    sgl_progress_t *progress = sgl_malloc(sizeof(sgl_progress_t));
    if(progress == NULL) {
        SGL_LOG_ERROR("sgl_progress_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(progress, 0, sizeof(sgl_progress_t));

    sgl_obj_t *obj = &progress->obj;
    sgl_obj_init(&progress->obj, parent);
    obj->construct_fn = sgl_progress_construct_cb;

    progress->body.alpha = SGL_THEME_ALPHA;
    progress->body.color = SGL_THEME_BG_COLOR;
    progress->body.border = SGL_THEME_BORDER_WIDTH;
    progress->body.border_color = SGL_THEME_BORDER_COLOR;

    progress->color = SGL_THEME_COLOR;
    progress->alpha = SGL_THEME_ALPHA;
    progress->knob_width = 4;
    progress->interval = 4;
    progress->value = 50;
    progress->knob_radius = 0;

    return obj;
}
