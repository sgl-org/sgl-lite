/* source/widgets/sgl_arc.c
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
#include "sgl_arc.h"


static void sgl_arc_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_arc_t *arc = (sgl_arc_t*)obj;

    arc->desc.cx = (obj->coords.x2 + obj->coords.x1) / 2;
    arc->desc.cy = (obj->coords.y2 + obj->coords.y1) / 2;

    if(arc->desc.start_angle == 0 && arc->desc.end_angle == 360) {
        sgl_draw_fill_ring(surf, area, arc->desc.cx, arc->desc.cy, arc->desc.radius_in, arc->desc.radius_out, arc->desc.color, arc->desc.alpha);
    }
    else {
        sgl_draw_fill_arc(surf, area, &arc->desc);
    }
}


/**
 * @brief create an arc object
 * @param parent parent object
 * @return arc object
 */
sgl_obj_t* sgl_arc_create(sgl_obj_t* parent)
{
    sgl_arc_t *arc = sgl_malloc(sizeof(sgl_arc_t));
    if(arc == NULL) {
        SGL_LOG_ERROR("sgl_arc_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(arc, 0, sizeof(sgl_arc_t));

    sgl_obj_t *obj = &arc->obj;
    sgl_obj_init(&arc->obj, parent);

    arc->desc.alpha = SGL_THEME_ALPHA;
    arc->desc.mode = SGL_ARC_MODE_NORMAL;
    arc->desc.color = SGL_THEME_BG_COLOR;
    arc->desc.bg_color = SGL_THEME_COLOR;
    arc->desc.start_angle = 0;
    arc->desc.end_angle = 360;
    arc->desc.radius_out = -1;
    arc->desc.radius_in = -1;
    arc->desc.cx = -1;
    arc->desc.cy = -1;

    obj->construct_fn = sgl_arc_construct_cb;

    return obj;
}
