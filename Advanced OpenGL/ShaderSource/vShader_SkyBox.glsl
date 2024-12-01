#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos = projection * view * vec4(aPos,1.0f);
    gl_Position = pos.xyww;//��պе����ֵz/w = w/w = 1.0����ԶΪ��Զ���ֵ
    TexCoord = vec3(aPos.x,aPos.y,aPos.z);
}