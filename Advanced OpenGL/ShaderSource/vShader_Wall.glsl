#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 NormalMatrix;

void main() 
{
    gl_Position = projection * view * model * vec4(aPos,1.0f);//������ɫ����Ҫ����Щ��������ת�����ü��ռ��У��Ա������ͼ�δ�����,������ʹ�õ���vec4

    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos,1.0f));
    Normal = NormalMatrix * aNormal;
}
