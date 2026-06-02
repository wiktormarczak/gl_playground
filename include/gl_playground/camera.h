#ifndef CAMERA_H
#define CAMERA_H

typedef struct
{
    float rotation_vector[3], translation_vector[3];

    float matrix[16];
    float view[16], projection[16];
    float translation_matrix[16], rotation_matrix[16];
} Camera;

Camera *camera_create();
void camera_destroy(Camera *camera);

void camera_move_horizontally(Camera *camera, float x, float z);
void camera_adjust_pitch(Camera *camera, float angle);
void camera_adjust_yaw(Camera *camera, float angle);

void camera_translate(Camera *camera, float x, float y, float z);
void camera_rotate(Camera *camera, float x, float y, float z);

void camera_update(Camera *camera);

#endif
