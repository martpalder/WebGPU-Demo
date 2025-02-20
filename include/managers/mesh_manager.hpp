#pragma once
#ifndef MESH_MANAGER_HPP_INCLUDED
#define MESH_MANAGER_HPP_INCLUDED

#include "./mesh.hpp"

#include <map>

using namespace std;

class MeshManager
{
private:
	// Constants
	constexpr static char MESH_DIR[] = "./data/models/%s";
	
	// Members
	map<const std::string, Mesh*> m_meshMap;

public:
	// Constructor/Destructor
	MeshManager();
	~MeshManager();
	
	// Members
	Mesh* Get(const std::string& fileName);
	Mesh* Load(const GPUEnv& gpuEnv, const char* fileName);
};

#endif	// MESH_MANAGER_HPP_INCLUDED
