#ifndef MESH_H
#define MESH_H
#include <glad/glad.h> 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

//网格顶点属性
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
//网格纹理数据
struct Texture {
	unsigned int id;//纹理id
	std::string type;//贴图类型（漫反射贴图/镜面反射贴图）
	std::string path;//纹理路径(用于判断纹理是否重复)
};


class Mesh {
public:
	/*渲染数据*/
	unsigned int VAO, VBO, EBO;
	/*网格数据*/
	std::vector<Vertex>	vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	
	Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	//封装了绑定VAO、纹理和画图的功能
	void Draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int emissionNr = 1;
		unsigned int ambientNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
			// 获取纹理序号（DiffuseColorSamplerN 中的 N）
			std::string number;
			std::string name = textures[i].type;
			if (name == "DiffuseColorSampler")
				number = std::to_string(diffuseNr++);
			else if (name == "SpecularColorSampler")
				number = std::to_string(specularNr++);
			else if (name == "EmissionColorSampler")
				number = std::to_string(emissionNr++);
			else if (name == "AmbientColorSampler")
				number = std::to_string(ambientNr++);

			shader.SetInt(("material." + name + number).c_str(), i);//拼接成material.DiffuseColorSampler1、material.DiffuseColorSampler2、material.DiffuseColorSampler3等
			glBindTexture(GL_TEXTURE_2D, textures[i].id);//将该纹理id绑定到激活过的纹理单元上
		}

		// 绘制网格
		glBindVertexArray(VAO);//绘制前仍需绑定VAO，因为需要重新激活顶点属性指针
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	//实例绘制
	void InstanceDraw(Shader& shader,int num)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int emissionNr = 1;
		unsigned int ambientNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
			// 获取纹理序号（DiffuseColorSamplerN 中的 N）
			std::string number;
			std::string name = textures[i].type;
			if (name == "DiffuseColorSampler")
				number = std::to_string(diffuseNr++);
			else if (name == "SpecularColorSampler")
				number = std::to_string(specularNr++);
			else if (name == "EmissionColorSampler")
				number = std::to_string(emissionNr++);
			else if (name == "AmbientColorSampler")
				number = std::to_string(ambientNr++);

			shader.SetInt(("material." + name + number).c_str(), i);//拼接成material.DiffuseColorSampler1、material.DiffuseColorSampler2、material.DiffuseColorSampler3等
			glBindTexture(GL_TEXTURE_2D, textures[i].id);//将该纹理id绑定到激活过的纹理单元上
		}

		// 绘制网格
		glBindVertexArray(VAO);//绘制前仍需绑定VAO，因为需要重新激活顶点属性指针
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0,num);
		glBindVertexArray(0);
	}

private:

	/*函数*/
	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);//生成一个缓冲对象，并将其ID储存在变量VBO中（VBO用于存储顶点数据，将数据从cpu上传到gpu中）
		glGenBuffers(1, &EBO);

		//将指定的 VAO 绑定为当前的 VAO
		glBindVertexArray(VAO);

		//填充VBO数据
		glBindBuffer(GL_ARRAY_BUFFER, VBO);//将创建好的缓冲对象VBO绑定到opengl的绑定点GL_ARRAY_BUFFER上（使得后续的操作（如数据上传、设置顶点属性）都会影响到这个特定的缓冲区对象）
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);//将数据复制到当前绑定的缓冲区中(缓冲目标、缓冲内容大小、缓冲内容、数据管理方式)
		//填充EBO数据
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//将EBO绑定到opengl的绑定点GL_ELEMENT_ARRAY_BUFFER上
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0], GL_STATIC_DRAW);

		// 顶点位置
		glEnableVertexAttribArray(0);//启用顶点属性数组(将顶点数据传递给着色器)（参数由location = 0 决定）
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);//告知opengl如何解析顶点数据(属性索引[让着色器知道aPos是位置数据，aColor是颜色数据],数据组成元素个数,数据类型,是否标准化,步长,偏移量)
		// 顶点法线
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// 顶点纹理坐标
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};




#endif
