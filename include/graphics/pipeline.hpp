#pragma once
#ifndef PIPELINE_HPP_INCLUDED
#define PIPELINE_HPP_INCLUDED

#include "./gpuenv.hpp"
#include "./state.hpp"
#include "./stdafx.h"

WGPURenderPipeline createRenderPipeline(const GPUEnv& gpuEnv,
WGPURenderPipelineDescriptor& pipelineDesc);

#endif  // PIPELINE_HPP_INCLUDED
