#ifndef MATRIX_H
#define MATRIX_H

void matrix_set_identity(float matrix[]);
void matrix_multiply(float matrix[], float left[], float right[]);
void matrix_multiply_3(float matrix[], float a[], float b[], float c[]);

void matrix_set_scaling(float matrix[], float x, float y, float z);
void matrix_set_translation(float matrix[], float x, float y, float z);
void matrix_set_rotation(float matrix[], float x, float y, float z);

void matrix_set_rotation_x(float matrix[], float theta);
void matrix_set_rotation_y(float matrix[], float theta);
void matrix_set_rotation_z(float matrix[], float theta);

void matrix_set_ortographic_projection(float matrix[], float height, float aspect, float near, float far);
void matrix_set_perspective_projection(float matrix[], float fov, float aspect, float near, float far);

#endif
