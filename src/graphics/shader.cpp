#include "./shader.hpp"
#include "./desc.hpp"
#include "./callback.hpp"
#include "./freader.hpp"

#include <iostream>
#include <cstring>

Shader::Shader()
{
	m_shaderMod = nullptr;
}

Shader::~Shader()
{
	this->Release();
}

WGPUShaderModule& Shader::GetShaderMod()
{
	return m_shaderMod;
}

void Shader::Release()
{
	// Release the Shader Module
	wgpuShaderModuleRelease(m_shaderMod);
	m_shaderMod = nullptr;
	puts("Released the Shader Module");
}

void Shader::Load(WGPUDevice device, const char* fileName)
{
	// Release the previous Module
	if (m_shaderMod != nullptr)
	{
		this->Release();
	}
	
	// Set the Filepath
	char path[48];
	sprintf(path, SHADER_DIR, fileName);
	
	// Read the Code and create the Shader Module
	const char* code = readFileText(path);
	m_shaderMod = createShaderMod(device, code);

	// Release the Code
	free((void*)code);
	code = NULL;
	
	if (m_shaderMod != nullptr)
	{
		printf("Loaded a Shader: '%s'\n", fileName);
	}
}

WGPUShaderModule createShaderMod(WGPUDevice device, const char* shaderCode)
{
	WGPUShaderModule shaderMod;

	// Shader Code Description
	WGPUShaderModuleWGSLDescriptor shaderCodeDesc = createShaderCodeDesc(shaderCode);

	// Shader Module Description
	WGPUShaderModuleDescriptor shaderModDesc = createShaderModDesc(&shaderCodeDesc.chain);
	
	// Create a Shader Module
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	shaderMod = wgpuDeviceCreateShaderModule(device, &shaderModDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);

	// Check for Error
	if (shaderMod == nullptr)
	{
		std::cerr << "[ERROR]: Failed to Create a Shader Module" << std::endl;
	}
	else
	{
		std::cout << "Created a Shader Module" << std::endl;
	}

	return shaderMod;
}
