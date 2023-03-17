#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
    ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode,fragmentCode);

}

std::string Shader::ReadFile(const char* fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if(!fileStream.is_open())
    {
	    std::cout<<"Failed to read "<<fileLocation <<" File doesn't exist."<<std::endl;
        return " ";
    }

    std::string line = " ";
    while (!fileStream.eof())
    {
	    std::getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

unsigned int  Shader::GetProjectionLocation()
{
    return uniformProjection;
}

unsigned int Shader::GetModelLocation()
{
    return uniformModel;
}

unsigned Shader::GetViewLocation()
{
    return uniformView;
}

unsigned Shader::GetAmbientIntensityLocation()
{
    return uniformAmbientIntensity;
}

unsigned Shader::GetAmbientColourLocation()
{
    return uniformAmbientColour;
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if(shaderID != 0)
    {
	    glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();         // Create a Program for Shader 
    if(!shaderID)
    {
        std::cout<<"Error Creating Shader Program!\n";
        return;
    }
    
    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);     // Add vertex Shader to Shader Program
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);   // Add fragment Shader to Shader Program 

    // Validate program
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout<<" Error Linking Program: \n"<<eLog<<std::endl;
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout<<" Error Validating Program: \n"<<eLog<<std::endl;
        return;
    }

    // Bind Uniform Variables 
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformAmbientColour = glGetUniformLocation(shaderID, "directionalLight.colour");
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
}

void Shader::AddShader(unsigned theProgram, const char* shaderCode, GLenum shaderType)
{
     GLuint theShader = glCreateShader(shaderType);  // create Shader 

    const GLchar* theCode[1];       // Grab The shader Code 
    theCode[0] = shaderCode;        // Get shader Code Length 

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);      // Put that into Shader
    glCompileShader(theShader);     // Compile Shader 

    // Validate Shader 
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(theShader, sizeof(eLog), nullptr, eLog);
        std::cout<<" Error Compiling the "<< shaderType <<" : \n"<<eLog<<std::endl;
        return;
    }
    glAttachShader(theProgram, theShader);
}


