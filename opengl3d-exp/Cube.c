#include "Cube.h"
#include "Camera.h"
#include "Shader.h"

#include <GLFW/glfw3.h>

#include <stdio.h>

void log_verbose(const char *message);

const Shader_t *getShader(void);
const Camera_t *getCamera(void);

mat4 transform;

Cube_t cube_new(float x, float y, float z)
{
	log_verbose("==== cube_new() call ====");
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	log_verbose("Generated and bound vertex array");

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	log_verbose("Generated and bound vertex buffer");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	log_verbose("Set vertex attributes, cleaning up...");

	//Cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	log_verbose("Done!");

	texture_t tex = texture_new("texture/box.jpg", GL_TRUE, GL_RGB);
	glm_mat4_identity(&transform);

	Cube_t tmp = { {x, y, z}, VAO, tex };
	
	log_verbose("==== end cube_new() ====");

	return tmp;
}

void doTransform(const Cube_t *c)
{
	float val = sin(glfwGetTime());
	vec3 pos = { 0.0f, val, 0.0f };
	vec3 axis = { 0.0f, 1.0f, 0.0f };
	glm_translate(&transform, &pos);
	glm_rotate(&transform, glfwGetTime(), &axis);
	//glm_scale(&transform[0], &scale);

	shader_set_model(getShader(), &transform);
	shader_mul(getShader());
}

void endTransform(void)
{
	glm_mat4_identity(getShader()->model);
	glm_mat4_identity(&transform);
}

int i = 0;
void cube_draw(const Cube_t *c, Shader_t *shader)
{
	shader_use(shader);

	doTransform(c);
	texture_use(c->texture);

	glBindVertexArray(c->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	endTransform();

	shader_usei(0);
}