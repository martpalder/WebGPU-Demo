#pragma once
#ifndef INSPECT_HPP_INCLUDED
#define INSPECT_HPP_INCLUDED

#include <webgpu/webgpu.h>

void inspectAdapter(const WGPUAdapter& adapter);
void inspectDevice(const WGPUDevice& device);

#endif	// INSPECT_HPP_INCLUDED
