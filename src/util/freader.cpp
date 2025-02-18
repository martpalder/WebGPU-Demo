#include "./freader.hpp"
#include "./stdafx.h"

char* readFileText(const char* PATH)
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
	fseek(fp, 0, SEEK_SET);

	// Allocate Memory
	char* text = (char*)malloc(fSize);
	if (text == nullptr)
	{
		perror("[ERROR]: Failed to allocate memory");
	}
	
	// Copy the File Contents
	long bytesRead = fread(text, 1, fSize, fp);
	
	// Add a Null Terminator
	text[bytesRead] = '\0';
	
	// Close the File
	fclose(fp);
	fp = nullptr;

	// Print
	//printf("File size: %ld\n", bytesRead);
	
	return text;
}
