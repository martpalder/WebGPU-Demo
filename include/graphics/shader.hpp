#pragma once
#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <webgpu/webgpu.h>

class Shader
{
private:
    // Methods
    void Release();

public:
    // Members
    WGPUShaderModule m_shaderMod;

    // Constructor/Destructor
    Shader();
    ~Shader();

    // Getters
    WGPUShaderModule& GetShaderMod();

    // Methods
    void Load(WGPUDevice device, const char* fileName);
};

WGPUShaderModule createShaderMod(WGPUDevice device, const char* SHADER_CODE);

#endif	// SHADER_HPP_INCLUDED
