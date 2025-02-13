#pragma once
#ifndef VIEW_HPP_INCLUDED
#define VIEW_HPP_INCLUDED

#include <webgpu/webgpu.h>

void getNextTargetView(const WGPUSurface& surf, WGPUTextureView* pTargetView);

#endif	// VIEW_HPP_INCLUDED
