#ifndef SKYBOX_H
#define SKYBOX_H

#include "Camera.h"

void skybox_setfull(const char *path);

void skybox_draw(const Camera_t *cam, const Shader_t *shader);

#endif