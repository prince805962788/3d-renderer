#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "vector.h"

// left hand coordinate system, so positive z is in the screen
#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

// 6 faces, 2 triangles per face
#define N_CUBE_FACES (6 * 2)
extern face_t cube_faces[N_CUBE_FACES];

#endif
