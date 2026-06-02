#ifndef GRID_H
#define GRID_H

typedef struct
{
    unsigned int shader_program;
    unsigned int vertex_buffer, vertex_array;
} Grid;

Grid *grid_create();
void grid_destroy(Grid *grid);

void grid_draw(Grid *grid);

#endif
