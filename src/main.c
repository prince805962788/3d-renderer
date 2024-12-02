#include "display.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"

triangle_t triangles_to_render[N_CUBE_FACES];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};
float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;

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
// Project 3D point to 2D using perspective projection
vec2_t project(vec3_t point) {
    vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z
    };
    return projected_point;
}
void update(void) {
    int time_to_wait =
        FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    previous_frame_time = SDL_GetTicks();
    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    // Loop all triangle faces of our cube
    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t cube_face = cube_faces[i];

        // Get the vertices of the triangle
        vec3_t face_vertices[3];
        face_vertices[0] = cube_vertices[cube_face.a - 1];
        face_vertices[1] = cube_vertices[cube_face.b - 1];
        face_vertices[2] = cube_vertices[cube_face.c - 1];

        triangle_t projected_triangle;

        // Loop all 3 vertices of this current face and apply transformations
        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];
            // Apply rotation
            transformed_vertex =
                vec3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex =
                vec3_rotate_y(transformed_vertex, cube_rotation.y);
            transformed_vertex =
                vec3_rotate_z(transformed_vertex, cube_rotation.z);

            // transform the vertex by the camera position
            transformed_vertex.z -= camera_position.z;

            // Project the current transformed vertex
            vec2_t projected_point = project(transformed_vertex);

            // Scale and translate projected point to the center of the screen
            projected_point.x += window_width / 2;
            projected_point.y += window_height / 2;

            // Store the projected point in the current triangle
            projected_triangle.points[j] = projected_point;
        }
        // Store the projected triangle in the array of triangles to render
        triangles_to_render[i] = projected_triangle;
    }
}
void render(void) {
    // we can reuse color buffer,so we can abolish the following code
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set color to black
    // SDL_RenderClear(renderer);                      // clear the screen

    draw_grid(0x666666ff); // render grid

    // Loop through all triangles to render
    for (int i = 0; i < N_CUBE_FACES; i++) {
        triangle_t current_triangle = triangles_to_render[i];
        draw_rect(
            current_triangle.points[0].x,
            current_triangle.points[0].y,
            3,
            3,
            0xffff00ff
        );
        draw_rect(
            current_triangle.points[1].x,
            current_triangle.points[1].y,
            3,
            3,
            0xffff00ff
        );
        draw_rect(
            current_triangle.points[2].x,
            current_triangle.points[2].y,
            3,
            3,
            0xffff00ff
        );
    }

    render_color_buffer(); // update color buffer to cache for rendering screen
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
