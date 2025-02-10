#include "./window.hpp"

GLFWwindow* createWindow()
{
	GLFWwindow* wnd;
	
	// Initialize GLFW
	if (!glfwInit())
	{
		// Initialization failed
		return nullptr;
	}
	
	// Set Window Hints
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // <-- extra info for glfwCreateWindow
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// Create the window
	wnd = glfwCreateWindow(640, 480, "Learn WebGPU", nullptr, nullptr);
	
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
