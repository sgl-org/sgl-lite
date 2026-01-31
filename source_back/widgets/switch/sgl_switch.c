/* source/widgets/sgl_switch.c
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
#include "sgl_switch.h"


static void sgl_switch_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_switch_t *p_switch = (sgl_switch_t*)(obj);
    sgl_draw_rect_t *bg_desc = &p_switch->bg_desc;
    int16_t width = obj->coords.y2 - obj->coords.y1 - 2 * bg_desc->border;
    sgl_rect_t knob_rect = { 
        .y1 = obj->coords.y1 + bg_desc->border,
        .y2 = obj->coords.y2 - bg_desc->border,
    };

    if(p_switch->status) {
        bg_desc->color = p_switch->color;
        knob_rect.x2 = obj->coords.x2 - bg_desc->border;
        knob_rect.x1 = knob_rect.x2 - width;
    }
    else {
        bg_desc->color = p_switch->bg_color;
        knob_rect.x1 = obj->coords.x1 + bg_desc->border;
        knob_rect.x2 = knob_rect.x1 + width;
    }

    sgl_draw_rect(surf, area, &obj->coords, &p_switch->bg_desc);
    sgl_draw_fill_round_rect(surf, area, &knob_rect, obj->radius - 2 * bg_desc->border, p_switch->knob_color, p_switch->bg_desc.alpha);
}


/**
 * @brief create a switch object
 * @param parent parent of the switch
 * @return switch object
 */
sgl_obj_t* sgl_switch_create(sgl_obj_t* parent)
{
    sgl_switch_t *p_switch = sgl_malloc(sizeof(sgl_switch_t));
    if(p_switch == NULL) {
        SGL_LOG_ERROR("sgl_switch_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(p_switch, 0, sizeof(sgl_switch_t));

    sgl_obj_t *obj = &p_switch->obj;
    sgl_obj_init(&p_switch->obj, parent);
    obj->construct_fn = sgl_switch_construct_cb;

    p_switch->bg_desc.alpha = SGL_THEME_ALPHA;
    p_switch->bg_desc.color = SGL_THEME_COLOR;
    p_switch->bg_desc.border_color = SGL_THEME_BORDER_COLOR;
    p_switch->bg_desc.border = SGL_THEME_BORDER_WIDTH;
    p_switch->bg_desc.radius = -1;

    p_switch->status = false;
    p_switch->bg_color = SGL_THEME_BG_COLOR;
    p_switch->knob_color = sgl_color_mixer(SGL_THEME_COLOR, SGL_THEME_BG_COLOR, 128);

    return obj;
}

