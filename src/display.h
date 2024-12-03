#ifndef DISPLAY_H
#define DISPLAY_H

#include </opt/homebrew/include/SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define FPS               60
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_grid(uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void draw_line(
    int x0,
    int y0, // start point
    int x1,
    int y1, // end point
    uint32_t color
);
void draw_triangle(
    int x0,
    int y0, // point 1
    int x1,
    int y1, // point 2
    int x2,
    int y2, // point 3
    uint32_t color
);
void draw_rect(
    int x,
    int y, // start point
    int width,
    int height,
    uint32_t color
);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif