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

	unsigned int ID;//着色器程序ID

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//1.从文件路径中获取顶点/片段着色器
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;//设置文件流对象
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;//创建字符串流对象

			//读取文件内容到字符串流对象中(追求简便也可直接将文件内容读取到字符串中)
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//关闭文件
			vShaderFile.close();
			fShaderFile.close();

			//将数据流转换到string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)//处理异常
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << e.what() << std::endl;
		}

		//将字符串转换成c风格的字符串
		const char* vShaderSource = vertexCode.c_str();
		const char* fShaderSource = fragmentCode.c_str();

		//2.编译着色器
			//创建着色器
		unsigned int vertexShader, fragmentShader;

		//顶点着色器
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderSource, NULL);//附着着色器源码（着色器对象,着色器源码字符串数量,指向源码字符串指针的指针,每个字符串取多长（NULL为取整个字符串））
		glCompileShader(vertexShader);//编译着色器
		CheckCompile(vertexShader, "SHADER");//若想检测着色器编译结果就输入（vertexShader/fragmentShader，"SHADER"),若想检测着色器程序链接结果就输入（shaderProgram,"PROGRAM"）

		//片段着色器
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
		glCompileShader(fragmentShader);
		CheckCompile(fragmentShader, "SHADER");

		//着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		CheckCompile(ID, "PROGRAM");
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	//激活着色器程序
	void Use()
	{
		glUseProgram(ID);
	}

	//设置Uniform值
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
	//检测着色器/着色器程序编译是否成功
	void CheckCompile(unsigned int& Shader, std::string type)
	{
		int success;//判断结果
		char infoLog[512];//错误报告
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
