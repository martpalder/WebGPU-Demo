#pragma once
#ifndef BUFFER_HPP_INCLUDED
#define BUFFER_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUBuffer createBufferVert(const WGPUDevice& device, const WGPUQueue& queue);

#endif	// BUFFER_HPP_INCLUDED
