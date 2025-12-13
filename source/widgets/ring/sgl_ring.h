/* source/widgets/sgl_ring.h
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

#ifndef __SGL_RING_H__
#define __SGL_RING_H__


#include <sgl_core.h>
#include <sgl_draw.h>
#include <sgl_math.h>
#include <sgl_log.h>
#include <sgl_mm.h>
#include <sgl_cfgfix.h>
#include <string.h>


/**
 * @brief sgl ring
 * @obj: sgl general object
 * @desc: ring draw description
 */
typedef struct sgl_ring {
    sgl_obj_t        obj;
    int16_t          cx;
    int16_t          cy;
    int16_t          radius_in;
    int16_t          radius_out;
    sgl_color_t      color;
    uint8_t          alpha;
}sgl_ring_t;


/**
 * @brief Create a ring object
 * @param parent The parent object of the ring
 * @return The ring object
 */
sgl_obj_t* sgl_ring_create(sgl_obj_t* parent);


/**
 * @brief Set the ring color
 * @param obj The ring object
 * @param color The ring color
 * @return none
 */
static inline void sgl_ring_set_color(sgl_obj_t *obj, sgl_color_t color)
{
    sgl_ring_t *ring = (sgl_ring_t*)obj;
    ring->color = color;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief Set the ring alpha
 * @param obj The ring object
 * @param alpha The ring alpha
 * @return none
 * @note The alpha value range is 0~255
 */
static inline void sgl_ring_set_alpha(sgl_obj_t *obj, uint8_t alpha)
{
    sgl_ring_t *ring = (sgl_ring_t*)obj;
    ring->alpha = alpha;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief Set the ring radius
 * @param obj The ring object
 * @param radius_in The ring inner radius
 * @param radius_out The ring outer radius
 * @return none
 */
static inline void sgl_ring_set_radius(sgl_obj_t *obj, uint16_t radius_in, uint16_t radius_out)
{
    sgl_ring_t *ring = (sgl_ring_t*)obj;
    ring->radius_in = radius_in;
    ring->radius_out = radius_out;
    sgl_obj_set_dirty(obj);
}

/**
 * @brief Set the ring center
 * @param obj The ring object
 * @param cx The ring center x
 * @param cy The ring center y
 * @return none
 */
static inline void sgl_ring_set_center(sgl_obj_t *obj, int16_t cx, int16_t cy)
{
    sgl_ring_t *ring = (sgl_ring_t*)obj;
    ring->cx = cx;
    ring->cy = cy;
    sgl_obj_set_dirty(obj);
}


#endif // !__SGL_RING_H__
