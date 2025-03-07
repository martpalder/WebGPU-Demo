#include "./bind.hpp"
#include "./desc.hpp"
#include "./layout.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

#include <linmath.h>
#include <cstdio>

WGPUBindGroupEntry createBinding(int idx, const WGPUBuffer& buffer)
{
	// Create a binding
	WGPUBindGroupEntry binding = {};
	
	binding.nextInChain = nullptr;
	// The index of the binding (the entries in bindGroupDesc can be in any order)
	binding.binding = idx;
	// The buffer it is actually bound to
	binding.buffer = buffer;
	// We can specify an offset within the buffer, so that a single buffer can hold
	// multiple uniform blocks.
	binding.offset = 0;
	// And we specify again the size of the buffer.
	binding.size = wgpuBufferGetSize(buffer);
	
	// Other Stuff
	binding.sampler = nullptr;
	binding.textureView = nullptr;
	puts("Created a Binding");
	
	return binding;
}

WGPUBindGroup createBindGroup(const WGPUDevice& device,
const WGPUBindGroupDescriptor& bindGroupDesc)
{
	WGPUBindGroup bindGroup = nullptr;
	
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
