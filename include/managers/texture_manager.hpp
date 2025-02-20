#pragma once
#ifndef TEXTURE_MANAGER_HPP_INCLUDED
#define TEXTURE_MANAGER_HPP_INCLUDED

#include "./env/gpuenv.hpp"
#include "./stdafx.h"

#include <map>

using namespace std;

class TextureManager
{
private:
	// Constants
	constexpr static char TEXTURE_DIR[] = "./data/textures/%s";
	
	// Members
	std::map<const char*, WGPUImageCopyTexture> m_texMap;

public:
	// Constructor/Destructor
	TextureManager();
	~TextureManager();
	
	//  Methods
	WGPUImageCopyTexture Get(const char* key);
	WGPUImageCopyTexture Load(const GPUEnv& gpuEnv, const char* fileName);
};

#endif	// TEXTURE_MANAGER_HPP_INCLUDED
