#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"

class Shader
{
public:

	unsigned int ID;//��ɫ������ID

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//1.���ļ�·���л�ȡ����/Ƭ����ɫ��
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;//�����ļ�������
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;//�����ַ���������

			//��ȡ�ļ����ݵ��ַ�����������(׷����Ҳ��ֱ�ӽ��ļ����ݶ�ȡ���ַ�����)
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//�ر��ļ�
			vShaderFile.close();
			fShaderFile.close();

			//��������ת����string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)//�����쳣
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << e.what() << std::endl;
		}

		//���ַ���ת����c�����ַ���
		const char* vShaderSource = vertexCode.c_str();
		const char* fShaderSource = fragmentCode.c_str();

		//2.������ɫ��
			//������ɫ��
		unsigned int vertexShader, fragmentShader;

		//������ɫ��
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderSource, NULL);//������ɫ��Դ�루��ɫ������,��ɫ��Դ���ַ�������,ָ��Դ���ַ���ָ���ָ��,ÿ���ַ���ȡ�೤��NULLΪȡ�����ַ�������
		glCompileShader(vertexShader);//������ɫ��
		CheckCompile(vertexShader, "SHADER");//��������ɫ�������������루vertexShader/fragmentShader��"SHADER"),��������ɫ���������ӽ�������루shaderProgram,"PROGRAM"��

		//Ƭ����ɫ��
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
		glCompileShader(fragmentShader);
		CheckCompile(fragmentShader, "SHADER");

		//��ɫ������
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		CheckCompile(ID, "PROGRAM");
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	//������ɫ������
	void Use()
	{
		glUseProgram(ID);
	}

	//����Uniformֵ
	void SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetFloatArray(const std::string& name, float* value, size_t count) const
	{
		glUniform1fv(glGetUniformLocation(ID, name.c_str()),count,value);
	}
	void SetVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void Setvec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	void SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void Setvec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void SetVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void Setvec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	//�����ɫ��/��ɫ����������Ƿ�ɹ�
	void CheckCompile(unsigned int& Shader, std::string type)
	{
		int success;//�жϽ��
		char infoLog[512];//���󱨸�
		if (type != "PROGRAM")
		{
			glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(Shader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::COMPILE_ERROR: " << infoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(Shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(Shader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER_PROGRAM_LINK_ERROR: " << infoLog << std::endl;
			}
		}


	}
};

#endif#pragma once
