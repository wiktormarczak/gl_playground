#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    double x, y, z;
} Vector;

Vector vector_get_diff(Vector left, Vector right);
Vector vector_get_cross_product(Vector left, Vector right);
double vector_get_dot_product(Vector left, Vector right);

void vector_multiply_by_matrix(float dst[], float matrix[], float vector[]);
void vector_transform(float vector[], float matrix[]);

#endif
