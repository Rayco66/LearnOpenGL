#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vShader_Out{
	vec3 Normal;
}vShader_out;

uniform mat4 model;
uniform mat4 view;
uniform mat3 NormalMatrix;

void main()
{
	vShader_out.Normal  = normalize(NormalMatrix * aNormal);
	gl_Position = view * model * vec4(aPos,1.0f);//�ڼ�����ɫ����Ϊ���ڹ۲�ռ��м�����ض���λ�ã�����ֻ�򼸺���ɫ���д��ݹ۲�ռ��е�����	
}