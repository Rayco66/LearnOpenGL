#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aInstanceModelMatrix;//使用实例化数组

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model_rotate;//单独传递model矩阵中的旋转分量
uniform mat3 NormalMatrix;
uniform vec3 Offsets[100];//如果要使用Offsets数组的话需要靠gl_InstanceID进行索引:Offsets[gl_InstanceID]

void main()
{
    mat4 InstanceModelMatrix = aInstanceModelMatrix * model_rotate;//注意这里是将model_rotate右乘到aInstanceModelMatrix上，这样才是先位移缩放后旋转
    gl_Position = projection * view * InstanceModelMatrix * vec4(aPos, 1.0);//这里采用了实例化数组传递modelmatrix
    TexCoord = aTexCoord;    
    FragPos = vec3(model * vec4(aPos,1.0f));
    Normal = NormalMatrix * aNormal;
}