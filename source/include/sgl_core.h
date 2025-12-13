/* source/include/sgl_core.h
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

#ifndef __SGL_CORE_H__
#define __SGL_CORE_H__

#include <sgl_cfgfix.h>
#include <stddef.h>
#include <sgl_log.h>
#include <sgl_list.h>
#include <sgl_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/* the maximum depth of object*/
#define  SGL_OBJ_DEPTH_MAX                 (16)
/* the maximum number of drawing buffers */
#define  SGL_DRAW_BUFFER_MAX               (2)
/* define default animation tick ms */
#define  SGL_SYSTEM_TICK_MS                CONFIG_SGL_SYSTICK_MS


/* the ASCII offset of fonts */
#define  SGL_TEXT_ASCII_OFFSET             (32)


/**
* @brief This enumeration type defines the alignment of controls in sgl,
*        i.e. coordinate positions
*/
typedef enum sgl_align_type {
    SGL_ALIGN_CENTER = 0,
    SGL_ALIGN_TOP_MID,
    SGL_ALIGN_TOP_LEFT,
    SGL_ALIGN_TOP_RIGHT,
    SGL_ALIGN_BOT_MID,
    SGL_ALIGN_BOT_LEFT,
    SGL_ALIGN_BOT_RIGHT,
    SGL_ALIGN_LEFT_MID,
    SGL_ALIGN_RIGHT_MID,
    SGL_ALIGN_VERT_LEFT,
    SGL_ALIGN_VERT_RIGHT,
    SGL_ALIGN_VERT_MID,
    SGL_ALIGN_HORIZ_TOP,
    SGL_ALIGN_HORIZ_BOT,
    SGL_ALIGN_HORIZ_MID,
    SGL_ALIGN_NUM,

} sgl_align_type_t;


/**
* @brief This enumeration type defines the layout of controls in sgl,
*        i.e. coordinate positions
*/
typedef enum sgl_layout_type {
    SGL_LAYOUT_NONE = 0,
    SGL_LAYOUT_HORIZONTAL = 1,
    SGL_LAYOUT_VERTICAL = 2,
    SGL_LAYOUT_GRID = 3,
    SGL_LAYOUT_NUM = 4,
} sgl_layout_type_t;


/**
* @brief This structure is a structure that describes the position of the control,
*        where x represents the position of the x coordinate, which is the row coordinate position,
*        and y represents the position of the y coordinate, which is the column coordinate position
*
* @x: x coordinate
* @y: y coordinate
*/
typedef struct sgl_pos {
    int16_t x;
    int16_t y;
} sgl_pos_t;


/**
* @brief This structure describes the size of the object, including width and height, in pixels
*
* @w: width
* @h: height
*/
typedef struct sgl_size {
    int16_t w;
    int16_t h;
} sgl_size_t;


/**
* @brief This structure describes a rectangular region, where x1 and y1 represent the coordinates
*        of the upper left corner of the rectangle, and x2 and y2 represent the coordinates of the
*        lower right corner of the rectangle
*
* @x1: x position left corner of the rectangle
* @y1: y position left corner of the rectangle
* @x2: x position right corner of the rectangle
* @y2: y position right corner of the rectangle
*/
typedef struct sgl_area {
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
} sgl_area_t;

#define sgl_rect_t sgl_area_t


/**
* @brief This structure defines a 32 bit color bit field
*
* @blue: Blue color component
* @green: Green color component
* @red: Red color component
* @alpha: Color transparency
*/
typedef union {
    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    } ch;
    uint32_t full;
} sgl_color32_t;


/**
* @brief This structure defines a 24 bit color bit field
*
* @blue: Blue color component
* @green: Green color component
* @red: Red color component
*/
typedef union {
    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    } ch;
    uint8_t full[3];
} sgl_color24_t;


/**
* @brief This structure defines a 16 bit color bit field
*
* @blue: Blue color component
* @green: Green color component
* @red: Red color component
* @alpha: Color transparency
*/
typedef union {
    struct {
        uint16_t blue : 5;
        uint16_t green : 6;
        uint16_t red : 5;
    } ch;
    uint16_t full;
} sgl_color16_t;


/**
* @brief This structure defines a 8 bit color bit field
*
* @blue: Blue color component
* @green: Green color component
* @red: Red color component
* @alpha: Color transparency
*/
typedef union {
    struct {
        uint8_t blue : 2;
        uint8_t green : 3;
        uint8_t red : 3;
    } ch;
    uint8_t full;
} sgl_color8_t;


#if (CONFIG_SGL_PANEL_PIXEL_DEPTH == 32)
#define sgl_color_t sgl_color32_t
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 24)
#define sgl_color_t sgl_color24_t
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 16)
#define sgl_color_t sgl_color16_t
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 8)
#define sgl_color_t sgl_color8_t
#endif


/**
 * @brief This structure defines a surface, which is a rectangular area of the screen.
 * @buffer: buffer pointer
 * @x:      x coordinate
 * @y:      y coordinate
 * @w:      width
 * @h:      height
 * @pitch:  bytes per line
 * @h_max:  maximum height
 */
typedef struct sgl_surf {
    sgl_color_t *buffer;
    int16_t      x;
    int16_t      y;
    uint16_t     w;
    int16_t      h;
    uint32_t     size;
} sgl_surf_t;


/**
* @brief This structure defines an image, with a bitmap pointing to the
*        bitmap of the image, while specifying the width and height of the image
*
* @width: pixmap width
* @height: pixmap height
* @format: bitmap format 0: no compression, 1:
* @bitmap: point to image bitmap
* @read: read pixel map from external storage
* @data: user private data for @read
*/
typedef struct sgl_pixmap {
    uint32_t width : 12;
    uint32_t height : 12;
    uint32_t format : 8;

#if (CONFIG_SGL_EXTERNAL_PIXMAP == 0)
    const uint8_t *bitmap;
#else
    void (*read)(void *buff, uint32_t pos, size_t size, void *data);
    union {
        const uint8_t *bitmap;
        void          *data;
    };
#endif
} sgl_pixmap_t;


/**
 * @brief This structure defines an icon, with a bitmap pointing to the
 * @bitmap: point to icon bitmap
 * @width: pixmap width
 * @height: pixmap height
 * @bpp: bitmap pixel deepth
 */
typedef struct sgl_icon_pixmap {
    const uint8_t *bitmap;
    uint32_t       width : 12;
    uint32_t       height : 12;
    uint32_t       bpp : 8;
} sgl_icon_pixmap_t;


/**
* @brief Font index table structure, used to describe the bitmap index positions of
*        all characters in a font, accelerating the search process
*
* @bitmap_index: point to bitmap index of font
* @box_h: height of font
* @box_w: width of font
* @ofs_x: offset of character x position
* @ofs_y: offset of character y position
*/
typedef struct sgl_font_table {
    const uint32_t bitmap_index;
    const uint16_t adv_w;
    const uint16_t box_h;
    const uint16_t box_w;
    const int8_t  ofs_x;
    const int8_t  ofs_y;
} sgl_font_table_t;


/**
 * @brief This structure defines a font unicode information, which is a collection of fonts
 * @offset: offset of unicode for unicode list
 * @len: length of unicode list
 * @list: point to unicode list
 * @tab_offset: offset of font table
 */
typedef struct sgl_font_unicode {
    const uint32_t offset;
    const uint32_t len;
    const uint16_t *list;
    const uint32_t tab_offset;
} sgl_font_unicode_t;


/**
* @brief A structure used to describe information about a font, Defining a font set requires
*        the use of this structure to describe relevant information
*
* @bitmap: point to bitmap of font
* @table: point to struct sgl_font_table
* @font_table_size: size of struct sgl_font_table
* @font_height: height of font
* @bpp: The anti aliasing level of the font
* @compress: compress flag, 0: no compress, 1: compress
* @unicode: point to struct sgl_font_unicode struct
* @unicode_num: number of unicode parts
* @base_line: base line of font
*/
typedef struct sgl_font {
    const uint8_t  *bitmap;
    const sgl_font_table_t  *table;
    const uint16_t  font_table_size;
    const uint16_t  font_height : 12;
    const uint16_t  bpp : 3;          // AA depth, only support 2, 4
    const uint16_t  compress : 1;     // compress flag
    const sgl_font_unicode_t *unicode;
    const uint32_t  unicode_num;
    const int32_t   base_line;
} sgl_font_t;


typedef struct sgl_obj {
    sgl_area_t         coords;
    void               (*construct_fn)(sgl_surf_t *surf, struct sgl_obj *obj, sgl_area_t *area);
    struct sgl_obj     *parent;
    struct sgl_obj     *child;
    struct sgl_obj     *sibling;
    uint8_t            dirty;
    uint8_t            hide;
    uint16_t           radius;
} sgl_obj_t;


/**
 * @brief Represents a page or layer object containing graphical content, child object slots, and background information.
 *
 * This structure defines a renderable page unit. It is itself a graphical object (sgl_obj_t)
 * with basic properties like position and size. It owns a drawing surface (surf) for pixel
 * storage and rendering operations. The structure manages a collection of child objects
 * (such as widgets or graphical elements) through the 'slot' array and 'slot_count'.
 * It also holds a default color for rendering operations and an optional pointer to a
 * background image (bg_img) that can be displayed behind the page content.
 *
 * Fields:
 *   obj        - The base graphical object, inheriting core properties and state.
 *   surf       - The drawing surface associated with this page, used for pixel operations.
 *   slot       - Array of pointers to child sgl_obj_t objects managed by this page.
 *   slot_count - The number of valid child object pointers currently in the 'slot' array.
 *   color      - The default color used for rendering operations on this page.
 *   bg_img     - Pointer to the background pixmap; NULL if no background image is set.
 */
typedef struct sgl_page {
    sgl_obj_t          obj;
    sgl_surf_t         surf;
    sgl_color_t        color;
    const sgl_pixmap_t *bg_img;
} sgl_page_t;


/**
 * @brief sgl framebuffer device struct
 * @buffer: framebuffer, this specify the memory address of the framebuffer
 * @framebuffer_size: framebuffer size
 * @xres: x resolution
 * @yres: y resolution
 * @xres_virtual: x virtual resolution
 * @yres_virtual: y virtual resolution
 * @flush_area: flush area callback function pointer
 */
typedef struct sgl_device_fb {
    void      *buffer[SGL_DRAW_BUFFER_MAX];
    uint32_t   buffer_size;
    int16_t    xres;
    int16_t    yres;
    int16_t    xres_virtual;
    int16_t    yres_virtual;
    void       (*flush_area)(int16_t x, int16_t y, int16_t w, int16_t h, sgl_color_t *src);
} sgl_device_fb_t;


/**
 * @brief sgl log print device struct
 * @log_puts: log print callback function pointer
 */
typedef struct sgl_device_log {
    void      (*log_puts)(const char *str);
} sgl_device_log_t;


/* current context, page pointer, and dirty area */
typedef struct sgl_context {
    sgl_page_t           *page;
    sgl_device_fb_t      fb_dev;
    sgl_device_log_t     log_dev;
    uint8_t              fb_swap;
    uint8_t              tick_ms;
    sgl_area_t           dirty;
#if (CONFIG_SGL_EXTERNAL_PIXMAP)
    sgl_color_t          *pixmap_buff;
#endif
} sgl_context_t;


/**
 * @brief sgl object foreach child macro
 * @param obj: The parent object whose children are being iterated.
 * @param child: The current child object being processed.
 */
#define  sgl_obj_foreach_child(obj, child)                  for ((child) = (obj)->child; (child) != NULL; (child) = (child)->sibling)


/* dont to use this variable, it is used internally by sgl library */
extern sgl_context_t sgl_ctx;


/**
 * @brief register the frame buffer device
 * @param fb_dev the frame buffer device
 * @return int, 0 if success, -1 if failed
 */
int sgl_device_fb_register(sgl_device_fb_t *fb_dev);


/**
 * @brief panel flush function
 * @param x [in] x coordinate
 * @param y [in] y coordinate
 * @param w [in] width
 * @param h [in] height
 * @param src [in] source color
 * @return none
 */
static inline void sgl_panel_flush_area(int16_t x, int16_t y, int16_t w, int16_t h, sgl_color_t *src)
{
#if CONFIG_SGL_COLOR16_SWAP
    uint16_t *dst = (uint16_t *)src;
    for (size_t i = 0; i < (size_t)(w * h); i++) {
        dst[i] = (dst[i] << 8) | (dst[i] >> 8);
    }
#endif
    sgl_ctx.fb_dev.flush_area(x, y, w, h, src);
}


/**
 * @brief get panel resolution width
 * @param none
 * @return panel resolution width
 */
static inline int16_t sgl_panel_resolution_width(void)
{
    return sgl_ctx.fb_dev.xres;
}

/**
 * @brief get panel resolution height
 * @param none
 * @return panel resolution height
 */
#define  SGL_SCREEN_WIDTH  sgl_panel_resolution_width()


/**
 * @brief get panel resolution height
 * @param none
 * @return panel resolution height
 */
static inline int16_t sgl_panel_resolution_height(void)
{
    return sgl_ctx.fb_dev.yres;
}

/**
 * @brief get panel resolution width
 * @param none
 * @return panel resolution width
 */
#define  SGL_SCREEN_HEIGHT  sgl_panel_resolution_height()


/**
 * @brief get panel buffer address
 * @param none
 * @return panel buffer address
 */
static inline void* sgl_panel_buffer_address(void)
{
    return sgl_ctx.fb_dev.buffer[0];
}


/**
 * @brief register log output device
 * @param log_puts log output function
 * @return none
 */
static inline void sgl_device_log_register(void (*log_puts)(const char *str))
{
    sgl_ctx.log_dev.log_puts = log_puts;
}


/**
 * @brief log output function
 * @param str log string
 * @return none
 */
static inline void sgl_log_stdout(const char *str)
{
    if (sgl_ctx.log_dev.log_puts) {
        sgl_ctx.log_dev.log_puts(str);
    }
}


/**
 * @brief get tick milliseconds
 * @param none
 * @return tick milliseconds
 */
static inline uint8_t sgl_tick_get(void)
{
    return sgl_ctx.tick_ms;
}


/**
 * @brief increase tick milliseconds
 * @param ms milliseconds
 * @return none
 * @note in general, you should call this function in the 1ms tick interrupt handler
 *       of course, you can use polling function to increase tick milliseconds.
 */
static inline void sgl_tick_inc(uint8_t ms)
{
    sgl_ctx.tick_ms += ms;
}


/**
 * @brief reset tick milliseconds
 * @param none
 * @return none
 */
static inline void sgl_tick_reset(void)
{
    sgl_ctx.tick_ms = 0;
}


/**
* @brief converts the color value of an integer into a color structure
* @param: color value
* @return: sgl_color_t
*/
static inline sgl_color_t sgl_int2color(uint32_t color)
{
    sgl_color_t c;
#if (CONFIG_SGL_PANEL_PIXEL_DEPTH == 32)
    c.full = color;
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 24)
    c.ch.blue    = (uint8_t)(color & 0xff);
    c.ch.green   = (uint8_t)((color >> 8) & 0xff);
    c.ch.red     = (uint8_t)((color >> 16) & 0xff);
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 16)
    c.ch.blue    = (uint8_t)(color & 0x1f);
    c.ch.green   = (uint8_t)((color >> 5) & 0x3f);
    c.ch.red     = (uint8_t)((color >> 11) & 0x1f);
#elif (CONFIG_SGL_PANEL_PIXEL_DEPTH == 8)
    c.ch.blue    = (uint8_t)(color & 0x3);
    c.ch.green   = (uint8_t)((color >> 2) & 0x7);
    c.ch.red     = (uint8_t)((color >> 5) & 0x7);
#endif
    return c;
}


/**
 * @brief converts the color structure into an integer
 * @param: color structure
 * @return: integer
 */
static inline uint32_t sgl_color2int(sgl_color_t color)
{
    uint32_t c;
#if (CONFIG_SGL_PANEL_PIXEL_DEPTH == 24)
    c = color.ch.blue | (color.ch.green << 8) | (color.ch.red << 16);
#else
    c = color.full;
#endif
    return c;
}


/**
* @brief Inline function that converts the color value of an (r,g,b) into a color structure
* @param: red    Red color component
* @param: green  Green color component
* @param: blue   Blue color component
* @return: sgl_color_t
*/
static inline sgl_color_t sgl_rgb2color(uint8_t red, uint8_t green, uint8_t blue)
{
    sgl_color_t color;
    color.ch.blue = blue;
    color.ch.green = green;
    color.ch.red = red;
    return color;
}


/**
 * @brief for each child object of parent
 * @param _child: pointer of child object
 * @param parent: pointer of parent object
 */
#define  sgl_obj_for_each_child(_child, parent)                   for (_child = parent->child; _child != NULL; _child = _child->sibling)


/**
 * @brief for each child object of parent safely
 * @param _child: pointer of child object
 * @param parent: pointer of parent object
 */
#define sgl_obj_for_each_child_safe(_child, n, parent)            for (_child = parent->child, n = (_child ? _child->sibling : NULL); _child != NULL; \
                                                                       _child = n, n = (_child ? _child->sibling : NULL))


/**
 * @brief init object node
 * @param obj: pointer of object
 * @return none
 */
static inline void sgl_obj_node_init(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);

    obj->sibling = NULL;
    obj->child = NULL;
}


/**
 * @brief add object to parent
 * @param parent: pointer of parent object
 * @param obj: pointer of object
 * @return none
 */
void sgl_obj_add_child(sgl_obj_t *parent, sgl_obj_t *obj);


/**
 * @brief remove an object from its parent
 * @param obj object to remove
 * @return none
 */
void sgl_obj_remove(sgl_obj_t *obj);


/**
 * @brief check if object has child
 * @param  obj object
 * @return true or false, true means object has child, false means object has no child
 */
static inline bool sgl_obj_has_child(sgl_obj_t *obj) {
    SGL_ASSERT(obj != NULL);
    return obj->child != NULL ? true : false;
}


/**
 * @brief get child of an object
 * @param obj the object
 * @return the child of the object
 */
static inline sgl_obj_t* sgl_obj_get_child(sgl_obj_t* obj)
{
    SGL_ASSERT(obj != NULL);
    return obj->child;
}


/**
 * @brief check if object has sibling
 * @param  obj object
 * @return true or false, true means object has sibling, false means object has no sibling
 */
static inline bool sgl_obj_has_sibling(sgl_obj_t *obj) {
    SGL_ASSERT(obj != NULL);
    return obj->sibling != NULL ? true : false;
}


/**
 * @brief get sibling of an object
 * @param obj the object
 * @return the sibling of the object
 */
static inline sgl_obj_t* sgl_obj_get_sibling(sgl_obj_t* obj)
{
    SGL_ASSERT(obj != NULL);
    return obj->sibling;
}



/**
 * @brief get child count of an object
 * @param obj object
 * @return child count
 */
static inline size_t sgl_obj_get_child_count(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    sgl_obj_t *child = NULL;
    size_t count = 0;

    sgl_obj_for_each_child(child, obj) {
        count ++;
    }

    return count;
}


/**
 * @brief merge area with current dirty area
 * @param merge [in] merge area
 * @return none
 */
void sgl_obj_dirty_merge(sgl_obj_t *obj);


/**
 * @brief Set object to dirty
 * @param obj point to object
 * @return none
 * @note this function will set object to dirty, include its children
 */
static inline void sgl_obj_set_dirty(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    obj->dirty = 1;
}


/**
 * @brief Clear object dirty flag
 * @param obj point to object
 * @return none
 */
static inline void sgl_obj_clear_dirty(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    obj->dirty = 0;
}


/**
 * @brief check object dirty flag
 * @param obj point to object
 * @return flag, false - clear, true - dirty
 */
static inline bool sgl_obj_is_dirty(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    return obj->dirty == 1 ? true : false;
}


/**
 * @brief set object hidden flag
 * @param obj point to object
 * @return none
 */
static inline void sgl_obj_set_hidden(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    obj->hide = 1;
    sgl_obj_dirty_merge(obj);
}


/**
 * @brief set object visible
 * @param obj point to object
 * @return none
 */
static inline void sgl_obj_set_visible(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    obj->hide = 0;
    sgl_obj_dirty_merge(obj);
}


/**
 * @brief check object hidden flag
 * @param obj point to object
 * @return flag, false - show, true - hidden
 */
static inline bool sgl_obj_is_hidden(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    return obj->hide == 1 ? true : false;
}


/**
 * @brief get fix radius of object
 * @param obj object
 * @return fix radius
 * @note if radius is larger than object's width or height, fix radius will be returned
 */
int16_t sgl_obj_fix_radius(sgl_obj_t *obj, size_t radius);


/**
 * @brief Set object position
 * @param obj point to object
 * @param x: x position
 * @param y: y position
 * @return none
 */
void sgl_obj_set_pos(sgl_obj_t *obj, int16_t x, int16_t y);


/**
 * @brief move object child position
 * @param obj point to object
 * @param ofs_x: x offset position
 * @param ofs_y: y offset position
 * @return none
 */
void sgl_obj_move_child_pos(sgl_obj_t *obj, int16_t ofs_x, int16_t ofs_y);


/**
 * @brief move object child x position
 * @param obj point to object
 * @param ofs_x: x offset position
 * @return none
 */
static inline void sgl_obj_move_child_pos_x(sgl_obj_t *obj, int16_t ofs_x)
{
    sgl_obj_move_child_pos(obj, ofs_x, 0);
}


/**
 * @brief move object child y position
 * @param obj point to object
 * @param ofs_y: y offset position
 * @return none
 */
static inline void sgl_obj_move_child_pos_y(sgl_obj_t *obj, int16_t ofs_y)
{
    sgl_obj_move_child_pos(obj, 0, ofs_y);
}


/**
 * @brief zoom object size
 * @param obj point to object
 * @param zoom zoom size
 * @return none
 * @note if you want to zoom out, the zoom should be positive, if you want to zoom in, the zoom should be negative
 */
void sgl_obj_size_zoom(sgl_obj_t *obj, int16_t zoom);


/**
 * @brief Get object position
 * @param obj point to object
 * @return sgl_pos_t: position of object
 * @note this function will return the top left corner position of the object
 */
static inline sgl_pos_t sgl_obj_get_pos(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);

    sgl_pos_t pos;
    pos.x = obj->coords.x1;
    pos.y = obj->coords.y1;
    return pos;
}


/**
 * @brief move object up a level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_up(sgl_obj_t *obj);


/**
 * @brief move object down a level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_down(sgl_obj_t *obj);


/**
 * @brief move object top level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_foreground(sgl_obj_t *obj);


/**
 * @brief move object bottom level layout
 * @param obj point to object
 * @return none
 * @note Only move among sibling objects
 */
void sgl_obj_move_background(sgl_obj_t *obj);


/**
 * @brief Set the alignment position of the object relative to its parent object.
 * @param obj The object to set the alignment position.
 * @param type The alignment type.
 * @return none
 * @note type should be one of the sgl_align_type_t values:
 *       - SGL_ALIGN_CENTER     : Center the object in the parent object.
 *       - SGL_ALIGN_TOP_MID    : Align the object at the top middle of the parent object.
 *       - SGL_ALIGN_TOP_LEFT   : Align the object at the top left of the parent object.
 *       - SGL_ALIGN_TOP_RIGHT  : Align the object at the top right of the parent object.
 *       - SGL_ALIGN_BOT_MID    : Align the object at the bottom middle of the parent object.
 *       - SGL_ALIGN_BOT_LEFT   : Align the object at the bottom left of the parent object.
 *       - SGL_ALIGN_BOT_RIGHT  : Align the object at the bottom right of the parent object.
 *       - SGL_ALIGN_LEFT_MID   : Align the object at the left middle of the parent object.
 *       - SGL_ALIGN_RIGHT_MID  : Align the object at the right middle of the parent object.
 */
void sgl_obj_set_pos_align(sgl_obj_t *obj, sgl_align_type_t type);


/**
 * @brief Set the alignment position of the object relative to its sibling object.
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
void sgl_obj_set_pos_align_ref(sgl_obj_t *ref, sgl_obj_t *obj, sgl_align_type_t type);


/**
 * @brief Set object size
 * @param obj point to object
 * @param width: width that you want to set
 * @param height: height that you want to set
 * @return none
 */
static inline void sgl_obj_set_size(sgl_obj_t *obj, int16_t width, int16_t height)
{
    SGL_ASSERT(obj != NULL);

    sgl_obj_dirty_merge(obj);

    obj->coords.x2 = obj->coords.x1 + width - 1;
    obj->coords.y2 = obj->coords.y1 + height - 1;
}


/**
 * @brief Get object size
 * @param obj point to object
 * @return sgl_size_t: size of object
 */
static inline sgl_size_t sgl_obj_get_size(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);

    sgl_size_t size;
    size.w = obj->coords.x2 - obj->coords.x1 + 1;
    size.h = obj->coords.y2 - obj->coords.y1 + 1;
    return size;
}


/**
 * @brief Set object width
 * @param obj point to object
 * @param width: width that you want to set
 * @return none
 */
static inline void sgl_obj_set_width(sgl_obj_t *obj, int16_t width)
{
    SGL_ASSERT(obj != NULL);
    obj->coords.x2 = obj->coords.x1 + width - 1;
}


/**
 * @brief Get object width
 * @param obj point to object
 * @return object width
 */
static inline int16_t sgl_obj_get_width(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    return obj->coords.x2 - obj->coords.x1 + 1;
}


/**
 * @brief Set object height
 * @param obj point to object
 * @param height: height that you want to set
 * @return none
 */
static inline void sgl_obj_set_height(sgl_obj_t *obj, int16_t height)
{
    SGL_ASSERT(obj != NULL);
    obj->coords.y2 = obj->coords.y1 + height - 1;
}


/**
 * @brief Get object height
 * @param obj point to object
 * @return object height
 */
static inline int16_t sgl_obj_get_height(sgl_obj_t *obj)
{
    SGL_ASSERT(obj != NULL);
    return obj->coords.y2 - obj->coords.y1 + 1;
}


/**
 * @brief set current object as screen object
 * @param obj object, that you want to set an object as active page
 * @return none
 */
void sgl_screen_load(sgl_obj_t *obj);


/**
 * @brief get current screen object
 * @param none
 * @return active current screen object
 */
static inline sgl_obj_t* sgl_screen_act(void)
{
    return &sgl_ctx.page->obj;
}


/**
 * @brief get active page
 * @param none
 * @return page: active page
 */
static inline sgl_page_t* sgl_page_get_active(void)
{
    return sgl_ctx.page;
}


/**
 * @brief Create an object
 * @param parent parent object
 * @return sgl_obj_t
 * @note if parent is NULL, the object will be as an new page
 */
sgl_obj_t* sgl_obj_create(sgl_obj_t *parent);


/**
 * @brief color mixer
 * @param fg_color : foreground color
 * @param bg_color : background color
 * @param factor   : color mixer factor
 * @return sgl_color_t: mixed color
 */
sgl_color_t sgl_color_mixer(sgl_color_t fg_color, sgl_color_t bg_color, uint8_t factor);


/**
 * @brief area init
 * @param area : area pointer
 * @return none
 */
static inline void sgl_area_init(sgl_area_t *area)
{
    SGL_ASSERT(area != NULL);
    area->x1 = area->y1 = SGL_POS_MAX;
    area->x2 = area->y2 = SGL_POS_MIN;
}


/**
 * @brief get pixel of pixmap
 * @pixmap: pointe to pixmap
 * @param x: x position
 * @param y: y position
 * @return sgl_color_t: pixel color
 */
static inline sgl_color_t sgl_pixmap_get_pixel(const sgl_pixmap_t *pixmap, int16_t x, int16_t y)
{
    uint32_t pos;

    SGL_ASSERT(pixmap != NULL);
    pos = y * pixmap->width + x;

#if (CONFIG_SGL_EXTERNAL_PIXMAP)
    if (pixmap->read) {
        pixmap->read(sgl_ctx.pixmap_buff, pos * sizeof(sgl_color_t), sizeof(sgl_color_t), pixmap->data);
        return *sgl_ctx.pixmap_buff;
    }
#endif

    return ((sgl_color_t*)pixmap->bitmap)[pos];
}


/**
 * @brief get pixel of pixmap buf
 * @pixmap: pointe to pixmap
 * @param x: x position
 * @param y: y position
 * @param size: pixel size
 * @return sgl_color_t: pixel color address
 */
static inline sgl_color_t* sgl_pixmap_get_buf(const sgl_pixmap_t *pixmap, int16_t x, int16_t y, size_t size)
{
    uint32_t pos;

    SGL_ASSERT(pixmap != NULL);
    pos = y * pixmap->width + x;

#if (CONFIG_SGL_EXTERNAL_PIXMAP)
    SGL_ASSERT(size <= sgl_panel_resolution_width());
    if (pixmap->read) {
        pixmap->read(sgl_ctx.pixmap_buff, pos * sizeof(sgl_color_t), size * sizeof(sgl_color_t), pixmap->data);
        return sgl_ctx.pixmap_buff;
    }
#else
    SGL_UNUSED(size);
#endif

    return &((sgl_color_t*)pixmap->bitmap)[pos];
}


/**
 * @brief check surf and other area is overlap
 * @param surf surfcare
 * @param area area b
 * @return true or false, true means overlap, false means not overlap
 * @note: this function is unsafe, you should check the surfcare and area is not NULL by yourself
 */
static inline bool sgl_surf_area_is_overlap(sgl_surf_t *surf, sgl_area_t *area)
{
    SGL_ASSERT(surf != NULL && area != NULL);
    int16_t h_pos = surf->y + surf->h - 1;
    int16_t w_pos = surf->x + surf->w - 1;

    if (area->y1 > h_pos || area->y2 < surf->y || area->x1 > w_pos || area->x2 < surf->x) {
        return false;
    }

    return true;
}


/**
 * @brief check two area is overlap
 * @param area_a area a
 * @param area_b area b
 * @return true or false, true means overlap, false means not overlap
 * @note: this function is unsafe, you should check the area_a and area_b is not NULL by yourself
 */
static inline bool sgl_area_is_overlap(sgl_area_t *area_a, sgl_area_t *area_b)
{
    SGL_ASSERT(area_a != NULL && area_b != NULL);
    if (area_b->y1 > area_a->y2 || area_b->y2 < area_a->y1 || area_b->x1 > area_a->x2 || area_b->x2 < area_a->x1) {
        return false;
    }

    return true;
}


/**
 * @brief  Get area intersection between surface and area
 * @param surf: surface
 * @param area: area
 * @param clip: intersection area
 * @return true: intersect, otherwise false
 * @note: this function is unsafe, you should check the surf and area is not NULL by yourself
 */
bool sgl_surf_clip(sgl_surf_t *surf, sgl_area_t *area, sgl_area_t *clip);


/**
 * @brief  Get area intersection between two areas
 * @param area_a: area a
 * @param area_b: area b
 * @param clip: intersection area
 * @return true: intersect, otherwise false
 * @note: this function is unsafe, you should check the area_a and area_b and clip is not NULL by yourself
 */
bool sgl_area_clip(sgl_area_t *area_a, sgl_area_t *area_b, sgl_area_t *clip);


/**
 * @brief clip area with another area
 * @param clip [in][out] clip area
 * @param area [in] area
 * @return true if clip area is valid, otherwise two area is not overlapped
 * @note: this function is unsafe, you should check the clip and area is not NULL by yourself
 */
bool sgl_area_selfclip(sgl_area_t *clip, sgl_area_t *area);


/**
 * @brief merge two area, the merge is result of the two area clip
 * @param area_a [in] area1
 * @param area_b [in] area2
 * @param merge  [out] merge result
 * @return none
 * @note: this function is unsafe, you should check the area_a and area_b and merge is not NULL by yourself
 */
void sgl_area_merge(sgl_area_t *area_a, sgl_area_t *area_b, sgl_area_t *merge);


/**
 * @brief merge two area, the merge is a new area
 * @param merge [in][out] merge area
 * @param area [in] area
 * @return none
 * @note: this function is unsafe, you should check the merge and area is not NULL by yourself
 */
void sgl_area_selfmerge(sgl_area_t *merge, sgl_area_t *area);


/**
 * @brief swap the framebuffer buffer
 * @param surf [in] surface
 * @return none
 */
static inline void sgl_surf_buffer_swap(sgl_surf_t *surf)
{
    if (sgl_ctx.fb_dev.buffer[1] != NULL) {
        surf->buffer = sgl_ctx.fb_dev.buffer[sgl_ctx.fb_swap ^= 1];
    }
}


/**
 * @brief sgl global initialization
 * @param none
 * @return none
 */
void sgl_init(void);


/**
 * @brief initialize object
 * @param obj object
 * @param parent parent object
 * @return int, 0 means successful, -1 means failed
 */
int sgl_obj_init(sgl_obj_t *obj, sgl_obj_t *parent);


/**
 * @brief Convert UTF-8 string to Unicode
 * @param utf8_str Pointer to the UTF-8 string to be converted
 * @param p_unicode_buffer Pointer to the buffer where the converted Unicode will be stored
 * @return The number of bytes in the UTF-8 string
 */
uint32_t sgl_utf8_to_unicode(const char *utf8_str, uint32_t *p_unicode_buffer);


/**
 * @brief Search for the index of a Unicode character in the font table
 * @param font Pointer to the font structure containing character data
 * @param unicode Unicode of the character to be searched
 * @return Index of the character in the font table
 */
uint32_t sgl_search_unicode_ch_index(const sgl_font_t *font, uint32_t unicode);


/**
 * @brief get height in font
 * @param font pointer to sgl_font_t
 * @return height of font, -1 means error
 * @note this function is used to get the height of a character in a font
 */
static inline int16_t sgl_font_get_height(const sgl_font_t *font)
{
    SGL_ASSERT(font != NULL);
    return font->font_height;
}


/**
 * @brief get the width of a string
 * @param str string
 * @param font sgl font
 * @return width of string
 */
int32_t sgl_font_get_string_width(const char *str, const sgl_font_t *font);


/**
 * @brief get the height of a string, which is in a rect area
 * @param width width of the rect area
 * @param str string
 * @param font sgl font of the string
 * @param line_space peer line space
 * @return height size of string
 */
int32_t sgl_font_get_string_height(int16_t width, const char *str, const sgl_font_t *font, uint8_t line_space);


/**
 * @brief get the alignment position
 * @param parent_size parent size
 * @param size object size
 * @param type alignment type
 * @return alignment position
 */
sgl_pos_t sgl_get_align_pos(sgl_size_t *parent_size, sgl_size_t *size, sgl_align_type_t type);


/**
 * @brief get the text position in the area
 * @param area point to area
 * @param font point to font
 * @param text text string
 * @param offset text offset
 * @param type alignment type
 * @return sgl_pos_t position of text
 */
sgl_pos_t sgl_get_text_pos(sgl_area_t *area, const sgl_font_t *font, const char *text, int16_t offset, sgl_align_type_t type);


/**
 * @brief get the icon position of area
 * @param area point to area
 * @param icon point to icon
 * @param offset offset
 * @param type align type
 */
sgl_pos_t sgl_get_icon_pos(sgl_area_t *area, const sgl_icon_pixmap_t *icon, int16_t offset, sgl_align_type_t type);


/**
 * @brief sgl task handle function
 * @param none
 * @return none
 * @note this function should be called in main loop or timer or thread
 */
void sgl_task_handle(void);


/**
 * @brief set page background color
 * @param obj point to object
 * @param color background color
 * @return none
 */
void sgl_page_set_color(sgl_obj_t* obj, sgl_color_t color);


/**
 * @brief set page background pixmap
 * @param obj point to object
 * @param pixmap background pixmap
 * @return none
 */
void sgl_page_set_pixmap(sgl_obj_t* obj, const sgl_pixmap_t *pixmap);


/**
 * @brief get patent of an object
 * @param obj the object
 * @return the patent of the object
 */
static inline sgl_obj_t* sgl_obj_get_patent(sgl_obj_t* obj)
{
    SGL_ASSERT(obj != NULL);
    return obj->parent;
}


#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif //__SGL_CORE_H__
