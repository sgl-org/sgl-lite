/* source/core/sgl_core.c
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
#include <sgl_anim.h>
#include <sgl_math.h>
#include <sgl_mm.h>
#include <sgl_log.h>
#include <string.h>
#include <sgl_draw.h>
#include <sgl_font.h>
#include <sgl_theme.h>


/* current context, page pointer, and dirty area */
sgl_context_t sgl_ctx = {
    .fb_dev = {
        .xres = 0,
        .yres = 0,
        .xres_virtual = 0,
        .yres_virtual = 0,
        .buffer[0] = NULL,
        .buffer[1] = NULL,
        .buffer_size = 0,
    },
    .log_dev = {
        .log_puts = NULL,
    },
    .page = NULL,
    .tick_ms = 0,
};


/**
 * the memory pool, it will be used to allocate memory for the page pool
*/
static uint8_t sgl_mem_pool[CONFIG_SGL_HEAP_MEMORY_SIZE];


/**
 * @brief register the frame buffer device
 * @param fb_dev the frame buffer device
 * @return int, 0 if success, -1 if failed
 * @note you must check the return value of this function
 */
int sgl_device_fb_register(sgl_device_fb_t *fb_dev)
{
    sgl_check_ptr_return(fb_dev, -1);

    if (fb_dev->buffer[0] == NULL) {
        SGL_LOG_ERROR("You haven't set up the frame buffer.");
        SGL_ASSERT(0);
        return -1;
    }

    if (fb_dev->flush_area == NULL) {
        SGL_LOG_ERROR("You haven't set up the flush area.");
        SGL_ASSERT(0);
        return -1;
    }

    if (fb_dev->buffer_size == 0) {
        SGL_LOG_ERROR("You haven't set up the frame buffer size.");
        SGL_ASSERT(0);
        return -1;
    }

    sgl_ctx.fb_dev.buffer[0]   = fb_dev->buffer[0];
    sgl_ctx.fb_dev.buffer[1]   = fb_dev->buffer[1];
    sgl_ctx.fb_dev.buffer_size = fb_dev->buffer_size;

    sgl_ctx.fb_dev.xres             = fb_dev->xres;
    sgl_ctx.fb_dev.yres             = fb_dev->yres;
    sgl_ctx.fb_dev.xres_virtual     = fb_dev->xres_virtual;
    sgl_ctx.fb_dev.yres_virtual     = fb_dev->yres_virtual;
    sgl_ctx.fb_dev.flush_area       = fb_dev->flush_area;

    return 0;
}


/**
 * @brief initialize dirty area
 * @param none
 * @return none
 */
static inline void sgl_dirty_area_init(void)
{
    sgl_area_init(&sgl_ctx.dirty);
}


/**
 * @brief add object to parent
 * @param parent: pointer of parent object
 * @param obj: pointer of object
 * @return none
 */
void sgl_obj_add_child(sgl_obj_t *parent, sgl_obj_t *obj)
{
    SGL_ASSERT(parent != NULL && obj != NULL);
    sgl_obj_t *tail = parent->child;

    if (parent->child) {
        while (tail->sibling != NULL) {
            tail = tail->sibling;
        };
        tail->sibling = obj;
    }
    else {
        parent->child = obj;
    }

    obj->parent = parent;
}


/**
 * @brief remove an object from its parent
 * @param obj object to remove
 * @return none
 */
void sgl_obj_remove(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);

    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *pos = NULL;

    if (parent->child != obj) {
        pos = parent->child;
        while (pos->sibling != obj) {
            pos = pos->sibling;
        }
        pos->sibling = obj->sibling;
    }
    else {
        parent->child = obj->sibling;
    }

    obj->sibling = NULL;
}


/**
 * @brief move object child position
 * @param obj point to object
 * @param ofs_x: x offset position
 * @param ofs_y: y offset position
 * @return none
 */
void sgl_obj_move_child_pos(sgl_obj_t *obj, int16_t ofs_x, int16_t ofs_y)
{
    SGL_ASSERT(obj != NULL);
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;

    if (obj->child == NULL) {
        return;
    }
    obj->dirty = 1;
    stack[top++] = obj->child;

    while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        obj->dirty = 1;
        obj->coords.x1 += ofs_x;
        obj->coords.x2 += ofs_x;
        obj->coords.y1 += ofs_y;
        obj->coords.y2 += ofs_y;

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }
}


/**
 * @brief Set object position
 * @param obj point to object
 * @param x: x position
 * @param y: y position
 * @return none
 */
void sgl_obj_set_pos(sgl_obj_t *obj, int16_t x, int16_t y)
{
    SGL_ASSERT(obj != NULL);
    int16_t x_inc = x - obj->coords.x1;
    int16_t y_inc = y - obj->coords.y1;

    obj->dirty = 1;
    obj->coords.x1 = x + obj->parent->coords.x1;
    obj->coords.x2 += x_inc;
    obj->coords.y1 = y + obj->parent->coords.y1;
    obj->coords.y2 += y_inc;

    sgl_obj_move_child_pos(obj, x_inc, y_inc);
}


/**
 * @brief zoom object size
 * @param obj point to object
 * @param zoom zoom size
 * @return none
 * @note if you want to zoom out, the zoom should be positive, if you want to zoom in, the zoom should be negative
 */
void sgl_obj_size_zoom(sgl_obj_t *obj, int16_t zoom)
{
    SGL_ASSERT(obj != NULL);
    obj->coords.x1 -= zoom;
    obj->coords.x2 += zoom;
    obj->coords.y1 -= zoom;
    obj->coords.y2 += zoom;
}


/**
 * @brief move object up a level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_up(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL;
    sgl_obj_t *next = NULL;

    /* if the object is the last child, do not move it */
    if (obj->sibling == NULL) {
        return;
    }
    else if (parent->child == obj) {
        parent->child = obj->sibling;
        obj->sibling = obj->sibling->sibling;
        /* mark object as dirty */
        sgl_obj_set_dirty(obj);
        return;
    }

    /* move the object to its next sibling */
    sgl_obj_for_each_child(prev, parent) {
        if (prev->sibling == obj) {
            next = obj->sibling;
            obj->sibling = next->sibling;
            prev->sibling = next;
            next->sibling = obj;
            /* mark object as dirty */
            sgl_obj_set_dirty(obj);
            return;
        }
    }
}


/**
 * @brief move object down a level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_down(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL;
    sgl_obj_t *gprev = NULL;

    /* if the object is the first child, do not move it */
    if (parent->child == obj || obj->sibling == NULL) {
        return;
    }
    else if (parent->child->sibling == obj) {
        parent->child->sibling = obj->sibling;
        obj->sibling = parent->child;
        parent->child = obj;
        /* mark object as dirty */
        sgl_obj_set_dirty(obj);
        return;
    }

    /* move the object to its prev sibling */
    sgl_obj_for_each_child(gprev, parent) {
        prev = gprev->sibling;

        if (prev->sibling == obj) {
            prev->sibling = obj->sibling;
            gprev->sibling = obj;
            obj->sibling = prev;
            /* mark object as dirty */
            sgl_obj_set_dirty(obj);
            return;
        }
    }
}


/**
 * @brief move object top level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_foreground(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL && obj->parent != NULL);

    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL, *curr = parent->child, *last = NULL;

    /* if the object is the last child, do not move it */
    if (obj->sibling == NULL) {
        return;
    }

    while (curr != NULL && curr != obj) {
        prev = curr;
        curr = curr->sibling;
    }

    if (prev == NULL) {
        parent->child = obj->sibling;
    }
    else {
        prev->sibling = obj->sibling;
    }

    last = parent->child;
    if (last == NULL) {
        parent->child = obj;
        obj->sibling = NULL;
    }
    else {
        while (last->sibling != NULL) {
            last = last->sibling;
        }
        last->sibling = obj;
        obj->sibling = NULL;
    }

    sgl_obj_set_dirty(obj);
}


/**
 * @brief move object bottom level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_background(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *parent = obj->parent;
    sgl_obj_t *prev = NULL;

    /* if the object is the first child, do not move it */
    if (parent->child == obj) {
        return;
    }

    sgl_obj_for_each_child(prev, parent) {
        if (prev->sibling == obj) {
            break;
        }
    }

    prev->sibling = obj->sibling;
    obj->sibling = parent->child;
    parent->child = obj;
    /* mark object as dirty */
    sgl_obj_set_dirty(obj);
}


/**
 * @brief get fix radius of object
 * @param obj object
 * @return fix radius
 * @note if radius is larger than object's width or height, fix radius will be returned
 */
int16_t sgl_obj_fix_radius(sgl_obj_t *obj, size_t radius)
{
    int16_t w = (obj->coords.x2 - obj->coords.x1 + 1) / 2;
    int16_t h = (obj->coords.y2 - obj->coords.y1 + 1) / 2;
    int16_t r_min = w > h ? h : w;

    if ((int16_t)radius > r_min) {
        radius = r_min;
    }

    obj->radius = radius & 0xFFF;
    return radius;
}


/**
 * @brief page construct callback function
 * @param surf surface pointer
 * @param obj page object
 * @param evt event
 * @return none
 * @note evt not used
 */
static void sgl_page_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_area_t *area)
{
    sgl_page_t *page = (sgl_page_t *)obj;
    const sgl_pixmap_t *pixmap = page->bg_img;

    if (pixmap == NULL) {
        sgl_draw_fill_rect(surf, area, &obj->coords, page->color, SGL_ALPHA_MAX);
    }
    else {
        sgl_draw_fill_rect_pixmap(surf, area, &obj->coords, page->bg_img, SGL_ALPHA_MAX);
    }
}


/**
 * @brief set page background color
 * @param obj point to object
 * @param color background color
 * @return none
 */
void sgl_page_set_color(sgl_obj_t* obj, sgl_color_t color)
{
    sgl_page_t* page = (sgl_page_t*)obj;
    page->color = color;
    sgl_obj_set_dirty(obj);
}


/**
 * @brief set page background pixmap
 * @param obj point to object
 * @param pixmap background pixmap
 * @return none
 */
void sgl_page_set_pixmap(sgl_obj_t* obj, const sgl_pixmap_t *pixmap)
{
    sgl_page_t* page = (sgl_page_t*)obj;
    page->bg_img = pixmap;
    sgl_obj_set_dirty(obj);
}


/**
 * @brief create a page
 * @param none
 * @return sgl_page_t* the page pointer
 */
static sgl_page_t* sgl_page_create(void)
{
    sgl_page_t *page = sgl_malloc(sizeof(sgl_page_t));
    if (page == NULL) {
        SGL_LOG_ERROR("sgl_page_create: malloc failed");
        return NULL;
    }

    /* clear the page all fields */
    memset(page, 0, sizeof(sgl_page_t));

    sgl_obj_t *obj = &page->obj;

    if (sgl_ctx.fb_dev.buffer[0] == NULL) {
        SGL_LOG_ERROR("sgl_page_create: framebuffer is NULL");
        sgl_free(page);
        return NULL;
    }

    page->surf.buffer = (sgl_color_t*)sgl_ctx.fb_dev.buffer[0];
    page->surf.x = 0;
    page->surf.y = 0;
    page->surf.w = sgl_ctx.fb_dev.xres;
    page->surf.h = sgl_ctx.fb_dev.yres;
    page->surf.size = sgl_ctx.fb_dev.buffer_size;
    page->color = SGL_THEME_DESKTOP;

    obj->parent = obj;
    obj->construct_fn = sgl_page_construct_cb;
    obj->dirty = 1;
    obj->coords = (sgl_area_t) {
        .x1 = 0,
        .y1 = 0,
        .x2 = sgl_ctx.fb_dev.xres - 1,
        .y2 = sgl_ctx.fb_dev.yres - 1,
    };

    /* init child list */
    sgl_obj_node_init(&page->obj);

    if (sgl_ctx.page == NULL) {
        sgl_ctx.page = page;
    }

    return page;
}


/**
 * @brief Create an object
 * @param parent parent object
 * @return sgl_obj_t
 * @note if parent is NULL, the object will be as an new page
 */
sgl_obj_t* sgl_obj_create(sgl_obj_t *parent)
{
    sgl_obj_t *obj;

    /* create page object */
    if (parent == NULL) {
        sgl_page_t *page = sgl_page_create();
        if (page == NULL) {
            SGL_LOG_ERROR("sgl_obj_create: create page failed");
            return NULL;
        }
        obj = &page->obj;
        return obj;
    }
    else {
        obj = (sgl_obj_t*)sgl_malloc(sizeof(sgl_obj_t));
        if (obj == NULL) {
            SGL_LOG_ERROR("malloc failed");
            return NULL;
        }

        obj->coords = parent->coords;
        obj->parent = parent;
        obj->construct_fn = NULL;
        obj->dirty = 1;

        /* init node */
        sgl_obj_node_init(obj);
        /* add the child into parent's child list */
        sgl_obj_add_child(parent, obj);

        return obj;
    }
}


/**
 * @brief sgl global initialization
 * @param none
 * @return none
 * @note you should call this function before using sgl and you should call this function after register framebuffer device
 */
void sgl_init(void)
{
    /* init memory pool */
    sgl_mm_init(sgl_mem_pool, sizeof(sgl_mem_pool));

    /* initialize current context */
    sgl_ctx.page = NULL;

#if (CONFIG_SGL_EXTERNAL_PIXMAP)
    sgl_ctx.pixmap_buff = sgl_malloc(sgl_panel_resolution_width() * sizeof(sgl_color_t));
    if (sgl_ctx.pixmap_buff == NULL) {
        SGL_LOG_ERROR("sgl pixmap buff memory alloc failed");
        SGL_ASSERT(0);
        return;
    }
#endif

    /* initialize dirty area */
    sgl_dirty_area_init();

    /* create a screen object for drawing */
    sgl_obj_create(NULL);
}


/**
 * @brief set current object as screen object
 * @param obj object, that you want to set an object as active page
 * @return none
 */
void sgl_screen_load(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_ctx.page = (sgl_page_t*)obj;

    /* initilize framebuffer swap */
    sgl_ctx.fb_swap = 0;

    /* initialize dirty area */
    sgl_dirty_area_init();
    sgl_obj_set_dirty(obj);
}


/**
 * @brief color mixer
 * @param fg_color : foreground color
 * @param bg_color : background color
 * @param factor   : color mixer factor
 * @return sgl_color_t: mixed color
 */
sgl_color_t sgl_color_mixer(sgl_color_t fg_color, sgl_color_t bg_color, uint8_t factor)
{
    sgl_color_t ret;
#if (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_RGB233)

    ret.ch.red   = bg_color.ch.red + ((fg_color.ch.red - bg_color.ch.red) * (factor >> 5) >> 3);
    ret.ch.green = bg_color.ch.green + ((fg_color.ch.green - bg_color.ch.green) * (factor >> 5) >> 3);
    ret.ch.blue  = bg_color.ch.blue + ((fg_color.ch.blue - bg_color.ch.blue) * (factor >>6) >> 2);

#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_RGB565)

    uint32_t _alpha = factor >> 3;     
    uint32_t fg = fg_color.full;
    uint32_t bg = bg_color.full;

    fg = (fg | fg << 16) & 0x07e0f81f;
    bg = (bg | bg << 16) & 0x07e0f81f;
    bg += (fg - bg) * _alpha >> 5;
    bg &= 0x07e0f81f;
    ret.full = (bg | bg >> 16);

#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_RGB888)

    ret.ch.red   = bg_color.ch.red + ((fg_color.ch.red - bg_color.ch.red) * factor >> 8);
    ret.ch.green = bg_color.ch.green + ((fg_color.ch.green - bg_color.ch.green) * factor >> 8);
    ret.ch.blue  = bg_color.ch.blue + ((fg_color.ch.blue - bg_color.ch.blue) * factor >> 8);

#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == SGL_COLOR_ARGB8888)

    ret.ch.alpha = bg_color.ch.alpha + ((fg_color.ch.alpha - bg_color.ch.alpha) * factor >> 8);
    ret.ch.red   = bg_color.ch.red + ((fg_color.ch.red - bg_color.ch.red) * factor >> 8);
    ret.ch.green = bg_color.ch.green + ((fg_color.ch.green - bg_color.ch.green) * factor >> 8);
    ret.ch.blue  = bg_color.ch.blue + ((fg_color.ch.blue - bg_color.ch.blue) * factor >> 8);

#endif
    return ret;
}


/**
 * @brief  Get area intersection between surface and area
 * @param surf: surface
 * @param area: area
 * @param clip: intersection area
 * @return true: intersect, otherwise false
 * @note: this function is unsafe, you should check the surf and area is not NULL by yourself
 */
bool sgl_surf_clip(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *clip)
{
    SGL_ASSERT(surf != NULL && area != NULL && clip != NULL);
    int16_t h_pos = surf->y + surf->h - 1;
    int16_t w_pos = surf->x + surf->w - 1;

    if (area->y1 > h_pos || area->y2 < surf->y || area->x1 > w_pos || area->x2 < surf->x) {
        return false;
    }

    clip->x1 = sgl_max(surf->x, area->x1);
    clip->x2 = sgl_min(w_pos, area->x2);
    clip->y1 = sgl_max(surf->y, area->y1);
    clip->y2 = sgl_min(h_pos, area->y2);

    return true;
}


/**
 * @brief  Get area intersection between two areas
 * @param area_a: area a
 * @param area_b: area b
 * @param clip: intersection area
 * @return true: intersect, otherwise false
 * @note: this function is unsafe, you should check the area_a and area_b and clip is not NULL by yourself
 */
bool sgl_area_clip(sgl_area_t *area_a, sgl_area_t *area_b, sgl_area_t *clip)
{
    SGL_ASSERT(area_a != NULL && area_b != NULL && clip != NULL);
    if (area_b->y1 > area_a->y2 || area_b->y2 < area_a->y1 || area_b->x1 > area_a->x2 || area_b->x2 < area_a->x1) {
        return false;
    }

    clip->x1 = sgl_max(area_a->x1, area_b->x1);
    clip->x2 = sgl_min(area_a->x2, area_b->x2);
    clip->y1 = sgl_max(area_a->y1, area_b->y1);
    clip->y2 = sgl_min(area_a->y2, area_b->y2);

    return true;
}


/**
 * @brief clip area with another area
 * @param clip [in][out] clip area
 * @param area [in] area
 * @return true if clip area is valid, otherwise two area is not overlapped
 * @note: this function is unsafe, you should check the clip and area is not NULL by yourself
 */
bool sgl_area_selfclip(sgl_area_t *clip, sgl_area_t *area)
{
    SGL_ASSERT(clip != NULL && area != NULL);
    if (area->y1 > clip->y2 || area->y2 < clip->y1 || area->x1 > clip->x2 || area->x2 < clip->x1) {
        return false;
    }

    clip->x1 = sgl_max(clip->x1, area->x1);
    clip->x2 = sgl_min(clip->x2, area->x2);
    clip->y1 = sgl_max(clip->y1, area->y1);
    clip->y2 = sgl_min(clip->y2, area->y2);

    return true;
}


/**
 * @brief merge two area, the merge is result of the two area clip
 * @param area_a [in] area1
 * @param area_b [in] area2
 * @param merge  [out] merge result
 * @return none
 * @note: this function is unsafe, you should check the area_a and area_b and merge is not NULL by yourself
 */
void sgl_area_merge(sgl_area_t *area_a, sgl_area_t *area_b, sgl_area_t *merge)
{
    SGL_ASSERT(area_a != NULL && area_b != NULL && merge != NULL);
    merge->x1 = sgl_min(area_a->x1, area_b->x1);
    merge->x2 = sgl_max(area_a->x2, area_b->x2);
    merge->y1 = sgl_min(area_a->y1, area_b->y1);
    merge->y2 = sgl_max(area_a->y2, area_b->y2);
}


/**
 * @brief merge two area, the merge is a new area
 * @param merge [in][out] merge area
 * @param area [in] area
 * @return none
 * @note: this function is unsafe, you should check the merge and area is not NULL by yourself
 */
void sgl_area_selfmerge(sgl_area_t *merge, sgl_area_t *area)
{
    SGL_ASSERT(merge != NULL && area != NULL);
    merge->x1 = sgl_min(merge->x1, area->x1);
    merge->x2 = sgl_max(merge->x2, area->x2);
    merge->y1 = sgl_min(merge->y1, area->y1);
    merge->y2 = sgl_max(merge->y2, area->y2);
}


/**
 * @brief merge area with current dirty area
 * @param merge [in] merge area
 * @return none
 */
void sgl_obj_dirty_merge(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    /* skip invalid area */
    if (obj->coords.x1 > obj->coords.x2 || obj->coords.y1 > obj->coords.y2) {
        return;
    }

    /* direct to merge object area with dirty area  */
    sgl_ctx.dirty.x1 = sgl_min(sgl_ctx.dirty.x1, obj->coords.x1);
    sgl_ctx.dirty.x2 = sgl_max(sgl_ctx.dirty.x2, obj->coords.x2);
    sgl_ctx.dirty.y1 = sgl_min(sgl_ctx.dirty.y1, obj->coords.y1);
    sgl_ctx.dirty.y2 = sgl_max(sgl_ctx.dirty.y2, obj->coords.y2);
}


/**
 * @brief initialize object
 * @param obj object
 * @param parent parent object
 * @return int, 0 means successful, -1 means failed
 */
int sgl_obj_init(sgl_obj_t *obj, sgl_obj_t *parent)
{
    SGL_ASSERT(obj != NULL);

    if (parent == NULL) {
        parent = sgl_screen_act();
        if (parent == NULL) {
            SGL_LOG_ERROR("sgl_obj_init: have no active page");
            return -1;
        }
    }

    /* set essential member */
    obj->coords = parent->coords;
    obj->parent = parent;
    obj->construct_fn = NULL;
    obj->dirty = 1;

    /* add the child into parent's child list */
    sgl_obj_add_child(parent, obj);

    return 0;
}


/**
 * @brief Convert UTF-8 string to Unicode
 * @param utf8_str Pointer to the UTF-8 string to be converted
 * @param p_unicode_buffer Pointer to the buffer where the converted Unicode will be stored
 * @return The number of bytes in the UTF-8 string
 */
uint32_t sgl_utf8_to_unicode(const char *utf8_str, uint32_t *p_unicode_buffer)
{
    int bytes = 0;
    if (((uint8_t)(*utf8_str)) < 0x80) { // 1-byte/7-bit ASCII
        bytes = 1;
        *p_unicode_buffer = utf8_str[0];
    }
    else if ((((uint8_t)(*utf8_str)) & 0xE0) == 0xC0) { // 2-byte
        bytes = 2;
        *p_unicode_buffer = (utf8_str[0] & 0x1F) << 6;
        *p_unicode_buffer |= (utf8_str[1] & 0x3F);
    }
    else if ((((uint8_t)(*utf8_str)) & 0xF0) == 0xE0) { // 3-byte
        bytes = 3;
        *p_unicode_buffer = (utf8_str[0] & 0x0F) << 12;
        *p_unicode_buffer |= (utf8_str[1] & 0x3F) << 6;
        *p_unicode_buffer |= (utf8_str[2] & 0x3F);
    }
    else if ((((uint8_t)(*utf8_str)) & 0xF8) == 0xF0) { // 4-byte
        bytes = 4;
        *p_unicode_buffer = (utf8_str[0] & 0x07) << 18;
        *p_unicode_buffer |= (utf8_str[2] & 0x3F) << 6;
        *p_unicode_buffer |= (utf8_str[1] & 0x3F) << 12;
        *p_unicode_buffer |= (utf8_str[3] & 0x3F);
    }
    return bytes;
}


/**
 * @brief Search for the index of a Unicode character in the font table
 * @param font Pointer to the font structure containing character data
 * @param unicode Unicode of the character to be searched
 * @return Index of the character in the font table
 */
uint32_t sgl_search_unicode_ch_index(const sgl_font_t *font, uint32_t unicode)
{
    uint32_t left = 0, right = 0, mid = 0;
    uint32_t target = unicode;
    const sgl_font_unicode_t *code = font->unicode;

    for (uint32_t i = 1; i < font->unicode_num; i ++) {
        if (target < (code->offset + code->len)) {
            break;
        }
        code ++;
    }

    target -= code->offset;

    if (code->list == NULL) {
        if (target >= code->len) {
            SGL_LOG_WARN("sgl_search_unicode_ch_index: [0x%x]unicode not found in font table", unicode);
            return 0;
        }
        return target + code->tab_offset;
    }

    right = code->len - 1;
    while (left <= right) {
        mid = left + (right - left) / 2;

        if (code->list[mid] == target) {
            return mid + code->tab_offset;
        }
        else if (code->list[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    SGL_LOG_WARN("sgl_search_unicode_ch_index: [0x%x]unicode not found in font table", unicode);
    return 0;
}


/**
 * @brief get the width of a string
 * @param str string
 * @param font sgl font
 * @return width of string
 */
int32_t sgl_font_get_string_width(const char *str, const sgl_font_t *font)
{
    SGL_ASSERT(font != NULL);
    int32_t len = 0;
    uint32_t unicode = 0;
    uint32_t ch_index = 0;
    while (*str) {
        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);
        len += (font->table[ch_index].adv_w >> 4);
    }
    return len;
}


/**
 * @brief get the height of a string, which is in a rect area
 * @param width width of the rect area
 * @param str string
 * @param font sgl font of the string
 * @param line_space peer line space
 * @return height size of string
 */
int32_t sgl_font_get_string_height(int16_t width, const char *str, const sgl_font_t *font, uint8_t line_space)
{
    int16_t offset_x = 0;
    int16_t ch_index;
    int16_t ch_width;
    int16_t lines = 1;
    uint32_t unicode = 0;

    while (*str) {
        if (*str == '\n') {
            lines ++;
            offset_x = 0;
            str ++;
            continue;
        }

        str += sgl_utf8_to_unicode(str, &unicode);
        ch_index = sgl_search_unicode_ch_index(font, unicode);

        ch_width = (font->table[ch_index].adv_w >> 4);

        if ((offset_x + ch_width) >= width) {
            offset_x = 0;
            lines ++;
        }

        offset_x += ch_width;
    }

    return lines * (font->font_height + line_space);
}


/**
 * @brief get the alignment position
 * @param parent_size parent size
 * @param size object size
 * @param type alignment type
 * @return alignment position offset
 */
sgl_pos_t sgl_get_align_pos(sgl_size_t *parent_size, sgl_size_t *size, sgl_align_type_t type)
{
    SGL_ASSERT(parent_size != NULL && size != NULL);
    sgl_pos_t ret = {.x = 0, .y = 0};
    switch (type) {
        case SGL_ALIGN_CENTER:
            ret.x = (parent_size->w - size->w) / 2;
            ret.y = (parent_size->h - size->h) / 2;
        break;

        case SGL_ALIGN_TOP_MID:
            ret.x = (parent_size->w - size->w) / 2;
            ret.y = 0;
        break;

        case SGL_ALIGN_TOP_LEFT:
            ret.x = 0;
            ret.y = 0;
        break;

        case SGL_ALIGN_TOP_RIGHT:
            ret.x = parent_size->w - size->w;
            ret.y = 0;
        break;

        case SGL_ALIGN_BOT_MID:
            ret.x = (parent_size->w - size->w) / 2;
            ret.y = parent_size->h - size->h;
        break;

        case SGL_ALIGN_BOT_LEFT:
            ret.x = 0;
            ret.y = parent_size->h - size->h;
        break;

        case SGL_ALIGN_BOT_RIGHT:
            ret.x = parent_size->w - size->w;
            ret.y = parent_size->h - size->h;
        break;

        case SGL_ALIGN_LEFT_MID:
            ret.x = 0;
            ret.y = (parent_size->h - size->h) / 2;
        break;

        case SGL_ALIGN_RIGHT_MID:
            ret.x = parent_size->w - size->w;
            ret.y = (parent_size->h - size->h) / 2;
        break;

        default: break;
    }
    return ret;
}


/**
 * @brief get the text position in the area
 * @param area point to area
 * @param font point to font
 * @param text text string
 * @param offset text offset
 * @param type alignment type
 * @return sgl_pos_t position of text
 */
sgl_pos_t sgl_get_text_pos(sgl_area_t *area, const sgl_font_t *font, const char *text, int16_t offset, sgl_align_type_t type)
{
    SGL_ASSERT(area != NULL && font != NULL);
    sgl_pos_t ret = {.x = 0, .y = 0};
    sgl_size_t parent_size = {
        .w = area->x2 - area->x1 + 1,
        .h = area->y2 - area->y1 + 1,
    };

    sgl_size_t text_size = {
        .w = sgl_font_get_string_width(text, font) + offset,
        .h = sgl_font_get_height(font),
    };

    ret = sgl_get_align_pos(&parent_size, &text_size, type);
    ret.x += area->x1;
    ret.y += area->y1;

    return ret;
}


/**
 * @brief get the icon position of area
 * @param area point to area
 * @param icon point to icon
 * @param offset offset
 * @param type align type
 */
sgl_pos_t sgl_get_icon_pos(sgl_area_t *area, const sgl_icon_pixmap_t *icon, int16_t offset, sgl_align_type_t type)
{
    SGL_ASSERT(area != NULL && icon != NULL);
    sgl_pos_t ret = {.x = 0, .y = 0};
    sgl_size_t parent_size = {
        .w = area->x2 - area->x1 + 1,
        .h = area->y2 - area->y1 + 1,
    };

    sgl_size_t text_size = {
        .w = icon->width + offset,
        .h = icon->height,
    };

    ret = sgl_get_align_pos(&parent_size, &text_size, type);
    ret.x += area->x1;
    ret.y += area->y1;

    return ret;
}


/**
 * @brief Set the alignment position of the object relative to its parent object.
 * @param obj The object to set the alignment position.
 * @param type The alignment type.
 * @return none
 * @note type should be one of the sgl_align_type_t values:
 *       - SGL_ALIGN_CENTER    : Center the object in the parent object.
 *       - SGL_ALIGN_TOP_MID   : Align the object at the top middle of the parent object.
 *       - SGL_ALIGN_TOP_LEFT  : Align the object at the top left of the parent object.
 *       - SGL_ALIGN_TOP_RIGHT : Align the object at the top right of the parent object.
 *       - SGL_ALIGN_BOT_MID   : Align the object at the bottom middle of the parent object.
 *       - SGL_ALIGN_BOT_LEFT  : Align the object at the bottom left of the parent object.
 *       - SGL_ALIGN_BOT_RIGHT : Align the object at the bottom right of the parent object.
 *       - SGL_ALIGN_LEFT_MID  : Align the object at the left middle of the parent object.
 *       - SGL_ALIGN_RIGHT_MID : Align the object at the right middle of the parent object.
 */
void sgl_obj_set_pos_align(sgl_obj_t *obj, sgl_align_type_t type)
{
    SGL_ASSERT(obj != NULL);

    sgl_size_t p_size   = {0};
    sgl_pos_t  p_pos    = {0};
    sgl_pos_t  obj_pos  = {0};
    sgl_size_t obj_size = {
        .w = obj->coords.x2 - obj->coords.x1 + 1,
        .h = obj->coords.y2 - obj->coords.y1 + 1,
    };

    p_size = (sgl_size_t){
        .w = obj->parent->coords.x2 - obj->parent->coords.x1 + 1,
        .h = obj->parent->coords.y2 - obj->parent->coords.y1 + 1,
    };
    p_pos = (sgl_pos_t){
        .x = obj->parent->coords.x1,
        .y = obj->parent->coords.y1,
    };

    obj_pos = sgl_get_align_pos(&p_size, &obj_size, type);

    sgl_obj_set_pos(obj, p_pos.x + obj_pos.x,
                         p_pos.y + obj_pos.y
                    );
}


/**
 * @brief Set the alignment position of the object relative to sibling object.
 * @param ref The reference object, it should be the sibling object.
 * @param obj The object to set the alignment position.
 * @param type The alignment type.
 * @return none
 * @note type should be one of the sgl_align_type_t values:
 *       - SGL_ALIGN_VERT_LEFT  : Align the object at the left side of the reference object.
 *       - SGL_ALIGN_VERT_RIGHT : Align the object at the right side of the reference object.
 *       - SGL_ALIGN_VERT_MID   : Align the object at the middle of the reference object.
 *       - SGL_ALIGN_HORIZ_TOP  : Align the object at the top side of the reference object.
 *       - SGL_ALIGN_HORIZ_BOT  : Align the object at the bottom side of the reference object.
 *       - SGL_ALIGN_HORIZ_MID  : Align the object at the middle of the reference object.
 */
void sgl_obj_set_pos_align_ref(sgl_obj_t *ref, sgl_obj_t *obj, sgl_align_type_t type)
{
    SGL_ASSERT(ref != NULL && obj != NULL);

    if (unlikely(ref == obj->parent)) {
        sgl_obj_set_pos_align(obj, type);
        return;
    }

    int16_t ref_w = ref->coords.x2 - ref->coords.x1 + 1;
    int16_t obj_w = obj->coords.x2 - obj->coords.x1 + 1;
    int16_t ref_h = ref->coords.y2 - ref->coords.y1 + 1;
    int16_t obj_h = obj->coords.y2 - obj->coords.y1 + 1;

    switch (type) {
    case SGL_ALIGN_VERT_MID:
        obj->coords.x1 = ref->coords.x1 + (ref_w - obj_w) / 2;
        obj->coords.x2 = obj->coords.x1 + obj_w - 1;
        break;

    case SGL_ALIGN_VERT_LEFT:
        obj->coords.x1 = ref->coords.x1;
        obj->coords.x2 = obj->coords.x1 + obj_w - 1;
        break;

    case SGL_ALIGN_VERT_RIGHT:
        obj->coords.x1 = ref->coords.x2 - obj_w;
        obj->coords.x2 = obj->coords.x1 + obj_w - 1;
        break;

    case SGL_ALIGN_HORIZ_MID:
        obj->coords.y1 = ref->coords.y1 + (ref_h - obj_h) / 2;
        obj->coords.y2 = obj->coords.y1 + obj_h - 1;
        break;

    case SGL_ALIGN_HORIZ_TOP:
        obj->coords.y1 = ref->coords.y1;
        obj->coords.y2 = obj->coords.y1 + obj_h - 1;
        break;

    case SGL_ALIGN_HORIZ_BOT:
        obj->coords.y1 = ref->coords.y2 - obj_h;
        obj->coords.y2 = obj->coords.y1 + obj_h - 1;
        break;

    default:
        SGL_LOG_WARN("invalid align type");
        break;
    }
}


/**
 * @brief draw object slice completely
 * @param obj it should point to active root object
 * @param surf surface that draw to
 * @param dirty_h dirty height
 * @return none
 */
static inline void draw_obj_slice(sgl_obj_t *obj, sgl_surf_t *surf, int16_t dirty_h)
{
    int top = 0;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];

	SGL_ASSERT(obj != NULL);
	stack[top++] = obj;
    sgl_area_t area = SGL_AREA_INVALID;

	while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

        if (sgl_obj_is_hidden(obj)) {
            continue;
        }

		if (sgl_surf_area_is_overlap(surf, &sgl_ctx.dirty)) {
			SGL_ASSERT(obj->construct_fn != NULL);
            sgl_area_clip(&obj->parent->coords, &sgl_ctx.dirty, &area);
			obj->construct_fn(surf, obj, &area);

            if (obj->child != NULL) {
                stack[top++] = obj->child;
            }
		}
	}

    /* flush dirty area into screen */
    sgl_panel_flush_area(surf->x, surf->y, surf->w, dirty_h, surf->buffer);
}


/**
 * @brief calculate dirty area by for each all object that is dirty and visible
 * @param obj it should point to active root object
 * @return none
 * @note if there is no dirty area, the dirty area will remain unchanged
 */
static inline bool sgl_dirty_area_calculate(sgl_obj_t *obj)
{
    bool need_update = false;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    stack[top++] = obj;

    /* for each all object from the first task of page */
	while (top > 0) {
        SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        /* if sibling exists, push it to stack, it will be pop in next loop */
		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

        /* if object is hidden, skip it */
        if (unlikely(sgl_obj_is_hidden(obj))) {
            continue;
        }

        /* check child dirty and merge all dirty area */
        if (sgl_obj_is_dirty(obj)) {
            /* merge dirty area */
            sgl_obj_dirty_merge(obj);

            /* clear dirty flag */
            sgl_obj_clear_dirty(obj);

            need_update = true;
        }

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }
    return need_update;
}


/**
 * @brief sgl to draw complete frame
 * @param dirty the dirty area that need to upate
 * @return none
 * @note this function should be called in deamon thread or cyclic thread
 */
static inline void sgl_draw_task(sgl_area_t *dirty)
{
    sgl_surf_t *surf = &sgl_ctx.page->surf;
    sgl_obj_t *head = &sgl_ctx.page->obj;
    uint16_t max_h = 0, draw_h = 0, remaining = 0, dirty_w = 0, dirty_h = 0;

    /* fix dirty area if it is out of screen */
    dirty->x1 = sgl_max(dirty->x1, 0);
    dirty->x2 = sgl_min(dirty->x2, sgl_panel_resolution_width() - 1);
    dirty->y1 = sgl_max(dirty->y1, 0);
    dirty->y2 = sgl_min(dirty->y2, sgl_panel_resolution_height() - 1);

    /* to set start x and y position for dirty area */
    dirty_w = dirty->x2 - dirty->x1 + 1;
    dirty_h = dirty->y2 - dirty->y1 + 1;

    SGL_ASSERT(dirty_w > 0);

    surf->x = dirty->x1;
    surf->y = dirty->y1;
    surf->w = dirty_w;

    max_h = surf->size / dirty_w;
    surf->h = (uint16_t)sgl_min(max_h, (uint16_t)dirty_h);

    SGL_LOG_TRACE("sgl_draw_task: dirty area: x: %d, y: %d, w: %d, h: %d", dirty->x1, dirty->y1, dirty_w, dirty_h);

    while (surf->y < dirty->y2) {
        remaining = dirty->y2 - surf->y + 1;
        draw_h = (remaining < surf->h) ? remaining : surf->h;

        draw_obj_slice(head, surf, draw_h);
        surf->y += draw_h;
        /* swap buffer for dma operation, but it depends on double buffer */
        sgl_surf_buffer_swap(surf);
    }
}


/**
 * @brief sgl task handle function
 * @param none
 * @return none
 * @note this function should be called in main loop or timer or thread
 */
void sgl_task_handle(void)
{
    /* If the system tick time has not been reached, skip directly. */
    if (sgl_tick_get() < SGL_SYSTEM_TICK_MS) {
        return;
    }

#if (CONFIG_SGL_ANIMATION)
    sgl_anim_task();
#endif // !CONFIG_SGL_ANIMATION
    sgl_tick_reset();

    /* calculate dirty area, if no dirty area, return directly */
    if (sgl_dirty_area_calculate(&sgl_ctx.page->obj)) { 
        sgl_draw_task(&sgl_ctx.dirty);
        sgl_dirty_area_init();
    }
}
