/* source/draw/sgl_draw_ring.c
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
 * @brief draw a ring on surface with alpha
 * @param surf: pointer of surface
 * @param area: pointer of area
 * @param cx: ring center x
 * @param cy: ring center y
 * @param radius_in: ring inner radius
 * @param radius_out: ring outer radius
 * @param color: ring color
 * @param alpha: ring alpha
 * @return none
 */
void sgl_draw_fill_ring(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius_in, int16_t radius_out, sgl_color_t color, uint8_t alpha)
{
    int y2 = 0, real_r2 = 0, edge_alpha = 0;
    int in_r2 = sgl_pow2(radius_in);
    int out_r2 = sgl_pow2(radius_out);
    
    int in_r2_max = sgl_pow2(radius_in - 1);
    int out_r2_max = sgl_pow2(radius_out + 1);
    sgl_color_t *buf = NULL;
    sgl_area_t clip;

    if (!sgl_surf_clip(surf, area, &clip)) {
        return;
    }

    for (int y = clip.y1; y <= clip.y2; y++) {
        y2 = sgl_pow2(y - cy);
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);

        for (int x = clip.x1; x <= clip.x2; x++, buf++) {
            real_r2 = sgl_pow2(x - cx) + y2;

            if (real_r2 >= out_r2_max) {
               if (x > cx) break;
                continue;
            }
            if (real_r2 < in_r2_max) {
                if (x < cx) {
                    buf += ((cx - x) * 2);
                    x = cx*2 - x;
                }
                continue;
            }

            if (alpha == SGL_ALPHA_MAX) {
                if (real_r2 < in_r2 ) {
                    *buf = sgl_color_mixer(color, *buf, sgl_sqrt_error(real_r2));
                }
                else if (real_r2 > out_r2) {
                    *buf = sgl_color_mixer(color, *buf, SGL_ALPHA_MAX - sgl_sqrt_error(real_r2));
                }
                else {
                    *buf = color;
                }
            }
            else {
                if (real_r2 < in_r2 ) {
                    edge_alpha = sgl_sqrt_error(real_r2);
                    sgl_color_t color_mix = sgl_color_mixer(color, *buf, edge_alpha);
                    *buf = sgl_color_mixer(color_mix, *buf, alpha);
                }
                else if (real_r2 > out_r2) {
                    edge_alpha = SGL_ALPHA_MAX - sgl_sqrt_error(real_r2);
                    sgl_color_t color_mix = sgl_color_mixer(color, *buf, edge_alpha);
                    *buf = sgl_color_mixer(color_mix, *buf, alpha);
                }
                else {
                    *buf = sgl_color_mixer(color, *buf, alpha);
                }
            }
        }
    }
}
