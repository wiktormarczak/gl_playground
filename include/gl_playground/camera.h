#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera Camera;

Camera *camera_create();
void camera_destroy(Camera *camera);

void camera_move(Camera *camera, float x, float y, float z);
void camera_turn(Camera *camera, float pitch, float yaw);
void camera_zoom(Camera *camera, float factor);

void camera_update(Camera *camera);

float camera_get_x(Camera *camera);
float camera_get_y(Camera *camera);
float camera_get_z(Camera *camera);
void camera_get_view_matrix(Camera *camera, float view_matrix[16]);
void camera_get_projection_matrix(Camera *camera, float view_matrix[16]);

#endif
