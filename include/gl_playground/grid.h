#ifndef GRID_H
#define GRID_H

typedef struct
{
    unsigned int shader_program;
    unsigned int view_matrix_location, projection_matrix_location;
    unsigned int vertex_buffer, vertex_array;
} Grid;

Grid *grid_create();
void grid_destroy(Grid *grid);

void grid_draw(Grid *grid, float view_matrix[], float projection_matrix[]);

#endif
