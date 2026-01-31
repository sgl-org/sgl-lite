/* source: sgl_widget.h
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


#define SGL_THEME_DESKTOP                    sgl_rgb(255, 255, 255)
#define SGL_THEME_COLOR                      sgl_rgb(255, 255, 255)
#define SGL_THEME_BG_COLOR                   sgl_rgb(0, 0, 0)
#define SGL_THEME_BORDER_WIDTH               2
#define SGL_THEME_BORDER_COLOR               sgl_rgb(0, 0, 0)
#define SGL_THEME_ALPHA                      255
#define SGL_THEME_TEXT_COLOR                 sgl_rgb(0, 0, 0)
#define SGL_THEME_RADIUS                     0
#define SGL_THEME_SCROLL_RADIUS              3
#define SGL_THEME_SCROLL_BG_COLOR            sgl_rgb(100, 100, 100)
#define SGL_THEME_SCROLL_FG_COLOR            sgl_rgb(200, 200, 200)


/**
 * @brief sgl rectangle
 * @obj: sgl general object
 * @desc: rectangle draw description
 */
typedef struct sgl_rectangle {  
    sgl_obj_t       obj;
    sgl_draw_rect_t desc;
}sgl_rectangle_t;


/**
 * @brief  create a rectangle
 * @param  parent: parent object
 * @retval rectangle object
 */
sgl_obj_t* sgl_rect_create(sgl_obj_t* parent);


/**
 * @brief  set rectangle color
 * @param  obj: rectangle object
 * @param  color: rectangle color
 * @retval none
 */
static inline void sgl_rect_set_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief  set rectangle alpha
 * @param  obj: rectangle object
 * @param  alpha: rectangle alpha
 * @retval none
 */
static inline void sgl_rect_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.alpha = alpha;
    sgl_obj_set_dirty(obj);
}


/**
 * @brief  set rectangle radius
 * @param  obj: rectangle object
 * @param  radius: rectangle radius
 * @retval none
 */
static inline void sgl_rect_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.radius = radius;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief  set rectangle border width
 * @param  obj: rectangle object
 * @param  width: rectangle border width
 * @retval none
 */
static inline void sgl_rect_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief  set rectangle border color
 * @param  obj: rectangle object
 * @param  color: rectangle border color
 * @retval none
 */
static inline void sgl_rect_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief  set rectangle pixmap
 * @param  obj: rectangle object
 * @param  pixmap: rectangle pixmap
 * @retval none
 */
static inline void sgl_rect_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_rectangle_t *rect = (sgl_rectangle_t *)obj;
    rect->desc.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}


/**
 * @brief sgl label object
 * @obj: sgl general object
 * @desc: draw task descriptor
 */
typedef struct sgl_label {
    sgl_obj_t        obj;
    const char       *text;
    const sgl_font_t *font;
    sgl_color_t      color;
    sgl_color_t      bg_color;
    uint8_t          alpha;
    uint8_t          align: 4;
    uint8_t          bg_flag : 1;
    int8_t           offset_x;
    int8_t           offset_y;
}sgl_label_t;


/**
 * @brief create a label object
 * @param parent parent of the label
 * @return pointer to the label object
 */
sgl_obj_t* sgl_label_create(sgl_obj_t* parent);


/**
 * @brief set label text
 * @param obj pointer to the label object
 * @param text text to be set
 * @return none
 */
static inline void sgl_label_set_text(sgl_obj_t *obj, const char *text)
{
    sgl_label_t *label = sgl_container_of(obj, sgl_label_t, obj);
    label->text = text;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set label font
 * @param obj pointer to the label object
 * @param font pointer to the font
 * @return none
 */
static inline void sgl_label_set_font(sgl_obj_t *obj, const sgl_font_t *font)
{
    sgl_label_t *label = sgl_container_of(obj, sgl_label_t, obj);
    label->font = font;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set label text color
 * @param obj pointer to the label object
 * @param color color to be set
 * @return none
 */
static inline void sgl_label_set_text_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_label_t *label = sgl_container_of(obj, sgl_label_t, obj);
    label->color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set label background color
 * @param obj pointer to the label object
 * @param color color to be set
 * @return none
 */
static inline void sgl_label_set_bg_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_label_t *label = sgl_container_of(obj, sgl_label_t, obj);
    label->bg_color = color;
    label->bg_flag = 1;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set label radius
 * @param obj pointer to the label object
 * @param radius radius to be set
 * @return none
 */
static inline void sgl_label_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_obj_set_radius(obj, radius);
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set label text align
 * @param obj pointer to the label object
 * @param align align to be set
 * @return none
 */
static inline void sgl_label_set_text_align(sgl_obj_t *obj, sgl_align_type_t align)
{
    sgl_label_t *label = sgl_container_of(obj, sgl_label_t, obj);
    label->align = align;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set label alpha
 * @param obj pointer to the label object
 * @param alpha alpha to be set
 * @return none
 */
static inline void sgl_label_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_label_t *label = sgl_container_of(obj, sgl_label_t, obj);
    label->alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set label text offset
 * @param obj pointer to the label object
 * @param offset_x offset_x to be set
 * @param offset_y offset_y to be set
 * @return none
 */
static inline void sgl_label_set_text_offset(sgl_obj_t *obj, int8_t offset_x, int8_t offset_y)
{
    sgl_label_t *label = sgl_container_of(obj, sgl_label_t, obj);
    label->offset_x = offset_x;
    label->offset_y = offset_y;
    sgl_obj_set_dirty(obj);
}
