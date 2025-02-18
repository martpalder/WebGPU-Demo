#pragma once
#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include "./actor.hpp"
#include "./stdafx.h"

const static vec3 VEC3_UP = { 0, 1, 0 };

class Camera
{
private:
	// Members
	float m_dist, m_deg;
	vec3 m_pos, m_dirF;
	float m_center[3];
	mat4x4 m_v;
	
	// Components
	Actor* m_pParent;

public:
	// Constructor
	Camera();
	
	// Getters
	mat4x4& GetView();
	
	// Setters
	void SetPos(float x, float y, float z);
	void SetParent(Actor* pActor);
	
	// Methods
	void Orbit(float y);
	void Update();
};

#endif	// CAMERA_HPP_INCLUDED
