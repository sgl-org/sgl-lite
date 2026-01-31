/* source: sgl_widget.c
 * Copyright (c) 2026-2028, Lishanwen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sgl_widget.h"
#include "sgl_mm.h"

/**
 * @brief rectangle construct callback
 * @param  surf: surface
 * @param  obj: object
 * @param  evt: event parameter
 * @retval none
 */
static void sgl_rectangle_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t*)obj;
    sgl_draw_rect(surf, area, &obj->coords, &rect->desc);
}


/**
 * @brief  create a rectangle
 * @param  parent: parent object
 * @retval rectangle object
 */
sgl_obj_t* sgl_rect_create(sgl_obj_t* parent)
{
    sgl_rectangle_t *rect = sgl_malloc(sizeof(sgl_rectangle_t));
    if(rect == NULL) {
        SGL_LOG_ERROR("sgl_rect_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(rect, 0, sizeof(sgl_rectangle_t));

    sgl_obj_t *obj = &rect->obj;
    sgl_obj_init(&rect->obj, parent);

    obj->construct_fn = sgl_rectangle_construct_cb;

    rect->desc.alpha = SGL_THEME_ALPHA;
    rect->desc.color = SGL_THEME_COLOR;
    rect->desc.border = SGL_THEME_BORDER_WIDTH;
    rect->desc.border_color = SGL_THEME_BORDER_COLOR;
    rect->desc.pixmap = NULL;

    return obj;
}


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

    align_pos = sgl_get_text_pos(&obj->coords, label->font, label->text, 0, (sgl_align_type_t)label->align);

    sgl_draw_string(surf, area, align_pos.x + label->offset_x, align_pos.y + label->offset_y, label->text, label->color, label->alpha, label->font);
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
