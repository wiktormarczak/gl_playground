#ifndef GRID_H
#define GRID_H

typedef struct
{
    unsigned int shader_program;
    unsigned int view_matrix_location, projection_matrix_location;

    unsigned int count;
    float spacing;
} Grid;

Grid *grid_create();
void grid_destroy(Grid *grid);

void grid_draw(Grid *grid, float view_matrix[], float projection_matrix[], float camera_x, float camera_y, float camera_z);

#endif
