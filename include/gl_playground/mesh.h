#ifndef MESH_H
#define MESH_H

typedef struct Mesh Mesh;

Mesh *mesh_create_cube(double size);
Mesh *mesh_load(const char *path);

void mesh_destroy(Mesh *mesh);

void mesh_gpu(Mesh *mesh);

void mesh_draw(Mesh *mesh);

#endif
