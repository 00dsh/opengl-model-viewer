#include "Monitor.h"
#include "Shader.h"
#include <GLFW/glfw3.h>

GLFWwindow *getWindow(void);
GLFWmonitor *monitor;
GLFWvidmode *vidmode;

static unsigned int win_width = 1920;
static unsigned int win_height = 1080;

void fbsizecallback(GLFWwindow *window, int width, int height);

unsigned int fullscreen = 0;

void monitor_fullscreen_toggle(void)
{
	fullscreen = !fullscreen;
	if (fullscreen)
	{
		glfwSetWindowMonitor(getWindow(), monitor, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(getWindow(), NULL, 0, 0, 1920, 1080, vidmode->refreshRate);
		glfwSetWindowPos(getWindow(), (vidmode->width / 2) - (win_width / 2), (vidmode->height / 2) - (win_height / 2));
	}
}

void monitor_init(void)
{
	int fwidth = 0;
	int fheight = 0;
	glfwGetFramebufferSize(getWindow(), &fwidth, &fheight);

	monitor = glfwGetPrimaryMonitor();
	vidmode = glfwGetVideoMode(monitor);

	glfwSetWindowPos(getWindow(), (vidmode->width / 2) - (fwidth / 2), (vidmode->height / 2) - (fheight / 2));

	glfwSetFramebufferSizeCallback(getWindow(), &fbsizecallback);
}

void fbsizecallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);

	win_width = width;
	win_height = height;

	unsigned int nr = 0;
	Shader_t **allshaders = shader_get_all(&nr);

	for (unsigned int i = 0; i < nr; i++)
	{
		shader_viewport_calc(allshaders[i]);
	}

}