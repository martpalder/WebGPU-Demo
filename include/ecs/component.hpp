#pragma once
#ifndef COMPONENT_HPP_INCLUDED
#define COMPONENT_HPP_INCLUDED

enum ComponentType {
	TRANSFORM,
	MESH
};

class Component
{
public:
	// Constructor
	Component(ComponentType type);
	
	// Methods
	virtual void RunSystem();
};

#endif	// COMPONENT_HPP_INCLUDED
