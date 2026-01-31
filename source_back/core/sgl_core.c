/* source/core/sgl_core.c
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL
 * Document reference link: https://sgl-docs.readthedocs.io
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


/* current sgl system variable */
sgl_system_t sgl_system;


/**
 * the memory pool, it will be used to allocate memory for the page pool
*/
static uint8_t sgl_mem_pool[CONFIG_SGL_HEAP_MEMORY_SIZE];


/**
 * @brief register the frame buffer device
 * @param fbinfo the frame buffer device information
 * @return int, 0 if success, -1 if failed
 * @note you must check the result of this function
 */
int sgl_fbdev_register(sgl_fbinfo_t *fbinfo)
{
    sgl_check_ptr_return(fbinfo, -1);

    if (fbinfo->buffer[0] == NULL) {
        SGL_LOG_ERROR("You haven't set up the frame buffer.");
        SGL_ASSERT(0);
        return -1;
    }

    if (fbinfo->flush_area == NULL) {
        SGL_LOG_ERROR("You haven't set up the flush area.");
        SGL_ASSERT(0);
        return -1;
    }

    if (fbinfo->buffer_size == 0) {
        SGL_LOG_ERROR("You haven't set up the frame buffer size.");
        SGL_ASSERT(0);
        return -1;
    }

    sgl_system.fbdev.fbinfo = *fbinfo;

    sgl_system.fbdev.surf.buffer = (sgl_color_t*)fbinfo->buffer[0];
    sgl_system.fbdev.surf.x1 = 0;
    sgl_system.fbdev.surf.y1 = 0;
    sgl_system.fbdev.surf.x2 = fbinfo->xres - 1;
    sgl_system.fbdev.surf.y2 = fbinfo->yres - 1;
    sgl_system.fbdev.surf.size = fbinfo->buffer_size;
    sgl_system.fbdev.surf.w = fbinfo->xres;

    return 0;
}


/**
 * @brief get pixmap format bits
 * @param pixmap pointer to pixmap
 * @return pixmap bits of per pixel
 */
uint8_t sgl_pixmal_get_bits(const sgl_pixmap_t *pixmap)
{
    SGL_ASSERT(pixmap != NULL);
    uint8_t bits = 0;
    switch (pixmap->format)
    {
    case SGL_PIXMAP_FMT_NONE:
        bits = sizeof(sgl_color_t); break;
    case SGL_PIXMAP_FMT_RGB332:
    case SGL_PIXMAP_FMT_RLE_RGB332:
        bits = 1; break;
    case SGL_PIXMAP_FMT_RGB565:
    case SGL_PIXMAP_FMT_RLE_RGB565:
        bits = 2; break;
    case SGL_PIXMAP_FMT_RGB888:
    case SGL_PIXMAP_FMT_RLE_RGB888:
        bits = 3; break;
    case SGL_PIXMAP_FMT_RLE_RGBA8888:
        bits = 4; break;
    default:
        SGL_LOG_ERROR("pixmap format error");
        break;
    }
    return bits;
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
 * @brief Set object absolute position
 * @param obj point to object
 * @param abs_x: x absolute position
 * @param abs_y: y absolute position
 * @return none
 */
void sgl_obj_set_abs_pos(sgl_obj_t *obj, int16_t abs_x, int16_t abs_y)
{
    SGL_ASSERT(obj != NULL);
    int16_t x_diff = abs_x - obj->coords.x1;
    int16_t y_diff = abs_y - obj->coords.y1;

    obj->dirty = 1;
    obj->coords.x1 += x_diff;
    obj->coords.x2 += x_diff;
    obj->coords.y1 += y_diff;
    obj->coords.y2 += y_diff;

    sgl_obj_move_child_pos(obj, x_diff, y_diff);
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
void sgl_obj_move_top(sgl_obj_t *obj)
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
void sgl_obj_move_bottom(sgl_obj_t *obj)
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
 * @param radius: radius that you want to set
 * @return none
 * @note if radius is larger than object's width or height, fix radius will be returned
 */
void sgl_obj_set_radius(sgl_obj_t *obj, size_t radius)
{
    int16_t w = (obj->coords.x2 - obj->coords.x1 + 1);
    int16_t h = (obj->coords.y2 - obj->coords.y1 + 1);
    int16_t d_min = w > h ? h : w;

    if ((int16_t)radius >= (d_min / 2)) {
        radius = sgl_is_odd(d_min) ? d_min / 2 : (d_min - 1) / 2;
    }

    obj->radius = radius & 0xFFF;
}


#if (CONFIG_SGL_OBJ_USE_NAME && CONFIG_SGL_DEBUG)
/**
 * @brief print object name that include this all child
 * @param obj point to object
 * @return none
 */
void sgl_obj_print_name(sgl_obj_t *obj)
{
    int top = 0;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    stack[top++] = obj;

    while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

        if (obj->name == NULL) {
            SGL_LOG_INFO("[OBJ NAME]: %s", "NULL");
        }
        else {
            SGL_LOG_INFO("[OBJ NAME]: %s", obj->name);
        }

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }
}

#endif


/**
 * @brief page construct callback function
 * @param surf surface pointer
 * @param obj page object
 * @param evt event
 * @return none
 * @note evt not used
 */
static void sgl_page_construct_cb(sgl_surf_t *surf, sgl_obj_t* obj, sgl_event_t *evt)
{
    sgl_page_t *page = (sgl_page_t *)obj;
    const sgl_pixmap_t *pixmap = page->pixmap;

    if (evt->type == SGL_EVENT_DRAW_MAIN) {
        if (pixmap == NULL) {
            sgl_draw_fill_rect(surf, &obj->area, &obj->coords, 0, page->color, SGL_ALPHA_MAX);
        }
        else {
            sgl_draw_fill_rect_pixmap(surf, &obj->area, &obj->coords, 0, pixmap, SGL_ALPHA_MAX);
        }
    }
    else {
        if (obj->event_fn) {
            obj->event_fn(evt);
        }
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
    page->pixmap = pixmap;
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

    if (sgl_system.fbdev.fbinfo.buffer[0] == NULL) {
        SGL_LOG_ERROR("sgl_page_create: framebuffer is NULL");
        sgl_free(page);
        return NULL;
    }

    page->color = SGL_THEME_DESKTOP;

    obj->parent = obj;
    obj->clickable = 0;
    obj->construct_fn = sgl_page_construct_cb;
    obj->dirty = 1;
    obj->page = 1;
    obj->border = 0;
    obj->coords = (sgl_area_t) {
        .x1 = sgl_system.fbdev.surf.x1,
        .y1 = sgl_system.fbdev.surf.y1,
        .x2 = sgl_system.fbdev.surf.x2,
        .y2 = sgl_system.fbdev.surf.y2,
    };

    obj->area = obj->coords;

    /* init child list */
    sgl_obj_node_init(&page->obj);

    if (sgl_system.fbdev.active == NULL) {
        sgl_system.fbdev.active = &page->obj;
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
        obj->event_fn = NULL;
        obj->event_data = 0;
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
 * @brief initialize global dirty area
 * @param none
 * @return none
 */
static inline void sgl_dirty_area_init(void)
{
    sgl_system.fbdev.dirty_num = 0;
}


/**
 * @brief sgl global initialization
 * @param none
 * @return int, 0 means success, others means failed
 * @note you should call this function before using sgl and you should call this function after register framebuffer device
 */
int sgl_init(void)
{
    sgl_obj_t *obj = NULL;

    /* init memory pool */
    sgl_mm_init(sgl_mem_pool, sizeof(sgl_mem_pool));

    /* initialize current context */
    sgl_system.fbdev.active = NULL;

    /* initialize dirty area */
    sgl_dirty_area_init();

    /* create a screen object for drawing */
    obj = sgl_obj_create(NULL);
    if (obj == NULL) {
        SGL_LOG_ERROR("sgl_init: create screen object failed");
        return -1;
    }

    /* if the rotation is not 0 or 180, we need to alloc a buffer for rotation */
#if (CONFIG_SGL_FBDEV_ROTATION != 0)
    sgl_system.rotation = (sgl_color_t*)sgl_malloc(sgl_system.fbdev.fbinfo.buffer_size * sizeof(sgl_color_t));
    if (sgl_system.rotation == NULL) {
        SGL_LOG_ERROR("sgl_init: alloc rotation buffer failed");
        return -1;
    }
#endif

    /* create event queue */
    return sgl_event_queue_init();
}


/**
 * @brief set current object as screen object
 * @param obj object, that you want to set an object as active page
 * @return none
 */
void sgl_screen_load(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_system.fbdev.active = obj;

    /* initilize framebuffer swap */
    sgl_system.fbdev.fb_swap = 0;

    /* initialize dirty area */
    sgl_dirty_area_init();
    sgl_obj_set_dirty(obj);
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
 * @brief Computes the total boundary expansion (in Manhattan distance) required to merge rectangle b into rectangle a.
 *
 * @param a[in]    Pointer to the b rectangle
 * @param b[in]    Pointer to the a rectangle
 * @return int32_t Total expansion amount (always non-negative)
 */
static inline int32_t sgl_area_growth(sgl_area_t *a, sgl_area_t *b)
{
    return (a->x1 - sgl_min(a->x1, b->x1)) + (sgl_max(a->x2, b->x2) - a->x2)
           + (a->y1 - sgl_min(a->y1, b->y1)) + (sgl_max(a->y2, b->y2) - a->y2);
}


/**
 * @brief Quickly determines if two rectangles are close enough to be merged.
 *
 * This fast heuristic is useful in performance-critical contexts (e.g., real-time segmentation or region merging)
 * to avoid excessive fragmentation while preventing merges between distant regions.
 *
 * @param a[in] Pointer to the first rectangle
 * @param b[in] Pointer to the second rectangle
 * @return bool true if the rectangles are sufficiently close for merging; false otherwise
 */
static inline bool sgl_merge_determines(sgl_area_t* a, sgl_area_t* b)
{
    int16_t gap_x = (a->x1 > b->x2) ? (a->x1 - b->x2) : (b->x1 > a->x2) ? (b->x1 - a->x2) : 0;
    int16_t gap_y = (a->y1 > b->y2) ? (a->y1 - b->y2) : (b->y1 > a->y2) ? (b->y1 - a->y2) : 0;    
    int16_t threshold = (sgl_min4(a->x2 - a->x1 + 1, a->y2 - a->y1 + 1, b->x2 - b->x1 + 1, b->y2 - b->y1 + 1) >> 2);

    return (gap_x <= threshold) && (gap_y <= threshold);
}


/**
 * @brief merge an area into global dirty area
 * 
 * This function calculates how much rectangle 'a' would need to grow in each direction (left, right, top, bottom)
 * to fully enclose both 'a' and 'b'. The result is the sum of the expansions along all four sides.
 * Note: This is not the increase in area, th is a lightweight heuristic for merge cost in bounding-box algorithms.
 * 
 * @param area [in] Pointer to the area
 * @return none
 */
void sgl_dirty_area_push(sgl_area_t *area)
{
    SGL_ASSERT(area != NULL);
    int32_t best_idx = -1, min_growth = INT32_MAX, growth = INT32_MAX;
    /* skip invalid area */
    if (area->x1 > area->x2 || area->y1 > area->y2) {
        return;
    }

    if (sgl_system.fbdev.dirty_num == 0) {
        sgl_system.fbdev.dirty[0] = *area;
        sgl_system.fbdev.dirty_num = 1;
        return;
    }

    for (uint8_t i = 0; i < sgl_system.fbdev.dirty_num; i++) {
        if (sgl_merge_determines(&sgl_system.fbdev.dirty[i], area)) {
            growth = sgl_area_growth(&sgl_system.fbdev.dirty[i], area);
            if (growth == 0) {
                /* already contains the area */
                return;
            }
            else if (growth < min_growth) {
                min_growth = growth;
                best_idx = i;
            }
        }
    }

    if (best_idx >= 0) {
        /* merge object area into best_idx dirty area */
        sgl_area_selfmerge(&sgl_system.fbdev.dirty[best_idx], area);
        return;
    }

    if (sgl_system.fbdev.dirty_num < SGL_DIRTY_AREA_NUM_MAX) {
        /* add new dirty area */
        sgl_system.fbdev.dirty[sgl_system.fbdev.dirty_num++] = *area;
    } else {
        /* merge object area into last dirty area */
        sgl_area_selfmerge(&sgl_system.fbdev.dirty[SGL_DIRTY_AREA_NUM_MAX - 1], area);
    }
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
    obj->event_fn = NULL;
    obj->event_data = 0;
    obj->construct_fn = NULL;
    obj->dirty = 1;
    obj->clickable = 0;

    /* init object area to invalid */
    sgl_area_init(&obj->area);

    /* add the child into parent's child list */
    sgl_obj_add_child(parent, obj);

    return 0;
}


/**
 * @brief  free an object
 * @param  obj: object to free
 * @retval none
 * @note this function will free all the children of the object
 */
void sgl_obj_free(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];
    int top = 0;
    stack[top++] = obj;

    while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}

        sgl_free(obj);
    }
}


/**
 * @brief delete object
 * @param obj point to object
 * @return none
 * @note this function will set object and his childs to be destroyed, then next draw cycle, the object will be removed.
 *       if object is NULL, the all objects of active page will be delete, but the page object will not be deleted.
 *       if object is a page, the page object will be deleted and all its children will be deleted.
 */
void sgl_obj_delete(sgl_obj_t *obj)
{
    if (obj == NULL || obj == sgl_screen_act()) {
        obj = sgl_screen_act();
        sgl_dirty_area_push(&obj->area);
        if (obj->child) {
            sgl_obj_free(obj->child);
        }
        sgl_obj_node_init(obj);
        return;
    }
    else if (obj->page == 1) {
        sgl_obj_free(obj);
        return;
    }

    sgl_obj_set_destroyed(obj);
    sgl_obj_set_dirty(obj);
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

    sgl_obj_set_abs_pos(obj, p_pos.x + obj_pos.x, p_pos.y + obj_pos.y);
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
static inline void draw_obj_slice(sgl_obj_t *obj, sgl_surf_t *surf)
{
    int top = 0;
	sgl_event_t evt;
	sgl_obj_t *stack[SGL_OBJ_DEPTH_MAX];

	SGL_ASSERT(obj != NULL);
	stack[top++] = obj;

	while (top > 0) {
		SGL_ASSERT(top < SGL_OBJ_DEPTH_MAX);
		obj = stack[--top];

		if (obj->sibling != NULL) {
			stack[top++] = obj->sibling;
		}

        if (sgl_obj_is_hidden(obj)) {
            continue;
        }

		if (sgl_surf_area_is_overlap(surf, &obj->area)) {
			evt.type = SGL_EVENT_DRAW_MAIN;
			SGL_ASSERT(obj->construct_fn != NULL);
			obj->construct_fn(surf, obj, &evt);

            if (obj->child != NULL) {
                stack[top++] = obj->child;
            }
		}
	}

    /* flush dirty area into screen */
    sgl_fbdev_flush_area((sgl_area_t*)surf, surf->buffer);
}


/**
 * @brief calculate dirty area by for each all object that is dirty and visible
 * @param obj it should point to active root object
 * @return none
 * @note if there is no dirty area, the dirty area will remain unchanged
 */
static inline void sgl_dirty_area_calculate(sgl_obj_t *obj)
{
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

        /* check if obj is destroyed */
        if (unlikely(sgl_obj_is_destroyed(obj))) {
            /* merge destroy area */
            sgl_dirty_area_push(&obj->area);

            /* remove obj from parent */
            sgl_obj_remove(obj);

            /* free obj resource */
            sgl_obj_free(obj);

            /* object is destroyed, skip */
            continue;
        }

        /* check child need init coords */
        if (unlikely(sgl_obj_is_needinit(obj))) {
            sgl_event_t evt = {
                .type = SGL_EVENT_DRAW_INIT,
            };

            /* check construct function */
            SGL_ASSERT(obj->construct_fn != NULL);
            obj->construct_fn(NULL, obj, &evt);
            /* maybe no need to clear flag */
            sgl_obj_clear_needinit(obj);
        }

        /* check child dirty and merge all dirty area */
        if (sgl_obj_is_dirty(obj)) {
            /* merge dirty area */
            sgl_dirty_area_push(&obj->area);

            sgl_area_t fill_area = sgl_obj_get_fill_rect(obj->parent);
            /* update obj area */
            if (unlikely(!sgl_area_clip(&fill_area, &obj->coords, &obj->area))) {
                sgl_area_init(&obj->area);
                sgl_obj_clear_dirty(obj);
                continue;
            }

            /* merge dirty area */
            sgl_dirty_area_push(&obj->area);

            /* clear dirty flag */
            sgl_obj_clear_dirty(obj);
        }

		if (obj->child != NULL) {
			stack[top++] = obj->child;
		}
    }
}


/**
 * @brief sgl to draw complete frame
 * @param fbdev point to  frame buffer device
 * @return none
 * @note this function should be called in deamon thread or cyclic thread
 */
static inline void sgl_draw_task(sgl_fbdev_t *fbdev)
{
    sgl_surf_t *surf = &fbdev->surf;
    sgl_obj_t  *head = fbdev->active;
    sgl_area_t *dirty = NULL;

    /* dirty area number must less than SGL_DIRTY_AREA_MAX */
    for (int i = 0; i < fbdev->dirty_num; i++) {
        dirty = &fbdev->dirty[i];
        surf->dirty = dirty;

        /* check dirty area, ensure it is valid */
        SGL_ASSERT(dirty != NULL && dirty->x1 >= 0 && dirty->y1 >= 0 && dirty->x2 < SGL_SCREEN_WIDTH && dirty->y2 < SGL_SCREEN_HEIGHT);

#if (!CONFIG_SGL_USE_FBDEV_VRAM)

        uint16_t draw_h = 0;
        surf->h = dirty->y2 - dirty->y1 + 1;

        surf->x1 = dirty->x1;
        surf->y1 = dirty->y1;
        surf->x2 = dirty->x2;
        surf->w  = surf->x2 - surf->x1 + 1;
        surf->h  = sgl_min(surf->size / surf->w, (uint32_t)(dirty->y2 - dirty->y1 + 1));

        SGL_LOG_TRACE("[fb:%d]sgl_draw_task: dirty area  x1:%d y1:%d x2:%d y2:%d", fbdev->fb_swap, dirty->x1, dirty->y1, dirty->x2, dirty->y2);

        while (surf->y1 <= dirty->y2) {
            draw_h = sgl_min(dirty->y2 - surf->y1 + 1, surf->h);

            surf->y2 = surf->y1 + draw_h - 1;
            fbdev->fb_status = (fbdev->fb_status & (1 << fbdev->fb_swap));

            draw_obj_slice(head, surf);
            surf->y1 += draw_h;

            /* wait flush ready */
            while (sgl_fbdev_flush_wait_ready(fbdev));

            /* swap the double buffer */
            sgl_surf_buffer_swap(surf);
        }
#else
        /* check dirty area, ensure it is valid */
        SGL_ASSERT(dirty != NULL && dirty->x1 >= 0 && dirty->y1 >= 0 && dirty->x2 < SGL_SCREEN_WIDTH && dirty->y2 < SGL_SCREEN_HEIGHT);

        SGL_LOG_TRACE("[fb:%d]sgl_draw_task: dirty area  x1:%d y1:%d x2:%d y2:%d", fbdev->fb_swap, dirty->x1, dirty->y1, dirty->x2, dirty->y2);
        draw_obj_slice(head, surf);
        sgl_surf_buffer_swap(surf);
#endif
    }
    /* clear dirty area */
    fbdev->dirty_num = 0;
}


/**
 * @brief sgl task handle function with sync mode
 * @param none
 * @return none
 * @note you can call this function for force update screen
 */
void sgl_task_handle_sync(void)
{
    /* event task */
    sgl_event_task();

#if (CONFIG_SGL_ANIMATION)
    sgl_anim_task();
#endif // !CONFIG_SGL_ANIMATION
    sgl_tick_reset();

    /* foreach all object tree and calculate dirty area */
    sgl_dirty_area_calculate(sgl_system.fbdev.active);

    /* draw all object into screen */
    sgl_draw_task(&sgl_system.fbdev);
}
