/* source/widgets/sgl_led.c
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
#include "sgl_led.h"


static void sgl_led_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_led_t *led = (sgl_led_t*)obj;
    sgl_color_t color = led->status ? led->color : led->bg_color;

    sgl_area_t clip;
    sgl_color_t *buf = NULL;

    led->cx = (led->obj.coords.x1 + led->obj.coords.x2) / 2;
    led->cy = (led->obj.coords.y1 + led->obj.coords.y2) / 2;

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    sgl_area_t c_rect = {
        .x1 = led->cx - obj->radius,
        .x2 = led->cx + obj->radius,
        .y1 = led->cy - obj->radius,
        .y2 = led->cy + obj->radius
    };
    if (!sgl_area_selfclip(&clip, &c_rect)) {
        return;
    }

    int y2 = 0, real_r2 = 0, edge_alpha = 0;
    int r2 = sgl_pow2(obj->radius);
    int r2_edge = sgl_pow2(obj->radius + 1);
    int ds_alpha = SGL_ALPHA_MIN;

    for (int y = clip.y1; y <= clip.y2; y++) {
        y2 = sgl_pow2(y - led->cy);
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        for (int x = clip.x1; x <= clip.x2; x++, buf++) {
            real_r2 = sgl_pow2(x - led->cx) + y2;
            if (real_r2 >= r2_edge) {
                if(x > led->cx)
                    break;
                continue;
            }
            else if (real_r2 >= r2) {
                edge_alpha = SGL_ALPHA_MAX - sgl_sqrt_error(real_r2);
                sgl_color_t color_mix = sgl_color_mixer(led->bg_color, *buf, edge_alpha);
                *buf = sgl_color_mixer(color_mix, *buf, led->alpha);
            }
            else {
                ds_alpha = real_r2 * SGL_ALPHA_NUM / r2;
                ds_alpha = sgl_pow2(ds_alpha) / SGL_ALPHA_NUM ;
                *buf = sgl_color_mixer(sgl_color_mixer(led->bg_color, color, ds_alpha), *buf, led->alpha);
            }
        }
    }
}


/**
 * @brief create a led object
 * @param parent parent of the led
 * @return led object
 */
sgl_obj_t* sgl_led_create(sgl_obj_t* parent)
{
    sgl_led_t *led = sgl_malloc(sizeof(sgl_led_t));
    if(led == NULL) {
        SGL_LOG_ERROR("sgl_led_create: malloc failed");
        return NULL;
    }

    /* set object all member to zero */
    memset(led, 0, sizeof(sgl_led_t));

    sgl_obj_t *obj = &led->obj;
    sgl_obj_init(&led->obj, parent);
    obj->construct_fn = sgl_led_construct_cb;

    led->alpha = SGL_ALPHA_MAX;
    led->color = SGL_THEME_COLOR;
    led->bg_color = SGL_THEME_BG_COLOR;
    led->cx = -1;
    led->cy = -1;
    obj->radius = SGL_RADIUS_INVALID;

    return obj;
}
