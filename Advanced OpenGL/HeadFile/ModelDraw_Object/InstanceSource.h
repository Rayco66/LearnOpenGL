#ifndef INSTANCE_SOURCE_H
#define INSTANCE_SOURCE_H
#include <vector>
#include <../HeadFile/Model.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

inline std::vector<glm::vec3> GetOffsets_Knight(int CubeRoot_NUM)//实例数的立方根数
{
	std::vector<glm::vec3> Offsets_Knight;
	int index = 0;//计数器
	float deltaOffset = 10.0f;
	
	for (int x = 0; x < CubeRoot_NUM; x++)
	{
		for (int y = 0; y < CubeRoot_NUM; y++)
		{
			for (int z = 0; z < CubeRoot_NUM; z++)
			{
				glm::vec3 offsets;
				offsets.x = (float)x * deltaOffset;
				offsets.y = (float)y * deltaOffset;
				offsets.z = (float)z * deltaOffset;
				Offsets_Knight.push_back(offsets);
			}
		}
	}
	return Offsets_Knight;
}

inline void InstancedArray_Knight(Model Knight,int InstanceNum)
{
    std::vector<glm::mat4> InstanceModelMatrix;

    for (int i = 0; i < InstanceNum; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(1.0f * i,-1.0f,1.0f));
        model = glm::scale(model,glm::vec3(0.1f));
        InstanceModelMatrix.push_back(model);
    }


    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, InstanceNum * sizeof(glm::mat4), &InstanceModelMatrix[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < Knight.meshes.size(); i++)
    {
        unsigned int VAO = Knight.meshes[i].VAO;
        glBindVertexArray(VAO);
        //由于OpenGL中顶点属性允许的最大数据大小是一个vec4，而我们想传入一个mat4(4个vec4),故需要四个顶点属性
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

#endif


