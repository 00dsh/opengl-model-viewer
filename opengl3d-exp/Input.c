#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "Time.h"
#include "Monitor.h"

GLFWwindow *getWindow(void);
Camera_t *getCamera(void);

// move these to camera object src
static float spd = 6.0f;
static float mvSpeed;

static int mlock = 1; // disable mouse by default

// Callback prototypes
static void calc_moffset(void);
static void keycallback(GLFWwindow *window, int scancode, int action, int key, int mods);
static void mposcallback(GLFWwindow *window, double xpos, double ypos);
static void mbuttoncallback(GLFWwindow *window, int button, int action);

// util function protos
void input_cursor_release(void);
void input_cursor_lock(void);

int input_keydown(int keycode)
{
	if (glfwGetKey(getWindow(), keycode) == GLFW_PRESS)
	{
		return GLFW_TRUE;
	}
	else
	{
		return GLFW_FALSE;
	}
}

int input_keyup(int keycode)
{
	if (glfwGetKey(getWindow(), keycode) == GLFW_RELEASE)
	{
		return GLFW_TRUE;
	}
	else
	{
		return GLFW_FALSE;
	}
}

void input_init(void)
{
	input_cursor_lock(); // Disable cursor

	glfwSetKeyCallback(getWindow(), &keycallback);
	glfwSetCursorPosCallback(getWindow(), &mposcallback);
	glfwSetMouseButtonCallback(getWindow(), &mbuttoncallback);
}

void input_process(void)
{
	calc_moffset();

	mvSpeed = spd * time_deltaTimef();

	if (input_keydown('A'))
	{
		camera_left(getCamera(), mvSpeed);
	}

	if (glfwGetKey(getWindow(), 'D') == GLFW_PRESS)
	{
		camera_right(getCamera(), mvSpeed);
	}

	if (glfwGetKey(getWindow(), 'W') == GLFW_PRESS)
	{
		camera_forward(getCamera(), mvSpeed);
	}

	if (glfwGetKey(getWindow(), 'S') == GLFW_PRESS)
	{
		camera_backward(getCamera(), mvSpeed);
	}

	if (glfwGetKey(getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera_up(getCamera(), mvSpeed);
	}

	if (glfwGetKey(getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera_down(getCamera(), mvSpeed);
	}
	
	if (glfwGetKey(getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		spd = 12.0f;
	}
	else
	{
		spd = 6.0f;
	}
}

static float oldX = 0.0f;
static float oldY = 0.0f;
static float sens = 0.1f;

static float m_pos[2]; // Actual mouse position on screen
static float m_off[2]; // Mouse offset from last known position

static int first_mouse = 1;

static void mposcallback(GLFWwindow *window, double xpos, double ypos)
{
	if (first_mouse)
	{
		oldX = (float)xpos;
		oldY = (float)ypos;
		first_mouse = 0;
	}

	m_pos[0] = (float) xpos;
	m_pos[1] = (float) ypos;
}

static void calc_moffset(void)
{
	m_off[0] = sens * ((float)m_pos[0] - oldX);
	m_off[1] = sens * (oldY - (float)m_pos[1]);

	oldX = (float)m_pos[0];
	oldY = (float)m_pos[1];
}

const float *get_m_off(void)
{
	return &m_off;
}

const float *get_m_pos(void)
{
	return &m_pos;
}

int shaderDebug = 0;
static void keycallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		int nr = 0;
		Shader_t **allshaders = shader_get_all(&nr);

		shaderDebug = !shaderDebug;

		for (int i = 0; i < nr; i++)
		{
			shader_use(allshaders[i]);
			shader_uniform1i(allshaders[i], shaderDebug, "shaderDebug");
			shader_usei(0);
		}
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		mlock = 0;
		input_cursor_release();
	}

	if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
	{
		monitor_fullscreen_toggle();
	}
}

static void mbuttoncallback(GLFWwindow *window, int button, int action)
{
	if (button == 0 && action == GLFW_PRESS)
	{
		if (!mlock)
		{
			first_mouse = 1;
			mlock = 1;
			input_cursor_lock();
		}
	}
}

void input_cursor_release(void)
{
	glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Enable cursor
}

void input_cursor_lock(void)
{
	glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Lock cursor
}