/* source/draw/sgl_draw_circle.c
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


/**
 * @brief Draw a circle
 * @param surf Surface
 * @param area Area of the circle
 * @param cx X coordinate of the center
 * @param cy Y coordinate of the center
 * @param radius Radius of the circle
 * @param color Color of the circle
 * @param alpha Alpha of the circle
 * @return none
 */
void sgl_draw_fill_circle(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_color_t color, uint8_t alpha)
{
    sgl_area_t clip = SGL_AREA_MAX;
    sgl_color_t *buf = NULL;

    sgl_surf_clip_area_return(surf, area, &clip);

    sgl_area_t c_rect = {
        .x1 = cx - radius,
        .x2 = cx + radius,
        .y1 = cy - radius,
        .y2 = cy + radius
    };
    if (!sgl_area_selfclip(&clip, &c_rect)) {
        return;
    }

    int y2 = 0, real_r2 = 0, edge_alpha = 0;
    int r2 = sgl_pow2(radius);
    int r2_edge = sgl_pow2(radius + 1);

    for (int y = clip.y1; y <= clip.y2; y++) {
        y2 = sgl_pow2(y - cy);
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        for (int x = clip.x1; x <= clip.x2; x++, buf++) {
            real_r2 = sgl_pow2(x - cx) + y2;

            if (real_r2 >= r2_edge) {
                if(x > cx)
                    break;
                continue;
            }
            else if (real_r2 >= r2) {
                edge_alpha = SGL_ALPHA_MAX - sgl_sqrt_error(real_r2);
                *buf = (alpha == SGL_ALPHA_MAX ? sgl_color_mixer(color, *buf, edge_alpha) : sgl_color_mixer(sgl_color_mixer(color, *buf, edge_alpha), *buf, alpha));
            }
            else {
                *buf = (alpha == SGL_ALPHA_MAX ? color : sgl_color_mixer(color, *buf, alpha));
            }
        }
    }
}


/**
 * @brief Draw a circle with pixmap and alpha
 * @param surf Surface
 * @param area Area of the circle
 * @param cx X coordinate of the center
 * @param cy Y coordinate of the center
 * @param radius Radius of the circle
 * @param pixmap Pixmap of image
 * @param alpha Alpha of the circle
 * @param px  center X coordinate of the pixmap
 * @param py  center Y coordinate of the pixmap
 * @return none
 */
void sgl_draw_fill_circle_pixmap(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, const sgl_pixmap_t *pixmap, uint8_t alpha, int16_t px, int16_t py)
{
    int y2 = 0, real_r2 = 0;
    int r2 = radius * radius;
    int r2_max = (radius + 1) * (radius + 1);
    sgl_color_t *buf = NULL;
    sgl_area_t clip = SGL_AREA_MAX;
    uint8_t edge_alpha = 0;

    sgl_surf_clip_area_return(surf, area, &clip);

    sgl_area_t c_rect = {
        .x1 = cx - radius,
        .x2 = cx + radius,
        .y1 = cy - radius,
        .y2 = cy + radius
    };
    if (!sgl_area_selfclip(&clip, &c_rect)) {
        return;
    }

    for (int y = clip.y1; y <= clip.y2; y++) {
        y2 = sgl_pow2(y - cy);
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
        sgl_color_t *pixmap_color = sgl_pixmap_get_buf(pixmap, px - (cx - clip.x1), py - (cy - y), clip.x2 - clip.x1 + 1);

        for (int x = clip.x1; x <= clip.x2; x++, buf++, pixmap_color++) {
            real_r2 = sgl_pow2(x - cx) + y2;

            if (real_r2 >= r2_max) {
                if(x > cx)
                    break;
                continue;
            }
            else if (real_r2 >= r2) {
                edge_alpha = SGL_ALPHA_MAX - sgl_sqrt_error(real_r2);
                *buf = (alpha == SGL_ALPHA_MAX ? sgl_color_mixer(*pixmap_color, *buf, edge_alpha) : sgl_color_mixer(sgl_color_mixer(*pixmap_color, *buf, edge_alpha), *buf, alpha));
            }
            else {
                *buf = (alpha == SGL_ALPHA_MAX ? *pixmap_color : sgl_color_mixer(*pixmap_color, *buf, alpha));
            }
        }
    }
}


/**
 * @brief Draw a circle with alpha and border
 * @param surf Surface
 * @param area Area of the circle
 * @param cx X coordinate of the center
 * @param cy Y coordinate of the center
 * @param radius Radius of the circle
 * @param color Color of the circle
 * @param border_color Color of the border
 * @param border_width Width of the border
 * @param alpha Alpha of the circle
 * @return none
 */
void sgl_draw_fill_circle_with_border(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_color_t color, sgl_color_t border_color, int16_t border_width, uint8_t alpha)
{
    int radius_in = sgl_max(radius - border_width + 1, 0);
    int y2 = 0, real_r2 = 0;
    int in_r2 = sgl_pow2(radius_in);
    int out_r2 = sgl_pow2(radius);
    sgl_color_t *buf = NULL;

    int in_r2_max = sgl_pow2(radius_in - 1);
    int out_r2_max = sgl_pow2(radius + 1);
    uint8_t edge_alpha = 0;
    sgl_area_t clip = SGL_AREA_MAX;

    sgl_surf_clip_area_return(surf, area, &clip);

    for (int y = clip.y1; y <= clip.y2; y++) {
        y2 = sgl_pow2(y - cy);
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        for (int x = clip.x1; x <= clip.x2; x++, buf++) {
            real_r2 = sgl_pow2(x - cx) + y2;

            if (real_r2 >= out_r2_max) {
                if (x > cx)
                    break;
                continue;
            }
            if (real_r2 < in_r2_max) {
                *buf = (alpha == SGL_ALPHA_MAX ? color : sgl_color_mixer(color, *buf, alpha));
                continue;
            }
            if (real_r2 < in_r2 ) {
                edge_alpha = sgl_sqrt_error(real_r2);
                *buf = (alpha == SGL_ALPHA_MAX ? sgl_color_mixer(border_color, color, sgl_sqrt_error(real_r2)) : sgl_color_mixer(sgl_color_mixer(border_color, color, edge_alpha), *buf, alpha));
            }
            else if (real_r2 > out_r2) {
                edge_alpha = SGL_ALPHA_MAX - sgl_sqrt_error(real_r2);
                *buf = (alpha == SGL_ALPHA_MAX ? sgl_color_mixer(border_color, *buf, edge_alpha) : sgl_color_mixer(sgl_color_mixer(border_color, *buf, edge_alpha), *buf, alpha));
            }
            else {
                *buf = (alpha == SGL_ALPHA_MAX ? border_color : sgl_color_mixer(border_color, *buf, alpha));
            }
        }
    }
}


/**
 * @brief draw task, the task contains the draw information and canvas
 * @param surf surface pointer
 * @param area the area of the task
 * @param desc the draw information
 * @return none
 */
void sgl_draw_circle(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_circle_t *desc)
{
    sgl_color_t color = desc->color;
    int16_t radius = desc->radius;
    uint8_t alpha = desc->alpha;
    uint16_t border = desc->border;
    sgl_color_t border_color = desc->border_color;

    if (desc->pixmap == NULL) {
        if (border) {
            sgl_draw_fill_circle_with_border(surf, area, desc->cx, desc->cy, radius, color, border_color, border, alpha);
        }
        else {
            sgl_draw_fill_circle(surf, area, desc->cx, desc->cy, radius, color, alpha);
        }
    }
    else {
        sgl_draw_fill_circle_pixmap(surf, area, desc->cx, desc->cy, radius, desc->pixmap, alpha, desc->pixmap->width / 2, desc->pixmap->height / 2);
    }
}
