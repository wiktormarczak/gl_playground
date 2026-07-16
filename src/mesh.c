#include <gl_playground/mesh.h>
#include <gl_playground/vector.h>
#include <gl_playground/uv.h>
#include <gl_playground/mesh_data.h>
#include <gl_playground/triangulate.h>
#include <glad/gl.h>
#include <SDL3/SDL_log.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Mesh
{
    unsigned int vertex_count, index_count;

    Vector *position, *normal;
    UV *uv;
    unsigned int *index;

    unsigned int position_vertex_buffer, uv_vertex_buffer, normal_vertex_buffer, index_buffer;
    unsigned int vertex_array;
};

Mesh *mesh_create_cube(double size)
{
    const double position[] = {
        // Front
        +size, +size, +size,
        -size, +size, +size,
        -size, -size, +size,
        +size, -size, +size,

        // Right
        +size, +size, -size,
        +size, +size, +size,
        +size, -size, +size,
        +size, -size, -size,

        // Back
        -size, +size, -size,
        +size, +size, -size,
        +size, -size, -size,
        -size, -size, -size,

        // Left
        -size, +size, +size,
        -size, +size, -size,
        -size, -size, -size,
        -size, -size, +size,

        // Top
        +size, +size, -size,
        -size, +size, -size,
        -size, +size, +size,
        +size, +size, +size,

        // Bottom
        +size, -size, +size,
        -size, -size, +size,
        -size, -size, -size,
        +size, -size, -size
    };

    const double uv[] = {
        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,

        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,

        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,

        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,

        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,

        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0
    };

    const double normal[] = {
        0.0, 0.0, +1.0,
        0.0, 0.0, +1.0,
        0.0, 0.0, +1.0,
        0.0, 0.0, +1.0,

        +1.0, 0.0, 0.0,
        +1.0, 0.0, 0.0,
        +1.0, 0.0, 0.0,
        +1.0, 0.0, 0.0,

        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,

        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,

        0.0, +1.0, 0.0,
        0.0, +1.0, 0.0,
        0.0, +1.0, 0.0,
        0.0, +1.0, 0.0,

        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0
    };

    const unsigned int index[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };

    Mesh *mesh = malloc(sizeof(Mesh));

    mesh->vertex_count = 24;
    mesh->index_count = 36;

    mesh->position = malloc(24 * sizeof(Vector));
    mesh->uv = malloc(24 * sizeof(UV));
    mesh->normal = malloc(24 * sizeof(Vector));
    mesh->index = malloc(36 * sizeof(unsigned int));

    for(int i = 0; i < 24; i++)
    {
        mesh->position[i].x = position[3 * i];
        mesh->position[i].y = position[3 * i + 1];
        mesh->position[i].z = position[3 * i + 2];
    }

    for(int i = 0; i < 24; i++)
    {
        mesh->uv[i].u = uv[2 * i];
        mesh->uv[i].v = uv[2 * i + 1];
    }

    for(int i = 0; i < 24; i++)
    {
        mesh->normal[i].x = normal[3 * i];
        mesh->normal[i].y = normal[3 * i + 1];
        mesh->normal[i].z = normal[3 * i + 2];
    }

    for(int i = 0; i < 36; i++)
        mesh->index[i] = index[i];

    mesh_gpu(mesh);

    return mesh;
}

Mesh *mesh_load(const char *path)
{
    Mesh *mesh = malloc(sizeof(Mesh));
    mesh->vertex_count = 0;
    mesh->index_count = 0;
    mesh->position = NULL;
    mesh->uv = NULL;
    mesh->normal = NULL;
    mesh->index = NULL;

    MeshData mesh_data = mesh_data_load(path);

    for(unsigned int i = 0; i < mesh_data.face_count; i++)
    {
        unsigned int k = mesh->vertex_count;
        mesh->vertex_count += mesh_data.face[i].vertex_count;
        mesh->position = realloc(mesh->position, mesh->vertex_count * sizeof(Vector));
        mesh->normal = realloc(mesh->normal, mesh->vertex_count * sizeof(Vector));
        mesh->uv = realloc(mesh->uv, mesh->vertex_count * sizeof(UV));

        for(unsigned int j = 0; j < mesh_data.face[i].vertex_count; j++)
        {
            mesh->position[j + k] = mesh_data.position[mesh_data.face[i].position[j]];
            mesh->normal[j + k] = mesh_data.normal[mesh_data.face[i].normal[j]];
            mesh->uv[j + k] = mesh_data.uv[mesh_data.face[i].uv[j]];
        }

        unsigned int l = mesh->index_count;
        mesh->index_count += 3 * (mesh_data.face[i].vertex_count - 2);
        mesh->index = realloc(mesh->index, mesh->index_count * sizeof(unsigned int));
        triangulate(mesh_data.face[i].vertex_count, &mesh->position[k], mesh->normal[k], k, &mesh->index[l]);
    }

    mesh_gpu(mesh);
    return mesh;
}

void mesh_destroy(Mesh *mesh)
{
    free(mesh->position);
    free(mesh->uv);
    free(mesh->normal);
    free(mesh->index);

    glDeleteBuffers(1, &mesh->position_vertex_buffer);
    glDeleteBuffers(1, &mesh->uv_vertex_buffer);
    glDeleteBuffers(1, &mesh->normal_vertex_buffer);
    glDeleteBuffers(1, &mesh->index_buffer);
    glDeleteVertexArrays(1, &mesh->vertex_array);
}

void mesh_gpu(Mesh *mesh)
{
    glGenBuffers(1, &mesh->position_vertex_buffer);
    glGenBuffers(1, &mesh->uv_vertex_buffer);
    glGenBuffers(1, &mesh->normal_vertex_buffer);
    glGenBuffers(1, &mesh->index_buffer);
    glGenVertexArrays(1, &mesh->vertex_array);

    glBindVertexArray(mesh->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->position_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->vertex_count * sizeof(double), mesh->position, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * mesh->vertex_count * sizeof(double), mesh->uv, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->vertex_count * sizeof(double), mesh->normal, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void *)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_count * sizeof(unsigned int), mesh->index, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void mesh_draw(Mesh *mesh)
{
    glBindVertexArray(mesh->vertex_array);
    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
}
