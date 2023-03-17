#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <glew.h>

class Shader
{
public:
	Shader();
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation); 

	unsigned int GetProjectionLocation();
	unsigned int GetModelLocation();
	unsigned int GetViewLocation();

	void UseShader();
	void ClearShader();

private:
	unsigned int shaderID, uniformProjection, uniformModel, uniformView;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);

};

