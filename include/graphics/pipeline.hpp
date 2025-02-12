#pragma once
#ifndef PIPELINE_HPP_INCLUDED
#define PIPELINE_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPURenderPipeline createRenderPipeline(const WGPUDevice& device, const WGPUShaderModule& shaderMod,
bool bCreateLayout);

#endif  // PIPELINE_HPP_INCLUDED
