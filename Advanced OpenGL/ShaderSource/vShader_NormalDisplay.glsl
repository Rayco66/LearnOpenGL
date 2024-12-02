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
	gl_Position = view * model * vec4(aPos,1.0f);//在几何着色器中为了在观察空间中计算相关顶点位置，我们只向几何着色器中传递观察空间中的坐标	
}