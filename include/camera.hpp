#pragma once
#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include "./actor.hpp"
#include "./stdafx.h"

# define M_PI 3.14159265358979323846f  /* pi */

class Camera
{
private:
	// Constants
	const vec3 VEC3_UP = { 0, 1, 0 };
	const float PITCH_MIN = -85.0f * M_PI / 180;
	const float PITCH_MAX = -5.0f * M_PI / 180;
	
	// Members
	float m_dist, m_rotSpeed;
	float m_pitch, m_yaw;
	vec3 m_pos, m_dirF;
	float m_center[3];
	mat4x4 m_v;
	
	// Components
	Actor* m_pParent;

public:
	// Constructor
	Camera();
	
	// Getters
	float GetYaw();
	vec3& GetForward();
	mat4x4& GetView();
	
	// Setters
	void SetPos(float x, float y, float z);
	void SetParent(Actor* pActor);
	
	// Methods
	void Orbit(const vec2& mDelta);
	void Update();
};

#endif	// CAMERA_HPP_INCLUDED
