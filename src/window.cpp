#include "./window.hpp"

#include <cstdio>

GLFWwindow* createWindow(int w, int h, const char* title)
{
	GLFWwindow* wnd = nullptr;
	
	// Initialize GLFW
	if (glfwInit() == false)
	{
		// Initialization failed
		perror("[ERROR]: Failed to initialize GLFW");
		
		return nullptr;
	}
	
	// Set Window Hints
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // <-- extra info for glfwCreateWindow
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	// Create the window
	wnd = glfwCreateWindow(w, h, title, nullptr, nullptr);
	if (wnd == nullptr)
	{
		// Window Creation failed
		perror("[ERROR]: Failed to create a Window");
	}
	
	return wnd;
}

void destroyWindow(GLFWwindow* wnd)
{
	// Destroy the Window
	glfwDestroyWindow(wnd);
	wnd = nullptr;
	
	// Quit GLFW
	glfwTerminate();
}
