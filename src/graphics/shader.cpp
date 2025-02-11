#include "./shader.hpp"
#include "./desc.hpp"
#include "./callback.hpp"

#include <iostream>

const char* SHADER_CODE = R"(
	@vertex
	fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f
	{
		var p = vec2f(0.0, 0.0);
		if (in_vertex_index == 0u) {
			p = vec2f(-0.5, -0.5);
		} else if (in_vertex_index == 1u) {
			p = vec2f(0.5, -0.5);
		} else {
			p = vec2f(0.0, 0.5);
		}
		return vec4f(p, 0.0, 1.0);
	}

	@fragment
	fn fs_main() -> @location(0) vec4f
	{
		return vec4f(0.0, 0.4, 1.0, 1.0);
	}
)";

WGPUShaderModule createShaderMod(WGPUDevice device)
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
