#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform samplerCube SkyBoxTexture;
uniform vec3 ViewPos;

void main()
{
	float ratio = 1.0/1.52;
	vec3 ViewDir = normalize(FragPos - ViewPos);
	vec3 ViewDir_Refract = refract(ViewDir,normalize(Normal),ratio);
	FragColor = vec4(texture(SkyBoxTexture,ViewDir_Refract).rgb,1.0f);
}