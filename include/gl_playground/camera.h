#ifndef CAMERA_H
#define CAMERA_H

typedef struct
{
    float view[16], projection[16];
} Camera;

Camera *camera_create();
void camera_destroy(Camera *camera);

void camera_translate(Camera *camera, float x, float y, float z);
void camera_rotate(Camera *camera, float x, float y, float z);

void camera_update(Camera *camera);

#endif
