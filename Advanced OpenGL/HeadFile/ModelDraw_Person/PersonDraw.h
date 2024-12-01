#ifndef PERSONDRAW_H
#define PERSONDRAW_H
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

class PersonModelDraw
{
protected:
    Shader ModelShader;
    Model PersonModel;
    Shader* FrameShader;

    //模型变换矩阵
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

    //边框变换矩阵
    glm::mat4 FModelMatrix = glm::mat4(1.0f);


public:
    PersonModelDraw(Shader& modelShader, Model& personModel, Shader* frameShader = nullptr) : ModelShader(modelShader), PersonModel(personModel), FrameShader(frameShader)
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


    //基础人物着色器设置
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
        ModelShader.SetVec3("pointLight.Specular", PointLightColor * glm::vec3(0.0f));
        ModelShader.SetFloat("pointLight.constant", 1.0f);
        ModelShader.SetFloat("pointLight.linear", 0.35f);
        ModelShader.SetFloat("pointLight.quadratic", 0.44f);
        //定向光
        ModelShader.SetVec3("directLight.Dir", DirectLightDirection);
        ModelShader.SetVec3("directLight.Ambient", DirectLightColor * glm::vec3(0.2f));
        ModelShader.SetVec3("directLight.Diffuse", DirectLightColor * glm::vec3(0.5f));
        ModelShader.SetVec3("directLight.Specular", DirectLightColor * glm::vec3(0.0f));
        //手电光
        ModelShader.SetVec3("flashLight.Pos", CameraPos);
        ModelShader.SetVec3("flashLight.Dir", CameraFront);
        ModelShader.SetVec3("flashLight.Diffuse", FlashLightColor * glm::vec3(0.5f));
        ModelShader.SetVec3("flashLight.Specular", FlashLightColor * glm::vec3(0.0f));
        ModelShader.SetFloat("flashLight.InnerAngle", InnerAngle);
        ModelShader.SetFloat("flashLight.OuterAngle", OuterAngle);
        ModelShader.SetFloat("flashLight.constant", 1.0f);
        ModelShader.SetFloat("flashLight.linear", 0.35f);
        ModelShader.SetFloat("flashLight.quadratic", 0.44f);
    }

    //基础描边着色器设置
    void BasicFShaderSet()
    {
        //若FrameShader指针不为空
        if (FrameShader)
        {
            FrameShader->Use();

            FrameShader->setMat4("model", FModelMatrix);
            FrameShader->setMat4("projection", ProjectionMatrix);
            FrameShader->setMat4("view", ViewMatrix);
            FrameShader->setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(FModelMatrix))));
            FrameShader->SetFloat("FixRate", 0.0001f);//从0.001到0.0001，直观效果是改变轮廓的粗细
            FrameShader->SetVec3("FrameColor", glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }

    //骑士绘制
    void KinghtDraw()
    {
        ModelShader.Use();
        //材质
        ModelShader.SetInt("SkyBoxTexture", 2);
        //点光源
        ModelShader.SetVec3("pointLight.Specular", PointLightColor * glm::vec3(1.0f));
        //平行光
        ModelShader.SetVec3("directLight.Specular", DirectLightColor * glm::vec3(1.0f));
        //手电光
        ModelShader.SetVec3("flashLight.Specular", FlashLightColor * glm::vec3(1.0f));
        //绘制
        PersonModel.Draw(ModelShader);
    }

    //今汐绘制
    void JinXiDraw()
    {
        ModelShader.Use();
        //绘制
        PersonModel.Draw(ModelShader);
    }

    //骑士描边绘制
    void KnightFrameDraw()
    {
        //若FrameShader指针不为空
        if (FrameShader)
        {
            FrameShader->Use();
            FrameShader->SetVec3("FrameColor", glm::vec3(1.0f, 0.0f, 0.0f));
            //绘制
            Shader shader = *FrameShader;
            PersonModel.Draw(shader);
        }
    }

    //今汐描边绘制
    void JinXiFrameDraw()
    {
        //若FrameShader指针不为空
        if (FrameShader)
        {
            FrameShader->Use();
            FrameShader->SetVec3("FrameColor", glm::vec3(0.0f, 0.0f, 1.0f));
            //绘制
            Shader shader = *FrameShader;
            PersonModel.Draw(shader);
        }
    }
};


#endif





////获取camera属性
//void GetCameraAttrabution(glm::vec3 cameraPos, glm::vec3 cameraFront)
//{
//    CameraPos = cameraFront;
//    CameraFront = cameraFront;
//}
//
////获取光源属性
//void GetLightAttribution(glm::vec3 pointLigntPos, glm::vec3 pointLightColor, glm::vec3 directLightDirection, glm::vec3 directLightColor, glm::vec3 flashLightColor, float innerAngle, float outerAngle)
//{
//    //点光源
//    PointLightColor = pointLigntPos;
//    PointLightColor = pointLightColor;
//    //平行光
//    DirectLightDirection = directLightDirection;
//    DirectLightColor = directLightColor;
//    //手电光
//    FlashLightColor = flashLightColor;
//    InnerAngle = innerAngle;
//    OuterAngle = outerAngle;
//}

