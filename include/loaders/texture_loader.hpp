#pragma once
#ifndef TEXTURE_LOADER_HPP_INCLUDED
#define TEXTURE_LOADER_HPP_INCLUDED

#include "./env/gpuenv.hpp"
#include "./stdafx.h"

WGPUImageCopyTexture loadTexture(const GPUEnv& gpuEnv, const char* fileName);

#endif	// TEXTURE_LOADER_HPP_INCLUDED
