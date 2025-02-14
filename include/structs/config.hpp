#pragma once
#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUSurfaceConfiguration createSurfConfig(int w, int h,
const WGPUDevice& device);

#endif	// CONFIG_HPP_INCLUDED
