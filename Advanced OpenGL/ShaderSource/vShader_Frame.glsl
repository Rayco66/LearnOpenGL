#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 NormalMatrix;

uniform float FixRate;//模型上移的修正参数 

void main()
{
    vec3 Normal = NormalMatrix * aNormal;
    gl_Position = projection * view * (model * vec4(aPos,1.0f) + vec4(FixRate * Normal,0.0f));//让轮廓在世界坐标系下沿法线方向移动一定距离，解决模型上移问题
}