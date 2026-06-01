#ifndef MATRIX_H
#define MATRIX_H

void matrix_set_identity(float matrix[]);

void matrix_set_scaling(float matrix[], float x, float y, float z);
void matrix_set_translation(float matrix[], float x, float y, float z);

void matrix_set_rotation_x(float matrix[], float theta);
void matrix_set_rotation_y(float matrix[], float theta);
void matrix_set_rotation_z(float matrix[], float theta);

#endif
