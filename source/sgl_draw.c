/* source: sgl_draw.c
 * Copyright (c) 2026-2028, Lishanwen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sgl_core.h"
#include "sgl_draw.h"

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
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x1, y - surf->y1);

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
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x1, y - surf->y1);

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
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x1, y - surf->y1);
        pbuf = sgl_pixmap_get_buf(pixmap, pick_cx - (cx - clip.x1 + 1), pick_cy - (cy - y + 1));

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
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x1, y - surf->y1);

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
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x1, y - surf->y1);

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
        buf = sgl_surf_get_buf(surf, clip.x1 - surf->x1, y - surf->y1);
        pbuf = sgl_pixmap_get_buf(pixmap, pick_cx - (cx - clip.x1 + 1), pick_cy - (cy - y + 1));

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


/**
 * @brief Alpha blending table for 4 bpp and 2 bpp
 */
static const uint8_t opa4_table[16] = {0,  17, 34,  51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255 };
static const uint8_t opa2_table[4]  = {0, 85, 170, 255};

#if (CONFIG_SGL_FONT_COMPRESSED)
/**
 * @brief RLE state, refrence LVGL
 */
typedef enum {
    RLE_STATE_SINGLE = 0,
    RLE_STATE_REPEATED,
    RLE_STATE_COUNTER,
} sgl_font_rle_state_t;

/**
 * @brief RLE decompress information structure
 */
typedef struct {
    uint32_t rdp;
    const uint8_t * in;
    uint8_t bpp;
    uint8_t prev_v;
    uint8_t count;
    sgl_font_rle_state_t state;
} sgl_font_rle_t;

static sgl_font_rle_t font_rle = {
    .rdp = 0,
    .in = NULL,
    .bpp = 0,
    .prev_v = 0,
    .count = 0,
    .state = RLE_STATE_SINGLE,
};

/**
 * @brief Get bits from a byte array
 * @param in the byte array
 * @param bit_pos the bit position
 * @param len the bit length
 * @return the bits of a byte
 */
static inline uint8_t get_bits(const uint8_t * in, uint32_t bit_pos, uint8_t len)
{
    uint8_t bit_mask;
    switch(len) {
        case 2:
            bit_mask = 0x3;
            break;
        case 4:
            bit_mask = 0xF;
            break;
        default:
            bit_mask = (uint16_t)((uint16_t) 1 << len) - 1;
    }

    uint32_t byte_pos = bit_pos >> 3;
    bit_pos = bit_pos & 0x7;

    if(bit_pos + len >= 8) {
        uint16_t in16 = (in[byte_pos] << 8) + in[byte_pos + 1];
        return (in16 >> (16 - bit_pos - len)) & bit_mask;
    }
    else {
        return (in[byte_pos] >> (8 - bit_pos - len)) & bit_mask;
    }
}

/**
 * @brief Decompress a line of RLE data
 * @param out the decompressed data
 * @param w the width of the decompressed data
 * @return none
 */
static inline void decompress_line(uint8_t *out, int32_t w)
{
    int32_t i;
    uint8_t v = 0;
    uint8_t ret = 0;
    sgl_font_rle_t *rle = &font_rle;

    for(i = 0; i < w; i++) {
        if(rle->state == RLE_STATE_SINGLE) {
            ret = get_bits(rle->in, rle->rdp, rle->bpp);
            if(rle->rdp != 0 && rle->prev_v == ret) {
                rle->count = 0;
                rle->state = RLE_STATE_REPEATED;
            }

            rle->prev_v = ret;
            rle->rdp += rle->bpp;
        }
        else if(rle->state == RLE_STATE_REPEATED) {
            v = get_bits(rle->in, rle->rdp, 1);
            rle->count++;
            rle->rdp += 1;
            if(v == 1) {
                ret = rle->prev_v;
                if(rle->count == 11) {
                    rle->count = get_bits(rle->in, rle->rdp, 6);
                    rle->rdp += 6;
                    if(rle->count != 0) {
                        rle->state = RLE_STATE_COUNTER;
                    }
                    else {
                        ret = get_bits(rle->in, rle->rdp, rle->bpp);
                        rle->prev_v = ret;
                        rle->rdp += rle->bpp;
                        rle->state = RLE_STATE_SINGLE;
                    }
                }
            }
            else {
                ret = get_bits(rle->in, rle->rdp, rle->bpp);
                rle->prev_v = ret;
                rle->rdp += rle->bpp;
                rle->state = RLE_STATE_SINGLE;
            }
        }
        else if(rle->state == RLE_STATE_COUNTER) {
            ret = rle->prev_v;
            rle->count--;
            if(rle->count == 0) {
                ret = get_bits(rle->in, rle->rdp, rle->bpp);
                rle->prev_v = ret;
                rle->rdp += rle->bpp;
                rle->state = RLE_STATE_SINGLE;
            }
        }
        if (out != NULL) {
            out[i] = ret;
        }
    }
}

/**
 * @brief Initialize the RLE decompression state
 * @param in Pointer to the input data
 * @param bpp Bits per pixel of the input data
 * @return none
 */
static inline void font_rle_init(const uint8_t * in, uint8_t bpp)
{
    font_rle.in = in;
    font_rle.bpp = bpp;
    font_rle.state = RLE_STATE_SINGLE;
    font_rle.rdp = 0;
    font_rle.prev_v = 0;
    font_rle.count = 0;
}
#endif // (!CONFIG_SGL_FONT_COMPRESSED)


/**
 * @brief Draw a character on the surface with alpha blending
 * @param surf Pointer to the surface where the character will be drawn
 * @param area Pointer to the area where the character will be drawn
 * @param x X coordinate where the character will be drawn
 * @param y Y coordinate where the character will be drawn
 * @param ch_index Index of the character in the font table
 * @param color Foreground color of the character
 * @param alpha Alpha value for blending
 * @param font Pointer to the font structure containing character data
 * @return none
 * @note this function is only support bpp:4
 */
void sgl_draw_character(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index, sgl_color_t color, uint8_t alpha, const sgl_font_t *font)
{
    int offset_y2 = font->font_height - font->table[ch_index].ofs_y - font->base_line;
    const uint8_t *dot = &font->bitmap[font->table[ch_index].bitmap_index];
    const uint8_t font_w = font->table[ch_index].box_w;
    const uint8_t font_h = font->table[ch_index].box_h;

    uint8_t shift = 0;
    uint32_t pixel_index, rel_x, rel_y;
    uint16_t byte_index, alpha_dot = 0;
    sgl_color_t color_mix, *buf = NULL, *blend = NULL;
    sgl_area_t clip;

    sgl_area_t text_rect = {
        .x1 = x + font->table[ch_index].ofs_x,
        .x2 = x + font->table[ch_index].ofs_x + font_w - 1,
        .y1 = y + offset_y2 - font_h,
        .y2 = y + offset_y2 - 1,
    };

    if (!sgl_surf_clip(surf, &text_rect, &clip)) {
        return;
    }

    if (!sgl_area_selfclip(&clip, area)) {
        return;
    }

    buf = sgl_surf_get_buf(surf, clip.x1 - surf->x1, clip.y1 - surf->y1);
#if (CONFIG_SGL_FONT_COMPRESSED)
    if (font->compress == 0) {
#endif // (!CONFIG_SGL_FONT_COMPRESSED == 0)
        for (int y = clip.y1; y <= clip.y2; y++) {
            blend = buf;
            rel_y = y - text_rect.y1;

            for (int x = clip.x1; x <= clip.x2; x++) {
                rel_x = x - text_rect.x1;
                pixel_index = rel_y * font_w + rel_x;

                if  (font->bpp == 4) {
                    byte_index = pixel_index >> 1;
                    alpha_dot =  opa4_table[(pixel_index & 1) ? (dot[byte_index] & 0x0F) : (dot[byte_index] >> 4)];
                }
                else if (font->bpp == 2) {
                    byte_index = pixel_index >> 2;
                    shift = (3 - (pixel_index & 0x3)) * 2;
                    alpha_dot = opa2_table[(dot[byte_index] >> shift) & 0x03];
                }

                color_mix = sgl_color_mixer(color, *blend, alpha_dot);
                *blend = sgl_color_mixer(color_mix, *blend, alpha);
                blend++;
            }
            buf += surf->w;
        }
#if (CONFIG_SGL_FONT_COMPRESSED)
    }  /* support compressed font */
    else {
        uint8_t line_buf[128] = {0};
        font_rle_init(dot, font->bpp);

        for (int y = text_rect.y1; y < clip.y1; y++) {
            decompress_line(NULL, font_w);
        }

        for (int y = clip.y1; y <= clip.y2; y++) {
            blend = buf;
            decompress_line(line_buf, font_w);

            for (int x = clip.x1; x <= clip.x2; x++) {
                if (font->bpp == 4) {
                    color_mix = sgl_color_mixer(color, *blend, opa4_table[line_buf[x - text_rect.x1]]);
                }
                else if (font->bpp == 2) {
                    color_mix = sgl_color_mixer(color, *blend, opa2_table[line_buf[x - text_rect.x1]]);
                }
                *blend = sgl_color_mixer(color_mix, *blend, alpha);
                blend++;
            }
            buf += surf->w;
        }
    }
#endif
}


/**
 * @brief Draw a string on the surface with alpha blending
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param alpha Alpha value for blending
 * @param font Pointer to the font structure containing character data
 * @return none
 */
void sgl_draw_string(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, uint8_t alpha, const sgl_font_t *font)
{
    uint32_t ch_index;
    uint32_t unicode = 0;

    while (*str) {
        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);
        sgl_draw_character(surf, area, x, y, ch_index, color, alpha, font);
        x += (font->table[ch_index].adv_w >> 4);
    }
}


/**
 * @brief Draw a string on the surface with alpha blending and multiple lines
 * @param surf Pointer to the surface where the string will be drawn
 * @param area Pointer to the area where the string will be drawn
 * @param x X coordinate of the top-left corner of the string
 * @param y Y coordinate of the top-left corner of the string
 * @param str Pointer to the string to be drawn
 * @param color Foreground color of the string
 * @param alpha Alpha value for blending
 * @param font Pointer to the font structure containing character data
 * @param line_margin Margin between lines
 * @return none
 */
void sgl_draw_string_mult_line(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, uint8_t alpha, const sgl_font_t *font, uint8_t line_margin)
{
    int16_t ch_index, ch_width;
    int16_t x_off = x;
    uint32_t unicode = 0;

    while (*str) {
        if (*str == '\n') {
            x_off = x;
            y += (font->font_height + line_margin);
            str ++;
            continue;
        }

        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);

        ch_width = (font->table[ch_index].adv_w >> 4);

        if ((x_off + ch_width) > area->x2) {
            x_off = x;
            y += (font->font_height + line_margin);
        }

        sgl_draw_character(surf, area, x_off, y, ch_index, color, alpha, font);
        x_off += ch_width;
    }
}
