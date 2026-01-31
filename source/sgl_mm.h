/* source: sgl_mm.h
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

#ifndef __SGL_MM_H__
#define __SGL_MM_H__

#include "sgl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  memory monitor info
 * @total_size: total size of memory
 * @free_size: free size of memory
 * @used_size: used size of memory
 * @used_rate: used rate of memory:
 *             |  8 bit  |  8 bit |          
 *             |   int   |   dec  |
 */
typedef struct sgl_mm_monitor {
    size_t  total_size;
    size_t  free_size;
    size_t  used_size;
    size_t  used_rate;

} sgl_mm_monitor_t;


/**
 * @brief  initialize memory pool
 * @param  mem_start  start address of memory pool
 * @param  len  length of memory pool
 */
void sgl_mm_init(void *mem_start, size_t len);


/**
 * @brief  add memory pool
 * @param  mem_start  start address of memory pool
 * @param  len  length of memory pool
 */
void sgl_mm_add_pool(void *mem_start, size_t len);


/**
 * @brief  memory alloc, the function is unsafe, you should ensure that 
 *         the requested size is smaller than the free size of memory
 * 
 * @param  size   request size of memory
 * 
 * @return point to request memory address
*/
void* sgl_malloc(size_t size);


/**
 * @brief  memory realloc, the function is unsafe, you should ensure that 
 *         the requested size is smaller than the free size of memory
 * @param  p      the pointer of request size of memory
 * @param  size   request size of memory
 */
void* sgl_realloc(void *p, size_t size);


/**
 * @brief  memory free
 * 
 * @param  p  the pointer of request size of memory
 * 
 * @return none
*/
void sgl_free(void *p);


/**
 * @brief  get memory monitor info
 * 
 * @param  none
 * @return memory monitor info
 */
sgl_mm_monitor_t sgl_mm_get_monitor(void);


#ifdef __cplusplus
}
#endif

#endif // !__SGL_MM_H__
