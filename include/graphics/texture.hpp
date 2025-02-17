#pragma once
#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUTexture createDepthTexture(const WGPUDevice& device, uint32_t w, uint32_t h);

#endif	// TEXTURE_HPP_INCLUDED
