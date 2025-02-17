#pragma once
#ifndef STRING_UTILS_HPP_INCLUDED
#define STRING_UTILS_HPP_INCLUDED

#include <cstring>

inline void copyLine(char* dst, char* src)
{
	size_t len = strlen(src);
	strcpy(dst, src);
	dst[len] = '\0';
}

inline void copyLineN(char* dst, char* src, size_t len)
{
	strncpy(dst, src, len);
	dst[len] = '\0';
}

inline void printXYZ(float x, float y, float z)
{
	printf("X: %f\tY: %f\tZ: %f\n", x, y, z);
}

#endif	// STRING_UTILS_HPP_INCLUDED
