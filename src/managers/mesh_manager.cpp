#include "./mesh_manager.hpp"
#include "./obj_loader.hpp"
#include "./stdafx.h"

MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{
	// Reöease the Meshes
	for (pair<const std::string, Mesh*> keyVal : m_meshMap)
	{
		delete keyVal.second;
		keyVal.second = nullptr;
	}
	puts("Released the Meshes");
	
	// Clear the Mesh Map
	m_meshMap.clear();
}

Mesh* MeshManager::Get(const std::string& fileName)
{
	return m_meshMap[fileName];
}

Mesh* MeshManager::Load(const GPUEnv& gpuEnv, const char* fileName)
{
	Mesh* pMesh = nullptr;
	
	// Set the Filepath
	char path[48];
	sprintf(path, "./data/models/%s", fileName);
	
	// Get the File Extension
	const char* fileExt = strrchr(fileName, '.');
	
	// Check the File Extension
	if (compareStrings(fileExt, ".obj"))
	{
		// Load an OBJ
		pMesh = loadOBJ(gpuEnv, path);
	}
	
	// Check if loaded
	if (pMesh != nullptr)
	{
		// Add to Mesh Map
		m_meshMap[fileName] = pMesh;
		printf("Loaded a Mesh: '%s'\n", fileName);
		
		return pMesh;
	}
	
	return nullptr;
}
