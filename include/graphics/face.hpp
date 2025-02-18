#pragma once
#ifndef FACE_HPP_INCLUDED
#define FACE_HPP_INCLUDED

typedef struct {
	uint16_t posIdx;
} Face1;

typedef struct {
	uint16_t posIdx, normalIdx, texIdx;
} Face3;

#endif	// FACE_HPP_INCLUDED
