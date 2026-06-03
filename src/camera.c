#include <gl_playground/camera.h>
#include <gl_playground/matrix.h>
#include <gl_playground/vector.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Camera
{
    float view_matrix[16], projection_matrix[16];
    float x, y, z, pitch, yaw;
    float fov, aspect, near, far;
};

Camera *camera_create()
{
    Camera *camera = malloc(sizeof(Camera));

    camera->x = 0.0f;
    camera->y = 0.0f;
    camera->z = 0.0f;
    camera->pitch = 0.0f;
    camera->yaw = 0.0f;
    camera->fov = M_PI / 4.0f;
    camera->aspect = 4.0f / 3.0f;
    camera->near = 0.1f;
    camera->far = 100.0f;

    return camera;
}

void camera_destroy(Camera *camera)
{
    free(camera);
}

void camera_move(Camera *camera, float x, float y, float z)
{
    camera->x += x * cos(camera->yaw) + z * sin(camera->yaw);
    camera->y += y;
    camera->z += -x * sin(camera->yaw) + z * cos(camera->yaw);
}

void camera_turn(Camera *camera, float pitch, float yaw)
{
    camera->pitch += pitch;
    camera->yaw += yaw;

    if(pitch > M_PI / 2.0f)
        pitch = M_PI / 2.0f;
    if(pitch < -M_PI / 2.0f)
        pitch = -M_PI / 2.0f;
}

void camera_zoom(Camera *camera, float factor)
{
    camera->fov *= factor;
}

void camera_update(Camera *camera)
{
    matrix_set_identity(camera->view_matrix);
    matrix_translate(camera->view_matrix, -camera->x, -camera->y, -camera->z);
    matrix_rotate(camera->view_matrix, -camera->pitch, -camera->yaw, 0.0f);

    matrix_set_perspective_projection(camera->projection_matrix, camera->fov, camera->aspect, camera->near, camera->far);
}

void camera_get_view_matrix(Camera *camera, float view_matrix[16])
{
    memcpy(view_matrix, camera->view_matrix, 16 * sizeof(float));
}

void camera_get_projection_matrix(Camera *camera, float projection_matrix[16])
{
    memcpy(projection_matrix, camera->projection_matrix, 16 * sizeof(float));
}
