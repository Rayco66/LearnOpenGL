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
    /*  ����   */
    Model(std::string path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }
    void Draw(Shader &shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
private:
    /*  ģ������  */
    std::vector<Texture> textures_loaded;//�洢�Ѽ��ع�������
    std::vector<Mesh> meshes;
    std::string directory;//ģ���ļ�����Ŀ¼
    bool gammaCorrection;//�Ƿ����٤��У��

    /*  ����   */
    void loadModel(std::string path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);//��ָ��·����ȡģ�ͣ��������ת��Ϊ�����β��ҷ�תUV���꣬sceneָ��ָ�����ģ�͵����ݽṹ
        //���ģ���Ƿ���سɹ�
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        //GetTexType(scene);//��ȡ��ģ�͵�������������

        directory = path.substr(0,path.find_last_of('/'));//��ȡģ���ļ�����Ŀ¼

        processNode(scene->mRootNode,scene);
    }

    //�ݹ鴦��ÿһ��scene�µĽڵ㣬����ÿһ���ڵ㣬��ȡ����������-����ȡÿ������-������ÿ������
    void processNode(aiNode* node, const aiScene* scene)
    {
        //�����ڵ����������
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];//��ȡ�ýڵ��µ���������ÿ���ڵ���԰���һ�������������������Щ���񲢲�һ����ֱ�Ӵ洢�ڸýڵ��£����Ǵ洢�ڳ����� mMeshes �����С�node->mMeshes[i] ���ص��Ǹýڵ������õ������� mMeshes �����е�������
            meshes.push_back(processMesh(mesh,scene));//������õ������������֮��Ҫʹ�õ�meshes������
        }
        //���������ӽڵ�
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i],scene);
        }
    }

    //��ÿһ�� aiMesh ���͵�����ת��Ϊ�����Զ���� Mesh ���ʵ��
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        //������
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            //����λ��
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            //���㷨��
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            //������������
            if (mesh->mTextureCoords[0])//ֻ���ǵ�һ���������꣨ÿ�������Ͽ����ж���������꣩
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else vertex.TexCoords = glm::vec2(0.0f,0.0f);

            vertices.push_back(vertex);
        }

        //��������
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)//���������ÿ����
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)//����ÿ�����ÿ������
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        //�������
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];//�ӳ�����mMaterials�����л�ȡaiMaterial����
            //������
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE,"DiffuseColorSampler");//��������������
            textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());//���������textures
            //���淴��
            std::vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR,"SpecularColorSampler");
            textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());
            //�Է���
            std::vector<Texture> emissionMaps = loadMaterialTextures(material,aiTextureType_EMISSIVE,"EmissionColorSampler");
            textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());
            //������
            std::vector<Texture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "AmbientColorSampler");
            textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
        }

        return Mesh(vertices,indices,textures); 

    }

    //���������������͵���������λ�ã���ȡ������ļ�λ�ã������ز���������������Ϣ������һ��Vertex�ṹ����
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)//mat->GetTextureCount(type)��ȡ������ָ���������������
        {
            aiString str;
            mat->GetTexture(type,i,&str);//��ȡtype���͵ĵ�i��������������·���洢��str��
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);//�����ظ��������������¼�������ֱ�Ӽ��뵽textures��
                    skip = true;
                    break;
                }
            }
            //��������ظ����������¼��ظ�����
            if (!skip)
            {
                Texture texture;
                std::string texturePath = str.C_Str();
                if (texturePath[0] == '/' || texturePath.find(':') != std::string::npos) {
                    // texturePath�Ǿ���·��ʱ��ֱ��ʹ��
                    texture.id = TextureFromFile(texturePath.c_str(), "");
                }
                else {
                    // texturePath�����·��ʱ����ƴ��Ŀ¼
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
    filename = directory + '/' + filename;//ƴ�ӳ����������·��������Ŀ¼·��+�������·����

    unsigned int textureID;//(�������������������Ĵ洢λ��)
    glGenTextures(1, &textureID);//�󶨸��������

    stbi_set_flip_vertically_on_load(false);//�ر�y�ᷴת(ȫ�����ú�������Ҫ��ת��ʱ��Ҫ��������true)
   
    //���ز���������
    int width, height, nrComponents;//�洢ͼ��Ŀ�ȣ��߶ȣ���ɫͨ����
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);//����ͼ���ļ�������ת��Ϊ��������
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
            // ����֧�ֵ����
            format = GL_RGB; // Ĭ�� fallback
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);//��ʵ�ʵ����������ϴ���GPU�����������(��ɫ������Ⱦʱֻ��֪������Ԫ���������꣬������ֱ�Ӵ�����������)
        glGenerateMipmap(GL_TEXTURE_2D);//Ϊ�󶨵Ķ�ά�������ɶ༶��������

        //Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//���Ʒ�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//���˷�ʽ
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


// ��ȡ������������
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

// ��ȡ����ӡģ���е���������
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