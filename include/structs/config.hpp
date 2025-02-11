#pragma once
#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUSurfaceConfiguration createSurfConfig(const WGPUDevice& device);

#endif	// CONFIG_HPP_INCLUDED
