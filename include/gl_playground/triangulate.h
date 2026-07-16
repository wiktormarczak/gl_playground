#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include <gl_playground/vector.h>

void triangulate(unsigned int vertex_count, Vector *vertex, Vector normal, unsigned int offset, unsigned int *index_dst);

#endif
