#include "display.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"

triangle_t *triangles_to_render = NULL;

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;

void setup(void) {
    // Allocate memory for color buffer
    color_buffer =
        (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
    // Create color buffer texture
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
    // 当程序在 build 目录下运行时，使用 ./assets/f22.obj 是正确的，因为 assets
    // 文件夹应该被复制到或链接到构建目录中。这是一种常见的做法，确保程序可以在构建目录中正确运行
    load_obj_file_data("./assets/f22.obj");
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
    // Initialize the array of triangles to render
    triangles_to_render = NULL;

    mesh.rotation.x += 0.01;
    mesh.rotation.y += 0.00;
    mesh.rotation.z += 0.00;

    // Loop all triangle faces of our cube
    int num_faces = array_length(mesh.faces);
    for (int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];

        // Get the vertices of the triangle
        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformed_vertices[3];

        // Check backface culling
        vec3_t vector_a = transformed_vertices[0]; /*   A   */
        vec3_t vector_b = transformed_vertices[1]; /*  / \  */
        vec3_t vector_c = transformed_vertices[2]; /* C---B */

        // Get the vector subtraction of B-A and C-A
        vec3_t vector_ab = vec3_sub(vector_b, vector_a);
        vec3_t vector_ac = vec3_sub(vector_c, vector_a);
        vec3_normalize(&vector_ab);
        vec3_normalize(&vector_ac);

        // Compute the face normal (using cross product to find perpendicular)
        vec3_t normal = vec3_cross(vector_ab, vector_ac);
        vec3_normalize(&normal);

        // Find the vector between vertex A in the triangle and the camera
        // origin
        vec3_t camera_ray = vec3_sub(camera_position, vector_a);

        // Calculate how aligned the camera ray is with the face normal (using
        // dot product)
        float dot_normal_camera = vec3_dot(normal, camera_ray);

        // angle > Math.PI / 2, is backface culling
        if (dot_normal_camera < 0) {
            continue;
        }

        triangle_t projected_triangle;

        // Loop all 3 vertices of this current face and apply transformations
        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];
            // Apply rotation
            transformed_vertex =
                vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex =
                vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex =
                vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Transform the vertex by the camera position
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
        // triangles_to_render[i] = projected_triangle;
        array_push(triangles_to_render, projected_triangle);
    }
}
void render(void) {
    // We can reuse color buffer,so we can abolish the following code
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set color to black
    // SDL_RenderClear(renderer);                      // clear the screen

    draw_grid(0x666666ff); // render grid

    // Loop through all triangles to render
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++) {
        triangle_t current_triangle = triangles_to_render[i];
        draw_triangle(
            current_triangle.points[0].x,
            current_triangle.points[0].y,
            current_triangle.points[1].x,
            current_triangle.points[1].y,
            current_triangle.points[2].x,
            current_triangle.points[2].y,
            0x0000ffff
        );
    }
    // Clear the array of triangles to render
    array_free(triangles_to_render);
    // Update color buffer to cache for rendering screen
    render_color_buffer();
    // Clear color buffer to prepare next frame
    clear_color_buffer(0x000000ff);
    // Render the screen
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
    destroy_window();
    return 0;
}
