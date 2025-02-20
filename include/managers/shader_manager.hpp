#pragma once
#ifndef SHADER_MANAGER_HPP_INCLUDED
#define SHADER_MANAGER_HPP_INCLUDED

#include "./stdafx.h"

#include <map>

using namespace std;

class ShaderManager
{
private:
	// Constants
	constexpr static char SHADER_DIR[] = "./data/shaders/%s";
	
	// Members
	map<const std::string, WGPUShaderModule> m_shaderMap;
	
    // Methods
    void Release();
    void CreateMod(const WGPUDevice& device, const char* shaderCode);

public:
    // Constructor/Destructor
    ShaderManager();
    ~ShaderManager();

    // Getters
    const WGPUShaderModule& Get(const std::string& key);

    // Methods
    void Load(WGPUDevice device, const char* fileName);
};

#endif	// SHADER_MANAGER_HPP_INCLUDED
