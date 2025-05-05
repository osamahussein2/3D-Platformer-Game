#version 460 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D textureImage;

vec4 textureColor;

void main()
{
	// Use texture function and make it equal to the fragment color
	fragColor = texture(textureImage, texCoords);
}