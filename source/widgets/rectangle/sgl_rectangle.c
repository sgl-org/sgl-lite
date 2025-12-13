/* source/widgets/sgl_rectangle.c
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
#include "sgl_rectangle.h"


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
