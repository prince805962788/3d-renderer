#include "display.h"

bool is_running = false;

void setup(void) {
    // allocate memory for color buffer
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
    // create color buffer texture
    color_buffer_texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width,
                                             window_height);
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
void update(void) {
}
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set color to black
    SDL_RenderClear(renderer);                      // clear the screen

    draw_grid();                    // render grid
    render_color_buffer();          // render color buffer result
    clear_color_buffer(0x000000ff); // clear color buffer to prepare next frame

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
