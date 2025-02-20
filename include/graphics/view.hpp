#pragma once
#ifndef VIEW_HPP_INCLUDED
#define VIEW_HPP_INCLUDED

#include "./stdafx.h"

void getNextTargetView(const WGPUSurface& surf, WGPUTextureView* pTargetView);
WGPUTextureView createDepthView(const WGPUTexture& depthTexture);

#endif	// VIEW_HPP_INCLUDED
