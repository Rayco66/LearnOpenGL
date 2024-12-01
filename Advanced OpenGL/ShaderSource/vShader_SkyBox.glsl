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
    gl_Position = pos.xyww;//天空盒的深度值z/w = w/w = 1.0，永远为最远深度值
    TexCoord = vec3(aPos.x,aPos.y,aPos.z);
}