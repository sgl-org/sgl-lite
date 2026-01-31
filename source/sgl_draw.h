/* source: sgl_draw.h
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

#ifndef __SGL_DRAW_H__
#define __SGL_DRAW_H__ 

#include "sgl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  SGL_ARC_MODE_NORMAL                                (0)
#define  SGL_ARC_MODE_RING                                  (1)
#define  SGL_ARC_MODE_NORMAL_SMOOTH                         (2)
#define  SGL_ARC_MODE_RING_SMOOTH                           (3)


/**
 * @brief rect description
 * @color: color of rect
 * @alpha: alpha of rect
 * @border: border of rect
 * @border_color: border color of rect
 * @pixmap: pixmap of rect
 */
typedef struct sgl_draw_rect {
    sgl_color_t             color;
    int16_t                 radius;
    uint8_t                 alpha;
    uint8_t                 border;
    sgl_color_t             border_color;
    const sgl_pixmap_t      *pixmap;
} sgl_draw_rect_t;


/**
 * @brief line draw description
 * @start: start point
 * @end: end point
 * @color: line color
 * @width: line width
 * @alpha: alpha
 */
typedef struct sgl_draw_line {
    sgl_pos_t        start;
    sgl_pos_t        end;
    sgl_color_t      color;
    int16_t          width;
    uint8_t          alpha;
} sgl_draw_line_t;


/**
 * @brief rectangle description
 * @cx: center x of rectangle
 * @cy: center y of rectangle
 * @color: color of rectangle
 * @radius: radius of rectangle
 * @alpha: alpha of rectangle
 * @border: border of rectangle
 * @border_color: border color of rectangle
 * @pixmap: pixmap of rectangle
 */
typedef struct sgl_draw_circle {
    int16_t            cx;
    int16_t            cy;
    sgl_color_t        color;
    int16_t            radius;
    uint8_t            alpha;
    uint8_t            border;
    sgl_color_t        border_color;
    const sgl_pixmap_t *pixmap;
} sgl_draw_circle_t;


/**
 * @brief arc description
 * @cx: center x
 * @cy: center y
 * @radius_in: inner radius of arc
 * @radius_out: outer radius of arc
 * @start_angle: start angle of arc
 * @end_angle: end angle of arc
 * @color: color of arc
 * @alpha: alpha of arc
 * @mode: mode of arc
 * @bg_color: background color of arc
 */
typedef struct sgl_draw_arc {
    int16_t          cx;
    int16_t          cy;
    int16_t          radius_in;
    int16_t          radius_out;
    sgl_color_t      color;
    uint8_t          alpha;
    uint32_t         start_angle: 9;
    uint32_t         end_angle: 9;
    uint32_t         mode: 2;
    sgl_color_t      bg_color;
} sgl_draw_arc_t;


/**
 * @brief icon description
 * @icon: icon pixmap
 * @color: icon color
 * @alpha: alpha of icon
 */
typedef struct sgl_draw_icon {
    const sgl_icon_pixmap_t *icon;
    sgl_color_t       color;
    uint8_t           alpha;
    uint8_t           align;
} sgl_draw_icon_t;


/** 
 * @brief clip area width of surface
 * @note if you want to check the area is overlap with surface, you can use this macro
 *       it will direct return if the area is not overlap with surface, otherwise, continue
 */
#if (CONFIG_SGL_USE_FULL_FB)
#define sgl_surf_clip_area_return(surf, rect, clip)         do {SGL_UNUSED(rect);} while(0)
#else
#define sgl_surf_clip_area_return(surf, rect, clip)         if (!sgl_surf_clip(surf, rect, clip)) return
#endif


/**
 * @brief set pixel on surface
 * @param surf: pointer of surface
 * @param x: x coordinate
 * @param y: y coordinate
 * @param color: color of pixel
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline void sgl_surf_set_pixel(sgl_surf_t *surf, int16_t x, int16_t y, sgl_color_t color) 
{
    surf->buffer[y * surf->w + x] = color;
}


/**
 * @brief get start buffer address that to set pixel on surface
 * @param surf: pointer of surface
 * @param x: x coordinate
 * @param y: y coordinate
 * @return pointer of start buffer address
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline sgl_color_t* sgl_surf_get_buf(sgl_surf_t *surf, int16_t x, int16_t y)
{
    return &surf->buffer[y * surf->w + x];
}


/**
 * @brief get pixel on surface
 * @param surf: pointer of surface
 * @param x: x coordinate
 * @param y: y coordinate
 * @return color of pixel
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline sgl_color_t sgl_surf_get_pixel(sgl_surf_t *surf, int16_t x, int16_t y) 
{
    return surf->buffer[y * surf->w + x];
}


/**
 * @brief draw a horizontal line on surface
 * @param surf: pointer of surface
 * @param y: y coordinate
 * @param x1: x1 coordinate
 * @param x2: x2 coordinate
 * @param color: color of line
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline void sgl_surf_hline(sgl_surf_t *surf, int16_t y, int16_t x1, int16_t x2, sgl_color_t color) 
{
    sgl_color_t *dst = surf->buffer + y * surf->w + x1;
    for (int16_t i = x1; i <= x2; i++) {
        *dst = color;
        dst++;
    }
}


/**
 * @brief draw a vertical line on surface
 * @param surf: pointer of surface
 * @param x: x coordinate
 * @param y1: y1 coordinate
 * @param y2: y2 coordinate
 * @param color: color of line
 * @note this function is not clip, you should clip it before call this function, and the coordinate should be in the surface.
 */
static inline void sgl_surf_vline(sgl_surf_t *surf, int16_t x, int16_t y1, int16_t y2, sgl_color_t color) 
{
    sgl_color_t *dst = surf->buffer + y1 * surf->w + x;
    for (int16_t i = y1; i <= y2; i++) {
        *dst = color;
        dst += surf->w;
    }
}


/**
 * @brief fill rect on surface with alpha
 * @param surf point to surface
 * @param area area of that you want to fill
 * @param rect rect of that you want to fill
 * @param color color of rect
 * @param alpha alpha of rect
 * @return none
 */
void sgl_draw_fill_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t color, uint8_t alpha);


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
void sgl_draw_fill_rect_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, sgl_color_t color, sgl_color_t border_color, int16_t border_width, uint8_t alpha);


/**
 * @brief fill rect on surface with pixmap and alpha
 * @param surf  surface pointer
 * @param area  area that you want to fill
 * @param rect  rect that you want to fill
 * @param pixmap pixmap pointer
 * @param alpha alpha
 * @return none
 */
void sgl_draw_fill_rect_pixmap(sgl_surf_t *surf, sgl_area_t *area, sgl_rect_t *rect, const sgl_pixmap_t *pixmap, uint8_t alpha);


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
void sgl_draw_fill_round_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color, uint8_t alpha);


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
void sgl_draw_fill_round_rect_with_border(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, sgl_color_t color, sgl_color_t border_color, uint8_t border_width, uint8_t alpha);


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
void sgl_draw_fill_round_rect_pixmap(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *rect, int16_t radius, const sgl_pixmap_t *pixmap, uint8_t alpha);


/**
 * @brief fill a round rectangle with alpha
 * @param surf point to surface
 * @param area area of rectangle that you want to draw
 * @param rect point to rectangle that you want to draw
 * @param desc rectangle description
 * @return none
 */
void sgl_draw_rect(sgl_surf_t *surf, sgl_area_t *area, sgl_rect_t *rect, sgl_draw_rect_t *desc);


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
void sgl_draw_fill_circle(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_color_t color, uint8_t alpha);


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
void sgl_draw_fill_circle_pixmap(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, const sgl_pixmap_t *pixmap, uint8_t alpha, int16_t px, int16_t py);


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

void sgl_draw_fill_circle_with_border(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius, sgl_color_t color, sgl_color_t border_color, int16_t border_width, uint8_t alpha);


/**
 * @brief draw task, the task contains the draw information and canvas
 * @param surf surface pointer
 * @param area the area of the task
 * @param desc the draw information
 * @return none
 */
void sgl_draw_circle(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_circle_t *desc);


/**
 * @brief draw icon with alpha
 * @param surf   surface
 * @param area   area of icon
 * @param x      x coordinate
 * @param y      y coordinate
 * @param color  color of icon
 * @param alpha  alpha of icon
 * @param icon   icon pixmap
 */
void sgl_draw_icon( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, sgl_color_t color, uint8_t alpha, const sgl_icon_pixmap_t *icon);


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
void sgl_draw_character( sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, uint32_t ch_index, sgl_color_t color, uint8_t alpha, const sgl_font_t *font);


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
void sgl_draw_string(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, uint8_t alpha, const sgl_font_t *font);


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
void sgl_draw_string_mult_line(sgl_surf_t *surf, sgl_area_t *area, int16_t x, int16_t y, const char *str, sgl_color_t color, uint8_t alpha, const sgl_font_t *font, uint8_t line_margin);


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
void sgl_draw_fill_ring(sgl_surf_t *surf, sgl_area_t *area, int16_t cx, int16_t cy, int16_t radius_in, int16_t radius_out, sgl_color_t color, uint8_t alpha);


/**
 * @brief draw a horizontal line with alpha
 * @param surf surface
 * @param y y coordinate
 * @param x1 x start coordinate
 * @param x2 x end coordinate
 * @param width line width
 * @param color line color
 * @param alpha alpha of color
 * @return none
 */
void sgl_draw_fill_hline(sgl_surf_t *surf, int16_t y, int16_t x1, int16_t x2, int16_t width, sgl_color_t color, uint8_t alpha);


/**
 * @brief draw a vertical line with alpha
 * @param surf surface
 * @param x x coordinate
 * @param y1 y start coordinate
 * @param y2 y end coordinate
 * @param width line width
 * @param color line color
 * @param alpha alpha of color
 * @return none
 */
void sgl_draw_fill_vline(sgl_surf_t *surf, int16_t x, int16_t y1, int16_t y2, int16_t width, sgl_color_t color, uint8_t alpha);


/**
 * @brief draw a line
 * @param surf surface
 * @param desc line description
 * @return none
 */
void sgl_draw_line(sgl_surf_t *surf, sgl_draw_line_t *desc);


/**
 * @brief draw an arc with alpha
 * @param surf pointer to surface
 * @param area pointer to area
 * @param desc pointer to arc description
 * @return none
 */
void sgl_draw_fill_arc(sgl_surf_t *surf, sgl_area_t *area, sgl_draw_arc_t *desc);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
