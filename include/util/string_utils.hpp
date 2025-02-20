#pragma once
#ifndef STRING_UTILS_HPP_INCLUDED
#define STRING_UTILS_HPP_INCLUDED

#include "./stdafx.h"

inline void copyLine(char* dst, char* src)
{
	size_t len = strlen(src);
	memcpy(dst, src, len);
	dst[len] = '\0';
}

inline void copyLineN(char* dst, char* src, size_t len)
{
	memcpy(dst, src, len);
	dst[len] = '\0';
}

inline bool compareStrings(const char* str1, const char* str2)
{
	return  (strcmp(str1, str2) == 0);
}

inline void printVec2(const vec2& vec)
{
	printf("X: %f\tY: %f\n", vec[0], vec[1]);
}

inline void printVec3(const vec3& vec)
{
	printf("X: %f\tY: %f\tZ: %f\n", vec[0], vec[1], vec[2]);
}

#endif	// STRING_UTILS_HPP_INCLUDED
