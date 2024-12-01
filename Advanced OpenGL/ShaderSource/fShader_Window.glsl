#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

struct Material
{
	sampler2D DiffuseColorSampler1;//��������ͼ���������(���������洢��Ӧ����Ԫ�е���������)
	sampler2D SpecularColorSampler1;//���淴����ͼ���������
};

uniform Material material;


void main()
{    
	vec4 ResultColor = texture(material.DiffuseColorSampler1,TexCoord);
    FragColor = ResultColor;
}
