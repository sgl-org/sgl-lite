/* source/widgets/sgl_2dball.c
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
#include "sgl_2dball.h"


static void sgl_2dball_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_2dball_t *ball = (sgl_2dball_t*)obj;

    sgl_area_t clip;
    sgl_color_t *buf = NULL;

    ball->cx = (ball->obj.coords.x1 + ball->obj.coords.x2) / 2;
    ball->cy = (ball->obj.coords.y1 + ball->obj.coords.y2) / 2;

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    sgl_area_t c_rect = {
        .x1 = ball->cx - ball->radius,
        .x2 = ball->cx + ball->radius,
        .y1 = ball->cy - ball->radius,
        .y2 = ball->cy + ball->radius
    };
    if (!sgl_area_selfclip(&clip, &c_rect)) {
        return;
    }

    int y2 = 0, real_r2 = 0, edge_alpha = 0;
    int r2 = sgl_pow2(ball->radius);
    int r2_edge = sgl_pow2(ball->radius + 1);
    int ds_alpha = SGL_ALPHA_MIN;

    for (int y = clip.y1; y <= clip.y2; y++) {
        y2 = sgl_pow2(y - ball->cy);
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        for (int x = clip.x1; x <= clip.x2; x++, buf++) {
            real_r2 = sgl_pow2(x - ball->cx) + y2;
            ds_alpha = real_r2 * SGL_ALPHA_NUM / r2;

            if (real_r2 >= r2_edge) {
                if(x > ball->cx)
                    break;
                continue;
            }
            else if (real_r2 >= r2) {
                edge_alpha = SGL_ALPHA_MAX - sgl_sqrt_error(real_r2);
                sgl_color_t color_mix = sgl_color_mixer(ball->bg_color, *buf, edge_alpha);
                *buf = sgl_color_mixer(color_mix, *buf, ball->alpha);
            }
            else {
                *buf = sgl_color_mixer(sgl_color_mixer(ball->bg_color, ball->color, ds_alpha), *buf, ball->alpha);
            }
        }
    }
}


/**
 * @brief create a 2dball object
 * @param parent parent of the 2dball
 * @return 2dball object
 */
sgl_obj_t* sgl_2dball_create(sgl_obj_t* parent)
{
    sgl_2dball_t *ball = sgl_malloc(sizeof(sgl_2dball_t));
    if(ball == NULL) {
        SGL_LOG_ERROR("sgl_2dball_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(ball, 0, sizeof(sgl_2dball_t));

    sgl_obj_t *obj = &ball->obj;
    sgl_obj_init(&ball->obj, parent);
    obj->construct_fn = sgl_2dball_construct_cb;

    ball->alpha = SGL_ALPHA_MAX;
    ball->color = SGL_THEME_COLOR;
    ball->bg_color = SGL_THEME_BG_COLOR;
    ball->cx = -1;
    ball->cy = -1;
    ball->radius = -1;

    return obj;
}
