#version 330 core
layout (triangles) in;
layout (triangle_strip,max_vertices = 3) out;

in vShader_Out{
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
}gShader_In[];

out vec2 TexCoord;	
out vec3 Normal;
out vec3 FragPos;

uniform float PassedTime;

//计算三角形图元的法向量
vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);//此处三角形图元顶点顺序为顺时针排列
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a,b));
}

//获取爆炸后的顶点位置
vec4 GetExplodedPos(vec4 position,vec3 normal)
{
	float ExplosionSpeed = 2.0f;
	vec3 Movement = normal * ((sin(PassedTime) + 1.0f) / 2.0f) * ExplosionSpeed;
	return position + vec4(Movement,0.0f);
}

void main()
{
	vec3 normal = GetNormal();

	//第一个顶点
	gl_Position = GetExplodedPos(gl_in[0].gl_Position,normal);
	TexCoord = gShader_In[0].TexCoord;
	Normal = gShader_In[0].Normal;
	FragPos = gShader_In[0].FragPos;
	EmitVertex();
	//第二个顶点
	gl_Position = GetExplodedPos(gl_in[1].gl_Position,normal);
	TexCoord = gShader_In[1].TexCoord;
	Normal = gShader_In[1].Normal;
	FragPos = gShader_In[1].FragPos;
	EmitVertex();
	//第三个顶点
	gl_Position = GetExplodedPos(gl_in[2].gl_Position,normal);
	TexCoord = gShader_In[2].TexCoord;
	Normal = gShader_In[2].Normal;
	FragPos = gShader_In[2].FragPos;
	EmitVertex();

	EndPrimitive();
}