#include "./texture_manager.hpp"
#include "./texture_loader.hpp"
#include "./string_utils.hpp"
#include "./myassert.hpp"

TextureManager::TextureManager()
{}

TextureManager::~TextureManager()
{
	for (pair<const char*, WGPUImageCopyTexture> keyVal : m_texMap)
	{
		wgpuTextureRelease(keyVal.second.texture);
		keyVal.second.texture = nullptr;
	}
	puts("Released the Texture");
	
	m_texMap.clear();
}

WGPUImageCopyTexture TextureManager::Get(const char* key)
{
	return m_texMap[key];
}

WGPUImageCopyTexture TextureManager::Load(const GPUEnv& gpuEnv, const char* fileName)
{
	WGPUImageCopyTexture copyTexture = {};
	
	// Set the Filepath
	char path[48];
	sprintf(path, "./data/textures/%s", fileName);
	
	// Get the File Extension
	const char* fileExt = strrchr(fileName, '.');
	
	// Check the File Extension
	bool bImage = false;
	if (compareStrings(fileExt, ".png"))
	{
		bImage = true;
	}
	else if (compareStrings(fileExt, ".jpg"))
	{
		bImage = true;
	}
	else if (compareStrings(fileExt, ".bmp"))
	{
		bImage = true;
	}
	
	// If is an Image Format
	if (bImage)
	{
		// Load the Texture
		copyTexture = loadTexture(gpuEnv, fileName);
		
		// Check if loaded
		if (copyTexture.texture != nullptr)
		{
			// Add to Texture Map
			m_texMap[fileName] = copyTexture;
			printf("Loaded a Texture: '%s'\n", fileName);
		}
		else
		{
			perror("[ERROR]: Failed to load a Texture");
			exit(-1);
		}
	}
	
	// Assert
	MY_ASSERT(copyTexture.texture != nullptr);
	
	return copyTexture;
}
