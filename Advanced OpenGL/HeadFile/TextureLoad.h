#ifndef TEXTURELOAD_H
#define TEXTURELOAD_H
#include<iostream>
#include<string>
#include<vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Include/stb_image.h"
inline unsigned int LoadTexture(char const* path)
{
    unsigned int texture;
    glGenTextures(1, &texture);//(�������������������Ĵ洢λ��)
    glBindTexture(GL_TEXTURE_2D, texture);//�󶨸�������󵽵�ǰ������

    //���ز���������
    int width, height, nrChannels;//�洢ͼ��Ŀ�ȣ��߶ȣ���ɫͨ����
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);//����ͼ���ļ�������ת��Ϊ��������
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)      format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);//��ʵ�ʵ����������ϴ���GPU�����������(��ɫ������Ⱦʱֻ��֪������Ԫ���������꣬������ֱ�Ӵ�����������)
        glGenerateMipmap(GL_TEXTURE_2D);//Ϊ�󶨵Ķ�ά�������ɶ༶��������

        //Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
        if (format == GL_RGBA)
        {   //���Ʒ�ʽ
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//���������곬��[0.0, 1.0]��Χʱ����������ᱻ������0��1֮�䡣
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //���������곬��[0.0, 1.0]��Χʱ��������ظ����������Ĳ��ֻ����¿�ʼ���������һ�˽���ӳ��
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        //���˷�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);//��תͼ��y��(ȫ�����ú���������Ҫ��ת��ʱ��Ҫ��������Ϊfalse)

        stbi_image_free(data);
    }
    else
    {
        std::cout << "TEXTURELOAD.H : FAILED TO LOAD TEXTURE\n";
        stbi_image_free(data);
    }
    return texture;
}


inline unsigned int LoadCubeTexture(std::vector<std::string> Faces_Path)
{
    unsigned int texture;
    glGenTextures(1, &texture);//(�������������������Ĵ洢λ��)
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);//�󶨸�������󵽵�ǰ������

    stbi_set_flip_vertically_on_load(false);//�ر�y�ᷴת

    //���ز���������
    int width, height, nrChannels;//�洢ͼ��Ŀ�ȣ��߶ȣ���ɫͨ����
    for (unsigned int i = 0; i < Faces_Path.size(); i++)
    {
        unsigned char* data = stbi_load(Faces_Path[i].c_str(), &width, &height, &nrChannels, 0);//����ͼ���ļ�������ת��Ϊ��������
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//��ʵ�ʵ����������ϴ���GPU�����������(��ɫ������Ⱦʱֻ��֪������Ԫ���������꣬������ֱ�Ӵ�����������)

            //Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
            //���Ʒ�ʽ
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//���������곬��[0.0, 1.0]��Χʱ����������ᱻ������0��1֮�䡣
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            //���˷�ʽ
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "TEXTURELOAD.H : FAILED TO LOAD CUBETEXTURE\n";
            stbi_image_free(data);
        }
    }

    return texture;
}


#endif

