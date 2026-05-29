#ifndef SHADERS_H
#define SHADERS_H

unsigned int get_shader_program(const char *vertex_shader_path, const char *fragment_shader_path);

unsigned int get_vertex_shader(const char *shader_path);
unsigned int get_fragment_shader(const char *shader_path);

unsigned int get_shader(const char *shader_path, unsigned int shader_type);

char *get_source(const char *shader_path);

#endif
