#pragma once
#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include "./env/gpuenv.hpp"
#include "./stdafx.h"

#include <cmath> // For std::log2 and std::floor

WGPUTexture createDepthTexture(const WGPUDevice& device, uint32_t w, uint32_t h,
WGPUTextureFormat format);

// Function to calculate mip level count
inline uint32_t calculateMipLevelCount(uint32_t width, uint32_t height) {
    return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
}

#endif	// TEXTURE_HPP_INCLUDED
