#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

struct Material
{
	sampler2D DiffuseColorSampler1;//漫反射贴图采样器标号(采样器：存储对应纹理单元中的纹理数据)
	sampler2D SpecularColorSampler1;//镜面反射贴图采样器标号
};

uniform Material material;


void main()
{    
	vec4 ResultColor = texture(material.DiffuseColorSampler1,TexCoord);
	if(ResultColor.a < 0.1f)
	{
		discard;//片段的透明度小于0.1则丢弃
	}

    FragColor = ResultColor;
}
