#pragma once
#ifndef PIPELINE_HPP_INCLUDED
#define PIPELINE_HPP_INCLUDED

#include "./stdafx.h"

WGPURenderPipeline createRenderPipeline(const WGPUDevice& device,
WGPURenderPipelineDescriptor& pipelineDesc);

#endif  // PIPELINE_HPP_INCLUDED
