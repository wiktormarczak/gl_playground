#include <gl_playground/grid.h>
#include <gl_playground/shader.h>
#include <glad/gl.h>
#include <stdlib.h>

Grid *grid_create()
{
    Grid *grid = malloc(sizeof(Grid));

    grid->shader_program = shader_create_program("glsl/grid_vertex_shader.glsl", "glsl/grid_fragment_shader.glsl");
    grid->view_matrix_location = glGetUniformLocation(grid->shader_program, "view");
    grid->projection_matrix_location = glGetUniformLocation(grid->shader_program, "projection");

    float vertex_data[] = {
        0.0f, 0.0f, -10.0f,
        0.0f, 0.0f, 10.0f
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

    grid->count = 41;
    grid->spacing = 1.0f;

    return grid;
}

void grid_destroy(Grid *grid)
{
    glDeleteProgram(grid->shader_program);
    grid->shader_program = 0;

    free(grid);
    grid = NULL;
}

void grid_draw(Grid *grid, float view_matrix[], float projection_matrix[], float camera_x, float camera_y, float camera_z)
{
    glUseProgram(grid->shader_program);
    glUniformMatrix4fv(grid->view_matrix_location, 1, true, view_matrix);
    glUniformMatrix4fv(grid->projection_matrix_location, 1, true, projection_matrix);
    glUniform1ui(glGetUniformLocation(grid->shader_program, "count"), grid->count);
    glUniform1f(glGetUniformLocation(grid->shader_program, "spacing"), grid->spacing);
    glUniform3f(glGetUniformLocation(grid->shader_program, "camera_position"), camera_x, camera_y, camera_z);
    glDrawArrays(GL_LINES, 0, 4 * grid->count);
    glUseProgram(0);
}
