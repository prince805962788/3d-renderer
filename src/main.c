#include </opt/homebrew/include/SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }
  // Create SDL Window
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       800, 600, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }
  // Create SDL Renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }
  return true;
}
void setup(void) {}
void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
    case SDL_QUIT:  // Close window with window close button
      is_running = false;
      break;
    case SDL_KEYDOWN:  // Close window with escape
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_running = false;
      }
      break;
  }
}
void update(void) {}
void render(void) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}
int main() {
  is_running = initialize_window();
  setup();
  while (is_running) {
    process_input();
    update();
    render();
  }
  return 0;
}