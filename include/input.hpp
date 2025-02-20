#pragma once
#ifndef INPUT_HPP_INCLUDED
#define INPUT_HPP_INCLUDED

#include "./stdafx.h"
#include <GLFW/glfw3.h>

inline void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);
inline void cursorPosCallback(GLFWwindow* wnd, double xpos, double ypos);

class Input
{
public:
	// Constructor
	Input();
	
	// Getters
	const float GetAxis(uint8_t axis) const;
	const vec2& GetMDelta() const;
	const bool IsKeyPressed(int key) const;
	
	// Methods
	void Init(GLFWwindow* wnd);
	void Reset();
};

#endif	// INPUT_HPP_INCLUDED
