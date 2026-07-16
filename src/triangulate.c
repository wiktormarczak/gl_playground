#include <gl_playground/vector.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static bool condition(unsigned int vertex_count, Vector *vertex, Vector normal, unsigned int prev, unsigned int curr, unsigned int next);
static bool is_point_in_triangle(Vector point, Vector a, Vector b, Vector c);
static unsigned int get_next(unsigned int vertex_count, bool *active, unsigned int curr);

void triangulate(unsigned int vertex_count, Vector *vertex, Vector normal, unsigned int offset, unsigned int *index_dst)
{
    if(index_dst == NULL)
        return;

    bool *active = malloc(vertex_count * sizeof(bool));
    for(int i = 0; i < vertex_count; i++)
        active[i] = true;

    unsigned int cut = 0;
    unsigned int prev = vertex_count - 1, curr = 0, next = 1;
    while(cut < vertex_count - 2)
    {
        if(condition(vertex_count, vertex, normal, prev, curr, next))
        {
            active[curr] = false;
            index_dst[3 * cut] = offset + prev;
            index_dst[3 * cut + 1] = offset + curr;
            index_dst[3 * cut + 2] = offset + next;

            curr = next;
            next = get_next(vertex_count, active, curr);
            cut++;

            continue;
        }

        prev = curr;
        curr = next;
        next = get_next(vertex_count, active, curr);
    }
}

static bool condition(unsigned int vertex_count, Vector *vertex, Vector normal, unsigned int prev, unsigned int curr, unsigned int next)
{
    Vector diff_left = vector_get_diff(vertex[curr], vertex[prev]);
    Vector diff_right = vector_get_diff(vertex[next], vertex[curr]);
    Vector breaking = vector_get_cross_product(diff_left, diff_right);
    double breaking_direction = vector_get_dot_product(breaking, normal);

    if(breaking_direction < 0.0)
        return false;

    for(int i = 0; i < vertex_count; i++)
        if(i != prev && i != curr && i != next && is_point_in_triangle(vertex[i], vertex[prev], vertex[curr], vertex[next]))
            return false;

    return true;
}

static bool is_point_in_triangle(Vector point, Vector a, Vector b, Vector c)
{
    Vector ab = vector_get_diff(b, a);
    Vector bc = vector_get_diff(c, b);
    Vector ca = vector_get_diff(a, c);

    Vector normal_triangle = vector_get_cross_product(ab, bc);

    Vector normal_ab = vector_get_cross_product(ab, normal_triangle);
    double value_ab = vector_get_dot_product(normal_ab, a);
    double value_ab_point = vector_get_dot_product(normal_ab, point);
    if(value_ab_point > value_ab)
        return false;

    Vector normal_bc = vector_get_cross_product(bc, normal_triangle);
    double value_bc = vector_get_dot_product(normal_bc, b);
    double value_bc_point = vector_get_dot_product(normal_bc, point);
    if(value_bc_point > value_bc)
        return false;

    Vector normal_ca = vector_get_cross_product(ca, normal_triangle);
    double value_ca = vector_get_dot_product(normal_ca, c);
    double value_ca_point = vector_get_dot_product(normal_ca, point);
    if(value_ca_point > value_ca)
        return false;

    return true;
}

static unsigned int get_next(unsigned int vertex_count, bool *active, unsigned int curr)
{
    while(!active[curr = (curr + 1) % vertex_count]);
    return curr;
}
