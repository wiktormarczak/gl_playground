#include <gl_playground/matrix.h>
#include <math.h>

void matrix_set_identity(float matrix[])
{
    for(int i = 0; i < 16; i++)
        matrix[i] = 0.0f;

    for(int i = 0; i < 16; i += 5)
        matrix[i] = 1.0f;
}

void matrix_multiply(float matrix[], float left[], float right[])
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            matrix[4 * i + j] = 0.0f;
            for(int k = 0; k < 4; k++)
                matrix[4 * i + j] += left[4 * i + k] * right[4 * k + j];
        }
    }
}

void matrix_multiply_3(float matrix[], float a[], float b[], float c[])
{
    float ab[16];
    matrix_multiply(ab, a, b);
    matrix_multiply(matrix, ab, c);
}

void matrix_set_scaling(float matrix[], float x, float y, float z)
{
    matrix_set_identity(matrix);

    matrix[0] = x;
    matrix[5] = y;
    matrix[10] = z;
}

void matrix_set_translation(float matrix[], float x, float y, float z)
{
    matrix_set_identity(matrix);

    matrix[3] = x;
    matrix[7] = y;
    matrix[11] = z;
}

void matrix_set_rotation(float matrix[], float x, float y, float z)
{
    float rotation_x[16], rotation_y[16], rotation_z[16];

    matrix_set_rotation_x(rotation_x, x);
    matrix_set_rotation_y(rotation_y, y);
    matrix_set_rotation_z(rotation_z, z);

    matrix_multiply_3(matrix, rotation_x, rotation_y, rotation_z);
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

void matrix_set_ortographic_projection(float matrix[], float height, float aspect, float near, float far)
{
    matrix_set_identity(matrix);

    matrix[0] = 2.0f / (height * aspect);
    matrix[5] = 2.0f / height;
    matrix[10] = -2.0f / (far - near);
    matrix[11] = -(far + near) / (far - near);
}

void matrix_set_perspective_projection(float matrix[], float fov, float aspect, float near, float far)
{
    matrix_set_identity(matrix);

    matrix[0] = 1.0f / (tan(fov / 2.0f) * aspect);
    matrix[5] = 1.0f / tan(fov / 2.0f);
    matrix[10] = -(far + near) / (far - near);
    matrix[11] = -2.0f * far * near / (far - near);
    matrix[14] = -1.0f;
    matrix[15] = 0.0f;
}
