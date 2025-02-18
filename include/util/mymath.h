#pragma once
#ifndef MYMATH_H_INCLUDED
#define MYMATH_H_INCLUDED

#include "./stdafx.h"

inline float clampf(float val, float vMin, float vMax)
{
	val = (val < vMin) ? vMin : val;
	val = (val > vMax) ? vMax : val;
	
	return val;
}

inline void setVec2(void* dst, const void* src)
{
	memcpy(dst, src, sizeof(vec2));
}

inline void calcMoveDir(float h, float v, float yaw, vec2& moveDir)
{
	// Calculate the Move Vectors
	// Forward
	vec2 moveF;
	moveF[0] = sin(yaw);
	moveF[1] = -cos(yaw);
	vec2_scale(moveF, moveF, v);
	// Right
	vec2 moveR;
	moveR[0] = sin(yaw + 1.57f);
	moveR[1] = -cos(yaw + 1.57f);
	vec2_scale(moveR, moveR, h);
	
	// Calculate the Move Direction
	vec2_add(moveDir, moveF, moveR);
	vec2_norm(moveDir, moveDir);
}

#endif	// MYMATH_H_INCLUDED
