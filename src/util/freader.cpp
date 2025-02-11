#include "./freader.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

const char* readFileText(const char* PATH)
{
	FILE* fp = fopen(PATH, "r");
	if (fp == nullptr)
	{
		char errMsg[64];
		sprintf(errMsg, "[ERROR]: Failed to open File: '%s'", PATH);
		perror(errMsg);
		
		return nullptr;
	}
	
	// Check the File Size
	fseek(fp, 0, SEEK_END);
	long fSize = ftell(fp);
	rewind(fp);

	// Allocate Memory
	const char* text = (const char*)malloc(fSize);
	if (text == nullptr){ perror("Malloc failed"); }
	// Copy the File Contents
	fread((void*)text, 1, (size_t)fSize, fp);
	
	// Close the File
	fclose(fp);
	fp = nullptr;

	// Print
	//printf("File size: %ld\n", fSize);
	
	return text;
}
