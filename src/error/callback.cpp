#include "./callback.hpp"

#include <iostream>

void errorCallback(WGPUErrorType type, const char* message, void* userData)
{
	switch (type)
	{
	case WGPUErrorType_NoError:
		std::cout << "No Error" << std::endl;
		break;
	case WGPUErrorType_Validation:
		std::cerr << "[Validation Error]: " << message << std::endl;
		break;
	case WGPUErrorType_OutOfMemory:
		std::cerr << "[Out of Memory Error]: " << message << std::endl;
		break;
	case WGPUErrorType_Internal:
		std::cerr << "[Internal Error]: " << message << std::endl;
		break;
	case WGPUErrorType_Unknown:
		std::cerr << "[Unknown Error]: " << message << std::endl;
		break;
	case WGPUErrorFilter_Force32:
		std::cerr << "[Force32 Error]: " << message << std::endl;
		break;
	case WGPUErrorType_DeviceLost:
		std::cerr << "[DeviceLost Error]: " << message << std::endl;
		break;
	}
}

void deviceLostCallback(WGPUDeviceLostReason reason, char const* message, void* /* pUserData */)
{
	// A function that is invoked whenever the device stops being available.
	std::cout << "Device lost: reason " << reason;
	if (message) std::cout << " (" << message << ")";
	std::cout << std::endl;
}

void onDeviceError(WGPUErrorType type, char const* message, void* /* pUserData */)
{
	std::cout << "Uncaptured device error: type " << type;
    if (message) std::cout << " (" << message << ")";
    std::cout << std::endl;
}

void onQueueWorkDone(WGPUQueueWorkDoneStatus status, void* /*pUserData*/) {
    std::cout << "Queued work finished with status: " << status << std::endl;
}
