/* source/widgets/sgl_icon.c
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
#include "sgl_icon.h"


/**
 * @brief icon construct callback
 * @param surf pointer to surface
 * @param obj pointer to object
 * @param evt pointer to event
 * @return none
 */
static void sgl_icon_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_icon_t *icon = (sgl_icon_t*)obj;
    sgl_pos_t icon_pos;

    icon_pos = sgl_get_icon_pos(area, icon->icon, 0, (sgl_align_type_t)icon->align);
    sgl_draw_icon(surf, area, icon_pos.x, icon_pos.y, icon->color, icon->alpha, icon->icon);
}


/**
 * @brief create icon object
 * @param parent pointer to parent object
 * @return pointer to icon object
 */
sgl_obj_t* sgl_icon_create(sgl_obj_t* parent)
{
    sgl_icon_t *icon = sgl_malloc(sizeof(sgl_icon_t));
    if(icon == NULL) {
        SGL_LOG_ERROR("sgl_icon_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(icon, 0, sizeof(sgl_icon_t));

    sgl_obj_t *obj = &icon->obj;
    sgl_obj_init(&icon->obj, parent);
    obj->construct_fn = sgl_icon_construct_cb;

    icon->alpha = SGL_THEME_ALPHA;
    icon->icon = NULL;
    icon->color = SGL_THEME_TEXT_COLOR;
    icon->align = SGL_ALIGN_CENTER;

    return obj;
}
