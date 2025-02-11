#include "./shader.hpp"
#include "./desc.hpp"
#include "./callback.hpp"
#include "./freader.hpp"

#include <iostream>

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
	wgpuShaderModuleRelease(m_shaderMod);
	m_shaderMod = nullptr;
	puts("Released the Shader Module");
}

void Shader::Load(WGPUDevice device, const char* PATH)
{
	// Release previous Module
	if (m_shaderMod != nullptr)
	{
		this->Release();
	}

	// Read the Code and create the Shader Module
	const char* code = readFileText(PATH);
	m_shaderMod = createShaderMod(device, code);

	// Release the Code
	free((void*)code);
	code = NULL;
}

WGPUShaderModule createShaderMod(WGPUDevice device, const char* SHADER_CODE)
{
	WGPUShaderModule shaderMod;

	// Shader Code Description
	WGPUShaderModuleWGSLDescriptor shaderCodeDesc = createShaderCodeDesc(SHADER_CODE);

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
