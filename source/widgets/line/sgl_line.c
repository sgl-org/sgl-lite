/* source/widgets/sgl_line.c
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
#include "sgl_line.h"


static void sgl_line_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_line_t *line = (sgl_line_t*)obj;
    SGL_UNUSED(area);
    sgl_draw_line(surf, &line->desc);
}


/**
 * @brief create a line object
 * @param parent parent of the line
 * @return line object
 */
sgl_obj_t* sgl_line_create(sgl_obj_t* parent)
{
    sgl_line_t *line = sgl_malloc(sizeof(sgl_line_t));
    if(line == NULL) {
        SGL_LOG_ERROR("sgl_line_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(line, 0, sizeof(sgl_line_t));

    sgl_obj_t *obj = &line->obj;
    sgl_obj_init(&line->obj, parent);
    obj->construct_fn = sgl_line_construct_cb;

    line->desc.color = SGL_THEME_BG_COLOR;
    line->desc.alpha = SGL_ALPHA_MAX;
    line->desc.width = 1;

    return obj;
}
