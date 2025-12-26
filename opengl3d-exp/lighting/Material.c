#include "Material.h"

const texture_t texture_default(void);

Material_t material_new(void)
{
	Material_t tmp;
	texture_t defaultTexture = texture_new("texture/tile.png", GL_TRUE, GL_RGBA);
	material_diffuse(&tmp, defaultTexture);
	material_specular(&tmp, 1.0f, 1.0f, 1.0f);
	material_shininess(&tmp, 32.0f);

	return tmp;
}

void material_use(Material_t *material, Shader_t *shader)
{
	shader_uniform3fv(shader, &material->specular, "material.specular");
	shader_uniform1f(shader, material->shininess, "material.shininess");
	texture_use(material->diffuse);
}


void material_diffuse(Material_t *material, texture_t texture)
{
	material->diffuse = texture;
}

void material_specular(Material_t *material, float r, float g, float b)
{
	material->specular[0] = r;
	material->specular[1] = g;
	material->specular[2] = b;
}

void material_shininess(Material_t *material, float shininess)
{
	material->shininess = shininess;
}