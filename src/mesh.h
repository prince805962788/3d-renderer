#ifndef MESH_H
#define MESH_H

#include "array.h"
#include "triangle.h"
#include "vector.h"

// left hand coordinate system, so positive z is in the screen
#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

// 6 faces, 2 triangles per face
#define N_CUBE_FACES (6 * 2)
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
    vec3_t *vertices; // dynamic array of vertices
    face_t *faces;    // dynamic array of faces
    vec3_t rotation;  // rotation with x, y, and z values
} mesh_t;

extern mesh_t mesh;

void load_obj_file_data(char *file_path);

#endif
