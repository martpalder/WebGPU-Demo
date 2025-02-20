#pragma once
#ifndef MYASSERT_HPP_INCLUDED
#define MYASSERT_HPP_INCLUDED

#include "./callback.hpp"
#include "./stdafx.h"

inline void my_assert(bool condition, const char* function, const char* file, int line)
{
    if (condition == false)
	{
        std::cerr << "Assertion failed: '" << function << "' at '" << file << "':'" << line << std::endl;
        std::abort();
    }
}

inline void pushError(const WGPUDevice& device)
{
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
}

inline void popError(const WGPUDevice& device)
{
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
}

#define MY_ASSERT(condition) my_assert(condition, __FUNCTION__, __FILE__, __LINE__)

#endif	// MYASSERT_HPP_INCLUDED
