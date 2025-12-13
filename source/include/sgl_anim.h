/* source/include/sgl_anim.h
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

#ifndef __SGL_ANIM_H__
#define __SGL_ANIM_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <sgl_cfgfix.h>
#include <stddef.h>
#include <sgl_list.h>
#include <sgl_types.h>
#include <sgl_mm.h>

#if (CONFIG_SGL_ANIMATION)

/* Forward declaration of sgl_pos sgl_anim structures */
struct sgl_pos;
struct sgl_anim;


/* Anim path callback */
typedef void (*sgl_anim_path_cb_t)(struct sgl_anim *anim, int32_t value);
typedef int32_t (*sgl_anim_path_algo_t)(uint32_t elaps, uint32_t duration, int32_t start, int32_t end);


typedef struct sgl_anim {
    void                  *data;
    struct sgl_anim       *next;
    uint32_t              act_time;
    uint32_t              act_delay;
    uint32_t              act_duration;
    int32_t               start_value;
    int32_t               end_value;
    sgl_anim_path_cb_t    path_cb;
    sgl_anim_path_algo_t  path_algo;
    void                  (*finish_cb)(struct sgl_anim *anim);
    uint32_t              repeat_cnt : 30;
    uint32_t              finished : 1;
    uint32_t              auto_free : 1;
} sgl_anim_t;


/**
 * @brief animation context, it will be used to store status of animation
 * @anim_list_head: animation list head
 * @anim_list_tail: animation list tail
 * @anim_cnt:       animation count
 * @tick_ms:        animation tick, ms
 */
typedef struct sgl_anim_ctx {
    sgl_anim_t *anim_list_head;
    sgl_anim_t *anim_list_tail;
    uint32_t    anim_cnt;
} sgl_anim_ctx_t;


#define  sgl_anim_for_each(anim, head)                 for ((anim) = (head)->anim_list_head; (anim) != NULL; (anim) = (anim)->next)
#define  sgl_anim_for_each_safe(anim, n, head)         for (anim = (head)->anim_list_head, n = (anim) ? (anim)->next : NULL; anim != NULL; anim = n, n = (anim) ? (anim)->next : NULL)

#define  SGL_ANIM_REPEAT_LOOP                          (0x3FFFFFFF)
#define  SGL_ANIM_REPEAT_ONCE                          (1)


/* Animation context it will be used internally */
extern sgl_anim_ctx_t anim_ctx;


/**
 * @brief  Animation static initialization
 * @param  anim - Animation object
 * @return none
 */
void sgl_anim_init(sgl_anim_t *anim);


/**
 * @brief dynamic alloc animation object with initialization
 * @param  none
 * @return animation object
*/
sgl_anim_t* sgl_anim_create(void);


/**
 * @brief add animation object to animation list
 * @param  anim animation object
 * @return none
*/
void sgl_anim_add(sgl_anim_t *anim);


/**
 * @brief remove animation object from animation list
 * @param  anim animation object
 * @return none
*/
void sgl_anim_remove(sgl_anim_t *anim);


/**
 * @brief start animation
 * @param  anim animation object
 * @return none
*/
static inline void sgl_anim_start(sgl_anim_t *anim)
{
    sgl_anim_add(anim);
}


/**
 * @brief stop animation
 * @param  anim animation object
 * @return none
*/
static inline void sgl_anim_stop(sgl_anim_t *anim)
{
    sgl_anim_remove(anim);
}


/**
 * @brief free animation object
 * @param  anim animation object
 * @return none
*/
static inline void sgl_anim_free(sgl_anim_t *anim)
{
    SGL_ASSERT(anim != NULL);
    sgl_free(anim);
}


/**
 * @brief set animation private data
 * @param  anim animation object
 * @param  data pointer to private data
 * @return none
 */
static inline void sgl_anim_set_data(sgl_anim_t *anim, void *data)
{
    SGL_ASSERT(anim != NULL);
    anim->data = data;
}


/**
 * @brief set animation path callback function
 * @param  anim animation object
 * @param  path_cb path callback function
 * @param  path_algo path algo callback function
 * @return none
 */
static inline void sgl_anim_set_path(sgl_anim_t *anim, sgl_anim_path_cb_t path_cb, sgl_anim_path_algo_t path_algo)
{
    SGL_ASSERT(anim != NULL && path_cb != NULL && path_algo != NULL);
    anim->path_cb = path_cb;
    anim->path_algo = path_algo;
}


/**
 * @brief set animation start value
 * @param  anim animation object
 * @param  value start value
 * @return none
 */
static inline void sgl_anim_set_start_value(sgl_anim_t *anim, uint16_t value)
{
    SGL_ASSERT(anim != NULL);
    anim->start_value = value;
}


/**
 * @brief set animation end value
 * @param  anim animation object
 * @param  value end value
 * @return none
 */
static inline void sgl_anim_set_end_value(sgl_anim_t *anim, uint16_t value)
{
    SGL_ASSERT(anim != NULL);
    anim->end_value = value;
}


/**
 * @brief set animation active delay time, ms
 * @param  anim animation object
 * @param  delay active delay time, ms
 * @return none
 */
static inline void sgl_anim_set_act_delay(sgl_anim_t *anim, uint32_t delay_ms)
{
    SGL_ASSERT(anim != NULL);
    anim->act_delay = delay_ms;
}


/**
 * @brief set animation active duration time, ms
 * @param  anim animation object
 * @param  duration active duration time, ms
 * @return none
 */
static inline void sgl_anim_set_act_duration(sgl_anim_t *anim, uint32_t duration_ms)
{
    SGL_ASSERT(anim != NULL);
    anim->act_duration = duration_ms;
}


/**
 * @brief set animation repeat count
 * @param  anim animation object
 * @param  repeat_cnt repeat count
 * @return none
 */
static inline void sgl_anim_set_repeat_cnt(sgl_anim_t *anim, int32_t repeat_cnt)
{
    SGL_ASSERT(anim != NULL);
    anim->repeat_cnt = ((uint32_t)repeat_cnt) & SGL_ANIM_REPEAT_LOOP;
}


/**
 * @brief set finish callback for animation
 * @param  anim animation object
 * @param  finish_cb finish callback
 * @return none
 */
static inline void sgl_anim_set_finish_cb(sgl_anim_t *anim, void (*finish_cb)(sgl_anim_t *anim))
{
    SGL_ASSERT(anim != NULL);
    anim->finish_cb = finish_cb;
}


/**
 * @brief check animation is finished or not
 * @param  anim animation object
 * @return true or false
 */
static inline bool sgl_anim_is_finished(sgl_anim_t *anim)
{
    SGL_ASSERT(anim != NULL);
    return anim->finished == 1;
}


/**
 * @brief set auto free flag for animation
 * @param  anim animation
 * @return none
 */
static inline void sgl_anim_set_auto_free(sgl_anim_t *anim)
{
    SGL_ASSERT(anim != NULL);
    anim->auto_free = 1;
}


/**
 * @brief animation task, it will foreach all animation
 * @param  none
 * @return none
 * @note   this function should be called in sgl_task()
 */
void sgl_anim_task(void);


/**
 * Linear animation path calculation function
 * 
 * Calculates the current interpolated value based on elapsed time and total duration
 * using linear interpolation.
 * 
 * @param elaps     Elapsed time in milliseconds
 * @param duration  Total animation duration in milliseconds
 * @param start     Start value
 * @param end       End value
 * 
 * @return          The interpolated value for the current time
 * 
 * @note            Returns 'end' if elaps >= duration (animation finished)
 *                  Returns 'start' if elaps == 0 (animation just started)
 *                  Uses 32-bit integer arithmetic to avoid floating-point operations
 *                  for better performance on embedded systems
 */
int32_t sgl_anim_path_linear(uint32_t elaps, uint32_t duration, int32_t start, int32_t end);
#define SGL_ANIM_PATH_LINEAR  sgl_anim_path_linear


/**
 * sgl_anim_path_ease_in_out - Cubic ease-in-out animation path
 *
 * This function creates a smooth animation curve that starts slow,
 * accelerates in the middle, and decelerates at the end.
 *
 * @param elaps     Elapsed time (ms)
 * @param duration  Total animation duration (ms)
 * @param start     Start value
 * @param end       End value
 * @return          Interpolated value at current time
 */
int32_t sgl_anim_path_ease_in_out(uint32_t elaps, uint32_t duration, int32_t start, int32_t end);
#define SGL_ANIM_PATH_EASE_IN_OUT  sgl_anim_path_ease_in_out


/**
 * sgl_anim_path_ease_in - Cubic ease-in animation path
 *
 * This function creates a smooth animation curve that starts slow,
 * accelerates in the after
 *
 * @param elaps     Elapsed time (ms)
 * @param duration  Total animation duration (ms)
 * @param start     Start value
 * @param end       End value
 * @return          Interpolated value at current time
 */
int32_t sgl_anim_path_ease_out(uint32_t elaps, uint32_t duration, int32_t start, int32_t end);
#define SGL_ANIM_PATH_EASE_OUT  sgl_anim_path_ease_out


/**
 * sgl_anim_path_ease_in - Cubic ease-in animation path
 *
 * This function creates a smooth animation curve that starts accelerates,
 * accelerates in the after
 *
 * @param elaps     Elapsed time (ms)
 * @param duration  Total animation duration (ms)
 * @param start     Start value
 * @param end       End value
 * @return          Interpolated value at current time
 */
int32_t sgl_anim_path_ease_in(uint32_t elaps, uint32_t duration, int32_t start, int32_t end);
#define SGL_ANIM_PATH_EASE_IN  sgl_anim_path_ease_in

#endif // ! CONFIG_SGL_ANIMATION

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // ! __SGL_ANIM_H__
