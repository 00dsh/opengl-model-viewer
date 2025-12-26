#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include "Shader.h"

typedef struct Camera
{
	vec3 position;
	vec3 direction;
	vec3 up;
	vec3 right;

	float pitch;
	float yaw;
} Camera_t;

Camera_t camera_new(float x, float y, float z, float pitch, float yaw);

void camera_update(Camera_t *cam, Shader_t *shader);

void camera_up(Camera_t *cam, float speed);

void camera_down(Camera_t *cam, float speed);

void camera_forward(Camera_t *cam, float speed);

void camera_backward(Camera_t *cam, float speed);

void camera_left(Camera_t *cam, float speed);

void camera_right(Camera_t *cam, float speed);

#endif