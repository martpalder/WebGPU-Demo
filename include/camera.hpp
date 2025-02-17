#pragma once
#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <linmath.h>

class Camera
{
private:
	vec3 m_pos;
	mat4x4 m_view;

public:
	// Constructor
	Camera();
	
	void SetPos(float x, float y, float z);
};

#endif	// CAMERA_HPP_INCLUDED
