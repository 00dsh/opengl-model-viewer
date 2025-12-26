#include "Skybox.h"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"

#include <glad/glad.h>
#include <cglm/cglm.h>

static texture_t skybox;
static unsigned int VAO;

const Camera_t *getCamera(void);
const Shader_t *getShader(void);

// Set a skybox 
void skybox_setfull(const char *path)
{
	const float l_s0 = 0.0f;
	const float l_s1 = 1.0f / 4.0f;
	const float l_t0 = 1.0f / 3.0f;
	const float l_t1 = 2.0f / 3.0f;

	const float f_s0 = 1.0f / 4.0f;
	const float f_s1 = 2.0f / 4.0f;
	const float f_t0 = l_t0;
	const float f_t1 = l_t1;

	const float r_s0 = 2.0f / 4.0f;
	const float r_s1 = 3.0f / 4.0f;
	const float r_t0 = l_t0;
	const float r_t1 = l_t1;

	const float b_s0 = 3.0f / 4.0f;
	const float b_s1 = 1.0f;
	const float b_t0 = l_t0;
	const float b_t1 = l_t1;

	const float t_s0 = f_s0;
	const float t_s1 = f_s1;
	const float t_t0 = 2.0f / 3.0f;
	const float t_t1 = 1.0f;

	const float bt_s0 = f_s0;
	const float bt_s1 = f_s1;
	const float bt_t0 = 0.0f;
	const float bt_t1 = l_t0;

	float data[] = {
		// Left face
		-0.5f,  0.5f, -0.5f,	l_s1, l_t1, // Top right
		-0.5f, -0.5f,  0.5f,	l_s0, l_t0, // Bottom left
		-0.5f, -0.5f, -0.5f,	l_s1, l_t0, // Bottom right
		-0.5f,  0.5f,  0.5f,	l_s0, l_t1, // Top left

		// Right face
		0.5f,  0.5f, -0.5f,		r_s0, r_t1, // Top right
		0.5f, -0.5f,  0.5f,		r_s1, r_t0, // Bottom left
		0.5f, -0.5f, -0.5f,		r_s0, r_t0, // Bottom right
		0.5f,  0.5f,  0.5f,		r_s1, r_t1, // Top left

		// Top face
		 0.5f, 0.5f,  0.5f,		t_s1, t_t1, // Top right
		-0.5f, 0.5f, -0.5f,		t_s0, t_t0, // Bottom left
		 0.5f, 0.5f, -0.5f,		t_s1, t_t0, // Bottom right
		-0.5f, 0.5f,  0.5f,		t_s0, t_t1, // Top left

		// Bottom face
		 0.5f, -0.5f,  0.5f,	bt_s1, bt_t0, // Top right
		-0.5f, -0.5f, -0.5f,	bt_s0, bt_t1, // Bottom left
		 0.5f, -0.5f, -0.5f,	bt_s1, bt_t1, // Bottom right
		-0.5f, -0.5f,  0.5f,	bt_s0, bt_t0, // Top left

		// Front face (this face is behind the camera, +z direction)
		 0.5f,  0.5f, 0.5f,		b_s0, b_t1, // Top right
		-0.5f, -0.5f, 0.5f,		b_s1, b_t0,	// Bottom left
		 0.5f, -0.5f, 0.5f,		b_s0, b_t0, // Bottom right
		-0.5f,  0.5f, 0.5f,		b_s1, b_t1, // Top left

		// Back face (this face is in front of the camera, -z direction)
		 0.5f,  0.5f, -0.5f,	f_s1, f_t1, // Top right
		-0.5f, -0.5f, -0.5f,	f_s0, f_t0,	// Bottom left
		 0.5f, -0.5f, -0.5f,	f_s1, f_t0, // Bottom right
		-0.5f,  0.5f, -0.5f,	f_s0, f_t1  // Top left
	};

	unsigned int indices[] = {
		// Left face
		0, 1, 2,
		3, 1, 0,

		// Top face
		8, 9, 10,
		11, 9, 8,

		// Back face
		20, 21, 22,
		23, 21, 20,

		// Bottom face
		14, 13, 12,
		12, 13, 15,

		// Right face
		6, 5, 4,
		4, 5, 7,

		// Front face
		18, 17, 16,
		16, 17, 19
		
	};

	skybox = texture_new(path, GL_TRUE, GL_RGB);

	unsigned int VBO;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	// Cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

static mat4 transform;
void skybox_draw(const Camera_t *cam, const Shader_t *shader)
{
	shader_use(shader);

	glm_translate(&transform, cam->position);

	shader_set_model(getShader(), &transform);
	shader_mul(getShader());

	glDisable(GL_DEPTH_TEST);
	texture_use(skybox);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);

	glm_mat4_identity(getShader()->model);
	glm_mat4_identity(&transform[0]);

	shader_usei(0);
}