#pragma once
#ifndef MYASSERT_HPP_INCLUDED
#define MYASSERT_HPP_INCLUDED

#include <iostream>

inline void my_assert(bool condition, const char* function, const char* file, int line)
{
    if (condition == false)
	{
        std::cerr << "Assertion failed: '" << function << "' at '" << file << "':'" << line << std::endl;
        std::abort();
    }
}

#define MY_ASSERT(condition) my_assert(condition, __FUNCTION__, __FILE__, __LINE__)

#endif	// MYASSERT_HPP_INCLUDED
