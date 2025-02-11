#pragma once
#ifndef PIPELINE_HPP_INCLUDED
#define PIPELINE_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPURenderPipeline createRenderPipeline(WGPUDevice device, const WGPUShaderModule& shaderMod);

#endif  // PIPELINE_HPP_INCLUDED
