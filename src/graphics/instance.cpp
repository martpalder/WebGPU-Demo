#include "./instance.hpp"

#include <iostream>
#include <cassert>

WGPUInstance createInstance()
{
	WGPUInstance instance;
	
	// CREATE
	// We create a descriptor
	WGPUInstanceDescriptor desc = {};
	desc.nextInChain = nullptr;

	// We create the instance using this descriptor
	#ifdef WEBGPU_BACKEND_EMSCRIPTEN
	instance = wgpuCreateInstance(nullptr);
	#else //  WEBGPU_BACKEND_EMSCRIPTEN
	instance = wgpuCreateInstance(&desc);
	#endif //  WEBGPU_BACKEND_EMSCRIPTEN
	
	// CHECK
    // We can check whether there is actually an instance created
	if (!instance) {
		std::cerr << "Could not initialize WebGPU!" << std::endl;
	}
	else {
		// Display the object (WGPUInstance is a simple pointer, it may be
		// copied around without worrying about its size).
		std::cout << "WGPU instance: " << instance << std::endl;
	}
	
	return instance;
}
