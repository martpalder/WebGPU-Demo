#pragma once
#ifndef CALLBACK_HPP_INCLUDED
#define CALLBACK_HPP_INCLUDED

#include <webgpu/webgpu.h>

void errorCallback(WGPUErrorType type, const char* message, void* /* pUserData */);
void deviceLostCallback(WGPUDeviceLostReason reason, char const* message, void* /* pUserData */);
void onDeviceError(WGPUErrorType type, char const* message, void* /* pUserData */);
void onQueueWorkDone(WGPUQueueWorkDoneStatus status, void* /*pUserData*/);

#endif	// CALLBACK_HPP_INCLUDED
