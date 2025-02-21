#include "./input.hpp"
#include "./stdafx.h"

// Key Buffer
bool g_keys[350];
vec2 g_mPosCurr, g_mPosLast;
vec2 g_mDelta;

inline void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) { g_keys[key] = true; }
	else if (action == GLFW_RELEASE) { g_keys[key] = false; }
};

inline void cursorPosCallback(GLFWwindow* wnd, double x, double y)
{
	// Set Mouse Positions
	g_mPosLast[0] = g_mPosCurr[0];
	g_mPosLast[1] = g_mPosCurr[1];
	g_mPosCurr[0] = x;
	g_mPosCurr[1] = y;
	
	// Set Mouse Delta
	g_mDelta[0] = g_mPosCurr[0] - g_mPosLast[0];
	g_mDelta[1] = g_mPosCurr[1] - g_mPosLast[1];
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

float Input::GetAxis(uint8_t axis) const
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

vec2& Input::GetMDelta() const
{	
	return g_mDelta;
}

bool Input::IsKeyPressed(int key) const
{
	return g_keys[key];
}

void Input::Reset()
{
	g_mDelta[0] = 0.0f;
	g_mDelta[1] = 0.0f;
}
