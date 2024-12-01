#version 330 core

in vec3 TexCoord;
out vec4 FragColor;

uniform samplerCube SkyBoxCMTexture;

void main()
{
	FragColor = texture(SkyBoxCMTexture,TexCoord);
}