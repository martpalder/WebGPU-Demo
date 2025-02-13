#include "./bind.hpp"
#include "./desc.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

#include <cstdio>

WGPUBindGroupEntry createBinding(size_t bufferSz, const WGPUBuffer& buffer)
{
	// Create a binding
	WGPUBindGroupEntry binding{};
	binding.nextInChain = nullptr;
	// The index of the binding (the entries in bindGroupDesc can be in any order)
	binding.binding = 0;
	// The buffer it is actually bound to
	binding.buffer = buffer;
	// We can specify an offset within the buffer, so that a single buffer can hold
	// multiple uniform blocks.
	binding.offset = 0;
	// And we specify again the size of the buffer.
	binding.size = bufferSz;
	puts("Created a Binding");
	
	return binding;
}

WGPUBindGroup createBindGroup(const WGPUDevice& device,
const WGPUBindGroupLayout& bindGroupLayout, WGPUBindGroupEntry* pBinding)
{
	WGPUBindGroup bindGroup = nullptr;
	
	// Create a Bind Group Descriptor
	WGPUBindGroupDescriptor bindGroupDesc = createBindGroupDesc(bindGroupLayout, pBinding);
	
	// Create a Bind Group
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	bindGroup = wgpuDeviceCreateBindGroup(device, &bindGroupDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// Check for Errors
	if (bindGroup == nullptr)
	{
		perror("[ERROR]: Failed to create a Bind Group");
	}
	else
	{
		puts("Created a Bind Group");
	}
	
	// Assert
	MY_ASSERT(bindGroup != nullptr);
	
	return bindGroup;
}
