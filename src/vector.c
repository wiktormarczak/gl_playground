#include <gl_playground/vector.h>
#include <string.h>
#include <math.h>

Vector vector_get_diff(Vector left, Vector right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    return left;
}

Vector vector_get_cross_product(Vector left, Vector right)
{
    Vector cross_product;
    cross_product.x = left.y * right.z - left.z * right.y;
    cross_product.y = left.z * right.x - left.x * right.z;
    cross_product.z = left.x * right.y - left.y * right.x;
    return cross_product;
}

double vector_get_dot_product(Vector left, Vector right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

Vector vector_get_normalized(Vector vector)
{
    double length = sqrt(vector_get_dot_product(vector, vector));
    vector.x  /= length;
    vector.y  /= length;
    vector.z  /= length;
    return vector;
}

void vector_multiply_by_matrix(float dst[], float matrix[], float vector[])
{
    for(int i = 0; i < 4; i++)
    {
        dst[i] = 0.0f;
        for(int j = 0; j < 4; j++)
            dst[i] += matrix[4 * i + j] * vector[j];
    }
}

void vector_transform(float vector[], float matrix[])
{
    float copy[4];
    memcpy(copy, vector, 4 * sizeof(float));
    vector_multiply_by_matrix(vector, matrix, copy);
}
