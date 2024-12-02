#version 330 core
layout (triangles) in;
layout (line_strip,max_vertices = 2) out;

in vShader_Out{
	vec3 Normal;
}gShader_In[];

uniform mat4 projection;
uniform float NormalLength;

//��������
void CreateNormal(int index)
{
	gl_Position = projection * gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = projection * (gl_in[index].gl_Position + vec4(gShader_In[index].Normal,0.0f) * NormalLength);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	//�ֱ�������ε�ÿ�����㴴������
	CreateNormal(0);
	CreateNormal(1);
	CreateNormal(2);
}