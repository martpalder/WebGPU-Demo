#include "./instance.hpp"
#include "./myassert.hpp"

#include <iostream>
#include <cstdio>

#define WGPU_DEBUG_LEVEL 1

WGPUInstance createInstance()
{
	WGPUInstance instance = nullptr;

	// Set the log level to 'Trace' for detailed logging
	//wgpuSetLog(WGPULogLevel_Trace);
	
	// CREATE
	// We create a descriptor
	WGPUInstanceDescriptor instDesc = {};
	instDesc.nextInChain = nullptr;

	// We create the instance using this descriptor
	#ifdef WEBGPU_BACKEND_EMSCRIPTEN
	instance = wgpuCreateInstance(nullptr);
	#else //  WEBGPU_BACKEND_EMSCRIPTEN
	instance = wgpuCreateInstance(&instDesc);
	#endif //  WEBGPU_BACKEND_EMSCRIPTEN
	
	// CHECK
    // We can check whether there is actually an instance created
	if (instance == nullptr)
	{
		perror("[ERROR]: Could not initialize WebGPU!");
	}
	else
	{
		// Display the object (WGPUInstance is a simple pointer, it may be
		// copied around without worrying about its size).
		std::cout << "WGPU instance: " << instance << std::endl;
	}
	
	// Assert
	MY_ASSERT(instance != nullptr);
	
	return instance;
}
