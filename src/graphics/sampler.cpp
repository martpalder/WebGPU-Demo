#include "./sampler.hpp"
#include "./myassert.hpp"

 WGPUSampler createSampler(const WGPUDevice& device)
{
	WGPUSampler sampler;
	
	// Define the Sampler
	WGPUSamplerDescriptor samplerDesc = {};
	samplerDesc.nextInChain = nullptr;
	samplerDesc.label = "Sampler";
	samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
	samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
	samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
	samplerDesc.magFilter = WGPUFilterMode_Nearest;
	samplerDesc.minFilter = WGPUFilterMode_Nearest;
	samplerDesc.lodMinClamp = 0.0f;
	samplerDesc.lodMaxClamp = 0.0f;
	samplerDesc.compare = WGPUCompareFunction_Less;
	samplerDesc.maxAnisotropy = 1;
	
	// Create the Sampler
	pushError(device);
	sampler = wgpuDeviceCreateSampler(device, &samplerDesc);
	popError(device);
	
	// Check for Error
	if (sampler == nullptr)
	{
		perror("[ERROR]: Failed to create a Sampler");
		exit(-1);
	}
	else
	{
		puts("Created a Sampler");
	}
	
	// Assert
	MY_ASSERT(sampler != nullptr);
	
	return sampler;
}
