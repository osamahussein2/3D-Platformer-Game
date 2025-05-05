#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader() : shaderProgram(NULL), vertexShader(NULL), fragmentShader(NULL)
{
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::InitializeShader(const char* vertexSource, const char* fragmentSource)
{
	// Create a shader program that will render both the vertex and fragment shaders to the window
	shaderProgram = glCreateProgram();

	InitializeVertexShader(vertexSource);
	InitializeFragmentShader(fragmentSource);

	PrintVertexShaderErrors();
	PrintFragmentShaderErrors();

	// Attach the vertex and fragment shaders to our created shader program ID object
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Link the attached vertex and fragment shaders together into one shader program
	glLinkProgram(shaderProgram);

	int successfullyCompiled; // An integer that checks if the shader program compilation was successful
	char compilationInformationLog[512]; // Gives information about its compilation in a log file (maybe?)

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successfullyCompiled);

	if (!successfullyCompiled)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, compilationInformationLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << compilationInformationLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::InitializeVertexShader(const char* vertexSource_)
{
	std::string vertexShaderCode;
	std::ifstream vertexShaderFile;
	std::stringstream vertexShaderStream;
	const char* vShaderCode;

	// Make sure the vertex shader file ifstream will throw an exception error
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open the vertex shader file
		vertexShaderFile.open(vertexSource_);

		// Read the vertex shader's file buffer contents into a stream of the vertex shader file
		vertexShaderStream << vertexShaderFile.rdbuf();

		// Close the vertex shader's file handlers
		vertexShaderFile.close();

		// Convert the vertex shader stream into a vertex shader string
		vertexShaderCode = vertexShaderStream.str();
	}

	catch (std::ifstream::failure error)
	{
		std::cout << "ERROR::SHADER::VERTEX::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	vShaderCode = vertexShaderCode.c_str();

	// Create a shader object ID to use it when we create the shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a vertex shader using OpenGL's version of it

	// Attach the shader source code (the GLSL one) to the shader object and compile it
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
}

void Shader::InitializeFragmentShader(const char* fragmentSource_)
{
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderFile;
	std::stringstream fragmentShaderStream;
	const char* fShaderCode;

	// Make sure the fragment shader file ifstream will throw an exception error
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open the fragment shader file
		fragmentShaderFile.open(fragmentSource_);

		// Read the fragment shader's file buffer contents into a stream of the fragment shader file
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		// Close the fragment shader's file handlers
		fragmentShaderFile.close();

		// Convert the fragment shader stream into a fragment shader string
		fragmentShaderCode = fragmentShaderStream.str();
	}

	catch (std::ifstream::failure error)
	{
		std::cout << "ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	fShaderCode = fragmentShaderCode.c_str();

	// Compile the fragment shader using the shader object and the shader source code (GLSL)
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
}

void Shader::PrintVertexShaderErrors()
{
	int successfullyCompiled; // An integer that checks if the vertex shader compilation was successful
	char compilationInformationLog[512]; // Gives information about its compilation in a log file (maybe?)

	// Returns a successful compilation of the vertex shader if it can compile successfully
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successfullyCompiled);

	// If the compilation failed, then return a log compilation error and explain the error
	if (!successfullyCompiled)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, compilationInformationLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << compilationInformationLog << std::endl;
	}
}

void Shader::PrintFragmentShaderErrors()
{
	int successfullyCompiled; // An integer that checks if the vertex shader compilation was successful
	char compilationInformationLog[512]; // Gives information about its compilation in a log file (maybe?)

	// Returns a successful compilation of the fragment shader if it can compile successfully
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successfullyCompiled);

	// If the compilation failed, then return a log compilation error and explain the error
	if (!successfullyCompiled)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, compilationInformationLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << compilationInformationLog << std::endl;
	}
}
