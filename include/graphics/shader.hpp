#pragma once
#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <webgpu/webgpu.h>

class Shader
{
private:
	// Constants
	constexpr static char SHADER_DIR[] = "./data/shaders/%s";
	
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

WGPUShaderModule createShaderMod(WGPUDevice device, const char* shaderCode);

#endif	// SHADER_HPP_INCLUDED
