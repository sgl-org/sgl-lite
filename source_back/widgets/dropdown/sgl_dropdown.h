/* source/widgets/sgl_dropdown.h
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

#ifndef __SGL_DROPDOWN_H__
#define __SGL_DROPDOWN_H__

#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


typedef struct sgl_dropdown_option {
    struct sgl_dropdown_option *next;
    const char *text;
    const sgl_icon_pixmap_t *icon;
} sgl_dropdown_option_t;


/**
 * @brief sgl dropdown struct
 * @desc: text description
 */
typedef struct sgl_dropdown {
    sgl_obj_t             obj;
    sgl_draw_rect_t       body_desc;
    const sgl_font_t      *font;
    sgl_color_t           text_color;
    sgl_dropdown_option_t *head;
    uint16_t              option_num;
    int16_t               selected;
    int16_t               option_h;
    int16_t               expand_h;
    sgl_dropdown_option_t *expend_start;
    uint8_t               clicked;
    bool                  is_open;
}sgl_dropdown_t;


/**
 * @brief create a dropdown object
 * @param parent parent of the dropdown
 * @return dropdown object
 */
sgl_obj_t* sgl_dropdown_create(sgl_obj_t* parent);

/**
 * @brief set dropdown object's color
 * @param obj dropdown object
 * @param color color
 */
static inline void sgl_dropdown_set_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    dropdown->body_desc.color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set dropdown object's border width
 * @param obj dropdown object
 * @param width border width
 */
static inline void sgl_dropdown_set_border_width(sgl_obj_t *obj, uint8_t width)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    dropdown->body_desc.border = width;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set dropdown object's border color
 * @param obj dropdown object
 * @param color border color
 */
static inline void sgl_dropdown_set_border_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    dropdown->body_desc.border_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set dropdown object's radius
 * @param obj dropdown object
 * @param radius radius
 */
static inline void sgl_dropdown_set_radius(sgl_obj_t *obj, uint8_t radius)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    dropdown->body_desc.radius = sgl_obj_fix_radius(obj, radius);
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set dropdown object's pixmap
 * @param obj dropdown object
 * @param pixmap pixmap
 */
static inline void sgl_dropdown_set_pixmap(sgl_obj_t *obj, const sgl_pixmap_t *pixmap)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    dropdown->body_desc.pixmap = pixmap;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set dropdown object's alpha
 * @param obj dropdown object
 * @param alpha alpha
 */
static inline void sgl_dropdown_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    dropdown->body_desc.alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set dropdown object's text color
 * @param obj dropdown object
 * @param color text color
 */
static inline void sgl_dropdown_set_text_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    dropdown->text_color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set dropdown object's text font
 * @param obj dropdown object
 * @param font text font
 * @return none
 * @note font must be initialized
 */
static inline void sgl_dropdown_set_text_font(sgl_obj_t *obj, const sgl_font_t* font)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    dropdown->font = font;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief set dropdown object's selected index
 * @param obj dropdown object
 * @param index selected index
 * @return none
 */
static inline void sgl_dropdown_set_selected_index(sgl_obj_t *obj, int index)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    SGL_ASSERT(obj != NULL && index >= 0 && index < dropdown->option_num);
    dropdown->selected = index;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief get dropdown object's selected index
 * @param obj dropdown object
 * @return selected index
 */
static inline int sgl_dropdown_get_selected_index(sgl_obj_t *obj)
{
    sgl_dropdown_t *dropdown = (sgl_dropdown_t*)obj;
    return dropdown->selected;
}

/**
 * @brief get dropdown object's selected text
 * @param obj dropdown object
 * @return selected text
 */
const char *sgl_dropdown_get_selected_text(sgl_obj_t *obj);

/**
 * @brief add an option to the dropdown
 * @param obj pointer to the dropdown object
 * @param text pointer to the text
 * @param icon pointer to the icon
 * @return none
 */
void sgl_dropdown_add_option(sgl_obj_t *obj, const char *text, const sgl_icon_pixmap_t *icon);

/**
 * @brief delete an option from the dropdown
 * @param obj pointer to the dropdown object
 * @param text pointer to the text
 * @return none
 */
void sgl_dropdown_delete_option_by_text(sgl_obj_t *obj, const char *text);

/**
 * @brief delete an option from the dropdown
 * @param obj pointer to the dropdown object
 * @param index index of the option
 * @return none
 */
void sgl_dropdown_delete_option_by_index(sgl_obj_t *obj, int index);


#endif // !__SGL_DROPDOWN_H__
