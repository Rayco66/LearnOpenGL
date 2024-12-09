#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <vector>
#include <chrono>
#include <map>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "HeadFile/Shader.h"
#include "HeadFile/Mesh.h"
#include "HeadFile/Model.h"
#include "HeadFile/Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



int WindowWidth = 1200;//���ڴ�С
int WindowHeight = 900;
int WindowPos_X = 300;//����(���Ͻ�)����λ��
int WindowPos_Y = 100;
float aspectRatio = (float)(WindowWidth / WindowHeight);//���ڳ����
bool IsPaused = false;//�Ƿ���ͣ
bool LastSpacePressd = false;//�����һ֡�Ƿ���space
bool CurrentSpacePressd = false;//��⵱ǰ֡�Ƿ���space
bool IsKey_F = false;//�Ƿ���F��
bool LastKeyPressd_F = false;//�����һ֡�Ƿ���F��
bool CurrentKeyPressd_F = false;//��⵱ǰ֡�Ƿ���F��
bool IsKey_E = false;//�Ƿ���E��
bool LastKeyPressd_E = false;//�����һ֡�Ƿ���E��
bool CurrentKeyPressd_E = false;//��⵱ǰ֡�Ƿ���E��
bool IsKey_M = false;//�Ƿ���M��
bool LastKeyPressd_M = false;//�����һ֡�Ƿ���M��
bool CurrentKeyPressd_M = false;//��⵱ǰ֡�Ƿ���M��
bool IsAlt = false;//�Ƿ���Alt
bool IsAltRealise = false;//�Ƿ��ɿ�Alt
float LastTime = 0.0f;//��һ֡ʱ��
float CurrentTime = 0.0f;//��ǰ֡ʱ��
float PassedTime = 0.0f;//����ͣʱ������ʱ��
float PeriodTime = 0.0f;//ÿ��֮֡���ʱ��
int FPSCount = 0;//֡��Ŀ
float OneSecond = 0.0f;//��¼һ����
float LastX = 960;//����ʼλ�ã���Ⱦ�������ģ�
float LastY = 540;
bool FirstMouse = true;//�Ƿ��״λ�ȡ�������
float rate = 0.0f;//����ɼ���

//���������
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
glm::vec3 CameraPos = camera.Pos;
glm::vec3 CameraFront = camera.Front;
glm::vec3 CameraUP = camera.Up;

//��������
//glm::vec3 LightColor(sin(glfwGetTime() * 1.0f)/2.0f+0.5f,sin(glfwGetTime() * 1.2f)/2.0f+0.5f,sin(glfwGetTime() * 1.5f)/2.0f+0.5f);
//���Դ
glm::vec3 PointLightColor;//���Դ��ɫ
glm::vec3 PointLightPos;//���Դλ��
//ƽ�й�
glm::vec3 DirectLightDirection;//ƽ�йⷽ��
glm::vec3 DirectLightColor;//ƽ�й���ɫ
//�ֵ��
glm::vec3 FlashLightColor;//�ֵ����ɫ
float InnerAngle;//�ֵ����Ȧ�Ƕȣ�����ֵ��
float OuterAngle;//�ֵ����Ȧ�Ƕȣ�����ֵ��


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double pos_x, double pos_y);
void scroll_callback(GLFWwindow* window, double offset_x, double offset_y);
void processInput(GLFWwindow* window);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //����һ������
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, WindowPos_X, WindowPos_Y);//���ô���λ��

    //��ʼ��glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //�����ӿ�
    glViewport(0, 0, WindowWidth, WindowHeight);

    //������Ⱦģʽ
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);//�߿�ģʽ
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//���ģʽ

    //������Ȳ��ԡ�ģ����ԣ�ȫ��״̬��������Ĭ��֡���廹���Խ�֡���嶼�����ò��ԣ�
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);//ģ����ʧ��ʱģ��ֵ���䣬ģ����ͨ������ȼ��ʧ��ʱģ��ֵ�滻Ϊ�ο�ֵ����ͨ��ʱģ��ֵ�滻Ϊ�ο�ֵ

    //�������޳�
    //glEnable(GL_CULL_FACE);

    //���ô��ڴ�С�ص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//ÿ���û��ֶ��������ڴ�Сʱ�ͻ���ûص�����(GLFW���Զ�����Ƿ���Ҫ���ú��������Բ���д��ѭ���ڲ�)
    //��ʹû���������GLFWҲ�����û��ֶ����ڴ��ڴ�С������������ǵ����ӿڴ�С�������������

    //�����
    //glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);//glfw�Զ��������λ�ò����ݸ�mouse_callback
    glfwSetScrollCallback(window, scroll_callback);//��������ƶ����������ݸ�scroll_callback

    //������ɫ��
    Shader PlaneShader("ShaderSource/vShader_Plane.glsl", "ShaderSource/fShader_Plane.glsl");
    Shader RockShader("ShaderSource/vShader_Rock.glsl", "ShaderSource/fShader_Rock.glsl");

    //����ģ��
    Model ObjectModel_Planet((std::filesystem::current_path() / "source/Planetary System/planet/planet.obj").string());
    Model ObjectModel_Rock((std::filesystem::current_path() / "source/Planetary System/rock/rock.obj").string());

    //С���Ǵ�ģ�;������
    int RockAmount = 1000;//С��������
    std::vector<glm::mat4> ModelMatrix_Rock;
    std::vector<glm::mat3> NormalMatrix_Rock;
    float Radius = 50.0f;//С���Ǵ��뾶
    float HalfWidth = 2.5f;//���С���Ǵ��Ŀ��
    srand(glfwGetTime());//��ȡ���������

    for (int i = 0; i < RockAmount; i++)
    {
        //��ȡλ��
        glm::mat4 model = glm::mat4(1.0f);
        float DeltaAngle = (float)i * (360.0f / (float)RockAmount);//ƽ��ÿ��С������Բ���ϵĽǶ�
        float Offset = (rand() % (int)(HalfWidth * 2 * 100)) / 100.0f - HalfWidth;//С������С���Ǵ��е�ƫ��[-HalfWidth,HalfWidth].(��100�ٳ�100��Ϊ����߾���)
        float x = cos(DeltaAngle) * Radius + Offset;//x����
        Offset = (rand() % (int)(HalfWidth * 2 * 100)) / 100.0f - HalfWidth;//���»�ȡһ�������Offset
        float y = Offset * 0.5f;//y����
        Offset = (rand() % (int)(HalfWidth * 2 * 100)) / 100.0f - HalfWidth;
        float z = sin(DeltaAngle) * Radius + Offset;//z����
        model = glm::translate(model,glm::vec3(x,y,z));

        //��ȡ����
        float scale = (rand() % 20) / 100.0f + 0.05f;//(0.05f,0.25f)֮������
        model = glm::scale(model,glm::vec3(scale));

        //��ȡ��ת
        float RotateAngle = rand() % 360;
        model = glm::rotate(model,glm::radians(RotateAngle),glm::vec3((rand() % 10)) / 10.0f);

        //���߾���
        glm::mat3 normal = glm::mat3(1.0f);
        normal = glm::mat3(glm::transpose(glm::inverse(model)));
        
        NormalMatrix_Rock.push_back(normal);
        ModelMatrix_Rock.push_back(model);
    }


    //ʵ������������
    unsigned int buffer_model;
    glGenBuffers(1, &buffer_model);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_model);
    glBufferData(GL_ARRAY_BUFFER, RockAmount * sizeof(glm::mat4), &ModelMatrix_Rock[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < ObjectModel_Rock.meshes.size(); i++)
    {
        unsigned int VAO = ObjectModel_Rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        //����OpenGL�ж������������������ݴ�С��һ��vec4���������봫��һ��mat4(4��vec4),����Ҫ�ĸ���������
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    unsigned int buffer_normal;
    glGenBuffers(1, &buffer_normal);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_normal);
    glBufferData(GL_ARRAY_BUFFER, RockAmount * sizeof(glm::mat3), &NormalMatrix_Rock[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < ObjectModel_Rock.meshes.size(); i++)
    {
        unsigned int VAO = ObjectModel_Rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        //����OpenGL�ж������������������ݴ�С��һ��vec4���������봫��һ��mat3(3��vec4),����Ҫ������������
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)0);
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3)));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(2 * sizeof(glm::vec3)));

        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);

        glBindVertexArray(0);
    }


    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        //�����������¼�
        glfwPollEvents();//�����ص�����

        //����������״̬
        processInput(window);

        //��ȡÿ֡ʱ��
        CurrentTime = (float)glfwGetTime();
        PeriodTime = CurrentTime - LastTime;
        if (!IsPaused)
        {
            PassedTime += CurrentTime - LastTime;
        }
        LastTime = CurrentTime;

        //����֡��
        FPSCount++;
        OneSecond += PeriodTime;
        if (OneSecond >= 1.0f)
        {
            float FPS = (float)FPSCount / OneSecond;
            std::string Title = "FPS:  " + std::to_string(FPS);
            glfwSetWindowTitle(window, Title.c_str());
            OneSecond = 0.0f;
            FPSCount = 0;
        }

        //ȫ��״̬������
            /*���ô��ڱ�����ɫ����ÿ��ѭ����������Ⱦ(�����ɫ�������Ȼ���)*/
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glStencilMask(0xFF);//ȷ��ģ�建�������������
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //��������
            /*���Դ*/
            //PointLightColor(sin(glfwGetTime() * 1.0f)/2.0f+0.5f,sin(glfwGetTime() * 1.2f)/2.0f+0.5f,sin(glfwGetTime() * 1.5f)/2.0f+0.5f);
        PointLightColor = glm::vec3(1.0f);//���Դ��ɫ
        PointLightPos = glm::vec3(0.9f * sin(PassedTime), 0.9f * cos(PassedTime) + 0.5f, sin(PassedTime) + 2.0f);//���Դλ��
        /*ƽ�й�*/
        DirectLightDirection = glm::vec3(1.0f, -1.0f, -1.0f);//ƽ�йⷽ��
        DirectLightColor = glm::vec3(1.0f);//ƽ�й���ɫ
        /*�ֵ��*/
        FlashLightColor = glm::vec3(0.0f);//�ֵ����ɫ
        InnerAngle = glm::cos(glm::radians(5.0f - rate));//�ֵ����Ȧ�Ƕȣ�����ֵ��
        OuterAngle = glm::cos(glm::radians(10.0f + rate));//�ֵ����Ȧ�Ƕȣ�����ֵ��

        //�任����
            /*ViewMatrix*/
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        /*PorjectMatrix*/
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);//��Ұ��Сʱ�����彫ռ�����������Ļ������λ�ã���������ø���
        
        //����ģ�ͻ���
        PlaneShader.Use();
        glm::mat4 ModelMatrix_Plane = glm::mat4(1.0f);
        ModelMatrix_Plane = glm::translate(ModelMatrix_Plane,glm::vec3(0.0f,-3.0f,0.0f));
        ModelMatrix_Plane = glm::scale(ModelMatrix_Plane,glm::vec3(4.0f));
        ModelMatrix_Plane = glm::rotate(ModelMatrix_Plane,glm::radians(25.0f) * PassedTime,glm::vec3(0.0f,1.0f,0.0f));

        //�������
        PlaneShader.setMat4("projection", projection);
        PlaneShader.setMat4("view", view);
        PlaneShader.setMat4("model", ModelMatrix_Plane);
        PlaneShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(ModelMatrix_Plane))));
        PlaneShader.SetVec3("ViewPos", CameraPos);
        //����
        PlaneShader.SetFloat("material.Shininess", 32.0f);
        //�����
        PlaneShader.SetVec3("directLight.Dir", DirectLightDirection);
        PlaneShader.SetVec3("directLight.Ambient", DirectLightColor * glm::vec3(0.2f));
        PlaneShader.SetVec3("directLight.Diffuse", DirectLightColor * glm::vec3(0.5f));
        PlaneShader.SetVec3("directLight.Specular", DirectLightColor * glm::vec3(0.0f));
        //�ֵ��
        PlaneShader.SetVec3("flashLight.Pos", CameraPos);
        PlaneShader.SetVec3("flashLight.Dir", CameraFront);
        PlaneShader.SetVec3("flashLight.Diffuse", FlashLightColor * glm::vec3(0.5f));
        PlaneShader.SetVec3("flashLight.Specular", FlashLightColor * glm::vec3(0.0f));
        PlaneShader.SetFloat("flashLight.InnerAngle", InnerAngle);
        PlaneShader.SetFloat("flashLight.OuterAngle", OuterAngle);
        PlaneShader.SetFloat("flashLight.constant", 1.0f);
        PlaneShader.SetFloat("flashLight.linear", 0.35f);
        PlaneShader.SetFloat("flashLight.quadratic", 0.44f);
        
        ObjectModel_Planet.Draw(PlaneShader);


        //С����ģ�ͻ���
        RockShader.Use();

        //�������
        RockShader.setMat4("projection", projection);
        RockShader.setMat4("view", view);
        RockShader.SetVec3("ViewPos", CameraPos);
        //RockShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(ModelMatrix_Plane))));
        //����
        RockShader.SetFloat("material.Shininess", 32.0f);
        //�����
        RockShader.SetVec3("directLight.Dir", DirectLightDirection);
        RockShader.SetVec3("directLight.Ambient", DirectLightColor* glm::vec3(0.2f));
        RockShader.SetVec3("directLight.Diffuse", DirectLightColor* glm::vec3(0.5f));
        RockShader.SetVec3("directLight.Specular", DirectLightColor* glm::vec3(0.0f));
        //�ֵ��
        RockShader.SetVec3("flashLight.Pos", CameraPos);
        RockShader.SetVec3("flashLight.Dir", CameraFront);
        RockShader.SetVec3("flashLight.Diffuse", FlashLightColor* glm::vec3(0.5f));
        RockShader.SetVec3("flashLight.Specular", FlashLightColor* glm::vec3(0.0f));
        RockShader.SetFloat("flashLight.InnerAngle", InnerAngle);
        RockShader.SetFloat("flashLight.OuterAngle", OuterAngle);
        RockShader.SetFloat("flashLight.constant", 1.0f);
        RockShader.SetFloat("flashLight.linear", 0.35f);
        RockShader.SetFloat("flashLight.quadratic", 0.44f);
        //����
        ObjectModel_Rock.InstanceDraw(RockShader, RockAmount);


        //����������
        glfwSwapBuffers(window);
    }

    //����
    glfwTerminate();
    return 0;
}


//���ڴ�С�ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (width == 0 || height == 0) return;//����С�����ڵ�ʱ���Զ��˳�����ֹaspectRatio�õ�����Ľ��
    glViewport(0, 0, width, height);

    aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

//���ص�����
void mouse_callback(GLFWwindow* window, double pos_x, double pos_y)
{
    float sensitivity = 0.05f;//������

    if (FirstMouse)
    {
        pos_x = (double)LastX;
        pos_y = (double)LastY;
        FirstMouse = false;
    }

    if (!glfwGetKey(window, GLFW_KEY_LEFT_ALT) && !glfwGetKey(window, GLFW_KEY_RIGHT_ALT))
    {
        float offset_x = ((float)pos_x - LastX);
        float offset_y = (LastY - (float)pos_y);
        LastX = (float)pos_x;
        LastY = (float)pos_y;

        //���¹����ɿ�Alt
        if (IsAlt && IsAltRealise)
        {
            //���¹�Alt���ɿ����Yaw��Pitch����RollBack
            camera.IfRollBack = true;
            IsAlt = false;
            IsAltRealise = false;
        }

        camera.ProcessMouseMove(offset_x, offset_y);
    }
}

//���ֻص�����
void scroll_callback(GLFWwindow* window, double offset_x, double offset_y)
{
    camera.ProcessMouseScroll((float)offset_x, (float)offset_y);
}

//���뺯��
void processInput(GLFWwindow* window)
{
    //���ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    //���space
    CurrentSpacePressd = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
    if (!LastSpacePressd && CurrentSpacePressd)
    {
        IsPaused = !IsPaused;
    }
    LastSpacePressd = CurrentSpacePressd;

    //���Alt
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//��ʾ��겢�ָ����Χ
        IsAlt = true;
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//������겢�����Ļ��Χ�����ڴ�����
        if (IsAlt)
        {
            IsAltRealise = true;//�����¹�Alt֮���ɿ�����Ч(��֤Alt�ɿ�֮���Ա�ĳ��֡��⵽��ʱ�򣬲������Իָ��Ƕ�)
        }
    }

    //���AWSD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessWASD(FORWARD, PeriodTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessWASD(BACKWARD, PeriodTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessWASD(LEFT, PeriodTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessWASD(RIGHT, PeriodTime);
    }

    //������·����
    float RateSpeed = 5.0f * PeriodTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (rate < 4.0f)
        {
            rate += RateSpeed;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (rate > -2.0f)
        {
            rate -= RateSpeed;
        }
    }

    //���F��
    CurrentKeyPressd_F = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);
    if (CurrentKeyPressd_F && !LastKeyPressd_F)
    {
        IsKey_F = !IsKey_F;
    }
    LastKeyPressd_F = CurrentKeyPressd_F;

    //���E��
    CurrentKeyPressd_E = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS);
    if (CurrentKeyPressd_E && !LastKeyPressd_E)
    {
        IsKey_E = !IsKey_E;
    }
    LastKeyPressd_E = CurrentKeyPressd_E;

    //���M��
    CurrentKeyPressd_M = (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS);
    if (CurrentKeyPressd_M && !LastKeyPressd_M)
    {
        IsKey_M = !IsKey_M;
    }
    LastKeyPressd_M = CurrentKeyPressd_M;
}



//������ɫ��: ������ɫ������ÿ�����㣬�������λ���Լ��������ܵ����ԣ�����ɫ�����ߵȣ���/**������ɫ����ֻ�ж��������еĵ㣨һ���Ǳ߽�㣩
//��դ�� : �ڹ�դ���׶Σ�OpenGL ����ݶ������Ļ�ռ�����ȷ�������εı߽磬�����������λ���������ڲ���Ƭ�Σ������أ���λ�á�
//Ƭ����ɫ�� : ÿ�����ɵ�Ƭ�λᴫ�ݸ�Ƭ����ɫ�������н�һ�������������ɫ�͹��յȡ�/**Ƭ����ɫ���������ͼ�ε��������ص�
//**�Ӷ�����ɫ����Ƭ����ɫ�����ݵ������ǲ�ֵ�����(vShader out FragColor(���ж�����ɫ) -> fShader in FragColor(��ÿ�����ص����ɫ))
//gl_PositonҲ���ֵ����Ƭ����ɫ���б�ʹ�ã������ǲ���Ҫ��ʽ�ؽ���gl_Positon

//�����������VAO����OpenGL�и��𽫶��㻺��������VBO���е����ݴ��ݸ�������ɫ��//
//һ��1.����һ��������VBO      2.���߸û�������δ洢����  3.�����ݴ��ݸ�VBO��
//����1.����һ������������VAO  2.����VAO����Ϊ��ǰVAO      3.����VAO��ν�������      4.�����ݴ��ݸ���ɫ��

/*����*/
//һ�������Ʒ�ʽ��������������Խ������
//����������ˣ�������ͬ����ɫ���ȡ����ʽ��ȡ�ٽ�ɫ���ȡ��ֵɫ�飩
//�����༶��Զ������������������Զʱ�����ѻ�ȡ�߷ֱ��ʵ���������Ԥ�ȴ���һ��Mipmap���ɺܶ�ֱ��ʲ����۰��ԭ������ɣ���Ȼ��ȡ�ٽ��ֱ��ʻ�ȡ��ֵ�ֱ���

/*����任*/
//�ֲ�����ϵ -> (ģ�ͱ任) -> ��������ϵ -> (��ͼ�任) -> �۲�����ϵ -> (ͶӰ�任) -> �ü�����ϵ -> (�ü�+͸�ӳ���) -> �淶�豸����ϵ(NDC) -> (�ӿڱ任) -> ��Ļ����ϵ -> ������Ȳ��Ե���Ⱦ����
//����ռ���һ���̶���(0,0,0)Ϊ����ϵԭ�㣬��ͼ�ռ��������λ��Ϊ����ϵԭ�㣬�������ռ䶼�����Կռ�(��������λ�ò���ı�)���ü��ռ�����ԭ������׶���ƽ���غ�(ͨ����������ƽ��ܽӽ�)���Ƿ����Կռ�
//ͶӰ���󽫶������ͼ�ռ�任���ü��ռ�(��һ������ͼ�任),��ͼ�任���������ֵ(z����ֵ)���з���������(ģ�����ԶС��Ч��:����������������ľ������ӣ����������ֵ���Է����Եķ�ʽ��С)���ʲü��ռ���һ�������Կռ�(ԭ����������λ�÷����˱仯)
//�任���ü��ռ�֮�����OpenGL�Զ���ɲü���͸�ӳ���(�����������w)��������任��NDC�У���ʱx/y/z��ֵ����(-1,1)֮��

/*����������w*/
// �ڲü��ռ���w�������λ�á���Զ�ü���ȼ���ó�
//1.͸�ӳ������ڽ��ü��ռ������ת��Ϊ��׼���豸���꣨NDC��ʱ��x��y �� z ��ֱ���� w����һ����ȷ����͸��Ч������ȷ�ԡ�
//2.����жϣ�����Ȳ����У�z �����ֵ���� w ���ʹ�ã�����ȷ���ĸ�ƬԪ�ǿɼ��ġ�ͨ����Ȼ������洢��һ��������ֵ��ȷ�������е����尴�վ�����ȷ��Ⱦ��
//3.�ü����ڲü������У�w �����ж϶����Ƿ��ڲü��ռ����Ч��Χ�ڣ��� - w �� w�������ĳ������� x / w��y / w �� z / w ���������Χ����ö���ᱻ�ü�����

/*���ڡ��ӿ��뻭��*/
//1.�����൱������ӿ��൱������еĻ�ֽ����Ⱦ��ͼ�ξ��ǻ�ֽ�ϵ����� 
//2.͸�Ӿ����еĿ�߱Ⱦ��ǻ�ֽ�����ݵĿ�߱ȣ����ô��ڴ�С����ʱ����������䣬��Ҫ�ڸı䴰�ڴ�С�ĸı�͸�Ӿ����߱�
//������������е����Ͻǵ�aPosƥ�������������(1,1),��������ͼƬ�����Ͻǵ����ؽ��������Ͻ�aPosλ�õ������غϣ��������������(0.5,0.5)�������Ͻ�aPosλ�õ����ػ�������ͼƬ�����ĵ����� 

/*ʹ�ò�ͬ��VAO����ɫ�����Ʋ�ͬͼ��*/
//һ�������ʹ�ò�ͬ�Ķ���������Ʋ�ͬ��ͼ�Σ�����Ҫ���ò�ͬ��VAO
//���������ʹ�ò�ͬ����Ⱦ��ʽ���Ʋ�ͬ��ͼ�Σ�����Ҫ���ò�ͬ����ɫ���������嶼��Shader.h,������Ķ�����ɫ����Ƭ����ɫ��������ͬ��
//���������������ɫ��֮�以���������ɫ�������еı���ֵ�����̳У����û������ĳ����ɫ�������еı��������������ΪĬ��ֵ0������ʣ�
//�ġ����Ƶڶ���ͼ��ʱ����Ҫ�ڶ�����ͬ��ģ�;���model2���õڶ�����ɫ��myshader2�����䴫����ɫ�����򣬵���ͼ�����͸�Ӿ�����Ҫ���ⶨ�壬ֱ����view1��projection1���ɣ��󶨵ڶ���ͼ�ε�VAO2���л���

/*��Ȼ���*/
//1.��Ȳ�����Ƭ����ɫ��֮��������ɫд����ɫ����֮ǰ����
//2.��Ȳ��Խ���ǰƬ�ε����ֵ����Ȼ����е����ֵ���бȽϣ�ͨ����Ȳ��Ե�Ƭ�����ֵ�����µ���Ȼ������� 
//3.���ղ�����Ȼ������е�Ƭ�β��ᱻ��Ⱦ�������ڵ������������Լ�������Ƭ�ε�ȫ�����ݣ����á���ǰ��Ȳ��ԡ��Ż���
//4.���ֵ���ȣ����۲�ռ��е�zֵ��������׶���ƽ���Զƽ���м��ĳ��ֵ���任����Ȼ������У��任��0.0��1.0֮�䣩����ʹ�÷����Է��̣�����Ļ���Ļ���z���ȸ��ߣ�����������ǰ���ϵ��

/*��ɫ���塢��Ȳ��ԡ�ģ�����*/
//1.��ɫ�������Ȳ��Եĸ���ͨ����������Ƭ����Ⱦ���֮��ͳһ���еģ����ֲ���ƽ�н��У�ֻ�����ֲ��Զ�ͨ����Ƭ�βŻᱻ������ɫ��������
//2.ģ�����������ÿһ��������Ⱦ�󣨵õ�Ƭ�κ󣩽��еġ�����Ⱦһ������ͽ���һ��ģ����ԡ�������Ⱦ�����幫��һ��ģ�建����
//3.ģ�建��������Ļ���Ӧ����Ļ��ÿһ�����ص㶼��ģ�建�����д���һ��8λ��ģ��ֵ��
//5.����������Ļ�����½ǻ��ƣ��������ģ��ֵҲ��ģ�建����(��Ӧ����Ļ����λ��)�����½ǡ�ģ����ʱҲֻ��������ڻ������ж�Ӧ��ģ��ֵ��
//6.ģ�建�����е�ģ��ֵĬ��Ϊ0
//7.ģ�建��������glClear(GL_STENCIL_BUFFER_BIT)���ܵ�ģ�������Ӱ�죬����Ѿ�������glStencilMask(0x00)�����޷�����
//8.ģ����ԣ�����ģ����� -> ����ģ�庯��(��������ģ����Ե�����)��ģ�����(������θ���ģ��ֵ) -> ����ģ������(����ģ��ֵ����Щλ���޸ģ�ģ�庯������������ļ�λ������) -> ��Ⱦ����(��ʱģ�建���������ģ����ԺͲ�������)
//9.���������������Ⱦ��֮������Ⱦ�߿���������ʹ�߿򲻻ᱻ��������(���ֵ��С��)����
//10.����Ȳ��ԡ�ģ������йصĲ���(���ò�����Func���������������OP����)����ȫ�ֵģ���ǰ���ĸ�֡�����������ĸ�֡�����¹���
//11.����glStencilOp()��
//(*��������͸���ڵ��￴ģ��) 
//(1).������Ⱦģ�ͺ���Ⱦ�ڵ���ʱ����ʱģ�͵�ģ�������ȼ���ͨ�������õ��ǵ������������ʵ�����������Ҫ����ΪGL_REPLACE���ܽ�ģ�Ͷ�Ӧλ�õ�ģ��ֵ����Ϊ1
//(2).������Ⱦ�ڵ������Ⱦģ��ʱ����ʱģ��ֻ��ͨ��ģ�����޷�ͨ����ȼ��,���õ��ǵڶ����������ʵڶ���������Ҫ����ΪGL_REPLACE���ܽ�ģ�Ͷ�Ӧλ�õ�ģ��ֵ����Ϊ1��������ó�GL_KEEP����ģ�Ͷ�Ӧλ��ģ��ֵ��Ϊ0                       
//(3).���벻����ģ�ͺ��ڵ������Ⱦ˳����Ȼ�ܴﵽ��Ҫ�Ľ������ڶ����͵���������������ΪGL_REPLACE

/*��ϲ���*/
//1.����ȫ͸������(������ı߿�)������Ҫ��ϲ��ԣ�ֱ�Ӷ�����Ӧ͸���ȵ�Ƭ�μ���
//2.���ڰ�͸�����壺ʹ�û�ϲ��ԣ�ȷ������Ⱦ��͸������������Ⱦ��͸�������壬����ȾԶ���İ�͸����������Ⱦ�����İ�͸������

/*֡����*/
//Ĭ��֡���壺�ɴ���ϵͳ�ṩ��֡���壬����Ļ��ʾ�����������Ⱦ��������һ��
//����֡���壺�ֶ�����G_Buffer����ֱ������Ļ��ʾ�����������м���Ⱦ����ʵ��������ȾЧ��
// һ������ʹ��Ĭ��֡���彫���ֱ����Ⱦ����Ļ��
// ��������ʹ��ʹ������֡���彫���(���Ǳ�����Ļ�ϻ���ֵ�����)����Ⱦ���Խ�G_Buffer����ɫ��������(��ʱ���൱�ڰ�ԭ��������Ļ�ϵ���������һ����ͼ)��
// �ٽ�G_Buffer�е����ݴ���֮������Ⱦ����Ļ��(��Ҫ��һ���������е������ͼ)
// 
//֡��������
//�� OpenGL �У�ͨ����Ҫ�� GPU ��׼����������λ�ã�
//һ������λ����Ϊ����Ŀ�꣺���ڶ�ȡ���ݣ�ͨ�����ں������㡢����ʹ��֮ǰ��Ⱦ����ĳ����С����磬����Ч����ʹ����Ⱦ���������Ϊ���롣
//һ������λ����Ϊд��Ŀ�꣺��֡����ĸ���������������ڴ洢��Ⱦ�����Ľ������Ⱦ��ͼ��ᱻд�뵽��������У�������ֱ����ʾ����Ļ�ϡ�
//��һ�� ֡�������Framebuffer, FBO�� �У������ж��������������
//�������������������ɫ�������������������ģ������������һ����ȸ��� �� һ��ģ�帽����
// 
//��������
//���������Է�Ϊ�������ͣ����磺��ɫ�������������������ģ��������
//������ֻ��һ��ͳ�ƣ�ʵ���Ͼ������ɫ���ݡ�����������ݡ�ģ���������ݶ������������µ���ɫ�����������������ģ����������
//��ȸ����������������ģ�帽����ģ����������
//��ȸ��� or ģ�帽��������Ȼ�������ģ�建����������ֻ��������ȼ���ģ���⣬���������޷�������
//��������� or ģ�������������������������ܴ洢�����Ϣ��ģ����Ϣ�����ܱ�����ʹ�ã���ɺ���Ч��

/*��������ͼ*/
//��պУ�
//һ��ʹ��glm::mat4(glm::mat3(view))ȥ��ƽ�ƣ����������Ƿ��ƶ����������λ��ʼ��������ԭ�㣬ʼ������պ��ڲ�
//����һֱ��һ�������ƶ��������壬��ʹ��׶�����õúܴ�������Ȼ�п����Ƴ���պеķ�Χ�����������ǽ�������պе����д�룬
//��պл�ʼ�ձ������ڳ�������Զ��(���ֵΪ1.0����Զ���)�������Ḳ���������壬���Լ�ʹ�����Ƴ�����պеķ�Χ����պ���Ȼ������Ⱦʱ��������������
//����������ǵ�һ����Ⱦ��պ��ҽ������д�룬�򲻻Ḳ�Ǻ���Ⱦ�����壬���ں�����Ⱦ��������ʱ����Ҫ�������ڵ�����պ�Ƭ�Σ�
//�������ǿ��������Ⱦ��պв���OpenGLʼ����Ϊ��պе����ֵΪ1.0(��Զ���ֵ)
//�ġ������������������˳����Ⱦ��պ�(�Ż�����Ƭ����Ⱦ��)������Ҫ��ʱ����ȼ�⺯����ΪLEAQAL,��֤��պн�����δ��Ļ����Ⱦ�������λ��ͨ����ȼ�⣬ͬʱ����Щλ�����ֵ��Ϊ1.0(��Ȼ��������1.0)

/*������ɫ��*/
//һ��������ɫ�����ڶ�����ɫ����Ƭ����ɫ��֮��(��û����ʽ��д������ɫ���򲻻�ʹ��)�����������µĶ�����޸����е�ͼԪ
//����layout () in�����Ӷ�����ɫ�����յ�ͼԪ���ͣ�layout () out���������Ƭ����ɫ����ͼԪ���ͺ���󶥵���
//����EmitVertex():����µĶ���;EndPrimitive():���б�EmitVertex����Ķ���ᱻ�ϳ�Ϊ������layout () out��ָ������ͼ��
//�ġ�������������˼�����ɫ������ô�Ӷ�����ɫ����out�ı�������Ҫ����������ɫ���ļ�Ӵ��ݲ�����Ƭ����ɫ���н���
//�塢�Ӷ�����ɫ���򼸺���ɫ���������ݶ���һ��һ�鴫�ݵģ�ÿһ�����һ��ͼԪ���е����ж�������(λ�á������������������)
//�����������Ϊ������ɫ�����ְ�����һ��ͼԪ������Ŀ�����Ķ�����ɫ��
//�ߡ�������ɫ���е�gl_Position��������ĸ��ռ�ģ�������ɫ�������ĸ��ռ������һ������
//�ˡ������ڶ�����ɫ���н�gl_Positon�任���ü��ռ䣬Ҳ�������ڶ�����ɫ���н�gl_Position�任���۲�ռ�Ȼ���ڼ�����ɫ���н������������ٽ���任���ü��ռ�
/*�š����Ҫ�ڼ�����ɫ����ʹ�ö���(gl_Position)֮������Թ�ϵ������������(�編��)ʱ��Ҫȷ���Ӷ�����ɫ���л�ȡ��gl_Positon�ǹ۲�ռ��е����������껹Ҫ�ٰ�gl_Positonת�����ü��ռ���*/

/*ʵ����*/
//һ��uniform����������ʵ���й���(��ʹ��ʵ��������n������:ÿһ֡��glsl�ϴ�n��uniformֵ;ʹ��ʵ��������n������:ÿһ֡��glsl�ϴ�1��uniformֵ)
//�����ڻ��ƶ��ʵ��ʱ���ϣ���ò�ͬ��model�������λ�õ��������ʹ��uniform����Ļ����ܻᳬ�����uniform��������Ӧ����ʵ��������(��model����Ϊ��������:���������ڻ���һ���µ�ʵ��ʱ�Żᱻ����)
//����ʹ��ʵ���������ģ�;���(����������)���ݸ���������ʱ��������Ⱦѭ�������





//std::vector<Vertex> BoxVertices = {
//    //aPos                  //aNormal           //aTexture
//    // ���� (z = -0.5)
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},// ���½�
//    {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},// ���½�
//    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},// ���Ͻ�
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},// ���½�
//
//    // ǰ�� (z = 0.5)
//    {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// ���½�
//    {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},// ���½�
//    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//    {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},// ���Ͻ�
//    {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// ���½�
//
//    // ���� (x = -0.5)
//    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
//    {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},// ���ҽ�
//    {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
//    {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
//    {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// �����
//    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
//
//    // ���� (x = 0.5)
//    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
//    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},// ���ҽ�
//    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
//    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
//    {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// �����
//    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
//
//    // ���� (y = -0.5)
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},// ���Ͻ�
//    {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},// ���Ͻ�
//    {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
//    {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
//    {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},// ���½�
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},// ���Ͻ�
//
//    // ���� (y = 0.5)
//    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},// ���Ͻ�
//    {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},// ���Ͻ�
//    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
//    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
//    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},// ���½�
//    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}// ���Ͻ�
//};
//
//std::vector<unsigned int> BoxIndices = {
//    // ����
//    0, 1, 2,
//    3, 4, 5,
//    // ǰ��
//    6, 7, 8,
//    9, 10, 11,
//    // ����
//    12, 13, 14,
//    15, 16, 17,
//    // ����
//    18, 19, 20,
//    21, 22, 23,
//    // ����
//    24, 25, 26,
//    27, 28, 29,
//    // ����
//    30, 31, 32,
//    33, 34, 35
//};
// 
//std::vector<Texture> BoxTextures = 
//{
//    {BoxTexture1,"DiffuseColorSampler","source/Makima.jpg"},//��ͼ��ַ�ɸ��ɲ���
//    {BoxTexture2,"DiffuseColorSampler","source/Makima3.jpg"},
//    {BoxTexture1,"SpecularColorSampler","source/Makima.jpg"},
//    {BoxTexture2,"SpecularColorSampler","source/Makima3.jpg"}
//};
// 
// 
//     /*�������*/
//        //������ɫ������
//BoxShader.Use();
//
////��������ɼ���
//RateControl(window, rate);
//BoxShader.SetFloat("rate", rate);
//
////ģ�;���
//glm::mat4 model1 = glm::mat4(1.0f);//ÿһ֡��Ҫ���´���model�������ʹ����һ֡�Ѿ��任����model
//model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, 0.5f));
////model1 = glm::rotate(model1, PassedTime * glm::radians(-50.0f), glm::vec3(1.0f, 1.0f, -1.0f));//������Ӧ����ת����Ӧ��λ��
//model1 = glm::scale(model1, glm::vec3(0.5f));
//BoxShader.setMat4("model", model1);
////���߾���
//BoxShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model1))));//ģ�;������Ͻ�3x3���ֵ�������ת�þ���
//BoxShader.setMat4("view", view);
//BoxShader.setMat4("projection", projection);
//BoxShader.SetVec3("ViewPos", CameraPos);
//
////���Դ
//BoxShader.SetVec3("pointLight.Pos", LightPos);
//BoxShader.SetVec3("pointLight.Ambient", LightColor* glm::vec3(0.2f));
//BoxShader.SetVec3("pointLight.Diffuse", LightColor* glm::vec3(0.5f));
//BoxShader.SetVec3("pointLight.Specular", LightColor* glm::vec3(1.0f));
//BoxShader.SetFloat("pointLight.constant", 1.0f);
//BoxShader.SetFloat("pointLight.linear", 0.35f);
//BoxShader.SetFloat("pointLight.quadratic", 0.44f);
////�����
//BoxShader.SetVec3("directLight.Dir", LightDirection);
//BoxShader.SetVec3("directLight.Ambient", DirectColor* glm::vec3(0.2f));
//BoxShader.SetVec3("directLight.Diffuse", DirectColor* glm::vec3(0.5f));
//BoxShader.SetVec3("directLight.Specular", DirectColor* glm::vec3(1.0f));
////�ֵ��
//BoxShader.SetVec3("flashLight.Pos", CameraPos);
//BoxShader.SetVec3("flashLight.Dir", CameraFront);
//BoxShader.SetVec3("flashLight.Diffuse", FlashColor* glm::vec3(0.5f));
//BoxShader.SetVec3("flashLight.Specular", FlashColor* glm::vec3(1.0f));
//BoxShader.SetFloat("flashLight.InnerAngle", glm::cos(glm::radians(5.0f)));
//BoxShader.SetFloat("flashLight.OuterAngle", glm::cos(glm::radians(10.0f)));
//BoxShader.SetFloat("flashLight.constant", 1.0f);
//BoxShader.SetFloat("flashLight.linear", 0.35f);
//BoxShader.SetFloat("flashLight.quadratic", 0.44f);
////���ò���
//BoxShader.SetFloat("material.Shininess", 32.0f);
//
////ģ����
//glStencilFunc(GL_ALWAYS, 1, 0xFF);
//glStencilMask(0xFF);//������������Ӧ�Ļ�����ģ��ֵ����Ϊ1
////��ʱ����ģ�建�����У������ӵĶ�Ӧλ�õ�ģ��ֵΪ1������λ��Ϊ0
//
////����  
//BoxMesh.Draw(BoxShader);