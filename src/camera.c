#include <gl_playground/camera.h>
#include <gl_playground/matrix.h>
#include <stdlib.h>

Camera *camera_create()
{
    Camera *camera = malloc(sizeof(Camera));

    matrix_set_translation(camera->view, 0.0f, 0.0f, -1.0f);
    matrix_set_perspective_projection(camera->projection, 3.14f / 4.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    return camera;
}

void camera_destroy(Camera *camera)
{
    free(camera);
}

void camera_translate(Camera *camera, float x, float y, float z)
{
    matrix_translate(camera->view, x, y, z);
}

void camera_rotate(Camera *camera, float x, float y, float z)
{
    matrix_rotate(camera->view, x, y, z);
}

void camera_update(Camera *camera)
{
}
