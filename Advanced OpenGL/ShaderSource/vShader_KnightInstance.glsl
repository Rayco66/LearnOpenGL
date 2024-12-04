#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aInstanceModelMatrix;//ʹ��ʵ��������

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model_rotate;//��������model�����е���ת����
uniform mat3 NormalMatrix;
uniform vec3 Offsets[100];//���Ҫʹ��Offsets����Ļ���Ҫ��gl_InstanceID��������:Offsets[gl_InstanceID]

void main()
{
    mat4 InstanceModelMatrix = aInstanceModelMatrix * model_rotate;//ע�������ǽ�model_rotate�ҳ˵�aInstanceModelMatrix�ϣ�����������λ�����ź���ת
    gl_Position = projection * view * InstanceModelMatrix * vec4(aPos, 1.0);//���������ʵ�������鴫��modelmatrix
    TexCoord = aTexCoord;    
    FragPos = vec3(model * vec4(aPos,1.0f));
    Normal = NormalMatrix * aNormal;
}