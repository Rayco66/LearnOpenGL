#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aModelMatrix;
layout (location = 7) in mat3 aNormalMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;
void main()
{
    TexCoord = aTexCoord;    
    gl_Position = projection * view * aModelMatrix * vec4(aPos, 1.0);
    FragPos = vec3(aModelMatrix * vec4(aPos,1.0f));
    Normal = aNormalMatrix * aNormal;
}
