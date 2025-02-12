#include "./bind.hpp"
#include "./desc.hpp"
#include "./layout.hpp"
#include "./callback.hpp"

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
	
	return binding;
}

WGPUBindGroup createBindGroup(const WGPUDevice& device,
size_t bufferSz, const WGPUBuffer& buffer)
{
	WGPUBindGroup bindGroup = nullptr;
	
	// Create a Binding
	WGPUBindGroupEntry binding = createBinding(bufferSz, buffer);

	// Create Layouts
	WGPUBindGroupLayoutEntry bindingLayout = createLayoutBinding();
	WGPUBindGroupLayout bindGroupLayout = createLayoutBindGroup(device, bindingLayout);
	
	// Create a Bind Group
	WGPUBindGroupDescriptor bindGroupDesc = createBindGroupDesc(bindGroupLayout, binding);
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	//bindGroup = wgpuDeviceCreateBindGroup(device, &bindGroupDesc);
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
	
	return bindGroup;
}
