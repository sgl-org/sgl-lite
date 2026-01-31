/* source/draw/sgl_draw_arc.c
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
#include <sgl_log.h>
#include <sgl_math.h>


typedef struct sgl_arc_dot
{
    int16_t  cx;
    int16_t  cy;
    int16_t  r;
    uint32_t r2;
    uint16_t rmax;
    uint16_t outer;
} sgl_arc_dot_t;


static void arc_dot_sin_cos(int16_t cx, int16_t cy, int16_t radius_in, int16_t radius_out, sgl_arc_dot_t *dot,int sin, int cos)
{
    int len = (radius_out + radius_in) / 2;
    int r = (radius_out - radius_in) / 2;
    if (unlikely(r == 0)) {
        return;
    }

    if (sin < 0) {
        dot->cx = (sin * len - 16348) / 32768;
    }
    else {
        dot->cx = (sin * len + 16348) / 32768;
    }
    if (cos<0) {
        dot->cy = (cos * len - 16348) / 32768;
    }
    else {
        dot->cy = (cos * len + 16348) / 32768;
    }

    dot->cx = cx - dot->cx;
    dot->cy = cy - dot->cy;
    dot->r = r + 1;
    dot->r2 =  sgl_pow2(r);
    dot->rmax = sgl_pow2(r + 1);
    dot->outer = 0;
}


static inline uint8_t arc_get_dot(sgl_arc_dot_t *dot,int ax, int ay)
{
    uint32_t temp;
    uint8_t alpha = SGL_ALPHA_MIN, max = SGL_ALPHA_MIN;
    sgl_arc_dot_t *p = dot;
    int32_t rate = (0xff00) / (dot->rmax - dot->r2);

    for (int k = 0; k < 2; k++, p++) {
        int x = ax > p->cx ? ax - p->cx : p->cx-ax;
        int y = ay > p->cy ? ay - p->cy : p->cy-ay;

        if ((x < p->r) && (y < p->r)) {
            temp = sgl_pow2(x) + sgl_pow2(y);
            if (temp >= p->rmax) {
                alpha = 0;
            }
            else if (temp > p->r2) {
                if(dot->outer==0) {
                    dot->outer = rate;
                }
                alpha = (p->rmax - temp) * dot->outer >> 8;
            }
            else {
                alpha = SGL_ALPHA_MAX;
            }
            max = sgl_max(max, alpha);
        }
    }
    return max;
}


/**
 * @brief draw an arc with alpha
 * @param surf pointer to surface
 * @param area pointer to area
 * @param desc pointer to arc description
 * @return none
 */
void sgl_draw_fill_arc(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_arc_t *desc)
{
    int y2 = 0, real_r2 = 0, edge_alpha = 0;
    int in_r2 = sgl_pow2(desc->radius_in);
    int out_r2 = sgl_pow2(desc->radius_out);
    int inv_inner = 0, inv_outer = 0;
    sgl_arc_dot_t arc_dot[2];

    int in_r2_max = sgl_pow2(desc->radius_in - 1);
    int out_r2_max = sgl_pow2(desc->radius_out + 1);
    int32_t rate = (0xff00) / (in_r2 - in_r2_max);
    int32_t rate2 = (0xff00) / (out_r2_max - out_r2);

    sgl_color_t *buf = NULL;
    int32_t dx, dy;
    uint8_t flag = 0xff, in_range;
    int32_t ds = 0, de = 0, sd = 0, ed = 0;
    int32_t sx = 0, sy = 0, ex = 0, ey = 0;
    sgl_color_t tmp_color;
    sgl_area_t clip = SGL_AREA_MAX;

    sgl_surf_clip_area_return(surf, area, &clip);

    sgl_area_t c_rect = {
        .x1 = desc->cx - desc->radius_out,
        .x2 = desc->cx + desc->radius_out,
        .y1 = desc->cy - desc->radius_out,
        .y2 = desc->cy + desc->radius_out
    };

    if (!sgl_area_selfclip(&clip, &c_rect)) {
        return;
    }

    if (desc->start_angle != 0 || desc->end_angle != 360) {
        flag = (desc->end_angle - desc->start_angle > 180) ? 1 : 0;
        sx = sgl_sin(desc->start_angle);
        sy = -sgl_cos(desc->start_angle);
        ex = sgl_sin(desc->end_angle);
        ey = -sgl_cos(desc->end_angle);

        if (desc->mode == SGL_ARC_MODE_NORMAL_SMOOTH || desc->mode == SGL_ARC_MODE_RING_SMOOTH) {
            arc_dot_sin_cos(desc->cx, desc->cy, desc->radius_in, desc->radius_out, &arc_dot[0], sx, sy);
            arc_dot_sin_cos(desc->cx, desc->cy, desc->radius_in, desc->radius_out, &arc_dot[1], ex, ey);
        }

        sx = sx >> 7;
        sy = sy >> 7;
        ex = ex >> 7;
        ey = ey >> 7;
    }

    for (int y = clip.y1; y <= clip.y2; y++) {
        dy = y - desc->cy;
        y2 = sgl_pow2(dy);
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        for (int x = clip.x1; x <= clip.x2; x++, buf++) {
            dx = x - desc->cx;
            real_r2 = sgl_pow2(x - desc->cx) + y2;

            if (real_r2 >= out_r2_max) {
                if (x > desc->cx) {
                    buf++;
                    break;
                }
                continue;
            }
            if (real_r2 < in_r2_max) {
                if (x < desc->cx) {
                    buf += ((desc->cx - x) * 2);
                    x = desc->cx * 2 - x;
                }
                continue;
            }
            if (real_r2 < in_r2 ) {
                if(inv_inner == 0) {
                    inv_inner = rate;
                }
                edge_alpha  = (real_r2 - in_r2_max) * inv_inner >> 8;
            }
            else if (real_r2 > out_r2) {
                if(inv_outer == 0) {
                    inv_outer = rate2;
                }
                edge_alpha = (out_r2_max - real_r2) * inv_outer >> 8;
            }
            else {
                edge_alpha = SGL_ALPHA_MAX;
            }

            tmp_color = desc->color;
            if (flag != 255) {
                ds = (dx *  sy - dy *  sx);
                de = (dy *  ex - dx *  ey);
                in_range =  flag > 0 ? (ds > 0 || de >0) : (ds >= 0 && de >= 0);
                if (!in_range) {

                    switch (desc->mode) {
                    case SGL_ARC_MODE_NORMAL:
                        sd = sgl_xy_has_component(dx,dy, sx, sy) ? sgl_abs(ds) : 256;
                        ed = sgl_xy_has_component(dx,dy, ex, ey) ? sgl_abs(de) : 256;
                        dx =  sgl_min(sd, ed);
                        tmp_color = (dx < SGL_ALPHA_MAX) ? sgl_color_mixer(desc->color, *buf, sgl_min(255 - dx, edge_alpha)) : *buf;
                        break;

                    case SGL_ARC_MODE_RING:
                        sd = sgl_xy_has_component(dx,dy, sx, sy) ? sgl_abs(ds) : 256;
                        ed = sgl_xy_has_component(dx,dy, ex, ey) ? sgl_abs(de) : 256;
                        dx =  sgl_min(sd, ed);
                        tmp_color = (dx < SGL_ALPHA_MAX) ? sgl_color_mixer(desc->color, desc->bg_color, sgl_min(255 - dx, edge_alpha)) : desc->bg_color;
                        break;

                    case SGL_ARC_MODE_NORMAL_SMOOTH:
                        dx = arc_get_dot(arc_dot, x, y);
                        tmp_color = (dx < SGL_ALPHA_MAX) ? sgl_color_mixer(desc->color, *buf, dx) : desc->color;
                        break;

                    case SGL_ARC_MODE_RING_SMOOTH:
                        dx = arc_get_dot(arc_dot, x, y);
                        tmp_color = (dx < SGL_ALPHA_MAX) ? sgl_color_mixer(desc->color, desc->bg_color, dx) : desc->color;
                        break;

                    default: break;
                    }
                }
            }

            *buf = desc->alpha == SGL_ALPHA_MAX ? sgl_color_mixer(tmp_color, *buf, edge_alpha) : sgl_color_mixer(sgl_color_mixer(tmp_color, *buf, edge_alpha), *buf, desc->alpha);
        }
    }
}
