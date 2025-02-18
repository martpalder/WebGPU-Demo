#pragma once
#ifndef INPUT_HPP_INCLUDED
#define INPUT_HPP_INCLUDED

#include "./stdafx.h"
#include <GLFW/glfw3.h>

inline static void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);
inline static void cursorPosCallback(GLFWwindow* wnd, double xpos, double ypos);

class Input
{
public:
	// Constructor
	Input();
	
	// Getters
	float GetAxis(uint8_t axis);
	vec2& GetMDelta();
	bool IsKeyPressed(int key);
	
	// Methods
	void Init(GLFWwindow* wnd);
	void Reset();
};

#endif	// INPUT_HPP_INCLUDED
