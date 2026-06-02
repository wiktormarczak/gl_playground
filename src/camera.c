#include <gl_playground/camera.h>
#include <gl_playground/matrix.h>
#include <gl_playground/vector.h>
#include <stdlib.h>

Camera *camera_create()
{
    Camera *camera = malloc(sizeof(Camera));

    camera->rotation_vector[0] = 0.0f;
    camera->rotation_vector[1] = 0.0f;
    camera->rotation_vector[2] = 0.0f;

    matrix_set_translation(camera->translation_matrix, 0.0f, 0.0f, -1.0f);
    matrix_set_identity(camera->rotation_matrix);
    matrix_set_perspective_projection(camera->projection, 3.14f / 4.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    return camera;
}

void camera_destroy(Camera *camera)
{
    free(camera);
}

void camera_move_horizontally(Camera *camera, float x, float z)
{
    float direction[4];
    direction[0] = x;
    direction[1] = 0.0f;
    direction[2] = z;
    direction[3] = 1.0f;

    vector_transform(direction, camera->rotation_matrix);
    direction[1] = 0.0f;
    direction[2] = -direction[2]; // ???

    camera->translation_vector[0] += direction[0];
    camera->translation_vector[1] += direction[1];
    camera->translation_vector[2] += direction[2];
}

void camera_adjust_pitch(Camera *camera, float angle)
{
    camera->rotation_vector[0] -= angle;

    if(camera->rotation_vector[0] > 3.14f / 2.0f)
        camera->rotation_vector[0] = 3.14f / 2.0f;

    if(camera->rotation_vector[0] < -3.14f / 2.0f)
        camera->rotation_vector[0] = -3.14f / 2.0f;
}

void camera_adjust_yaw(Camera *camera, float angle)
{
    camera->rotation_vector[1] -= angle;
}

void camera_translate(Camera *camera, float x, float y, float z)
{
    camera->translation_vector[0] += x;
    camera->translation_vector[1] += y;
    camera->translation_vector[2] += z;
}

void camera_rotate(Camera *camera, float x, float y, float z)
{
    matrix_rotate(camera->rotation_matrix, x, y, z);
}

void camera_update(Camera *camera)
{
    matrix_set_translation(camera->translation_matrix, camera->translation_vector[0], camera->translation_vector[1], camera->translation_vector[2]);
    matrix_set_rotation(camera->rotation_matrix, camera->rotation_vector[0], camera->rotation_vector[1], camera->rotation_vector[2]);
    matrix_multiply(camera->view, camera->rotation_matrix, camera->translation_matrix);
    matrix_multiply(camera->matrix, camera->projection, camera->view);
}
