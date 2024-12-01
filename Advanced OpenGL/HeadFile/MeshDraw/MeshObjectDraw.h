#ifndef MESH_OBJECT_DRAW_H
#define MESH_OBJECT_DRAW_H
#include "../Shader.h"
#include "../Mesh.h"
#include "../Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//camera����
extern glm::vec3 CameraPos;//cameraλ��
extern glm::vec3 CameraFront;

//light����
//���Դ
extern glm::vec3 PointLightPos;
extern glm::vec3 PointLightColor;
//ƽ�й�
extern glm::vec3 DirectLightDirection;
extern glm::vec3 DirectLightColor;
//�ֵ��
extern glm::vec3 FlashLightColor;
extern float InnerAngle;
extern float OuterAngle;

//�����������(ʹ�ù���ģ��)
class ObjectMeshDrawWL
{
private:
    Shader ObjectShader;
    Mesh ObjectMesh;
    Shader* FrameShader;

    //ģ�ͱ任����
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

    //�߿�任����
    glm::mat4 FModelMatrix = glm::mat4(1.0f);


public:
    ObjectMeshDrawWL(Shader& objectShader, Mesh objectMesh, Shader* frameShader = nullptr) : ObjectShader(objectShader), ObjectMesh(objectMesh), FrameShader(frameShader)
    {}

    //��ȡ��ر任����
    void GetMatrix(glm::mat4 modelmatrix, glm::mat4 viewmatrix, glm::mat4 projectionmatrix)
    {
        ModelMatrix = modelmatrix;
        ViewMatrix = viewmatrix;
        ProjectionMatrix = projectionmatrix;
    }

    //��ȡ�߿�任����
    void GetFMatrix(glm::mat4 fmodelmatrix)
    {
        FModelMatrix = fmodelmatrix;
    }


    //������ɫ������
    void BasicShaderSet()
    {
        ObjectShader.Use();
        //�������
        ObjectShader.setMat4("model", ModelMatrix);
        ObjectShader.setMat4("view", ViewMatrix);
        ObjectShader.setMat4("projection", ProjectionMatrix);
        ObjectShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(ModelMatrix))));
        ObjectShader.SetVec3("ViewPos", CameraPos);
        //����
        ObjectShader.SetFloat("material.Shininess", 32.0f);
        //���Դ
        ObjectShader.SetVec3("pointLight.Pos", PointLightPos);
        ObjectShader.SetVec3("pointLight.Ambient", PointLightColor * glm::vec3(0.2f));
        ObjectShader.SetVec3("pointLight.Diffuse", PointLightColor * glm::vec3(0.5f));
        ObjectShader.SetVec3("pointLight.Specular", PointLightColor * glm::vec3(1.0f));
        ObjectShader.SetFloat("pointLight.constant", 1.0f);
        ObjectShader.SetFloat("pointLight.linear", 0.35f);
        ObjectShader.SetFloat("pointLight.quadratic", 0.44f);
        //�����
        ObjectShader.SetVec3("directLight.Dir", DirectLightDirection);
        ObjectShader.SetVec3("directLight.Ambient", DirectLightColor * glm::vec3(0.2f));
        ObjectShader.SetVec3("directLight.Diffuse", DirectLightColor * glm::vec3(0.5f));
        ObjectShader.SetVec3("directLight.Specular", DirectLightColor * glm::vec3(1.0f));
        //�ֵ��
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
        //����
        ObjectShader.SetFloat("material.EmissionStrength", 0.75f);
        ObjectShader.SetFloat("material.EmissionMovement", 1.5f);
        //����
        ObjectMesh.Draw(ObjectShader);
    }

};


//�����������(��ʹ�ù���ģ��)
class ObjectMeshDrawNL
{
private:
    Shader ObjectShader;
    Mesh ObjectMesh;
    Shader* FrameShader;

    //ģ�ͱ任����
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

    //�߿�任����
    glm::mat4 FModelMatrix = glm::mat4(1.0f);


public:
    ObjectMeshDrawNL(Shader& objectShader, Mesh objectMesh, Shader* frameShader = nullptr) : ObjectShader(objectShader), ObjectMesh(objectMesh), FrameShader(frameShader)
    {}

    //��ȡ��ر任����
    void GetMatrix(glm::mat4 modelmatrix, glm::mat4 viewmatrix, glm::mat4 projectionmatrix)
    {
        ModelMatrix = modelmatrix;
        ViewMatrix = viewmatrix;
        ProjectionMatrix = projectionmatrix;
    }

    //��ȡ�߿�任����
    void GetFMatrix(glm::mat4 fmodelmatrix)
    {
        FModelMatrix = fmodelmatrix;
    }

    //������ɫ������
    void BasicShaderSet()
    {
        ObjectShader.Use();
        //�������
        ObjectShader.setMat4("model", ModelMatrix);
        ObjectShader.setMat4("view", ViewMatrix);
        ObjectShader.setMat4("projection", ProjectionMatrix);
    }

    void GrassDraw()
    {
        ObjectShader.Use();
        //����
        ObjectMesh.Draw(ObjectShader);
    }

    void WindowDraw()
    {
        ObjectShader.Use();
        //����
        ObjectMesh.Draw(ObjectShader);
    }

    void SkyBoxDraw()
    {
        ObjectShader.Use();
        ObjectShader.SetInt("SkyBoxCMTexture", 0);
        //����
        ObjectMesh.Draw(ObjectShader);
    }
};
#endif





////��ȡcamera����
//void GetCameraAttrabution(glm::vec3 cameraPos, glm::vec3 cameraFront)
//{
//    CameraPos = cameraFront;
//    CameraFront = cameraFront;
//}
//
////��ȡ��Դ����
//void GetLightAttribution(glm::vec3 pointLigntPos, glm::vec3 pointLightColor, glm::vec3 directLightDirection, glm::vec3 directLightColor, glm::vec3 flashLightColor, float innerAngle, float outerAngle)
//{
//    //���Դ
//    PointLightColor = pointLigntPos;
//    PointLightColor = pointLightColor;
//    //ƽ�й�
//    DirectLightDirection = directLightDirection;
//    DirectLightColor = directLightColor;
//    //�ֵ��
//    FlashLightColor = flashLightColor;
//    InnerAngle = innerAngle;
//    OuterAngle = outerAngle;
//}


