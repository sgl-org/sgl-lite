#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <sgl.h>
#include <math.h>

typedef struct sgl_port_sdl2 sgl_port_sdl2_t;

sgl_port_sdl2_t* sgl_port_sdl2_init(void);
size_t sgl_port_sdl2_get_frame_count(sgl_port_sdl2_t* sdl2_dev);
void sgl_port_sdl2_increase_frame_count(sgl_port_sdl2_t* sdl2_dev);
void sgl_port_sdl2_deinit(sgl_port_sdl2_t* sdl2_dev);
void flush_window_callback(void *param);


/* 颜色定义 */
#define SGL_COLOR_UI_BACKGROUND                    sgl_rgb(17, 17, 17)
#define SGL_COLOR_PIDV_BACKGROUND                  sgl_rgb(34, 34, 34)
#define SGL_COLOR_PIDV_COLOR                       sgl_rgb(70, 70, 70)

#define SGL_COLOR_TEMP_BACKGROUND                  sgl_rgb(26, 26, 46)
#define SGL_COLOR_TEMP_COLOR                       sgl_rgb(255, 204, 6)
#define SGL_COLOR_SET_TEMP_COLOR                   sgl_rgb(136, 204, 255)
#define SGL_COLOR_MODE_BACKGROUND                  sgl_rgb(30, 58, 95)
#define SGL_COLOR_MODE_COLOR                       sgl_rgb(81, 204, 255)
#define SGL_COLOR_WORK_BACKGROUND                  sgl_rgb(58, 26, 26)
#define SGL_COLOR_WORK_COLOR                       sgl_rgb(200, 50, 61)
#define SGL_COLOR_FAN_BACKGROUND                   sgl_rgb(26, 58, 26)
#define SGL_COLOR_FAN_COLOR                        sgl_rgb(80, 255, 153)


int main(int argc, char *argv[])
{
    int quit = 0;
    SDL_Event MouseEvent;
    sgl_port_sdl2_t* sdl2_dev = NULL;

    /* 初始化SDL2端口 */
    sdl2_dev = sgl_port_sdl2_init();
    if(sdl2_dev == NULL) {
        return -1;
    }

    extern const sgl_font_t consolas24;

    sgl_page_set_color(sgl_screen_act(), SGL_COLOR_BLACK);

    sgl_obj_t *rect = sgl_rect_create(NULL);
    sgl_obj_set_size(rect, 132, 180);
    sgl_obj_set_pos(rect, 53, 120);
    sgl_rect_set_color(rect, SGL_COLOR_BLACK);
    sgl_rect_set_border_width(rect, 3);
    sgl_rect_set_border_color(rect, SGL_COLOR_WHITE);
    sgl_rect_set_radius(rect, 20);

    // sgl_obj_t *label = sgl_label_create(NULL);
    // sgl_obj_set_pos(label, 40, 70);
    // sgl_obj_set_size(label, 100, 50);
    // sgl_label_set_font(label, &consolas24);
    // sgl_label_set_text(label, "Hello World!");

    /* 主循环 */
    while (!quit) {
        SDL_PollEvent(&MouseEvent);
        switch (MouseEvent.type) {
        case SDL_QUIT:
            quit = 1;
            break;
        }

        sgl_task_handle();
        /* sgl_anim_t回调驱动的动画更新对象位置；只需标记页面为脏 */
        //sgl_obj_set_dirty(sgl_screen_act());
        sgl_port_sdl2_increase_frame_count(sdl2_dev);
    }

    sgl_port_sdl2_deinit(sdl2_dev);

    return 0;
}
