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

#endif	// STRING_UTILS_HPP_INCLUDED
