#version 330 core
out vec4 FragColor;

uniform vec3 FrameColor;

void main()
{
	FragColor = vec4(FrameColor,1.0f);
}