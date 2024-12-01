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
    glGenTextures(1, &texture);//(生成纹理的数量，纹理的存储位置)
    glBindTexture(GL_TEXTURE_2D, texture);//绑定该纹理对象到当前上下文

    //加载并生成纹理
    int width, height, nrChannels;//存储图像的宽度，高度，颜色通道数
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);//加载图像文件并将其转换为像素数据
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)      format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);//将实际的纹理数据上传到GPU的纹理对象中(着色器在渲染时只需知道纹理单元和纹理坐标，而无需直接处理纹理数据)
        glGenerateMipmap(GL_TEXTURE_2D);//为绑定的二维纹理生成多级渐进纹理

        //为当前绑定的纹理对象设置环绕、过滤方式
        if (format == GL_RGBA)
        {   //环绕方式
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//当纹理坐标超出[0.0, 1.0]范围时，纹理坐标会被调整到0和1之间。
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //当纹理坐标超出[0.0, 1.0]范围时，纹理会重复，即超出的部分会重新开始从纹理的另一端进行映射
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        //过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);//反转图像y轴(全局设置函数，不需要反转的时候要重新设置为false)

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
    glGenTextures(1, &texture);//(生成纹理的数量，纹理的存储位置)
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);//绑定该纹理对象到当前上下文

    stbi_set_flip_vertically_on_load(false);//关闭y轴反转

    //加载并生成纹理
    int width, height, nrChannels;//存储图像的宽度，高度，颜色通道数
    for (unsigned int i = 0; i < Faces_Path.size(); i++)
    {
        unsigned char* data = stbi_load(Faces_Path[i].c_str(), &width, &height, &nrChannels, 0);//加载图像文件并将其转换为像素数据
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//将实际的纹理数据上传到GPU的纹理对象中(着色器在渲染时只需知道纹理单元和纹理坐标，而无需直接处理纹理数据)

            //为当前绑定的纹理对象设置环绕、过滤方式
            //环绕方式
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//当纹理坐标超出[0.0, 1.0]范围时，纹理坐标会被调整到0和1之间。
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            //过滤方式
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

