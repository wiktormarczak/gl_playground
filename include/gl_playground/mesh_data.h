#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <gl_playground/vector.h>
#include <gl_playground/uv.h>

typedef struct
{
    unsigned int vertex_count;
    unsigned int *position, *normal, *uv;
} Face;

typedef struct
{
    unsigned int position_count, normal_count, uv_count, face_count;
    Vector *position, *normal;
    UV *uv;
    Face *face;
} MeshData;

MeshData mesh_data_load(const char *path);

#endif
