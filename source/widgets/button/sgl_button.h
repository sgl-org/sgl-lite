/* source/widgets/sgl_button.h
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

#ifndef __SGL_BUTTON_H__
#define __SGL_BUTTON_H__


#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief Button object
 * @obj: Button object
 * @desc: Button object description
 * @desc_text: Button object description text
 */
typedef struct sgl_button {
    sgl_obj_t               obj;
    sgl_draw_rect_t         rect;
    const char              *text;
    const sgl_font_t        *font;
    const sgl_icon_pixmap_t *icon;
    sgl_color_t             text_color;
    uint8_t                 align;
}sgl_button_t;


/**
 * @brief  create a button object
 * @param  parent: parent object
 * @return object pointer
 * @note  if parent is NULL, the button object will be created at the top level.
 */
sgl_obj_t* sgl_button_create(sgl_obj_t* parent);

/**
 * @brief set button color
 * @param obj: object pointer
 * @param color: color
 * @return none
 */
static inline void sgl_button_set_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->rect.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button alpha
 * @param obj: object pointer
 * @param alpha: alpha
 * @return none
 */
static inline void sgl_button_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->rect.alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button border width
 * @param obj: object pointer
 * @param width: border width
 * @return none
 */
static inline void sgl_button_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->rect.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button border color
 * @param obj: object pointer
 * @param color: border color
 * @return none
 */
static inline void sgl_button_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->rect.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button pixmap
 * @param obj: object pointer
 * @param pixmap: pixmap pointer
 * @return none
 */
static inline void sgl_button_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->rect.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button radius
 * @param obj: object pointer
 * @param radius: radius
 * @return none
 */
static inline void sgl_button_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->rect.radius = sgl_obj_fix_radius(obj, radius);
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button text
 * @param obj: object pointer
 * @param text: text
 * @return none
 */
static inline void sgl_button_set_text(sgl_obj_t *obj, const char *text)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->text = text; 
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button text color
 * @param obj: object pointer
 * @param color: text color
 * @return none
 */
static inline void sgl_button_set_text_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->text_color = color; 
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button text align
 * @param obj: object pointer
 * @param align: align type
 * @return none
 */
static inline void sgl_button_set_text_align(sgl_obj_t *obj, uint8_t align)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->align = align; 
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button text font
 * @param obj: object pointer
 * @param font: font pointer
 * @return none
 */
static inline void sgl_button_set_font(sgl_obj_t *obj, const sgl_font_t *font)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->font = font; 
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set button icon
 * @param obj: object pointer
 * @param icon: icon pointer
 * @return none
 */
static inline void sgl_button_set_icon(sgl_obj_t *obj, const sgl_icon_pixmap_t *icon)
{
    sgl_button_t *button = (sgl_button_t*)obj;
    button->icon = icon; 
    sgl_obj_set_dirty(obj);
}

#endif // !__SGL_BUTTON_H__
