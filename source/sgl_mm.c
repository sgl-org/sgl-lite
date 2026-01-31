/* source: sgl_mm.c
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

#include "sgl_mm.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static sgl_mm_monitor_t mem = {
    .total_size = 0,
    .free_size = 0,
    .used_size = 0,
};

/**
 * @brief  initialize memory pool
 * @param  mem_start  start address of memory pool
 * @param  len  length of memory pool
 */
void sgl_mm_init(void *mem_start, size_t len)
{
    SGL_UNUSED(mem_start);
    mem.total_size += len;
}


/**
 * @brief  add memory pool
 * @param  mem_start  start address of memory pool
 * @param  len  length of memory pool
 */
void sgl_mm_add_pool(void *mem_start, size_t len)
{
    SGL_UNUSED(mem_start);
    mem.total_size += len;
}


/**
 * @brief  memory alloc, the function is unsafe, you should ensure that 
 *         the requested size is smaller than the free size of memory
 * 
 * @param  size   request size of memory
 * 
 * @return point to request memory address
*/
void* sgl_malloc(size_t size)
{
    void *p = malloc(size);
    memset(p, 0, size);
    return p;
}


/**
 * @brief  memory realloc, the function is unsafe, you should ensure that 
 *         the requested size is smaller than the free size of memory
 * @param  p      the pointer of request size of memory
 * @param  size   request size of memory
 */
void* sgl_realloc(void *p, size_t size)
{
    return realloc(p, size);
}


/**
 * @brief  memory free
 * 
 * @param  p  the pointer of request size of memory
 * 
 * @return none
*/
void sgl_free(void *p)
{
    //mem.used_size -= ;
    free(p);
}


sgl_mm_monitor_t sgl_mm_get_monitor(void)
{
    int integer = (mem.used_size * 100) / mem.total_size;
    int decimal = (mem.used_size * 10000) / mem.total_size - (integer * 100);
    mem.used_rate = integer << 8 | decimal;
    mem.free_size = mem.total_size - mem.used_size;

    return mem;
}
