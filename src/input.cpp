#include "./input.hpp"
#include "./stdafx.h"

// Key Buffer
bool g_keys[350];
vec2 g_mPosCurr, g_mPosLast;

static void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) { g_keys[key] = true; }
	else if (action == GLFW_RELEASE) { g_keys[key] = false; }
};

static void cursorPosCallback(GLFWwindow* wnd, double x, double y)
{
	g_mPosLast[0] = g_mPosCurr[0];
	g_mPosLast[1] = g_mPosCurr[1];
	g_mPosCurr[0] = x;
	g_mPosCurr[1] = y;
}

Input::Input()
{}

void Input::Init(GLFWwindow* wnd)
{
	// Set the Callbacks
	glfwSetKeyCallback(wnd, keyCallback);
	glfwSetCursorPosCallback(wnd, cursorPosCallback);
	puts("Initialized Input");
}

float Input::GetAxis(uint8_t axis)
{
	switch (axis)
	{
		case 0:
		{
			bool bRight = g_keys[GLFW_KEY_RIGHT] || g_keys[GLFW_KEY_D];
			bool bLeft = g_keys[GLFW_KEY_LEFT] || g_keys[GLFW_KEY_A];
			
			return (bRight - bLeft);
		}
		case 1:
		{
			bool bUp = g_keys[GLFW_KEY_UP] || g_keys[GLFW_KEY_W];
			bool bDown = g_keys[GLFW_KEY_DOWN] || g_keys[GLFW_KEY_S];
			
			return (bUp - bDown);
		}
	}
	
	return 0.0f;
}

vec2& Input::GetMouseDelta()
{
	vec2 mDelta;
	mDelta[0] = g_mPosCurr[0] - g_mPosLast[0];
	mDelta[1] = g_mPosCurr[1] - g_mPosLast[1];
	
	return mDelta;
}

bool Input::IsKeyPressed(int key)
{
	return g_keys[key];
}
