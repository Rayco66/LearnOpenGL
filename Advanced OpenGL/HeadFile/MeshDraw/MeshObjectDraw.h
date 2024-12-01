#ifndef MESH_OBJECT_DRAW_H
#define MESH_OBJECT_DRAW_H
#include "../Shader.h"
#include "../Mesh.h"
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

//物体网格绘制(使用光照模型)
class ObjectMeshDrawWL
{
private:
    Shader ObjectShader;
    Mesh ObjectMesh;
    Shader* FrameShader;

    //模型变换矩阵
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

    //边框变换矩阵
    glm::mat4 FModelMatrix = glm::mat4(1.0f);


public:
    ObjectMeshDrawWL(Shader& objectShader, Mesh objectMesh, Shader* frameShader = nullptr) : ObjectShader(objectShader), ObjectMesh(objectMesh), FrameShader(frameShader)
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


    //基础着色器设置
    void BasicShaderSet()
    {
        ObjectShader.Use();
        //计算相关
        ObjectShader.setMat4("model", ModelMatrix);
        ObjectShader.setMat4("view", ViewMatrix);
        ObjectShader.setMat4("projection", ProjectionMatrix);
        ObjectShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(ModelMatrix))));
        ObjectShader.SetVec3("ViewPos", CameraPos);
        //材质
        ObjectShader.SetFloat("material.Shininess", 32.0f);
        //点光源
        ObjectShader.SetVec3("pointLight.Pos", PointLightPos);
        ObjectShader.SetVec3("pointLight.Ambient", PointLightColor * glm::vec3(0.2f));
        ObjectShader.SetVec3("pointLight.Diffuse", PointLightColor * glm::vec3(0.5f));
        ObjectShader.SetVec3("pointLight.Specular", PointLightColor * glm::vec3(1.0f));
        ObjectShader.SetFloat("pointLight.constant", 1.0f);
        ObjectShader.SetFloat("pointLight.linear", 0.35f);
        ObjectShader.SetFloat("pointLight.quadratic", 0.44f);
        //定向光
        ObjectShader.SetVec3("directLight.Dir", DirectLightDirection);
        ObjectShader.SetVec3("directLight.Ambient", DirectLightColor * glm::vec3(0.2f));
        ObjectShader.SetVec3("directLight.Diffuse", DirectLightColor * glm::vec3(0.5f));
        ObjectShader.SetVec3("directLight.Specular", DirectLightColor * glm::vec3(1.0f));
        //手电光
        ObjectShader.SetVec3("flashLight.Pos", CameraPos);
        ObjectShader.SetVec3("flashLight.Dir", CameraFront);
        ObjectShader.SetVec3("flashLight.Diffuse", FlashLightColor * glm::vec3(0.5f));
        ObjectShader.SetVec3("flashLight.Specular", FlashLightColor * glm::vec3(1.0f));
        ObjectShader.SetFloat("flashLight.InnerAngle", InnerAngle);
        ObjectShader.SetFloat("flashLight.OuterAngle", OuterAngle);
        ObjectShader.SetFloat("flashLight.constant", 1.0f);
        ObjectShader.SetFloat("flashLight.linear", 0.35f);
        ObjectShader.SetFloat("flashLight.quadratic", 0.44f);
    }

    void WallDraw()
    {
        ObjectShader.Use();
        //材质
        ObjectShader.SetFloat("material.EmissionStrength", 0.75f);
        ObjectShader.SetFloat("material.EmissionMovement", 1.5f);
        //绘制
        ObjectMesh.Draw(ObjectShader);
    }

};


//物体网格绘制(不使用光照模型)
class ObjectMeshDrawNL
{
private:
    Shader ObjectShader;
    Mesh ObjectMesh;
    Shader* FrameShader;

    //模型变换矩阵
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

    //边框变换矩阵
    glm::mat4 FModelMatrix = glm::mat4(1.0f);


public:
    ObjectMeshDrawNL(Shader& objectShader, Mesh objectMesh, Shader* frameShader = nullptr) : ObjectShader(objectShader), ObjectMesh(objectMesh), FrameShader(frameShader)
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

    //基础着色器设置
    void BasicShaderSet()
    {
        ObjectShader.Use();
        //计算相关
        ObjectShader.setMat4("model", ModelMatrix);
        ObjectShader.setMat4("view", ViewMatrix);
        ObjectShader.setMat4("projection", ProjectionMatrix);
    }

    void GrassDraw()
    {
        ObjectShader.Use();
        //绘制
        ObjectMesh.Draw(ObjectShader);
    }

    void WindowDraw()
    {
        ObjectShader.Use();
        //绘制
        ObjectMesh.Draw(ObjectShader);
    }

    void SkyBoxDraw()
    {
        ObjectShader.Use();
        ObjectShader.SetInt("SkyBoxCMTexture", 0);
        //绘制
        ObjectMesh.Draw(ObjectShader);
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


