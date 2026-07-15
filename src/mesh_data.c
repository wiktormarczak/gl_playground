#include <gl_playground/mesh_data.h>
#include <gl_playground/shader.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static void process_file(MeshData *mesh_data, char *file);
static void process_line(MeshData *mesh_data, char *line);
static char **split(char *string, const char *separator, unsigned int *count_dst);

MeshData mesh_data_load(const char *path)
{
    MeshData mesh_data = { 0, 0, 0, 0, NULL, NULL, NULL, NULL };

    char *file = shader_get_source(path);

    process_file(&mesh_data, file);

    free(file);
    return mesh_data;
}

static void process_file(MeshData *mesh_data, char *file)
{
    unsigned int line_count;
    char **line = split(file, "\n", &line_count);

    for(unsigned int i = 0; i < line_count; i++)
        process_line(mesh_data, line[i]);

}

static void process_line(MeshData *mesh_data, char *line)
{
    unsigned int word_count;
    char **word = split(line, " ", &word_count);

    if(strcmp(word[0], "v") == 0)
    {
        mesh_data->position_count++;
        mesh_data->position = realloc(mesh_data->position, mesh_data->position_count * sizeof(Vector));
        mesh_data->position[mesh_data->position_count - 1].x = atof(word[1]);
        mesh_data->position[mesh_data->position_count - 1].y = atof(word[2]);
        mesh_data->position[mesh_data->position_count - 1].z = atof(word[3]);
        return;
    }

    if(strcmp(word[0], "vn") == 0)
    {
        mesh_data->normal_count++;
        mesh_data->normal = realloc(mesh_data->normal, mesh_data->normal_count * sizeof(Vector));
        mesh_data->normal[mesh_data->normal_count - 1].x = atof(word[1]);
        mesh_data->normal[mesh_data->normal_count - 1].y = atof(word[2]);
        mesh_data->normal[mesh_data->normal_count - 1].z = atof(word[3]);
        return;
    }

    if(strcmp(word[0], "vt") == 0)
    {
        mesh_data->uv_count++;
        mesh_data->uv = realloc(mesh_data->uv, mesh_data->uv_count * sizeof(UV));
        mesh_data->uv[mesh_data->uv_count - 1].u = atof(word[1]);
        mesh_data->uv[mesh_data->uv_count - 1].v = atof(word[2]);
        return;
    }

    if(strcmp(word[0], "f") == 0)
    {
        unsigned int vertex_count = word_count - 1;

        mesh_data->face_count++;
        mesh_data->face = realloc(mesh_data->face, mesh_data->face_count * sizeof(Face));
        mesh_data->face[mesh_data->face_count - 1].vertex_count = vertex_count;
        mesh_data->face[mesh_data->face_count - 1].position = malloc(vertex_count * sizeof(unsigned int));
        mesh_data->face[mesh_data->face_count - 1].normal = malloc(vertex_count * sizeof(unsigned int));
        mesh_data->face[mesh_data->face_count - 1].uv = malloc(vertex_count * sizeof(unsigned int));

        for(unsigned int i = 0; i < vertex_count; i++)
        {
            char **value = split(word[i + 1], "/", NULL);
            mesh_data->face[mesh_data->face_count - 1].position[i] = atoi(value[0]) - 1;
            mesh_data->face[mesh_data->face_count - 1].normal[i] = atoi(value[2]) - 1;
            mesh_data->face[mesh_data->face_count - 1].uv[i] = atoi(value[1]) - 1;
        }
    }
}

static char **split(char *string, const char *separator, unsigned int *count_dst)
{
    char **word = NULL;
    unsigned int count = 0;

    char *curr_word = strtok(string, separator);
    while(curr_word != NULL)
    {
        word = realloc(word, ++count * sizeof(char *));
        word[count - 1] = strdup(curr_word);
        curr_word = strtok(NULL, separator);
    }

    if(count_dst != NULL)
        *count_dst = count;

    return word;
}

// typedef enum
// {
//     DEFAULT,
//     COMMENT,
//     POSITION,
//     UV,
//     NORMAL,
//     FACE
// } State;
//
// static void process_word(MeshData *mesh_data, State *state, const char *word);
// static void face_split(const char *word, unsigned int *position_i, unsigned int *uv_i, unsigned int *normal_i);
//
// MeshData *mesh_data_load(const char *path)
// {
//     MeshData mesh_data = { 0, 0, 0, NULL, NULL, NULL, 0, 0, NULL, NULL };
//     State state = DEFAULT;
//
//     FILE *file = fopen(path, "r");
//
//     unsigned int sign_count = 0, word_count = 0;
//     char sign, word[64];
//     while((sign = fgetc(file)) != EOF)
//     {
//         if(sign == ' ')
//         {
//             word[sign_count] = '\0';
//             process_word(&mesh_data, &state, word);
//             sign_count = 0;
//             word_count++;
//             continue;
//         }
//
//         if(sign == '\n')
//         {
//             word[sign_count] = '\0';
//             process_word(&mesh_data, &state, word);
//             state = DEFAULT;
//             sign_count = 0;
//             word_count = 0;
//             continue;
//         }
//
//         word[sign_count++] = sign;
//     }
//
//     fclose(file);
// }
//
// static void process_word(MeshData *mesh_data, State *state, const char *word)
// {
//     if(*state == DEFAULT)
//     {
//         if(strcmp(word, "v"))
//         {
//             *state = POSITION;
//             return;
//         }
//
//         if(strcmp(word, "vt"))
//         {
//             *state = UV;
//             return;
//         }
//
//         if(strcmp(word, "vn"))
//         {
//             *state = NORMAL;
//             return;
//         }
//
//         if(strcmp(word, "f"))
//         {
//             *state = FACE;
//             return;
//         }
//
//         *state = COMMENT;
//         return;
//     }
//
//     if(*state == POSITION)
//     {
//         if(word_count > 3)
//             return;
//
//         vertex_list->position[3 * vertex_list->position_count - 4 + word_count] = atof(word);
//         return;
//     }
//
//     if(*state == UV)
//     {
//         if(uv > 2)
//             return;
//
//         vertex_list->uv[2 * vertex_list->uv - 3 + word_count] = atof(word);
//         return;
//     }
//
//     if(*state == NORMAL)
//     {
//         if(word_count > 3)
//             return;
//
//         vertex_list->normal[3 * vertex_list->normal_count - 4 + word_count] = atof(word);
//         return;
//     }
//
//     if(*state == FACE)
//     {
//         mesh_data->face[mesh_data->face_count - 1]++;
//         mesh_data->face_vertex_count++;
//
//         mesh_data->face_vertex = realloc(mesh_data->face_vertex, 3 * mesh_data->face_vertex_count * sizeof(unsigned int)); 
//
//         unsigned int position_i, uv_i, normal_i;
//         face_split(word, &position_i, &uv_i, &normal_i);
//         mesh_data->face_vertex[3 * (mesh_data->face_vertex_count - 1) + 0] = position_i;
//         mesh_data->face_vertex[3 * (mesh_data->face_vertex_count - 1) + 1] = uv_i;
//         mesh_data->face_vertex[3 * (mesh_data->face_vertex_count - 1) + 2] = normal_i;
//         return;
//     }
// }
//
// static void face_split(const char *word, unsigned int *position_i, unsigned int *uv_i, unsigned int *normal_i)
// {
//     char i_str[3][32]
//
//     int i = 0, j = 0, k = 0;
//     while(true)
//     {
//         if(word[k] == '/')
//         {
//             i_str[i++][j] = '\0';
//             continue;
//         }
//
//         if(word[k] == '\0')
//         {
//             i_str[i++][j] = '\0';
//             break;
//         }
//
//         i_str[i][j++] = word[k++];
//     }
//
//     *position_i = atof(i_str[0]);
//     *uv_i = atof(i_str[1]);
//     *normal_i = atof(i_str[2]);
// }
