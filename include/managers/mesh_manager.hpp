#pragma once
#ifndef MESH_MANAGER_HPP_INCLUDED
#define MESH_MANAGER_HPP_INCLUDED

#include "./mesh.hpp"

#include <vector>

class MeshManager
{
private:
	// Members
	std::vector<Mesh*> m_meshes;

public:
	// Constructor/Destructor
	MeshManager();
	~MeshManager();
	
	// Members
	Mesh* Load(const GPUEnv& gpuEnv, const char* fileName);
};

#endif	// MESH_MANAGER_HPP_INCLUDED
