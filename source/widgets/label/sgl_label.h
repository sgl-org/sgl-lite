/* source/widgets/sgl_label.h
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

#ifndef __SGL_LABEL_H__
#define __SGL_LABEL_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl label object
 * @obj: sgl general object
 * @desc: draw task descriptor
 */
typedef struct sgl_label {
    sgl_obj_t        obj;
    const char       *text;
    const sgl_font_t *font;
    const sgl_icon_pixmap_t *icon;
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
 * @brief set label icon
 * @param obj pointer to the label object
 * @param icon pointer to the icon
 * @return none
 */
static inline void sgl_label_set_icon(sgl_obj_t *obj, const sgl_icon_pixmap_t *icon)
{
    sgl_label_t *label = sgl_container_of(obj, sgl_label_t, obj);
    label->icon = icon;
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
    sgl_obj_fix_radius(obj, radius);
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


#endif // !__SGL_LABEL_H__
