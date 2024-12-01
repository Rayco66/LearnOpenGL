#ifndef OBJECTDRAW_H
#define OBJECTDRAW_H
#include "../Shader.h"
#include "../Mesh.h"
#include "../Model.h"
#include "../Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//camera属性
extern glm::vec3 CameraPos;//camera位置
extern glm::vec3 CameraFront;

//light属性
//点光源
extern glm::vec3 PointLightPos;
extern glm::vec3 PointLightColor;
//平行光
extern glm::vec3 DirectLightDirection;
extern glm::vec3 DirectLightColor;
//手电光
extern glm::vec3 FlashLightColor;
extern float InnerAngle;
extern float OuterAngle;


//物体模型绘制(使用光照模型)
class ObjectModelDrawWL
{
protected:
    Shader ModelShader;
    Model ObjectModel;
    Shader* FrameShader;

    //模型变换矩阵
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

    //边框变换矩阵
    glm::mat4 FModelMatrix = glm::mat4(1.0f);


public:
    ObjectModelDrawWL(Shader& modelShader, Model& objectModel, Shader* frameShader = nullptr) : ModelShader(modelShader), ObjectModel(objectModel), FrameShader(frameShader)
    {}

    //获取相关变换矩阵
    void GetMatrix(glm::mat4 modelmatrix, glm::mat4 viewmatrix, glm::mat4 projectionmatrix)
    {
        ModelMatrix = modelmatrix;
        ViewMatrix = viewmatrix;
        ProjectionMatrix = projectionmatrix;
    }

    //获取边框变换矩阵
    void GetFMatrix(glm::mat4 fmodelmatrix)
    {
        FModelMatrix = fmodelmatrix;
    }


    //基础物体着色器设置
    void BasicShaderSet()
    {
        ModelShader.Use();
        //计算相关
        ModelShader.setMat4("projection", ProjectionMatrix);
        ModelShader.setMat4("view", ViewMatrix);
        ModelShader.setMat4("model", ModelMatrix);
        ModelShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(ModelMatrix))));
        ModelShader.SetVec3("ViewPos", CameraPos);
        //材质
        ModelShader.SetFloat("material.Shininess", 32.0f);
        //点光源
        ModelShader.SetVec3("pointLight.Pos", PointLightPos);
        ModelShader.SetVec3("pointLight.Ambient", PointLightColor * glm::vec3(0.2f));
        ModelShader.SetVec3("pointLight.Diffuse", PointLightColor * glm::vec3(0.5f));
        ModelShader.SetVec3("pointLight.Specular", PointLightColor * glm::vec3(1.0f));
        ModelShader.SetFloat("pointLight.constant", 1.0f);
        ModelShader.SetFloat("pointLight.linear", 0.35f);
        ModelShader.SetFloat("pointLight.quadratic", 0.44f);
        //定向光
        ModelShader.SetVec3("directLight.Dir", DirectLightDirection);
        ModelShader.SetVec3("directLight.Ambient", DirectLightColor * glm::vec3(0.2f));
        ModelShader.SetVec3("directLight.Diffuse", DirectLightColor * glm::vec3(0.5f));
        ModelShader.SetVec3("directLight.Specular", DirectLightColor * glm::vec3(1.0f));
        //手电光
        ModelShader.SetVec3("flashLight.Pos", CameraPos);
        ModelShader.SetVec3("flashLight.Dir", CameraFront);
        ModelShader.SetVec3("flashLight.Diffuse", FlashLightColor * glm::vec3(0.5f));
        ModelShader.SetVec3("flashLight.Specular", FlashLightColor * glm::vec3(1.0f));
        ModelShader.SetFloat("flashLight.InnerAngle", InnerAngle);
        ModelShader.SetFloat("flashLight.OuterAngle", OuterAngle);
        ModelShader.SetFloat("flashLight.constant", 1.0f);
        ModelShader.SetFloat("flashLight.linear", 0.35f);
        ModelShader.SetFloat("flashLight.quadratic", 0.44f);
    }

    //DiscoBall绘制
    void DiscoBallDraw()
    {
        ModelShader.Use();
        //材质
        ModelShader.SetInt("SkyBoxTexture", 0);
        //点光源
        ModelShader.SetVec3("pointLight.Ambient", PointLightColor * glm::vec3(0.0f));
        ModelShader.SetVec3("pointLight.Diffuse", PointLightColor * glm::vec3(0.0f));
        //定向光
        ModelShader.SetVec3("directLight.Ambient", DirectLightColor * glm::vec3(0.0f));
        ModelShader.SetVec3("directLight.Diffuse", DirectLightColor * glm::vec3(0.0f));
        //手电光
        ModelShader.SetVec3("flashLight.Diffuse", FlashLightColor * glm::vec3(0.0f));
        //绘制
        ObjectModel.Draw(ModelShader);
    }
};



//物体模型绘制(不使用光照模型)
class ObjectModelDrawNL
{
protected:
    Shader ModelShader;
    Model ObjectModel;
    Shader* FrameShader;

    //模型变换矩阵
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

    //边框变换矩阵
    glm::mat4 FModelMatrix = glm::mat4(1.0f);


public:
    ObjectModelDrawNL(Shader& modelShader, Model& objectModel, Shader* frameShader = nullptr) : ModelShader(modelShader), ObjectModel(objectModel), FrameShader(frameShader)
    {}

    //获取相关变换矩阵
    void GetMatrix(glm::mat4 modelmatrix, glm::mat4 viewmatrix, glm::mat4 projectionmatrix)
    {
        ModelMatrix = modelmatrix;
        ViewMatrix = viewmatrix;
        ProjectionMatrix = projectionmatrix;
    }

    //获取边框变换矩阵
    void GetFMatrix(glm::mat4 fmodelmatrix)
    {
        FModelMatrix = fmodelmatrix;
    }


    //基础物体着色器设置
    void BasicShaderSet()
    {
        ModelShader.Use();
        //计算相关
        ModelShader.setMat4("projection", ProjectionMatrix);
        ModelShader.setMat4("view", ViewMatrix);
        ModelShader.setMat4("model", ModelMatrix);
        ModelShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(ModelMatrix))));
        ModelShader.SetVec3("ViewPos", CameraPos);
    }


    //光源Ball绘制
    void LightBallDraw()
    {
        ModelShader.Use();
        //绘制
        ObjectModel.Draw(ModelShader);
    }

};
#endif


