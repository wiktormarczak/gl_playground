#include <gl_playground/vector.h>
#include <string.h>

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
