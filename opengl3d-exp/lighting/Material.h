#ifndef MATERIAL_H
#define MATERIAL_H

#include <cglm/cglm.h>
#include "../Shader.h"
#include "../Texture.h"

typedef struct Material
{
	texture_t diffuse;
	vec3 specular;

	float shininess;
} Material_t;

Material_t material_new(void);

void material_use(Material_t *material, Shader_t *shader);

void material_diffuse(Material_t *material, texture_t texture);

void material_specular(Material_t *material, float r, float g, float b);

void material_shininess(Material_t *material, float shininess);

#endif