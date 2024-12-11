#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aModelMatrix;
layout (location = 7) in mat3 aNormalMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model_rotation;///自转模型矩阵
uniform mat4 model_revolution;//公转模型矩阵
uniform mat4 view;
uniform mat4 projection;
void main()
{
    mat4 ModelMatrix = model_revolution * aModelMatrix * model_rotation;//矩阵的旋转轴：左乘是世界坐标系中的轴，右乘是自身坐标系中的轴
    gl_Position = projection * view * ModelMatrix * vec4(aPos, 1.0);
    TexCoord = aTexCoord;    
    FragPos = vec3(aModelMatrix * vec4(aPos,1.0f));
    Normal = aNormalMatrix * aNormal;
}
