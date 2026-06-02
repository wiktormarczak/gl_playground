#include <gl_playground/grid.h>
#include <gl_playground/shader.h>
#include <glad/gl.h>
#include <stdlib.h>

Grid *grid_create()
{
    Grid *grid = malloc(sizeof(Grid));

    grid->shader_program = shader_create_program("glsl/grid_vertex_shader.glsl", "glsl/grid_fragment_shader.glsl");

    float vertex_data[] = {
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    glCreateBuffers(1, &grid->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, grid->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &grid->vertex_array);
    glBindVertexArray(grid->vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, grid->vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return grid;
}

void grid_destroy(Grid *grid)
{
    glDeleteProgram(grid->shader_program);
    grid->shader_program = 0;

    free(grid);
    grid = NULL;
}

void grid_draw(Grid *grid)
{
    glUseProgram(grid->shader_program);
    glBindVertexArray(grid->vertex_array);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0);
}
