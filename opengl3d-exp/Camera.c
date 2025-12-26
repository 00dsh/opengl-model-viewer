#include "Camera.h"
#include <math.h>
#include <cglm/cglm.h>

const float *get_m_off(void);

static vec3 worldUp = {
	0.0f, 1.0f, 0.0f
};

static double toRad(float degrees)
{
	return degrees * (M_PI / 180.0f);
}

static double toDeg(float radians)
{
	return radians * (180.0f / M_PI);
}

Camera_t camera_new(float x, float y, float z, float pitch, float yaw)
{
	Camera_t tmpCam = {0.0f};

	tmpCam.position[0] = x;
	tmpCam.position[1] = y;
	tmpCam.position[2] = z;

	tmpCam.pitch = pitch;
	tmpCam.yaw = yaw;
	
	return tmpCam;
}

static void update_direction(Camera_t *cam)
{
	// Get the pitch extremely close to 90 but not over so it is
	// not noticable to the user when they hit the limit
	if (cam->pitch >= 90.0f)
	{
		cam->pitch = 89.99999f;
	}
	else if (cam->pitch <= -90.0f)
	{
		cam->pitch = -89.99999f;
	}
	
	cam->direction[0] = (float) (cos(toRad(cam->yaw)) * cos(toRad(cam->pitch)));
	cam->direction[2] = (float) (sin(toRad(cam->yaw)) * cos(toRad(cam->pitch)));
	cam->direction[1] = (float) (sin(toRad(cam->pitch)));
	glm_normalize(&cam->direction);

	glm_cross(&worldUp, &cam->direction, &cam->right);
	glm_normalize(&cam->right);

	glm_cross(&cam->direction, &cam->right, &cam->up);
	glm_normalize(&cam->up);
}

void camera_update(Camera_t *cam, Shader_t *shader)
{
	shader_use(shader);
	update_direction(cam);

	const float *m_off = get_m_off();

	cam->yaw += m_off[0];
	cam->pitch += m_off[1];

	vec3 d = {0.0f, 0.0f, 0.0f};
	glm_vec3_add(&cam->direction, &cam->position, &d);

	glm_lookat(&cam->position, &d, &cam->up, &shader->view);

	shader_mul(shader);
	shader_usei(0);
}

void camera_up(Camera_t *cam, float speed)
{
	cam->position[1] += speed;
}

void camera_down(Camera_t *cam, float speed)
{
	cam->position[1] -= speed;
}

void camera_forward(Camera_t *cam, float speed)
{
	glm_vec3_muladds(&cam->direction, speed, &cam->position);
}

void camera_backward(Camera_t *cam, float speed)
{
	glm_vec3_muladds(&cam->direction, -speed, &cam->position);
}

void camera_left(Camera_t *cam, float speed)
{
	glm_vec3_muladds(&cam->right, speed, &cam->position);
}

void camera_right(Camera_t *cam, float speed)
{
	glm_vec3_muladds(&cam->right, -speed, &cam->position);
}