/* source/widgets/sgl_ring.c
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
#include "sgl_ring.h"


static void sgl_ring_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_ring_t *ring = (sgl_ring_t*)obj;

    ring->cx = (obj->coords.x2 + obj->coords.x1) / 2;
    ring->cy = (obj->coords.y2 + obj->coords.y1) / 2;
    sgl_draw_fill_ring(surf, area, ring->cx, ring->cy, ring->radius_in, ring->radius_out, ring->color, ring->alpha);
}


/**
 * @brief Create a ring object
 * @param parent The parent object of the ring
 * @return The ring object
 */
sgl_obj_t* sgl_ring_create(sgl_obj_t* parent)
{
    sgl_ring_t *ring = sgl_malloc(sizeof(sgl_ring_t));
    if(ring == NULL) {
        SGL_LOG_ERROR("sgl_ring_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(ring, 0, sizeof(sgl_ring_t));

    sgl_obj_t *obj = &ring->obj;
    sgl_obj_init(&ring->obj, parent);
    obj->construct_fn = sgl_ring_construct_cb;

    ring->radius_in = -1;
    ring->radius_out = -1;
    ring->cx = -1;
    ring->cy = -1;
    ring->alpha = SGL_THEME_ALPHA;
    ring->color = SGL_THEME_COLOR;

    return obj;
}

