#pragma once
#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <GLFW/glfw3.h>

GLFWwindow* createWindow(int w, int h, const char* title);
void destroyWindow(GLFWwindow* wnd);

#endif	// WINDOW_HPP_INCLUDED
