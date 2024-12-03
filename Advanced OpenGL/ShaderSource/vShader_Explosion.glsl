#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vShader_Out{
    vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
}vShader_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 NormalMatrix;

void main()
{
    vShader_out.TexCoord = aTexCoord;    
    vShader_out.FragPos = vec3(model * vec4(aPos,1.0f));
    //vShader_out.Normal = NormalMatrix * aNormal;
    vShader_out.Normal = mat3(transpose(inverse(view * model))) * aNormal;//因为我们在gShader中使用观察空间下的gl_Positon计算法线，故传给gShader的顶点法线也应该使用观察空间下的
    gl_Position = view * model * vec4(aPos, 1.0);
}
