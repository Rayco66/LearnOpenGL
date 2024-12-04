#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Include/stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
void GetTexType(const aiScene* scene);

class Model
{
public:
    /*  模型数据  */
    std::vector<Texture> textures_loaded;//存储已加载过的纹理
    std::vector<Mesh> meshes;
    std::string directory;//模型文件所在目录
    bool gammaCorrection;//是否采用伽马校正

    /*  函数   */
    Model(std::string path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }
    void Draw(Shader &shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
    void InstanceDraw(Shader& shader,int num)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].InstanceDraw(shader,num);
    }
private:

    /*  函数   */
    void loadModel(std::string path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);//从指定路径读取模型，将多边形转换为三角形并且翻转UV坐标，scene指针指向这个模型的数据结构
        //检测模型是否加载成功
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        //GetTexType(scene);//获取该模型的所有纹理类型

        directory = path.substr(0,path.find_last_of('/'));//获取模型文件所在目录

        processNode(scene->mRootNode,scene);
    }

    //递归处理每一个scene下的节点，对于每一个节点，获取其网格索引-》获取每个网格-》处理每个网格
    void processNode(aiNode* node, const aiScene* scene)
    {
        //处理本节点的所有网格
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];//获取该节点下的所有网格（每个节点可以包含一个或多个网格的索引。这些网格并不一定是直接存储在该节点下，而是存储在场景的 mMeshes 数组中。node->mMeshes[i] 返回的是该节点所引用的网格在 mMeshes 数组中的索引）
            meshes.push_back(processMesh(mesh,scene));//将处理好的网格放入我们之后要使用的meshes容器中
        }
        //处理所有子节点
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i],scene);
        }
    }

    //将每一个 aiMesh 类型的网格转换为我们自定义的 Mesh 类的实例
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        //处理顶点
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            //顶点位置
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            //顶点法线
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            //顶点纹理坐标
            if (mesh->mTextureCoords[0])//只考虑第一组纹理坐标（每个顶点上可能有多个纹理坐标）
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else vertex.TexCoords = glm::vec2(0.0f,0.0f);

            vertices.push_back(vertex);
        }

        //处理索引
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)//处理网格的每个面
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)//处理每个面的每个索引
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        //处理材质
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];//从场景的mMaterials数组中获取aiMaterial对象
            //漫反射
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE,"DiffuseColorSampler");//加载漫反射纹理
            textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());//将纹理放入textures
            //镜面反射
            std::vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR,"SpecularColorSampler");
            textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());
            //自发光
            std::vector<Texture> emissionMaps = loadMaterialTextures(material,aiTextureType_EMISSIVE,"EmissionColorSampler");
            textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());
            //环境光
            std::vector<Texture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "AmbientColorSampler");
            textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
        }

        return Mesh(vertices,indices,textures); 

    }

    //遍历给定纹理类型的所有纹理位置，获取纹理的文件位置，并加载并和生成纹理，将信息储存在一个Vertex结构体中
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)//mat->GetTextureCount(type)获取材质中指定类型纹理的数量
        {
            aiString str;
            mat->GetTexture(type,i,&str);//获取type类型的第i个纹理并将该纹理路径存储到str中
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);//若是重复纹理，则无需重新加载纹理，直接加入到textures中
                    skip = true;
                    break;
                }
            }
            //如果不是重复纹理，则重新加载该纹理
            if (!skip)
            {
                Texture texture;
                std::string texturePath = str.C_Str();
                if (texturePath[0] == '/' || texturePath.find(':') != std::string::npos) {
                    // texturePath是绝对路径时，直接使用
                    texture.id = TextureFromFile(texturePath.c_str(), "");
                }
                else {
                    // texturePath是相对路径时，则拼接目录
                    texture.id = TextureFromFile(texturePath.c_str(), this->directory);
                }
                //texture.id = TextureFromFile(str.C_Str(),directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
            }
        }
        return textures;
    }
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;//拼接出纹理的完整路径（材质目录路径+纹理相对路径）

    unsigned int textureID;//(生成纹理的数量，纹理的存储位置)
    glGenTextures(1, &textureID);//绑定该纹理对象

    stbi_set_flip_vertically_on_load(false);//关闭y轴反转(全局设置函数，需要反转的时候要重新设置true)
   
    //加载并生成纹理
    int width, height, nrComponents;//存储图像的宽度，高度，颜色通道数
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);//加载图像文件并将其转换为像素数据
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Error: Model.h  " << "Unsupported number of components: " << nrComponents << std::endl;
            // 处理不支持的情况
            format = GL_RGB; // 默认 fallback
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);//将实际的纹理数据上传到GPU的纹理对象中(着色器在渲染时只需知道纹理单元和纹理坐标，而无需直接处理纹理数据)
        glGenerateMipmap(GL_TEXTURE_2D);//为绑定的二维纹理生成多级渐进纹理

        //为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }       
    else
    {
        std::cout << "MODEL.H : Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


// 获取纹理类型名称
const char* GetTextureTypeName(aiTextureType type) {
    switch (type) {
    case aiTextureType_DIFFUSE: return "Diffuse";
    case aiTextureType_SPECULAR: return "Specular";
    case aiTextureType_AMBIENT: return "Ambient";
    case aiTextureType_SHININESS: return "Shininess";
    case aiTextureType_OPACITY: return "Opacity";
    case aiTextureType_NORMALS: return "Normals";
    case aiTextureType_HEIGHT: return "Height";
    case aiTextureType_EMISSIVE: return "Emissive";
    case aiTextureType_REFLECTION: return "Reflection";
    case aiTextureType_LIGHTMAP: return "Lightmap";
    case aiTextureType_DIFFUSE_ROUGHNESS: return "Diffuse Roughness";
    case aiTextureType_UNKNOWN: return "Unknown";
    default: return "Unknown";
    }
}

// 获取并打印模型中的纹理类型
void GetTexType(const aiScene* scene) {
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        for (unsigned int t = aiTextureType_DIFFUSE; t <= aiTextureType_UNKNOWN; ++t) {
            aiTextureType type = static_cast<aiTextureType>(t);
            unsigned int numTextures = material->GetTextureCount(type);

            if (numTextures > 0) {
                std::cout << "Found " << numTextures << " textures of type: " << GetTextureTypeName(type) << std::endl;

                for (unsigned int j = 0; j < numTextures; ++j) {
                    aiString texturePath;
                    material->GetTexture(type, j, &texturePath);
                    std::cout << "Texture " << j << ": " << texturePath.C_Str() << std::endl;
                }
            }
        }
    }
}


#endif
