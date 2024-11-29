#include "display.h"
#include "vector.h"

const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

float fov_factor = 128;
bool is_running = false;

void draw_cube(void) {
    int point_count = 0;
    // 使用浮点数进行循环，并确保不会超出 N_POINTS
    for (float x = -1.0f; x <= 1.0f; x += 0.25f) {
        for (float y = -1.0f; y <= 1.0f; y += 0.25f) {
            for (float z = -1.0f; z <= 1.0f; z += 0.25f) {
                if (point_count >= N_POINTS)
                    break; // 添加安全检查
                vec3_t new_point = {.x = x, .y = y, .z = z};
                cube_points[point_count++] = new_point;
            }
        }
    }
}
void setup(void) {
    // allocate memory for color buffer
    color_buffer =
        (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
    // create color buffer texture
    if (color_buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for color buffer\n");
        is_running = false;
        return;
    }
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );
    draw_cube();
}
void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT: // close window with window close button
        is_running = false;
        break;
    case SDL_KEYDOWN: // close window with escape
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            is_running = false;
        }
        break;
    }
}
/// @brief Project 3D point to 2D using perspective projection
/// @param point
/// @return 2D point
vec2_t project(vec3_t point) {
    vec2_t projected_point = {
        .x = (fov_factor * point.x), .y = (fov_factor * point.y)
    };
    return projected_point;
}
void update(void) {
    for (int i = 0; i < N_POINTS; i++) {
        vec3_t point = cube_points[i];
        vec2_t projected_point = project(point);
        projected_points[i] = projected_point;
    }
}
void render(void) {
    // we can reuse color buffer,so we can abolish the following code
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set color to black
    // SDL_RenderClear(renderer);                      // clear the screen

    draw_grid(0x333333ff); // render grid

    // loop all projected points and draw them
    for (int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];
        draw_rect(
            projected_point.x + (window_width / 2),
            projected_point.y + (window_height / 2),
            4,
            4,
            0xffff00ff
        );
    }

    render_color_buffer(); // update color buffer to cache for rendering screen
    clear_color_buffer(0xffffffff); // clear color buffer to prepare next frame

    SDL_RenderPresent(renderer); // render the screen
}
int main() {
    is_running = initialize_window();
    setup();
    while (is_running) {
        process_input();
        update();
        render();
    }
    destroy_window();
    return 0;
}
