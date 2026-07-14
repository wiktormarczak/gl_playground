#include <gl_playground/mesh.h>
#include <glad/gl.h>
#include <SDL3/SDL_log.h>
#include <stdio.h>
#include <stdlib.h>

struct Mesh
{
    unsigned int vertex_count, index_count;

    double *position, *uv, *normal;
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

    mesh->position = malloc(24 * 3 * sizeof(double));
    mesh->uv = malloc(24 * 2 * sizeof(double));
    mesh->normal = malloc(24 * 3 * sizeof(double));
    mesh->index = malloc(36 * sizeof(unsigned int));

    for(int i = 0; i < 24 * 3; i++)
        mesh->position[i] = position[i];

    for(int i = 0; i < 24 * 2; i++)
        mesh->uv[i] = uv[i];

    for(int i = 0; i < 24 * 3; i++)
        mesh->normal[i] = normal[i];

    for(int i = 0; i < 36; i++)
        mesh->index[i] = index[i];

    mesh_gpu(mesh);

    return mesh;
}

Mesh *mesh_load(const char *path)
{
    FILE *file = fopen(path, "r");

    if(!file)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not open the object file %s.\n", path);
        return NULL;
    }

    unsigned int position_count = 0, uv_count = 0, normal_count = 0;
    double *position = NULL, *uv = NULL, *normal = NULL;

    char line[128];
    while(fgets(line, 128, file))
    {
        // if(line[0] == 'v' && line[1] == ' ')
        // {
        //     float x, y, z;
        //     sscanf(line, "v %f %f %f\n", &x, &y, &z);
        //     printf("Position: %f %f %f\n", x, y, z);
        //
        //     position_count++;
        //     position = realloc(position, 3 * position_count * sizeof(double));
        //     position[3 * position_count - 3] = x;
        //     position[3 * position_count - 2] = y;
        //     position[3 * position_count - 1] = z;
        //     continue;
        // }
        //
        // if(line[0] == 'v' && line[1] == 't')
        // {
        //     float u, v;
        //     sscanf(line, "vt %f %f\n", &u, &v);
        //     printf("UV: %f %f\n", u, v);
        //
        //     uv_count++;
        //     uv = realloc(uv, 2 * uv_count * sizeof(double));
        //     uv[2 * position_count - 2] = u;
        //     uv[2 * position_count - 1] = v;
        //     continue;
        // }
        //
        // if(line[0] == 'v' && line[1] == 'n')
        // {
        //     float x, y, z;
        //     sscanf(line, "vn %f %f %f\n", &x, &y, &z);
        //     printf("Normal: %f %f\n", x, y, z);
        //
        //     normal_count++;
        //     normal = realloc(normal, 3 * normal_count * sizeof(double));
        //     normal[3 * position_count - 3] = x;
        //     normal[3 * position_count - 2] = y;
        //     normal[3 * position_count - 1] = z;
        //     continue;
        // }

        printf("Line skipped!\n");
    }

    fclose(file);

    free(position);
    free(uv);
    free(normal);

    return NULL;
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
