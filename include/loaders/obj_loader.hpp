#pragma once
#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include "./mesh.hpp"
#include "./string_utils.hpp"
#include "./stdafx.h"

Mesh* loadOBJ(const GPUEnv& gpuEnv, const char* path);

inline float extractFloat(char* el, char** pStart)
{
	// Get the Element Span
	size_t span = strcspn(*pStart, " ");
	// Copy the Element String
	copyLineN(el, *pStart, span);
	// Change the Start Location
	*pStart += span + 1;
	
	// Convert to Float
	return atof(el);
}

inline float extractUInt16(char* el, char** pStart)
{
	// Get the Element Span
	size_t span = strcspn(*pStart, " ");
	// Copy the Element String
	copyLineN(el, *pStart, span);
	// Change the Start Location
	*pStart += span + 1;
	
	// Convert to UInt16
	return (uint16_t)atol(el);
}

#endif	// OBJ_LOADER_HPP

