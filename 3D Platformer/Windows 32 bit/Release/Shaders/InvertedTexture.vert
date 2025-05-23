#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinates;

out vec2 texCoords;

uniform mat4 modelMatrix, projectionMatrix, viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	texCoords = textureCoordinates;
}