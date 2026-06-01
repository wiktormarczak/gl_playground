#include <gl_playground/matrix.h>
#include <math.h>

void matrix_set_identity(float matrix[])
{
    for(int i = 0; i < 16; i++)
        matrix[i] = 0.0f;

    for(int i = 0; i < 16; i += 5)
        matrix[i] = 1.0f;
}

void matrix_set_rotation_x(float matrix[], float theta)
{
    matrix_set_identity(matrix);

    matrix[5] = cos(theta);
    matrix[6] = -sin(theta);
    matrix[9] = sin(theta);
    matrix[10] = cos(theta);
}

void matrix_set_rotation_y(float matrix[], float theta)
{
    matrix_set_identity(matrix);

    matrix[0] = cos(theta);
    matrix[2] = sin(theta);
    matrix[8] = -sin(theta);
    matrix[10] = cos(theta);
}

void matrix_set_rotation_z(float matrix[], float theta)
{
    matrix_set_identity(matrix);

    matrix[0] = cos(theta);
    matrix[1] = -sin(theta);
    matrix[4] = sin(theta);
    matrix[5] = cos(theta);
}
