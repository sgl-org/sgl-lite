/* source/widgets/sgl_button.c
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
#include <sgl_cfgfix.h>
#include <sgl_theme.h>
#include <string.h>
#include "sgl_button.h"


/**
 * @brief  button construct callback
 * @param  surf: surface pointer
 * @param  obj: object pointer
 * @param  evt: event pointer
 * @return none
 * @note   this function is called when the object is created or redraw
 */
static void sgl_button_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    sgl_pos_t   align_pos;
    int text_x = 0, icon_y = 0;

    sgl_draw_rect(surf, area, &obj->coords, &button->rect);

    if(button->text) {
        SGL_ASSERT(button->font != NULL);

        if (button->icon) {
            text_x = button->icon->width + 2;
        }

        align_pos = sgl_get_text_pos(&obj->coords, button->font, button->text, text_x, (sgl_align_type_t)button->align);

        if (button->icon) {
            icon_y = ((obj->coords.y2 - obj->coords.y1) - (button->icon->height)) / 2;
            sgl_draw_icon(surf, area, align_pos.x, obj->coords.y1 + icon_y, button->text_color, button->rect.alpha, button->icon);
        }

        sgl_draw_string(surf, area, align_pos.x + text_x, align_pos.y, button->text, button->text_color, button->rect.alpha, button->font);
    }
}


/**
 * @brief  create a button object
 * @param  parent: parent object
 * @return object pointer
 * @note  if parent is NULL, the button object will be created at the top level.
 */
sgl_obj_t* sgl_button_create(sgl_obj_t* parent)
{
    sgl_button_t *button = sgl_malloc(sizeof(sgl_button_t));
    if(button == NULL) {
        SGL_LOG_ERROR("sgl_button_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(button, 0, sizeof(sgl_button_t));

    sgl_obj_t *obj = &button->obj;
    sgl_obj_init(&button->obj, parent);

    obj->construct_fn = sgl_button_construct_cb;

    button->rect.alpha = SGL_THEME_ALPHA;
    button->rect.color = SGL_THEME_COLOR;
    button->rect.border = SGL_THEME_BORDER_WIDTH;
    button->rect.border_color = SGL_THEME_BORDER_COLOR;
    button->rect.pixmap = NULL;
    button->rect.radius = 0;

    button->text = NULL;
    button->text_color = SGL_THEME_TEXT_COLOR;
    button->font = NULL;
    button->icon = NULL;
    button->align = SGL_ALIGN_CENTER;

    return obj;
}
