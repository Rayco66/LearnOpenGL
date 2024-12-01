#ifndef MESH_H
#define MESH_H
#include <glad/glad.h> 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

//���񶥵�����
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
//������������
struct Texture {
	unsigned int id;//����id
	std::string type;//��ͼ���ͣ���������ͼ/���淴����ͼ��
	std::string path;//����·��(�����ж������Ƿ��ظ�)
};


class Mesh {
public:
	/*��������*/
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

	//��װ�˰�VAO������ͻ�ͼ�Ĺ���
	void Draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int emissionNr = 1;
		unsigned int ambientNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
			// ��ȡ������ţ�DiffuseColorSamplerN �е� N��
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

			shader.SetInt(("material." + name + number).c_str(), i);//ƴ�ӳ�material.DiffuseColorSampler1��material.DiffuseColorSampler2��material.DiffuseColorSampler3��
			glBindTexture(GL_TEXTURE_2D, textures[i].id);//��������id�󶨵������������Ԫ��
		}

		// ��������
		glBindVertexArray(VAO);//����ǰ�����VAO����Ϊ��Ҫ���¼��������ָ��
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	/*��Ⱦ����*/
	unsigned int VAO, VBO, EBO;

	/*����*/
	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);//����һ��������󣬲�����ID�����ڱ���VBO�У�VBO���ڴ洢�������ݣ������ݴ�cpu�ϴ���gpu�У�
		glGenBuffers(1, &EBO);

		//��ָ���� VAO ��Ϊ��ǰ�� VAO
		glBindVertexArray(VAO);

		//���VBO����
		glBindBuffer(GL_ARRAY_BUFFER, VBO);//�������õĻ������VBO�󶨵�opengl�İ󶨵�GL_ARRAY_BUFFER�ϣ�ʹ�ú����Ĳ������������ϴ������ö������ԣ�����Ӱ�쵽����ض��Ļ���������
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);//�����ݸ��Ƶ���ǰ�󶨵Ļ�������(����Ŀ�ꡢ�������ݴ�С���������ݡ����ݹ���ʽ)
		//���EBO����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//��EBO�󶨵�opengl�İ󶨵�GL_ELEMENT_ARRAY_BUFFER��
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0], GL_STATIC_DRAW);

		// ����λ��
		glEnableVertexAttribArray(0);//���ö�����������(���������ݴ��ݸ���ɫ��)��������location = 0 ������
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);//��֪opengl��ν�����������(��������[����ɫ��֪��aPos��λ�����ݣ�aColor����ɫ����],�������Ԫ�ظ���,��������,�Ƿ��׼��,����,ƫ����)
		// ���㷨��
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// ������������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};




#endif
