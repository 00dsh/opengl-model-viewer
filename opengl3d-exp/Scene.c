#include "Scene.h"
#include "Shader.h"
#include "Camera.h"

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

const Shader_t *getShader(void);
const Camera_t *getCamera(void);

static Mesh_t *meshes;

/** 
Return a Mesh_t struct loaded with the pointers to the
block of memory that represents a mesh loaded from ASSIMP
Must be freed, only use **scene_free()** to free.
*/
Mesh_t scene_process_mesh(struct aiMesh *mesh, const struct aiScene *scene)
{
	unsigned int numVerts = mesh->mNumVertices;
	unsigned int numIndices = mesh->mNumFaces * 3; // It's always number of faces * 3 because we triangulate each face

	size_t vertexSize = sizeof(float) * 8 * numVerts;
	size_t indicesSize = sizeof(unsigned int) * numIndices;

	Vertex_t *data = malloc(vertexSize);
	unsigned int *indices = malloc(indicesSize);

	Mesh_t tmp = { NULL, NULL, 0, 0 };
	tmp.vertexSize = vertexSize;
	tmp.indicesSize = numIndices;

	if (data)
	{
		for (unsigned int i = 0; i < numVerts; i++)
		{
			data[i].pos[0] = mesh->mVertices[i].x;
			data[i].pos[1] = mesh->mVertices[i].y;
			data[i].pos[2] = mesh->mVertices[i].z;

			data[i].norm[0] = mesh->mNormals[i].x;
			data[i].norm[0] = mesh->mNormals[i].y;
			data[i].norm[0] = mesh->mNormals[i].z;

			data[i].tex[0] = mesh->mTextureCoords[0][i].x;
			data[i].tex[1] = mesh->mTextureCoords[0][i].y;
		}

		tmp.vertices = data;
	}
	else
	{
		printf("ERROR: Unable to malloc() for meshes array buffer");
	}

	if (indices)
	{
		unsigned int offset = 0;
		for (int b = 0; b < mesh->mNumFaces; b++)
		{
			struct aiFace face = mesh->mFaces[b];
			for (int c = 0; c < face.mNumIndices; c++)
			{
				indices[c + offset] = face.mIndices[c];
			}
			offset += face.mNumIndices;
		}

		tmp.indices = indices; // Set the pointer to the index array
	}
	else
	{
		puts("ERROR::scene_process_mesh::Unable to malloc() for meshes index buffer");
	}

	return tmp;
}

static unsigned int offset = 0;
void scene_process_node(const struct aiNode *node, const struct aiScene *scene, Mesh_t *meshes)
{
	unsigned int nrMeshes = node->mNumMeshes;
	unsigned int nrChildren = node->mNumChildren;

	// Process the nodes meshes (if any)
	for (unsigned int i = 0; i < nrMeshes; i++, offset++)
	{
		struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes[offset] = scene_process_mesh(mesh, scene);
	}

	// Process the node's children (if any)
	for (unsigned int i = 0; i < nrChildren; i++)
	{
		scene_process_node(node->mChildren[i], scene, meshes);
	}
}

/**
Pass an array of Mesh_t structs and send the data to OpenGL, return an array of Model_t structs
containing the VAO, the position vector and the transformation matrix
*/
Model_t scene_setup_mesh(const Mesh_t *mesh)
{
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertexSize, &mesh->vertices->pos[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesSize * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	Model_t tmp = { {0.0f, 0.0f, 0.0f}, {0.0f}, VAO, mesh->indicesSize };
	glm_mat4_identity(&tmp.transform[0]);
	return tmp;
}

Model_t *scene_load(const char *fpath, unsigned int *nrMeshes)
{
	const struct aiScene *scene = aiImportFile(fpath,
		aiProcess_OptimizeMeshes |
		aiProcess_FindInvalidData |
		aiProcess_FlipUVs |
		aiProcess_Triangulate |
		aiProcess_SortByPType |
		aiProcess_JoinIdenticalVertices
	);

	if (!scene)
	{
		printf("ERROR: Assimp: %s\n", aiGetErrorString());
		return NULL;
	}

	*nrMeshes = scene->mNumMeshes;

	meshes = malloc(sizeof(Mesh_t) * scene->mNumMeshes);

	if (meshes)
	{
		scene_process_node(scene->mRootNode, scene, meshes);
	}
	else
	{
		printf("ERROR: Unable to malloc() for meshes array buffer\n");
	}

	Model_t *models = malloc(sizeof(Model_t) * scene->mNumMeshes);

	if (models && meshes)
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			models[i] = scene_setup_mesh(&meshes[i]);
		}
	}
	else
	{
		printf("ERROR: Unable to malloc() for models array buffer\n");
	}

	mesh_free(meshes, scene->mNumMeshes);

	if (meshes)
	{
		free(meshes);
		meshes = NULL;
	}

	aiReleaseImport(scene);

	if (models)
	{
		return models;
	}
	else 
	{
		return NULL;
	}
}

/**
Frees the meshes array, sets NULL to all pointers
*/
void mesh_free(Mesh_t *meshes, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++, meshes++)
	{
		if (meshes)
		{
			if (meshes->vertices)
			{
				free(meshes->vertices);
				meshes->vertices = NULL;
			}

			if (meshes->indices)
			{
				free(meshes->indices);
				meshes->indices = NULL;
			}
		}
	}
}

void scene_draw(Model_t *m, unsigned int size, Shader_t *shader)
{
	for (unsigned int i = 0; i < size; i++, m++)
	{
		glm_translate(m->transform, m->position);

		shader_set_model(shader, m->transform);
		shader_mul(shader);

		glBindVertexArray(m->VAO);
		glDrawElements(GL_TRIANGLES, m->size, GL_UNSIGNED_INT, 0);

		glm_mat4_identity(shader->model);
		glm_mat4_identity(m->transform);
	}
}