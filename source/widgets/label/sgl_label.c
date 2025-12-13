/* source/widgets/sgl_label.c
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
#include "sgl_label.h"


/**
 * @brief construct the label object
 * @param surf pointer to the surface
 * @param obj pointer to the label object
 * @param evt pointer to the event
 * @return none
 */
static void sgl_label_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_label_t *label = (sgl_label_t*)obj;
    int text_x = 0, icon_y = 0;
    sgl_pos_t align_pos;

    SGL_ASSERT(label->font != NULL);

    if (label->bg_flag) {
        if (obj->radius > 0) {
            sgl_draw_fill_round_rect(surf, area, &obj->coords, obj->radius, label->bg_color, label->alpha);
        }
        else {
            sgl_draw_fill_rect(surf, area, &obj->coords, label->bg_color, label->alpha);
        }
    }
    if (label->icon) {
        text_x = label->icon->width + 2;
    }

    align_pos = sgl_get_text_pos(&obj->coords, label->font, label->text, text_x, (sgl_align_type_t)label->align);

    if (label->icon) {
        icon_y = ((obj->coords.y2 - obj->coords.y1) - (label->icon->height)) / 2 + 1;
        sgl_draw_icon(surf, area, align_pos.x + label->offset_x, obj->coords.y1 + icon_y + label->offset_y, label->color, label->alpha, label->icon);
    }

    sgl_draw_string(surf, area, align_pos.x + text_x + label->offset_x, align_pos.y + label->offset_y, label->text, label->color, label->alpha, label->font);
}


/**
 * @brief create a label object
 * @param parent parent of the label
 * @return pointer to the label object
 */
sgl_obj_t* sgl_label_create(sgl_obj_t* parent)
{
    sgl_label_t *label = sgl_malloc(sizeof(sgl_label_t));
    if(label == NULL) {
        SGL_LOG_ERROR("sgl_label_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(label, 0, sizeof(sgl_label_t));

    sgl_obj_t *obj = &label->obj;
    sgl_obj_init(&label->obj, parent);
    obj->construct_fn = sgl_label_construct_cb;

    label->alpha = SGL_ALPHA_MAX;
    label->bg_flag = 0;
    label->color = SGL_THEME_TEXT_COLOR;
    label->text = "";

    return obj;
}
