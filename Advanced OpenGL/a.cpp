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
#include "HeadFile/Camera.h"
#include "HeadFile/TextureLoad.h"
#include "HeadFile/MeshDraw/MeshSource.h"
#include "HeadFile/MeshDraw/MeshObjectDraw.h"
#include "HeadFile/ModelDraw_Object/ObjectDraw.h"
#include "HeadFile/ModelDraw_Person/PersonDraw.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



int WindowWidth = 1200;//窗口大小
int WindowHeight = 900;
int WindowPos_X = 300;//窗口(左上角)生成位置
int WindowPos_Y = 100;
float aspectRatio = (float)(WindowWidth / WindowHeight);//窗口长宽比
bool IsPaused = false;//是否暂停
bool LastSpacePressd = false;//检测上一帧是否按下space
bool CurrentSpacePressd = false;//检测当前帧是否按下space
bool IsKey_F = false;//是否按下F键
bool LastKeyPressd_F = false;//检测上一帧是否按下F键
bool CurrentKeyPressd_F = false;//检测当前帧是否按下F键
bool IsKey_E = false;//是否按下E键
bool LastKeyPressd_E = false;//检测上一帧是否按下E键
bool CurrentKeyPressd_E = false;//检测当前帧是否按下E键
bool IsKey_M = false;//是否按下M键
bool LastKeyPressd_M = false;//检测上一帧是否按下M键
bool CurrentKeyPressd_M = false;//检测当前帧是否按下M键
bool IsAlt = false;//是否按下Alt
bool IsAltRealise = false;//是否松开Alt
float LastTime = 0.0f;//上一帧时间
float CurrentTime = 0.0f;//当前帧时间
float PassedTime = 0.0f;//非暂停时经过的时间
float PeriodTime = 0.0f;//每两帧之间的时间
int FPSCount = 0;//帧数目
float OneSecond = 0.0f;//记录一秒钟
float LastX = 960;//鼠标初始位置（渲染窗口中心）
float LastY = 540;
bool FirstMouse = true;//是否首次获取鼠标输入
float rate = 0.0f;//纹理可见度

//摄像机设置
Camera camera(glm::vec3(0.0f,0.0f,2.0f));
glm::vec3 CameraPos = camera.Pos;
glm::vec3 CameraFront = camera.Front;
glm::vec3 CameraUP = camera.Up;

//光照设置
//glm::vec3 LightColor(sin(glfwGetTime() * 1.0f)/2.0f+0.5f,sin(glfwGetTime() * 1.2f)/2.0f+0.5f,sin(glfwGetTime() * 1.5f)/2.0f+0.5f);
//点光源
glm::vec3 PointLightColor;//点光源颜色
glm::vec3 PointLightPos;//点光源位置
//平行光
glm::vec3 DirectLightDirection;//平行光方向
glm::vec3 DirectLightColor;//平行光颜色
//手电光
glm::vec3 FlashLightColor;//手电光颜色
float InnerAngle;//手电光内圈角度（余弦值）
float OuterAngle;//手电光外圈角度（余弦值）


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

    //创建一个窗口
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, WindowPos_X, WindowPos_Y);//设置窗口位置

    //初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置视口
    glViewport(0, 0, WindowWidth, WindowHeight);

    //设置渲染模式
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);//线框模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式

    //启用深度测试、模板测试（全局状态，不管是默认帧缓冲还是自建帧缓冲都会启用测试）
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP,GL_REPLACE,GL_REPLACE);//模板检测失败时模板值不变，模板检查通过但深度检测失败时模板值替换为参考值，都通过时模板值替换为参考值

    //启用面剔除
    //glEnable(GL_CULL_FACE);

    //调用窗口大小回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//每当用户手动调整窗口大小时就会调用回调函数(GLFW会自动检测是否需要调用函数，所以不用写在循环内部)
    //即使没有这个函数GLFW也允许用户手动调节窗口大小，但不会帮我们调节视口大小和其他相关配置

    //鼠标检测
    //glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window,mouse_callback);//glfw自动捕获鼠标位置并传递给mouse_callback
    glfwSetScrollCallback(window,scroll_callback);//捕获滚轮移动增量并传递给scroll_callback

    //设置着色器
    //Shader BoxShader("ShaderSource/vShader_Box.glsl", "ShaderSource/fShader_Box.glsl");
    Shader WallShader("ShaderSource/vShader_Wall.glsl", "ShaderSource/fShader_Wall.glsl");
    Shader LightBallShader("ShaderSource/vShader_Light.glsl", "ShaderSource/fShader_Light.glsl");
    Shader GrassShader("ShaderSource/vShader_Grass.glsl", "ShaderSource/fShader_Grass.glsl");
    Shader WindowShader("ShaderSource/vShader_Window.glsl", "ShaderSource/fShader_Window.glsl");
    Shader ScreenShader("ShaderSource/vShader_Screen.glsl", "ShaderSource/fShader_Screen.glsl");
    Shader SkyBoxShader("ShaderSource/vShader_SkyBox.glsl", "ShaderSource/fShader_SkyBox.glsl");
    Shader ModelShader_Knight("ShaderSource/vShader_Model.glsl", "ShaderSource/fShader_Knight.glsl");
    Shader ModelShader_JinXi("ShaderSource/vShader_Model.glsl", "ShaderSource/fShader_PersonModel.glsl");
    Shader FrameShader("ShaderSource/vShader_Frame.glsl", "ShaderSource/fShader_Frame.glsl");
    Shader DiscoBallShader("ShaderSource/vShader_DiscoBall.glsl", "ShaderSource/fShader_DiscoBall.glsl");

    //设置纹理
    //unsigned int BoxTexture1 = LoadTexture((std::filesystem::current_path() / "source/Makima.jpg").string().c_str());//确保c++17以上
    //unsigned int BoxTexture2 = LoadTexture((std::filesystem::current_path()   / "source/Makima3.jpg").string().c_str());
    unsigned int WallTexture1 = LoadTexture((std::filesystem::current_path() / "source/Wall.png").string().c_str());
    unsigned int WallTexture2 = LoadTexture((std::filesystem::current_path() / "source/Specular Map.png").string().c_str());
    unsigned int WallTexture3 = LoadTexture((std::filesystem::current_path() / "source/Code.jpg").string().c_str());
    unsigned int GrassTexture1 = LoadTexture((std::filesystem::current_path() / "source/grass.png").string().c_str());
    unsigned int WindowTexture1 = LoadTexture((std::filesystem::current_path() / "source/window.png").string().c_str());

    //设置立方体贴图纹理
    std::vector<std::string> SkyBoxFaces_Path = {
        (std::filesystem::current_path() / "source/skybox/skybox/right.jpg").string().c_str(),
        (std::filesystem::current_path() / "source/skybox/skybox/left.jpg").string().c_str(),
        (std::filesystem::current_path() / "source/skybox/skybox/top.jpg").string().c_str(),
        (std::filesystem::current_path() / "source/skybox/skybox/bottom.jpg").string().c_str(),
        (std::filesystem::current_path() / "source/skybox/skybox/front.jpg").string().c_str(),
        (std::filesystem::current_path() / "source/skybox/skybox/back.jpg").string().c_str()
    };
    unsigned int SkyBoxCMTexture = LoadCubeTexture(SkyBoxFaces_Path);


    //网格纹理
    std::vector<Texture> WallTextures =
    {
        {WallTexture1,"DiffuseColorSampler","source/Wall.png"},
        {WallTexture2,"SpecularColorSampler","source/Specular Map.png"},
        {WallTexture3,"EmissionColorSampler","source/Code.jpg"}
    };
    std::vector<Texture> GrassTextures = {
        {GrassTexture1,"DiffuseColorSampler","source/grass.png"}
    };
    std::vector<Texture> WindowTextures = {
        {WindowTexture1,"DiffuseColorSampler","source/window.png"}
    };
    std::vector<Texture> ScreenTextures = {};
    std::vector<Texture> SkyBoxTextures = {};

    //加载网格
    //Mesh BoxMesh(BoxVertices,BoxIndices,BoxTextures);
    Mesh WallMesh(WallVertices,WallIndices,WallTextures);
    Mesh GrassMesh(SquareVertices,SquareIndices,GrassTextures);
    Mesh WindowMesh(SquareVertices,SquareIndices,WindowTextures);
    Mesh ScreenMesh(ScreenVertices,SquareIndices,ScreenTextures);
    Mesh SkyBoxMesh(CubeVertices,CubeIndices,SkyBoxTextures);

    //加载模型
    Model PersonModel_Knight((std::filesystem::current_path() / "source/nanosuit_reflection/nanosuit.obj").string());
    Model PersonModel_JinXi((std::filesystem::current_path() / "source/JinXi/01.fbx").string());
    Model LightBallModel((std::filesystem::current_path() / "source/Hollow Sphere/linked_star.obj").string());
    Model DiscoBallModel((std::filesystem::current_path() / "source/Ball/DiscoBall.fbx").string());
    


    //创建帧缓冲
    unsigned int G_Buffer;
    glGenFramebuffers(1,&G_Buffer);//创建帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER,G_Buffer);//绑定G_Buffer
    //创建颜色纹理附件
    unsigned int ColorTexAttachment;
    glGenTextures(1,&ColorTexAttachment);//创建颜色纹理附件
    glBindTexture(GL_TEXTURE_2D,ColorTexAttachment);//绑定
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);//创建纹理但不初始化
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,ColorTexAttachment,0);//把颜色纹理附件附加到当前绑定的G_Buffer上
    //创建渲染缓冲对象(深度模板附件)
    unsigned int RBO;
    glGenRenderbuffers(1,&RBO);//创建渲染缓冲
    glBindRenderbuffer(GL_RENDERBUFFER,RBO);//绑定
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,WindowWidth,WindowHeight);//为渲染缓冲对象分配存储空间(深度缓冲区24位，模板缓冲区8位)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO);//把深度模板附件附加到当前绑定的G_Buffer上
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)//检测G_Buffer是否完整
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);//解绑G_Buffer


    //ModelDraw_Person
    PersonModelDraw Knight(ModelShader_Knight, PersonModel_Knight, &FrameShader);
    PersonModelDraw JinXi(ModelShader_JinXi, PersonModel_JinXi, &FrameShader);
    ObjectModelDrawWL DiscoBall(DiscoBallShader,DiscoBallModel,&FrameShader);
    ObjectModelDrawNL LightBall(LightBallShader,LightBallModel,&FrameShader);

    //MeshDraw
    ObjectMeshDrawWL Wall(WallShader, WallMesh);
    ObjectMeshDrawNL Grass(GrassShader,GrassMesh);
    ObjectMeshDrawNL Window(WindowShader,WindowMesh);
    ObjectMeshDrawNL SkyBox(SkyBoxShader,SkyBoxMesh);


//渲染循环
    while (!glfwWindowShouldClose(window))
    {
    //捕获处理输入事件
        glfwPollEvents();//触发回调函数

    //检查具体输入状态
        processInput(window);

    //获取每帧时间
        CurrentTime = (float)glfwGetTime();
        PeriodTime = CurrentTime - LastTime;
        if (!IsPaused)
        {
            PassedTime += CurrentTime - LastTime;
        }
        LastTime = CurrentTime;

    //计算帧率
        FPSCount++;
        OneSecond += PeriodTime;
        if (OneSecond >= 1.0f)
        {
            float FPS = (float)FPSCount / OneSecond;
            std::string Title = "FPS:  " + std::to_string(FPS);
            glfwSetWindowTitle(window,Title.c_str());
            OneSecond = 0.0f;
            FPSCount = 0;
        }

    //启用帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER,G_Buffer);

    //全局状态量设置
        /*设置窗口背景颜色并在每次循环后重新渲染(清除颜色缓冲和深度缓冲)*/
        glClearColor(0.678f, 0.847f, 0.902f, 1.0f);
        glStencilMask(0xFF);//确保模板缓冲区能正常清空
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

    //光照设置
        /*点光源*/
        //PointLightColor(sin(glfwGetTime() * 1.0f)/2.0f+0.5f,sin(glfwGetTime() * 1.2f)/2.0f+0.5f,sin(glfwGetTime() * 1.5f)/2.0f+0.5f);
        PointLightColor = glm::vec3(1.0f);//点光源颜色
        PointLightPos = glm::vec3(0.9f * sin(PassedTime), 0.9f * cos(PassedTime) + 0.5f, sin(PassedTime) + 2.0f);//点光源位置
        /*平行光*/
        DirectLightDirection = glm::vec3(1.0f, -1.0f, -1.0f);//平行光方向
        DirectLightColor = glm::vec3(1.0f);//平行光颜色
        /*手电光*/
        FlashLightColor = glm::vec3(0.0f);//手电光颜色
        InnerAngle = glm::cos(glm::radians(5.0f - rate));//手电光内圈角度（余弦值）
        OuterAngle = glm::cos(glm::radians(10.0f + rate));//手电光外圈角度（余弦值）

    //变换矩阵
        /*ViewMatrix*/
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        /*PorjectMatrix*/
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);//视野变小时，物体将占据摄像机（屏幕）更多位置，看起来变得更大


    //人物模型绘制
        /*武士模型绘制*/
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxCMTexture);
        //变换矩阵
        glm::mat4 model_Knight = glm::mat4(1.0f);
        model_Knight = glm::translate(model_Knight, glm::vec3(0.5f, -1.0f, 1.0f));
        model_Knight = glm::scale(model_Knight, glm::vec3(0.1f));
        //模板测试
        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 0b00000001, 0xFF);//模板检测总是通过，且深度检测也能通过，根据glStencilOp第三个参数，该图形的模板值将被替换为00000001
        glStencilMask(0b00000001);//将该物体所对应的缓冲区模板值更新为00000001；此时整个模板缓冲区中：该物体所对应位置的模板值为00000001、其余位置为0
        //绘制
        Knight.GetMatrix(model_Knight, view, projection);
        Knight.BasicShaderSet();
        Knight.KinghtDraw();


        /*今汐模型绘制*/
        //变换矩阵
        glm::mat4 model_JinXi = glm::mat4(1.0f);
        model_JinXi = glm::translate(model_JinXi, glm::vec3(-0.5f, -1.0f, 1.0f));
        model_JinXi = glm::scale(model_JinXi, glm::vec3(0.08f));
        //模板测试
        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 0b00000010, 0xFF);
        glStencilMask(0b00000010);//将该物体所对应的缓冲区模板值更新为00000010//此时整个模板缓冲区中：该model所对应位置的模板值为00000010、另一个model所对应位置为00000001，若两者有重叠部分，则重叠部分为00000011
        //绘制
        JinXi.GetMatrix(model_JinXi,view,projection);
        JinXi.BasicShaderSet();
        JinXi.JinXiDraw();
        


    //物体模型绘制
        /*Disco球绘制*/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxCMTexture);
        //变换矩阵
        glm::mat4 model_DiscoBall = glm::mat4(1.0f);
        model_DiscoBall = glm::translate(model_DiscoBall, glm::vec3(0.0f,1.0f,1.0f));
        model_DiscoBall = glm::scale(model_DiscoBall, glm::vec3(0.005f, 0.005f, 0.005f));
        model_DiscoBall = glm::rotate(model_DiscoBall,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
        model_DiscoBall = glm::rotate(model_DiscoBall, PassedTime * glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //模板测试
        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);//光源通过检测，不丢弃片段
        glStencilMask(0x00);//不更新该物体所对应的模板缓冲区//此时整个模板缓冲区中：只有两个model所在位置有非零的模板值，其余部分均为0
        //绘制
        DiscoBall.GetMatrix(model_DiscoBall,view,projection);
        DiscoBall.BasicShaderSet();
        DiscoBall.DiscoBallDraw();
        

        /*光源绘制*/
        //变换矩阵
        glm::mat4 model_LightBall = glm::mat4(1.0f);
        model_LightBall = glm::translate(model_LightBall, PointLightPos);
        model_LightBall = glm::scale(model_LightBall, glm::vec3(0.002f, 0.002f, 0.002f));
        //模板测试
        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);//光源通过检测，不丢弃片段
        glStencilMask(0x00);//不更新该物体所对应的模板缓冲区//此时整个模板缓冲区中：只有两个model所在位置有非零的模板值，其余部分均为0
        //绘制
        LightBall.GetMatrix(model_LightBall, view, projection);
        LightBall.BasicShaderSet();
        LightBall.LightBallDraw();



    //网格物体绘制
        /*墙体绘制*/
        glm::mat4 model_Wall = glm::mat4(1.0f);
        model_Wall = glm::translate(model_Wall, glm::vec3(0.0f, -0.2f, 1.5f));
        model_Wall = glm::scale(model_Wall, glm::vec3(1.5f));
        //模板测试
        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
        glStencilFunc(GL_ALWAYS,1,0xFF);//墙壁通过检测，不丢弃片段
        glStencilMask(0x00);//不更新该物体所对应的模板缓冲区//此时整个模板缓冲区中：只有两个model所在位置有非零的模板值，其余部分均为0
        //绘制
        Wall.GetMatrix(model_Wall,view,projection);
        Wall.BasicShaderSet();
        Wall.WallDraw();


        /*环境绘制*/
        if (IsKey_E)
        {
            /*草地*/
            //对于全透明物体(草纹理的边框)：不需要混合测试，直接丢弃相应透明度的片段即可
            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);//通过检测，不丢弃片段
            glStencilMask(0x00);//不更新该物体所对应的模板缓冲区
            std::vector<glm::vec3> vegetation;
            vegetation.push_back(glm::vec3(-0.7f, -0.5f, 1.0f));
            vegetation.push_back(glm::vec3(0.7f, -0.5f, 1.0f));
            vegetation.push_back(glm::vec3(0.0f, -0.5f, 1.0f));

            glm::mat4 model_Grass = glm::mat4(1.0f);

            for (unsigned int i = 0; i < vegetation.size(); i++)
            {
                model_Grass = glm::mat4(1.0f);
                model_Grass = glm::translate(model_Grass,vegetation[i]);
                Grass.GetMatrix(model_Grass,view,projection);
                Grass.BasicShaderSet();
                Grass.GrassDraw();
            }

            /*窗户*/
            //对于半透明物体：使用混合测试，确保先渲染不透明的物体再渲染半透明的物体，先渲染远处的半透明物体再渲染近处的半透明物体
            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);//通过检测，不丢弃片段
            glStencilMask(0x00);//不更新该物体所对应的模板缓冲区
            glEnable(GL_BLEND);//启用混合
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            std::vector<glm::vec3> vegetation2;
            vegetation2.push_back(glm::vec3(-0.35f, -0.5f, 1.0f));
            vegetation2.push_back(glm::vec3(0.35f, -0.5f, 1.5f));
            vegetation2.push_back(glm::vec3(0.0f, -0.5f, 2.0f));
            std::map<float,glm::vec3> Sorted;
            for (unsigned int i = 0; i < vegetation2.size(); i++)
            {
                float distence = glm::length(CameraPos - vegetation2[i]);
                Sorted[distence] = vegetation2[i];
            }
            glm::mat4 model_Window = glm::mat4(1.0f);

            for (auto it = Sorted.rbegin(); it != Sorted.rend(); ++it)
            {
                model_Window = glm::mat4(1.0f);
                model_Window = glm::translate(model_Window,it->second);
                Window.GetMatrix(model_Window, view, projection);
                Window.BasicShaderSet();
                Window.GrassDraw();
            }
            glDisable(GL_BLEND);
        }
            
        /*天空盒*/
        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);//通过模板检测
        glStencilMask(0x00);//不更新该物体所对应的模板缓冲区
        glDepthFunc(GL_LEQUAL);//深度值小于等于深度缓冲区的片段被保留(已经渲染过其他物体的位置的天空盒片段被丢弃，尚未渲染过其他物体的位置通过深度检测，相应深度缓冲区被设为1.0(但这些地方本来就是1.0因为深度缓冲区默认值为1.0))
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxCMTexture);
        SkyBox.GetMatrix(glm::mat4(1.0f), glm::mat4(glm::mat3(view)), projection);//去除view矩阵中的平移部分，即摄像机始终处于坐标原点(移动时也是)
        SkyBox.BasicShaderSet();
        SkyBox.GrassDraw();
        glDepthFunc(GL_LESS);//重新设置深度检测函数为LESS通过



    //边框绘制
        if (IsKey_F)
        {
            /*武士边框绘制*/
            //变换矩阵
            glm::mat4 Fmodel_Knight = glm::mat4(1.0f);
            Fmodel_Knight = glm::translate(Fmodel_Knight, glm::vec3(0.5f, -1.0f, 1.0f));
            Fmodel_Knight = glm::scale(Fmodel_Knight, glm::vec3(0.1001f));//轮廓的缩放倍数需要只比模型的缩放倍数大一点点即可
            //模板检测
            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
            glStencilFunc(GL_NOTEQUAL, 0b00000001, 0b00000001);//两个model对应位置的模板值分别为00000001、00000010，重叠部分为00000011，凡是第一位不等于1的均被丢弃，故只有该model的边框能通过检测
            glStencilMask(0x00);//不更新该物体所对应的模板缓冲区//此时整个模板缓冲区中：只有两个model所在位置有非零的模板值，其余部分均为0
            //深度检测
            glDisable(GL_DEPTH_TEST);//暂时禁用深度检测，使边框永远能通过深度检测
            //绘制
            Knight.GetFMatrix(Fmodel_Knight);
            Knight.BasicFShaderSet();
            Knight.KnightFrameDraw();
            //恢复深度检测
            glEnable(GL_DEPTH_TEST);


            /*今夕边框绘制*/
            //变换矩阵
            glm::mat4 Fmodel_JinXi = glm::mat4(1.0f);
            Fmodel_JinXi = glm::translate(Fmodel_JinXi, glm::vec3(-0.5f, -1.0f, 1.0f));
            Fmodel_JinXi = glm::scale(Fmodel_JinXi, glm::vec3(0.0801f));
            //模板检测
            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
            glStencilFunc(GL_NOTEQUAL, 0b00000010, 0b00000010);//两个model对应位置的模板值分别为00000001、00000010，重叠部分为00000011，凡是第二位不等于1的均被丢弃，故只有该model的边框能通过检测
            glStencilMask(0x00);//不更新该物体所对应的模板缓冲区//此时整个模板缓冲区中：只有两个model所在位置有非零的模板值，其余部分均为0
            //深度检测
            glDisable(GL_DEPTH_TEST);//暂时禁用深度检测，使边框永远能通过深度检测
            //绘制
            JinXi.GetFMatrix(Fmodel_JinXi);
            JinXi.BasicFShaderSet();
            JinXi.JinXiFrameDraw();
            //恢复深度检测
            glEnable(GL_DEPTH_TEST);
        }


    //屏幕绘制
        glBindFramebuffer(GL_FRAMEBUFFER,0);//解绑G_Buffer，恢复到默认帧缓冲
        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);//通过检测，不丢弃片段
        glStencilMask(0x00);//不更新该物体所对应的模板缓冲区
        glDisable(GL_DEPTH_TEST);//需要暂时禁用深度检测，因为我们希望只及将带有ColorTexAttachment纹理的ScreenMesh绘制到屏幕上，所以不需要深度检测
        glClearColor(1.0f,1.0f,1.0f,1.0f);//没被ScreenMesh渲染的地方将会是白色的
        glClear(GL_COLOR_BUFFER_BIT);

        ScreenShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,ColorTexAttachment);//将ColorTexAttachment绑定到GL_TEXTURE0上
        float SharpenKernel[9] = {
            -1,-1,-1,
            -1, 9,-1,
            -1,-1,-1
        };//锐化卷积核
        float BlurKernel[9] = {
            1.0/16.0, 2.0/16.0, 1.0/16.0,
            2.0/16.0, 4.0/16.0, 2.0/16.0,
            1.0/16.0, 2.0/16.0, 1.0/16.0
        };//模糊卷积核
        float NoKernel[9] = {
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 0.0
        };//不使用任何核效果

        //主屏幕
        glm::mat4 model_Screen = glm::mat4(1.0f);
        ScreenShader.setMat4("model",model_Screen);
        ScreenShader.SetBool("IfKernel",false);
        ScreenShader.SetBool("IfGray", false);
        ScreenShader.SetBool("IfInverse", false);
        ScreenShader.SetInt("ScreenTexture",0);
        ScreenMesh.Draw(ScreenShader);

        //副屏幕
        if (IsKey_M)
        {
            glm::mat4 model_Screen1 = glm::mat4(1.0f);
            model_Screen1 = glm::scale(model_Screen1,glm::vec3(0.25));
            model_Screen1 = glm::translate(model_Screen1,glm::vec3(-2.95f,-3.0f,0.0f));
            ScreenShader.setMat4("model", model_Screen1);
            ScreenShader.SetBool("IfKernel", true);
            ScreenShader.SetBool("IfGray", false);
            ScreenShader.SetBool("IfInverse", false);
            ScreenShader.SetFloatArray("Kernel",SharpenKernel, 9);
            ScreenShader.SetInt("ScreenTexture", 0);
            ScreenMesh.Draw(ScreenShader);
        }
        glEnable(GL_DEPTH_TEST);//恢复深度检测


        //交换缓冲区
        glfwSwapBuffers(window);
    }

    //清理
    glfwTerminate();
    return 0;
}


//窗口大小回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (width == 0 || height == 0) return;//当最小化窗口的时候，自动退出，防止aspectRatio得到错误的结果
    glViewport(0, 0, width, height);

    aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

//鼠标回调函数
void mouse_callback(GLFWwindow* window,double pos_x,double pos_y)
{
    float sensitivity = 0.05f;//灵敏度

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
        
        //按下过并松开Alt
        if (IsAlt && IsAltRealise)
        {
            //按下过Alt并松开后对Yaw、Pitch进行RollBack
            camera.IfRollBack = true;
            IsAlt = false;
            IsAltRealise = false;
        }

        camera.ProcessMouseMove(offset_x,offset_y);
    }
}

//滚轮回调函数
void scroll_callback(GLFWwindow* window,double offset_x,double offset_y)
{
    camera.ProcessMouseScroll((float)offset_x,(float)offset_y);
}

//输入函数
void processInput(GLFWwindow* window)
{
    //检测ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    //检测space
    CurrentSpacePressd = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
    if (!LastSpacePressd && CurrentSpacePressd)
    {
        IsPaused = !IsPaused;
    }
    LastSpacePressd = CurrentSpacePressd;

    //检测Alt
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//显示鼠标并恢复活动范围
        IsAlt = true;
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏鼠标并将鼠标的活动范围限制在窗口内
        if (IsAlt)
        {
            IsAltRealise = true;//当按下过Alt之后，松开才有效(保证Alt松开之后仍被某几帧检测到的时候，不会擅自恢复角度)
        }
    }

    //检测AWSD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)  
    {
        camera.ProcessWASD(FORWARD,PeriodTime);
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

    //检测上下方向键
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

    //检测F键
    CurrentKeyPressd_F = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);
    if (CurrentKeyPressd_F && !LastKeyPressd_F)
    {
        IsKey_F = !IsKey_F;
    }
    LastKeyPressd_F = CurrentKeyPressd_F;

    //检测E键
    CurrentKeyPressd_E = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS);
    if (CurrentKeyPressd_E && !LastKeyPressd_E)
    {
        IsKey_E = !IsKey_E;
    }
    LastKeyPressd_E = CurrentKeyPressd_E;

    //检测M键
    CurrentKeyPressd_M = (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS);
    if (CurrentKeyPressd_M && !LastKeyPressd_M)
    {
        IsKey_M = !IsKey_M;
    }
    LastKeyPressd_M = CurrentKeyPressd_M;
}



//顶点着色器: 顶点着色器处理每个顶点，并输出其位置以及其他可能的属性（如颜色、法线等）。/**顶点着色器中只有顶点数组中的点（一般是边界点）
//光栅化 : 在光栅化阶段，OpenGL 会根据顶点的屏幕空间坐标确定三角形的边界，并计算出所有位于三角形内部的片段（即像素）的位置。
//片段着色器 : 每个生成的片段会传递给片段着色器，进行进一步处理，如计算颜色和光照等。/**片段着色器中有这个图形的所有像素点
//**从顶点着色器向片段着色器传递的量都是插值后的量(vShader out FragColor(仅有顶点颜色) -> fShader in FragColor(有每个像素点的颜色))

//顶点数组对象（VAO）在OpenGL中负责将顶点缓冲区对象（VBO）中的数据传递给顶点着色器//
//一、1.创建一个缓冲区VBO      2.告诉该缓冲区如何存储数据  3.将数据传递给VBO，
//二、1.创建一个“解释器”VAO  2.将该VAO设置为当前VAO      3.设置VAO如何解释数据      4.将数据传递给着色器

/*纹理*/
//一、纹理环绕方式：处理纹理坐标越界的情况
//二、纹理过滤：决定不同像素色块的取样方式（取临近色块或取插值色块）
//三、多级渐远纹理：当摄像机离物体很远时，很难获取高分辨率的纹理，于是预先创建一个Mipmap（由很多分辨率不断折半的原纹理组成），然后取临近分辨率或取插值分辨率

/*坐标变换*/
//局部坐标系 -> (模型变换) -> 世界坐标系 -> (视图变换) -> 观察坐标系 -> (投影变换) -> 裁剪坐标系 -> (透视除法) -> 规范设备坐标系 -> (视口变换) -> 屏幕坐标系 -> 进行深度测试等渲染过程


/*齐次坐标分量w*/
// 在裁剪空间中w由摄像机位置、近远裁剪面等计算得出
//1.透视除法：在将裁剪空间的坐标转换为标准化设备坐标（NDC）时，x、y 和 z 会分别除以 w。这一步骤确保了透视效果的正确性。
//2.深度判断：在深度测试中，z 坐标的值会与 w 结合使用，帮助确定哪个片元是可见的。通过深度缓冲区存储归一化后的深度值，确保场景中的物体按照距离正确渲染。
//3.裁剪：在裁剪过程中，w 用于判断顶点是否在裁剪空间的有效范围内（即 - w 到 w）。如果某个顶点的 x / w、y / w 或 z / w 超出这个范围，则该顶点会被裁剪掉。

/*窗口、视口与画面*/
//1.窗口相当于相框，视口相当于相框中的画纸，渲染的图形就是画纸上的内容 
//2.透视矩阵中的宽高比就是画纸上内容的宽高比，想让窗口大小调整时画面比例不变，就要在改变窗口大小的改变透视矩阵宽高比
//如果顶点数组中的右上角的aPos匹配的纹理坐标是(1,1),代表纹理图片的右上角的像素将会与右上角aPos位置的像素重合；如果纹理坐标是(0.5,0.5)，则右上角aPos位置的像素会是纹理图片的中心点像素 

/*使用不同的VAO、着色器绘制不同图形*/
//一、如果想使用不同的顶点数组绘制不同的图形，则需要设置不同的VAO
//二、如果想使用不同的渲染方式绘制不同的图形，则需要设置不同的着色器程序（主体都是Shader.h,但具体的顶点着色器和片段着色器有所不同）
//三、创造的两个着色器之间互相独立，着色器程序中的变量值互不继承，如果没有设置某个着色器程序中的变量，则这个变量为默认值0（大概率）
//四、绘制第二个图形时，需要第二个不同的模型矩阵model2并用第二个着色器myshader2来将其传入着色器程序，但视图矩阵和透视矩阵不需要额外定义，直接用view1和projection1即可，绑定第二个图形的VAO2进行绘制

/*深度缓冲*/
//1.深度测试在片段着色器之后、最终颜色写入颜色缓冲之前进行
//2.深度测试将当前片段的深度值跟深度缓冲中的深度值进行比较，通过深度测试的片段深度值被更新到深度缓冲区中 
//3.最终不再深度缓冲区中的片段不会被渲染（即被遮挡），但我们仍计算了其片段的全部数据（可用“提前深度测试”优化）
//4.深度值精度：将观察空间中的z值（介于视锥体近平面和远平面中间的某个值）变换到深度缓冲区中（变换到0.0至1.0之间），常使用非线性方程（离屏幕近的画面z精度更高，更易于区分前后关系）

/*颜色缓冲、深度测试、模板测试*/
//1.颜色缓冲和深度测试的更新通常是在所有片段渲染完成之后统一进行的，两种测试平行进行，只有两种测试都通过的片段才会被放入颜色缓冲区中
//2.模板测试则是在每一个物体渲染后（得到片段后）进行的。即渲染一个物体就进行一次模板测试。所有渲染的物体公用一个模板缓冲区
//3.模板缓冲区和屏幕相对应，屏幕上每一个像素点都在模板缓冲区中存有一个8位的模板值。
//5.若物体在屏幕的左下角绘制，则物体的模板值也在模板缓冲区(对应的屏幕像素位置)的左下角。模板检测时也只检测物体在缓冲区中对应的模板值。
//6.模板缓冲区中的模板值默认为0
//7.模板缓冲区清零glClear(GL_STENCIL_BUFFER_BIT)会受到模板掩码的影响，如果已经设置了glStencilMask(0x00)，则无法清零
//8.模板测试：启动模板测试 -> 设置模板函数(决定进行模板测试的条件)、模板操作(决定如何更新模板值) -> 设置模板掩码(决定模板值的哪些位被修改，模板函数中掩码决定哪几位被测试) -> 渲染物体(此时模板缓冲区会根据模板测试和操作更新)
//9.最好在其他物体渲染完之后再渲染边框，这样才能使边框不会被其他物体(深度值更小的)覆盖
//10.跟深度测试、模板测试有关的操作(启用操作、Func操作、掩码操作、OP操作)都是全局的，当前绑定哪个帧缓冲它就在哪个帧缓冲下工作
//11.关于glStencilOp()：
//(*假设我们透过遮挡物看模型) 
//(1).当先渲染模型后渲染遮挡物时，此时模型的模板检测和深度检测均通过，调用的是第三个参数，故第三个参数需要调整为GL_REPLACE才能将模型对应位置的模板值设置为1
//(2).当先渲染遮挡物后渲染模型时，此时模型只能通过模板检测无法通过深度检测,调用的是第二个参数，故第二个参数需要调整为GL_REPLACE才能将模型对应位置的模板值设置为1。如果设置成GL_KEEP，则模型对应位置模板值仍为0                       
//(3).若想不考虑模型和遮挡物的渲染顺序依然能达到想要的结果，则第二个和第三个参数均设置为GL_REPLACE

/*混合测试*/
//1.对于全透明物体(草纹理的边框)：不需要混合测试，直接丢弃相应透明度的片段即可
//2.对于半透明物体：使用混合测试，确保先渲染不透明的物体再渲染半透明的物体，先渲染远处的半透明物体再渲染近处的半透明物体

/*帧缓冲*/
//默认帧缓冲：由窗口系统提供的帧缓冲，与屏幕显示相关联，是渲染物体的最后一步
//离屏帧缓冲：手动创建G_Buffer，不直接于屏幕显示关联，用于中间渲染过程实现其他渲染效果
// 一、可以使用默认帧缓冲将结果直接渲染到屏幕上
// 二、或者使用使用离屏帧缓冲将结果(就是本来屏幕上会出现的内容)先渲染到自建G_Buffer的颜色纹理附件中(此时就相当于把原本整个屏幕上的内容做成一张贴图)，
// 再将G_Buffer中的内容处理之后再渲染到屏幕上(需要另一个载体来承担这个贴图)
// 
//帧缓冲纹理：
//在 OpenGL 中，通常需要在 GPU 上准备两个纹理位置：
//一个纹理位置作为读入目标：用于读取数据，通常是在后处理、计算、或者使用之前渲染结果的场景中。例如，后处理效果会使用渲染结果纹理作为输入。
//一个纹理位置作为写入目标：即帧缓冲的附件。这个纹理用于存储渲染操作的结果，渲染的图像会被写入到这个纹理中，而不是直接显示到屏幕上。
//在一个 帧缓冲对象（Framebuffer, FBO） 中，可以有多个附件，包括：
//多个纹理附件（可以是颜色纹理附件、深度纹理附件、模板纹理附件）、一个深度附件 、 一个模板附件。
// 
//纹理附件：
//纹理附件可以分为多种类型，例如：颜色纹理附件、深度纹理附件、模板纹理附件
//纹理附件只是一个统称，实际上具体的颜色数据、深度纹理数据、模板纹理数据都存在纹理附件下的颜色附件、深度纹理附件、模板纹理附件中
//深度附件与深度纹理附件，模板附件与模板纹理附件：
//深度附件 or 模板附件：由深度缓冲区、模板缓冲区创建，只能用于深度检测和模板检测，具体数据无法被采样
//深度纹理附件 or 模板纹理附件：由纹理创建，不仅能存储深度信息和模板信息，还能被采样使用，完成后处理效果

/*立方体贴图*/
//天空盒：
//一、使用glm::mat4(glm::mat3(view))去除平移，这样不管是否移动，摄像机的位置始终在坐标原点，始终在天空盒内部
//二、一直朝一个方向移动其他物体，即使视锥体设置得很大，物体仍然有可能移出天空盒的范围。但由于我们禁用了天空盒的深度写入，
//天空盒会始终被绘制在场景的最远处(深度值为1.0，最远深度)，而不会覆盖其他物体，所以即使物体移出了天空盒的范围，天空盒依然会在渲染时覆盖整个背景。
//三、如果我们第一个渲染天空盒且禁用深度写入，则不会覆盖后渲染的物体，但在后续渲染其他物体时还需要丢弃被遮挡的天空盒片段，
//所以我们可以最后渲染天空盒并让OpenGL始终认为天空盒的深度值为1.0(最远深度值)
//四、如果我们在想在任意顺序渲染天空盒(优化减少片段渲染量)，则需要暂时将深度检测函数设为LEAQAL,保证天空盒仅在尚未屏幕中渲染过物体的位置通过深度检测，同时将这些位置深度值改为1.0(虽然默认值就是1.0)








//std::vector<Vertex> BoxVertices = {
//    //aPos                  //aNormal           //aTexture
//    // 后面 (z = -0.5)
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},// 左下角
//    {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},// 右下角
//    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},// 右上角
//    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},// 右上角
//    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},// 左上角
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},// 左下角
//
//    // 前面 (z = 0.5)
//    {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// 左下角
//    {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},// 右下角
//    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// 右上角
//    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// 右上角
//    {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},// 左上角
//    {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// 左下角
//
//    // 左面 (x = -0.5)
//    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// 上左角
//    {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},// 上右角
//    {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// 下右角
//    {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// 下右角
//    {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// 下左角
//    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// 上左角
//
//    // 右面 (x = 0.5)
//    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// 上左角
//    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},// 上右角
//    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// 下右角
//    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// 下右角
//    {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// 下左角
//    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// 上左角
//
//    // 下面 (y = -0.5)
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},// 左上角
//    {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},// 右上角
//    {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},// 右下角
//    {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},// 右下角
//    {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},// 左下角
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},// 左上角
//
//    // 上面 (y = 0.5)
//    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},// 左上角
//    {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},// 右上角
//    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},// 右下角
//    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},// 右下角
//    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},// 左下角
//    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}// 左上角
//};
//
//std::vector<unsigned int> BoxIndices = {
//    // 后面
//    0, 1, 2,
//    3, 4, 5,
//    // 前面
//    6, 7, 8,
//    9, 10, 11,
//    // 左面
//    12, 13, 14,
//    15, 16, 17,
//    // 右面
//    18, 19, 20,
//    21, 22, 23,
//    // 下面
//    24, 25, 26,
//    27, 28, 29,
//    // 上面
//    30, 31, 32,
//    33, 34, 35
//};
// 
//std::vector<Texture> BoxTextures = 
//{
//    {BoxTexture1,"DiffuseColorSampler","source/Makima.jpg"},//贴图地址可给可不给
//    {BoxTexture2,"DiffuseColorSampler","source/Makima3.jpg"},
//    {BoxTexture1,"SpecularColorSampler","source/Makima.jpg"},
//    {BoxTexture2,"SpecularColorSampler","source/Makima3.jpg"}
//};
// 
// 
//     /*方块绘制*/
//        //激活着色器程序
//BoxShader.Use();
//
////调整纹理可见度
//RateControl(window, rate);
//BoxShader.SetFloat("rate", rate);
//
////模型矩阵
//glm::mat4 model1 = glm::mat4(1.0f);//每一帧都要重新创建model，否则会使用上一帧已经变换过的model
//model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, 0.5f));
////model1 = glm::rotate(model1, PassedTime * glm::radians(-50.0f), glm::vec3(1.0f, 1.0f, -1.0f));//这里先应用旋转，再应用位移
//model1 = glm::scale(model1, glm::vec3(0.5f));
//BoxShader.setMat4("model", model1);
////法线矩阵
//BoxShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model1))));//模型矩阵左上角3x3部分的逆矩阵的转置矩阵
//BoxShader.setMat4("view", view);
//BoxShader.setMat4("projection", projection);
//BoxShader.SetVec3("ViewPos", CameraPos);
//
////点光源
//BoxShader.SetVec3("pointLight.Pos", LightPos);
//BoxShader.SetVec3("pointLight.Ambient", LightColor* glm::vec3(0.2f));
//BoxShader.SetVec3("pointLight.Diffuse", LightColor* glm::vec3(0.5f));
//BoxShader.SetVec3("pointLight.Specular", LightColor* glm::vec3(1.0f));
//BoxShader.SetFloat("pointLight.constant", 1.0f);
//BoxShader.SetFloat("pointLight.linear", 0.35f);
//BoxShader.SetFloat("pointLight.quadratic", 0.44f);
////定向光
//BoxShader.SetVec3("directLight.Dir", LightDirection);
//BoxShader.SetVec3("directLight.Ambient", DirectColor* glm::vec3(0.2f));
//BoxShader.SetVec3("directLight.Diffuse", DirectColor* glm::vec3(0.5f));
//BoxShader.SetVec3("directLight.Specular", DirectColor* glm::vec3(1.0f));
////手电光
//BoxShader.SetVec3("flashLight.Pos", CameraPos);
//BoxShader.SetVec3("flashLight.Dir", CameraFront);
//BoxShader.SetVec3("flashLight.Diffuse", FlashColor* glm::vec3(0.5f));
//BoxShader.SetVec3("flashLight.Specular", FlashColor* glm::vec3(1.0f));
//BoxShader.SetFloat("flashLight.InnerAngle", glm::cos(glm::radians(5.0f)));
//BoxShader.SetFloat("flashLight.OuterAngle", glm::cos(glm::radians(10.0f)));
//BoxShader.SetFloat("flashLight.constant", 1.0f);
//BoxShader.SetFloat("flashLight.linear", 0.35f);
//BoxShader.SetFloat("flashLight.quadratic", 0.44f);
////设置材质
//BoxShader.SetFloat("material.Shininess", 32.0f);
//
////模板检测
//glStencilFunc(GL_ALWAYS, 1, 0xFF);
//glStencilMask(0xFF);//将该箱子所对应的缓冲区模板值更新为1
////此时整个模板缓冲区中：该箱子的对应位置的模板值为1，其余位置为0
//
////绘制  
//BoxMesh.Draw(BoxShader);