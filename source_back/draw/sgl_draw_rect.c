/* source/draw/sgl_draw_rect.c
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
 * @brief fill rect on surface with alpha
 * @param surf point to surface
 * @param area area of that you want to fill
 * @param rect rect of that you want to fill
 * @param color color of rect
 * @param alpha alpha of rect
 * @return none
 */
void sgl_draw_fill_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t color, uint8_t alpha)
{
    sgl_area_t clip;
    sgl_color_t *buf = NULL;

    if (!sgl_surf_clip(surf, rect, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, area)) {
        return;
    }

    for (int y = clip.y1; y <= clip.y2; y++) {
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        for (int x = clip.x1; x <= clip.x2; x++, buf++) {
            if (alpha == SGL_ALPHA_MAX) {
                *buf = color;
            }
            else if (alpha > SGL_ALPHA_MIN) {
                *buf = sgl_color_mixer(color, *buf, alpha);
            }
        }
    }
}


/**
 * @brief fill rect on surface with alpha
 * @param surf point to surface
 * @param area area of that you want to fill
 * @param rect rect of that you want to fill
 * @param color color of rect
 * @param border_color color of border
 * @param border_width width of border
 * @param alpha alpha of rect
 * @return none
 */
void sgl_draw_fill_rect_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t color, sgl_color_t border_color, int16_t border_width, uint8_t alpha)
{
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    int16_t b_x1 = rect->x1 + border_width - 1;
    int16_t b_x2 = rect->x2 - border_width + 1;
    int16_t b_y1 = rect->y1 + border_width - 1;
    int16_t b_y2 = rect->y2 - border_width + 1;

    if (!sgl_surf_clip(surf, rect, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, area)) {
        return;
    }

    for (int y = clip.y1; y <= clip.y2; y++) {
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        for (int x = clip.x1; x <= clip.x2; x++, buf++) {
            if (x > b_x1 && x < b_x2 && y > b_y1 && y < b_y2) {
                *buf = alpha == SGL_ALPHA_MAX ? color : sgl_color_mixer(color, *buf, alpha);
            }
            else {
                *buf = alpha == SGL_ALPHA_MAX ? border_color : sgl_color_mixer(border_color, *buf, alpha);
            }
        }
    }
}


/**
 * @brief fill rect on surface with pixmap and alpha
 * @param surf  surface pointer
 * @param area  area that you want to fill
 * @param rect  rect that you want to fill
 * @param pixmap pixmap pointer
 * @param alpha alpha
 * @return none
 */
void sgl_draw_fill_rect_pixmap(sgl_surf_t *surf, sgl_area_t *area, sgl_rect_t *rect, const sgl_pixmap_t *pixmap, uint8_t alpha)
{
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    sgl_color_t *pbuf = NULL;

    if (!sgl_surf_clip(surf, rect, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, area)) {
        return;
    }

    int cx = (rect->x1 + rect->x2) / 2;
    int cy = (rect->y1 + rect->y2) / 2;
    int pick_cx = pixmap->width / 2;
    int pick_cy = pixmap->height / 2;

    for (int y = clip.y1; y <= clip.y2; y++) {
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
        pbuf = sgl_pixmap_get_buf(pixmap, pick_cx - (cx - clip.x1 + 1), pick_cy - (cy - y + 1), clip.x2 - clip.x1 + 1);

        for (int x = clip.x1; x <= clip.x2; x++, buf++) {
            *buf = (alpha == SGL_ALPHA_MAX ? *pbuf : sgl_color_mixer(*pbuf, *buf, alpha));
            pbuf ++;
        }
    }
}


/**
 * @brief fill a round rectangle with alpha
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param radius radius of round
 * @param color color of rectangle
 * @param alpha alpha of rectangle
 * @return none
 */
void sgl_draw_fill_round_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color, uint8_t alpha)
{
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    uint8_t edge_alpha = 0;
    int cx1 = rect->x1 + radius;
    int cx2 = rect->x2 - radius;
    int cy1 = rect->y1 + radius;
    int cy2 = rect->y2 - radius;
    int cx_tmp = 0;
    int cy_tmp = 0;

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, rect)) {
        return;
    }

    int y2 = 0, real_r2 = 0;
    int r2 = sgl_pow2(radius);
    int r2_edge = sgl_pow2(radius + 1);

    for (int y = clip.y1; y <= clip.y2; y++) {
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        if (y > cy1 && y < cy2) {
            for (int x = clip.x1; x <= clip.x2; x++, buf++) {
                *buf = (alpha == SGL_ALPHA_MAX ? color : sgl_color_mixer(color, *buf, alpha));
            }
        }
        else {
            cy_tmp = y > cy1 ? cy2 : cy1;
            y2 = sgl_pow2(y - cy_tmp);

            for (int x = clip.x1; x <= clip.x2; x++, buf++) {
                if (x > cx1 && x < cx2) {
                    *buf = (alpha == SGL_ALPHA_MAX ? color : sgl_color_mixer(color, *buf, alpha));
                }
                else {
                    cx_tmp = x > cx1 ? cx2 : cx1;
                    real_r2 = sgl_pow2(x - cx_tmp) + y2;
                    if (real_r2 >= r2_edge) {
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
    }
}


/**
 * @brief fill a round rectangle with alpha and border
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param radius radius of round
 * @param color color of rectangle
 * @param border_color color of border
 * @param border_width width of border
 * @param alpha alpha of rectangle
 * @return none
 */
void sgl_draw_fill_round_rect_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color, sgl_color_t border_color, uint8_t border_width, uint8_t alpha)
{
    int radius_in = sgl_max(radius - border_width + 1, 0);
    int y2 = 0, real_r2 = 0;
    int in_r2 = sgl_pow2(radius_in);
    int out_r2 = sgl_pow2(radius);
    sgl_color_t *buf = NULL;
    uint8_t edge_alpha = 0;

    int cx1 = rect->x1 + radius;
    int cx2 = rect->x2 - radius;
    int cy1 = rect->y1 + radius;
    int cy2 = rect->y2 - radius;
    int cx1i = rect->x1 + border_width;
    int cx2i = rect->x2 - border_width;
    int cyi1 = rect->y1 + border_width;
    int cyi2 = rect->y2 - border_width;
    int cx_tmp = 0;
    int cy_tmp = 0;

    int in_r2_max = sgl_pow2(radius_in - 1);
    int out_r2_max = sgl_pow2(radius + 1);

    sgl_area_t clip;

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, rect)) {
        return;
    }

    for (int y = clip.y1; y <= clip.y2; y++) {
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        if (y > cy1 && y < cy2) {
            for (int x = clip.x1; x <= clip.x2; x++, buf++) {
                if (x < cx1i || x > cx2i) {
                    *buf = (alpha == SGL_ALPHA_MAX ? border_color : sgl_color_mixer(border_color, *buf, alpha));
                }
                else {
                    *buf = (alpha == SGL_ALPHA_MAX ? color: sgl_color_mixer(color, *buf, alpha));
                }
            }
        }
        else {
            cy_tmp = y > cy1 ? cy2 : cy1;
            y2 = sgl_pow2(y - cy_tmp);

            for (int x = clip.x1; x <= clip.x2; x++, buf++) {
                if (x > cx1 && x < cx2) {
                    if (y < cyi1 || y > cyi2) {
                        *buf = (alpha == SGL_ALPHA_MAX ? border_color : sgl_color_mixer(border_color, *buf, alpha));
                    }
                    else {
                        *buf = (alpha == SGL_ALPHA_MAX ? color : sgl_color_mixer(color, *buf, alpha));
                    }
                }
                else {
                    cx_tmp = x > cx1 ? cx2 : cx1;
                    real_r2 = sgl_pow2(x - cx_tmp) + y2;

                    if (real_r2 >= out_r2_max) {
                        continue;
                    }
                    if (real_r2 < in_r2_max) {
                        *buf = (alpha == SGL_ALPHA_MAX ? color : sgl_color_mixer(color, *buf, alpha));
                        continue;
                    }
                    if (real_r2 < in_r2 ) {
                        edge_alpha = sgl_sqrt_error(real_r2);
                        *buf = (alpha == SGL_ALPHA_MAX ? sgl_color_mixer(border_color, color, edge_alpha) : sgl_color_mixer(sgl_color_mixer(border_color, color, edge_alpha), *buf, alpha));
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
    }
}


/**
 * @brief fill a round rectangle pixmap with alpha
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param radius radius of round
 * @param pixmap pixmap of rectangle
 * @param alpha alpha of rectangle
 * @return none
 */
void sgl_draw_fill_round_rect_pixmap(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, const sgl_pixmap_t *pixmap, uint8_t alpha)
{
    sgl_area_t clip;
    sgl_color_t *buf = NULL;
    sgl_color_t *pbuf = NULL;
    uint8_t edge_alpha = 0;
    int cx1 = rect->x1 + radius;
    int cx2 = rect->x2 - radius;
    int cy1 = rect->y1 + radius;
    int cy2 = rect->y2 - radius;
    int cx_tmp = 0;
    int cy_tmp = 0;
    int cx = (rect->x1 + rect->x2) / 2;
    int cy = (rect->y1 + rect->y2) / 2;
    int pick_cx = pixmap->width / 2;
    int pick_cy = pixmap->height / 2;

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, rect)) {
        return;
    }

    int y2 = 0, real_r2 = 0;
    int r2 = sgl_pow2(radius);
    int r2_edge = sgl_pow2(radius + 1);

    for (int y = clip.y1; y <= clip.y2; y++) {
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
        pbuf = sgl_pixmap_get_buf(pixmap, pick_cx - (cx - clip.x1 + 1), pick_cy - (cy - y + 1), clip.x2 - clip.x1 + 1);

        if (y > cy1 && y < cy2) {
            for (int x = clip.x1; x <= clip.x2; x++, buf++, pbuf++) {
                *buf = (alpha == SGL_ALPHA_MAX ? *pbuf : sgl_color_mixer(*pbuf, *buf, alpha));
            }
        }
        else {
            cy_tmp = y > cy1 ? cy2 : cy1;
            y2 = sgl_pow2(y - cy_tmp);

            for (int x = clip.x1; x <= clip.x2; x++, buf++, pbuf++) {
                if(x > cx1 && x < cx2) {
                    *buf = (alpha == SGL_ALPHA_MAX ? *pbuf : sgl_color_mixer(*pbuf, *buf, alpha));
                }
                else {
                    cx_tmp = x > cx1 ? cx2 : cx1;
                    real_r2 = sgl_pow2(x - cx_tmp) + y2;
                    if (real_r2 >= r2_edge) {
                        continue;
                    }
                    else if (real_r2 >= r2) {
                        edge_alpha = SGL_ALPHA_MAX - sgl_sqrt_error(real_r2);
                        *buf = (alpha == SGL_ALPHA_MAX ? sgl_color_mixer(*pbuf, *buf, edge_alpha) : sgl_color_mixer(sgl_color_mixer(*pbuf, *buf, edge_alpha), *buf, alpha));
                    }
                    else {
                        *buf = (alpha == SGL_ALPHA_MAX ? *pbuf : sgl_color_mixer(*pbuf, *buf, alpha));
                    }
                }
            }
        }
    }
}


/**
 * @brief fill a round rectangle with alpha
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param desc rectangle description
 * @return none
 */
void sgl_draw_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_rect_t *rect, sgl_draw_rect_t *desc)
{
    if (desc->radius == 0) {
        if (desc->pixmap == NULL) {
            if (desc->border == 0) {
                sgl_draw_fill_rect(surf, area, rect, desc->color, desc->alpha);
            }
            else {
                sgl_draw_fill_rect_with_border(surf, area, rect, desc->color, desc->border_color, desc->border, desc->alpha);
            }
        }
        else {
            sgl_draw_fill_rect_pixmap(surf, area, rect, desc->pixmap, desc->alpha);
        }
    }
    else {
        if (desc->pixmap == NULL) {
            if (desc->border == 0) {
                sgl_draw_fill_round_rect(surf, area, rect, desc->radius, desc->color, desc->alpha);
            }
            else {
                sgl_draw_fill_round_rect_with_border(surf, area, rect, desc->radius, desc->color, desc->border_color, desc->border, desc->alpha);
            }
        }
        else {
            sgl_draw_fill_round_rect_pixmap(surf, area, rect, desc->radius, desc->pixmap, desc->alpha);
        }
    }
}
