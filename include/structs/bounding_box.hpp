#pragma once
#ifndef BOUNDING_BOX_HPP_INCLUDED
#define BOUNDING_BOX_HPP_INCLUDED

#include "./stdafx.h"

typedef struct {
	vec3 posMin;
	vec3 posMax;
} BoundingBox;

inline void updateBoundingBox(BoundingBox& box, const vec3& pos, const vec3& radius)
{
	vec3_sub(box.posMin, pos, radius);
	vec3_add(box.posMax, pos, radius);
}

inline bool intersects(const BoundingBox& b1, const BoundingBox& b2)
{
	return (b1.posMin[0] < b2.posMax[0] && b1.posMax[0] > b2.posMin[0]
		&& b1.posMin[1] < b2.posMax[1] && b1.posMax[1] > b2.posMin[1]
		&& b1.posMin[2] < b2.posMax[2] && b1.posMax[2] > b2.posMin[2]
	);
}

#endif	// BOUNDING_BOX_HPP_INCLUDED
