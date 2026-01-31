/* source/draw/sgl_draw_text.c
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
#include <sgl_log.h>
#include <sgl_draw.h>
#include <sgl_math.h>

/**
    font bitmap example is:
    <----------------Width--------------->   font is 4bpp Anti-Aliasing.   
    |                                    |
    |                                    |  
    |               x..@@@@@@@..x        |
  Height            x@.........@x        |
    |               @@.........@@        |
    |               @@.........@@        |
    |               .@@@.....@@@@        | 
    |               ....@@@@@..@@        |
    |               ...........@@        |
    |               ...........@@        |
    |               .@@.......@@.        |
    |<--offset_x-->|x..@@@@@@@..x        |
    |              |       ^             |
    |              |       |             |
    |              |    offset_y         |
    |              |       |             |
    |              |       V             |
    --------------------------------------
***/

/**
 * @brief Alpha blending table for 4 bpp and 2 bpp
 */
static const uint8_t opa4_table[16] = {0,  17, 34,  51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255 };
static const uint8_t opa2_table[4]  = {0, 85, 170, 255};


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
    uint16_t byte_index, alpha_dot;
    sgl_color_t color_mix, *buf = NULL;
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

    if (font->bpp == 4) {
        for (int y = clip.y1; y <= clip.y2; y++) {
            buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
            rel_y = y - text_rect.y1;

            for (int x = clip.x1; x <= clip.x2; x++) {
                rel_x = x - text_rect.x1;

                pixel_index = rel_y * font_w + rel_x;

                byte_index = pixel_index >> 1;
                alpha_dot =  opa4_table[(pixel_index & 1) ? (dot[byte_index] & 0x0F) : (dot[byte_index] >> 4)];

                color_mix = sgl_color_mixer(color, *buf, alpha_dot);
                *buf = sgl_color_mixer(color_mix, *buf, alpha);
                buf++;
            }
        }
    }
    else if (font->bpp == 2) {
        for (int y = clip.y1; y <= clip.y2; y++) {
            buf = sgl_surf_get_buf(surf, clip.x1 - surf->x, y - surf->y);
            rel_y = y - text_rect.y1;

            for (int x = clip.x1; x <= clip.x2; x++) {
                rel_x = x - text_rect.x1;
                pixel_index = rel_y * font_w + rel_x;

                byte_index = pixel_index >> 2;
                shift = (3 - (pixel_index & 0x3)) * 2;
                alpha_dot = opa2_table[(dot[byte_index] >> shift) & 0x03];

                color_mix = sgl_color_mixer(color, *buf, alpha_dot);
                *buf = sgl_color_mixer(color_mix, *buf, alpha);
                buf++;
            }
        }
    }
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
