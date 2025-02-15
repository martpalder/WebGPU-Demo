#pragma once
#ifndef INPUT_HPP_INCLUDED
#define INPUT_HPP_INCLUDED

#include <GLFW/glfw3.h>

inline void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);

class Input
{
public:
	// Constructor
	Input();
	
	// Getters
	float GetAxis(uint8_t axis);
	bool IsKeyPressed(int key);
	
	// Methods
	void Init(GLFWwindow* wnd);
};

#endif	// INPUT_HPP_INCLUDED
