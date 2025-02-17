#include "./mesh_manager.hpp"
#include "./obj_loader.hpp"

#include <cstdio>

MeshManager::MeshManager()
{
	puts("Created a Mesh Manager");
}

MeshManager::~MeshManager()
{
	// Clear the Meshes
	m_meshes.clear();
	puts("Cleared the Meshes");
}

Mesh* MeshManager::Load(const GPUEnv& gpuEnv, const char* fileName)
{
	//this->CreateTransform(gpuEnv);
	Mesh* pMesh = nullptr;
	
	// Set the Filepath
	char path[48];
	sprintf(path, "./data/models/%s", fileName);
	
	// Get the File Extension
	const char* fileExt = strrchr(fileName, '.');
	
	// Check the File Extension
	if (strcmp(fileExt, ".obj") == 0)
	{
		// Load an OBJ
		pMesh = loadOBJ(gpuEnv, path);
		printf("Loaded a Mesh: '%s'\n", fileName);
	}
	
	if (pMesh != nullptr)
	{
		m_meshes.push_back(pMesh);
	}
	
	return m_meshes.back();
}
