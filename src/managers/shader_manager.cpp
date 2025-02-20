#include "./shader_manager.hpp"
#include "./desc.hpp"
#include "./freader.hpp"
#include "./myassert.hpp"

ShaderManager::ShaderManager()
{}

ShaderManager::~ShaderManager()
{
	this->Release();
}

const WGPUShaderModule& ShaderManager::Get(const std::string& key)
{
	if (m_shaderMap.count(key) > 0)
	{
		return m_shaderMap[key];
	}
	else
	{
		perror("[ERROR]: No such Module");
		exit(-1);
	}
}

void ShaderManager::Release()
{
	for (pair<const std::string, WGPUShaderModule> keyVal : m_shaderMap)
	{
		// Release the Shader Module
		wgpuShaderModuleRelease(keyVal.second);
		keyVal.second = nullptr;
	}
	puts("Released the Shaders");
	m_shaderMap.clear();
}


void ShaderManager::CreateMod(const WGPUDevice& device, const char* shaderCode)
{
	WGPUShaderModule shaderMod;
	
	// Shader Code Description
	WGPUShaderModuleWGSLDescriptor shaderCodeDesc = createShaderCodeDesc(shaderCode);
	// Shader Module Description
	WGPUShaderModuleDescriptor shaderModDesc = createShaderModDesc(&shaderCodeDesc.chain);
	
	// Create a Shader Module
	pushError(device);
	shaderMod = wgpuDeviceCreateShaderModule(device, &shaderModDesc);
	popError(device);

	// Check for Error
	if (shaderMod == nullptr)
	{
		std::cerr << "[ERROR]: Failed to Create a Shader Module" << std::endl;
		return;
	}
	
	// Assert
	MY_ASSERT(shaderMod != nullptr);
	// Add the Shader Module
	m_shaderMap["shader1"] = shaderMod;
}

void ShaderManager::Load(WGPUDevice device, const char* fileName)
{
	// Set the Filepath
	char path[48];
	sprintf(path, SHADER_DIR, fileName);
	
	// Read the Code and create the Shader Module
	const char* code = readFileText(path);
	this->CreateMod(device, code);

	// Release the Code
	free((void*)code);
	code = NULL;
}
