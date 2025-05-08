#include "PlayerCube.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include "Camera.h"

PlayerCube::PlayerCube(glm::vec3 position_, glm::vec3 size_) : position(position_), size(size_)
{
}

PlayerCube::~PlayerCube()
{
}

void PlayerCube::InitializeMainMenuObject()
{
	cubeVertices =
	{
		// back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
		0.5f, 0.5f, -0.5f, -1.0f, -1.0f, // top-right
		0.5f, -0.5f, -0.5f, -1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f, -1.0f, -1.0f, // top-right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
		-0.5f, 0.5f, -0.5f, 0.0f, -1.0f, // top-left

		// front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f, -1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, 0.5f, -1.0f, -1.0f, // top-right
		0.5f, 0.5f, 0.5f, -1.0f, -1.0f, // top-right
		-0.5f, 0.5f, 0.5f, 0.0f, -1.0f, // top-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left

		// left face
		-0.5f, 0.5f, 0.5f, -1.0f, -1.0f, // top-right
		-0.5f, 0.5f, -0.5f, 0.0f, -1.0f, // top-left
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, 0.5f, -1.0f, -1.0f, // top-right

		// right face
		0.5f, 0.5f, 0.5f, 0.0f, -1.0f, // top-left
		0.5f, -0.5f, -0.5f, -1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f, -1.0f, -1.0f, // top-right
		0.5f, -0.5f, -0.5f, -1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, 0.5f, 0.0f, -1.0f, // top-left
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left

		// bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right

		// top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f // bottom-left
	};

	// Generate a vertex attribute object ID
	glGenVertexArrays(1, &cubeVAO);

	// Generate the buffer ID here
	glGenBuffers(1, &cubeVBO);

	// Bind the vertex array object using its ID
	glBindVertexArray(cubeVAO);

	// This binds the buffers more than once at the same time as long as they're different buffer types
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	// Copies the previously defined vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

	// Set the position attribute's location to 0 like our vertex shader GLSL file
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec2), (void*)0);

	glEnableVertexAttribArray(0); // Position attribute location occurs at 0

	// Set the normals attribute's location to 1 like our vertex shader GLSL file
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec2), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(1); // Normal attribute location occurs at 1
}
void PlayerCube::InitializeMainMenuObjectTextures(const char* file_)
{
	int width, height, nrChannels;

	unsigned char* cubeData = stbi_load(file_, &width, &height, &nrChannels, 0);

	// After the texture ahs been binded, we can generate textures using the previously loaded image data
	// Textures are generated with glTexImage2D
	if (cubeData)
	{
		// Generate the texture in OpenGL first before binding it
		glGenTextures(1, &cubeTexture);

		// Then, we need to bind the textures to configure the currently bound texture on subsequent texture commands
		glBindTexture(GL_TEXTURE_2D, cubeTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format;

		if (nrChannels == 1) format = GL_RED;
		if (nrChannels == 3) format = GL_RGB;
		if (nrChannels == 4) format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, cubeData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	else
	{
		std::cout << "This texture has failed to load!" << std::endl;
	}

	// Free the image memory after generating the texture and its corresponding mipmaps
	stbi_image_free(cubeData);
}

void PlayerCube::DrawMainMenuObject()
{
	glUseProgram(shader.shaderProgram);

	glUniform1i(glGetUniformLocation(shader.shaderProgram, "textureImage"), 0);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(1200 / 900), 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "projectionMatrix"), 1, GL_FALSE,
		glm::value_ptr(projectionMatrix));

	glm::mat4 viewMatrix = glm::lookAt(Camera::cameraPosition, Camera::cameraPosition + Camera::cameraFront,
		Camera::cameraUp);

	//viewMatrix = glm::rotate(viewMatrix, glm::radians(Camera::pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	//viewMatrix = glm::rotate(viewMatrix, glm::radians(Camera::yaw), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix = scale(modelMatrix, size);

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "modelMatrix"), 1,
		GL_FALSE, glm::value_ptr(modelMatrix));

	glBindVertexArray(cubeVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
