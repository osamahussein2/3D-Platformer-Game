#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Include the GLM header files (OpenGL Mathematics Library)
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <array>

using namespace std;
using namespace glm;

class Shader
{
public:
	Shader();
	~Shader();

	void InitializeShader(const char* vertexSource, const char* fragmentSource);

	unsigned int shaderProgram;

private:
	unsigned int vertexShader, fragmentShader;

	void InitializeVertexShader(const char* vertexSource_);
	void InitializeFragmentShader(const char* fragmentSource_);

	void PrintVertexShaderErrors();
	void PrintFragmentShaderErrors();
};

#endif