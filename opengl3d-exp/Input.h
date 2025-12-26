#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

int input_keydown(int keycode);

int input_keyup(int keycode);

void input_init(void);

void input_process(void);

const float *get_m_off(void);

const float *get_m_pos(void);

#endif