#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform samplerCube SkyBoxTexture;
uniform vec3 ViewPos;

void main()
{
	vec3 ViewDir = normalize(FragPos - ViewPos);
	vec3 ViewDir_Reflect = reflect(ViewDir,normalize(Normal));
	FragColor = vec4(texture(SkyBoxTexture,ViewDir_Reflect).rgb,1.0f);
}