#pragma once
#ifndef GPUENV_HPP_INCLUDED
#define GPUENV_HPP_INCLUDED

#include "./stdafx.h"

#include <GLFW/glfw3.h>

typedef struct {
	WGPUInstance inst;
	WGPUSurface surf;
	WGPUDevice dev;
	WGPUQueue queue;
} GPUEnv;

WGPURequestAdapterOptions createAdapterOpts(const WGPUSurface& surf);

GPUEnv initGPUEnv(GLFWwindow* wnd);
void quitGPUEnv(const GPUEnv& gpuEnv);

inline void printRelease(const char* name)
{
	printf("Released the %s\n", name);
}

#endif	// GPUENV_HPP_INCLUDED
