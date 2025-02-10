#pragma once
#ifndef REQUEST_HPP_INCLUDED
#define REQUEST_HPP_INCLUDED

#include <webgpu/webgpu.h>

void onAdapterRequestEnded(
    WGPURequestAdapterStatus status, // a success status
    WGPUAdapter adapter, // the returned adapter
    char const* message, // error message, or nullptr
    void* userdata // custom user data, as provided when requesting the adapter
);

WGPUAdapter requestAdapterSync(WGPUInstance instance, WGPURequestAdapterOptions const * options);
WGPUDevice requestDeviceSync(WGPUAdapter adapter, WGPUDeviceDescriptor const* descriptor);

#endif	// REQUEST_HPP_INCLUDED
