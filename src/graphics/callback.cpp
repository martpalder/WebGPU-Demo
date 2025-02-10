#include "./callback.hpp"

#include <iostream>

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
