/* 
 * This program is written as a small side-project and a small testament
 * to my learning of C and the procedural language paradigm. 
 * This program will render any 3D model passed into it via argv[1] 
 * 
 * File: Game.c
 * Author: Dilan Shabani
 */

#include <Windows.h>

#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Monitor.h"
#include "Input.h"
#include "Shader.h"
#include "Time.h"
#include "Camera.h"
#include "Skybox.h"
#include "Scene.h"
#include "Cube.h"

#define DEBUG_MODE 0

void log_verbose(const char *message)
{
	if (DEBUG_MODE == 1)
	{
		printf("DEBUG: %s\n", message);
	}
}

// Window handle + attributes
GLFWwindow *window;
const int width = 1920;
const int height = 1080;
const char *title = "3d model loading";

/* 
 * TODO::
 * Get shaders to work (file ops, etc); | DONE
 * Get input working | DONE
 * Draw a cube to the screen, create cube source | DONE
 * Add working camera controls | DONE
 * Add skybox | DONE
 * Create a source file for ASSIMP | DONE
 * Get model to load and process model data | DONE
 * Draw a model to the screen on basic vertex/fragment shaders | DONE
 * Get textures to work with the models (texture coords) | WIP
 */

// Function prototypes
void initScene(void);

// Shader objects
Shader_t shader;
Camera_t cam;

// Scene objects
Model_t *scene;
Cube_t c;

static const char *modelPath;

unsigned int numMeshes;
void init(void)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		printf("Unable to initialize GLFW\n");
		glfwTerminate();
	}

	window = glfwCreateWindow(width, height, title, NULL, NULL); // Create window struct

	// Set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwMakeContextCurrent(window); // Set context before initializing GLAD

	// Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Unable to load GLAD\n");
		glfwTerminate();
	}

	monitor_init();

	input_init();
	
	// Create a new new shader, return shader struct
	shader = shader_new("shader/vertex.glsl", "shader/fragment.glsl");
	shader_init(&shader);
	shader_use(&shader);

	cam = camera_new(0.0f, 0.0f, 3.0f, 0.0f, -90.0f);

	// Enable GL features
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	initScene();

	glfwSwapInterval(0);

	glfwShowWindow(window);
}

inline void initScene(void)
{
	skybox_setfull("texture/skybox/full.jpg");
	c = cube_new(0.0f, 0.0f, 0.0f);

	scene = scene_load(modelPath, &numMeshes);

	if (!scene)
	{
		printf("ERROR: Scene could not load\n");
	}
}

void loop(void)
{	
	glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera_update(&cam, &shader);

		skybox_draw(&cam, &shader);

		cube_draw(&c, &shader);
		scene_draw(scene, numMeshes, &shader);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		input_process();
		time_calc();
	}
}

// Readonly gets
const GLFWwindow *getWindow(void)
{
	return window;
}

const Shader_t *getShader(void)
{
	return &shader;
}

const Camera_t *getCamera(void)
{
	return &cam;
}

void cleanup(void)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		printf("Loading model from path: %s\n", argv[1]);
		modelPath = argv[1];
	}
	else
	{
		printf("Path left blank or incorrect args, using default cube.\n");
		modelPath = "model/chair.FBX";
	}

	init();
	loop();
	cleanup();

	return 0;
}