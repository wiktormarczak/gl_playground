#include <gl_playground/matrix.h>
#include <math.h>

void matrix_set_identity(float matrix[])
{
    for(int i = 0; i < 16; i++)
        matrix[i] = 0.0f;

    for(int i = 0; i < 16; i += 5)
        matrix[i] = 1.0f;
}

void matrix_set_rotation(float matrix[], float theta)
{
    matrix_set_identity(matrix);

    matrix[0] = cos(theta);
    matrix[1] = -sin(theta);
    matrix[4] = sin(theta);
    matrix[5] = cos(theta);
}
