#ifndef SCENE_H
#define SCENE_H

#include <cglm/cglm.h>
#include "Shader.h"

// TODO: process all meshes in the scene
// Put each individual mesh from the scene in its own VAO
// Use the mesh struct to achieve this
// We want to be able to render every sort of model without
// any visual artifacts or ugliness going on

// Individual mesh drawing | WIP
// Retrieve texture coords
// Retrieve normal coords

typedef struct Vertex
{
	vec3 pos;
	vec3 norm;
	vec2 tex;
} Vertex_t;

typedef struct Mesh
{
	Vertex_t *vertices;
	unsigned int *indices;
	size_t vertexSize;
	size_t indicesSize;
} Mesh_t;

typedef struct Model
{
	vec3 position;
	mat4 transform;
	unsigned int VAO;
	unsigned int size;
} Model_t;

Model_t *scene_load(const char *fpath, unsigned int *nrMeshes);

void scene_draw(Model_t *models, unsigned int size, Shader_t *shader);

void mesh_free(Mesh_t *meshes, unsigned int size);

#endif