#include "./input.hpp"

#include <cstdio>

// Key Buffer
bool g_keys[350];

void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) { g_keys[key] = true; }
	else if (action == GLFW_RELEASE) { g_keys[key] = false; }
};

Input::Input()
{}

void Input::Init(GLFWwindow* wnd)
{
	// Set the Key Callback
	glfwSetKeyCallback(wnd, keyCallback);
}

float Input::GetAxis(uint8_t axis)
{
	switch (axis)
	{
		case 0: return g_keys[GLFW_KEY_D] - g_keys[GLFW_KEY_A];
		case 1: return g_keys[GLFW_KEY_S] - g_keys[GLFW_KEY_W];
	}
	
	return 0.0f;
}

bool Input::IsKeyPressed(int key)
{
	return g_keys[key];
}
