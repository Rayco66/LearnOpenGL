#version 330 core

in vec3 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform samplerCube EnvironmentCubeMap;
uniform vec3 ViewPos;

void main()
{
	FragColor = texture(EnvironmentCubeMap,TexCoord);
}