#pragma once
#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include "./gpuenv.hpp"
#include "./mesh.hpp"
#include "./freader.hpp"
#include "./string_utils.hpp"

typedef struct {
	float val;
	size_t span;
} ExtFloat;

typedef struct {
	uint16_t val;
	size_t span;
} ExtUInt16;

Mesh* loadOBJ(const GPUEnv& gpuEnv, const char* path);

inline ExtFloat extractFloat(char* el, char* start)
{
	ExtFloat ext;
	
	// Get an Element Span
	ext.span = strcspn(start, " ");
	// Copy the Element String
	copyLineN(el, start, ext.span);
	// Convert to Float
	ext.val = atof(el);
	
	return ext;
}

inline ExtUInt16 extractUInt16(char* el, char* start)
{
	ExtUInt16 ext;
	
	// Get an Element Span
	ext.span = strcspn(start, " ");
	// Copy the Element String
	copyLineN(el, start, ext.span);
	// Convert to UInt16
	ext.val = (uint16_t)atol(el);
	
	return ext;
}

#endif	// OBJ_LOADER_HPP
