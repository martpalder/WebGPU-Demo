#pragma once
#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUTextureView getNextTextureView(WGPUSurface surf);

#endif	// TEXTURE_HPP_INCLUDED
