#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 TexCoord;//纹理坐标
out vec3 Normal;//法线方向
out vec3 FragPos;//图形片段的位置

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 NormalMatrix;

void main() 
{
    gl_Position = projection * view * model * vec4(aPos,1.0f);//顶点着色器需要将这些顶点数据转换到裁剪空间中，以便后续的图形处理步骤,故这里使用的是vec4

    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos,1.0f));//在世界坐标系中进行光照计算，故将fragPos转换到世界系中

    Normal = NormalMatrix * aNormal;//因为顶点数组中的aNormal都是固定值，但图形一直在旋转，故法向量也应该跟随图形旋转
}