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
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
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
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);//模板检测失败时模板值不变，模板检查通过但深度检测失败时模板值替换为参考值，都通过时模板值替换为参考值

    //启用面剔除
    //glEnable(GL_CULL_FACE);

    //调用窗口大小回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//每当用户手动调整窗口大小时就会调用回调函数(GLFW会自动检测是否需要调用函数，所以不用写在循环内部)
    //即使没有这个函数GLFW也允许用户手动调节窗口大小，但不会帮我们调节视口大小和其他相关配置

    //鼠标检测
    //glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);//glfw自动捕获鼠标位置并传递给mouse_callback
    glfwSetScrollCallback(window, scroll_callback);//捕获滚轮移动增量并传递给scroll_callback

    //设置着色器
    Shader PlaneShader("ShaderSource/vShader_Plane.glsl", "ShaderSource/fShader_Plane.glsl");
    Shader RockShader("ShaderSource/vShader_Rock.glsl", "ShaderSource/fShader_Rock.glsl");

    //加载模型
    Model ObjectModel_Planet((std::filesystem::current_path() / "source/Planetary System/planet/planet.obj").string());
    Model ObjectModel_Rock((std::filesystem::current_path() / "source/Planetary System/rock/rock.obj").string());

    //小行星带模型矩阵计算
    int RockAmount = 1000;//小行星数量
    std::vector<glm::mat4> ModelMatrix_Rock;
    std::vector<glm::mat3> NormalMatrix_Rock;
    float Radius = 50.0f;//小行星带半径
    float HalfWidth = 2.5f;//半个小行星带的宽度
    srand(glfwGetTime());//获取随机数种子

    for (int i = 0; i < RockAmount; i++)
    {
        //获取位移
        glm::mat4 model = glm::mat4(1.0f);
        float DeltaAngle = (float)i * (360.0f / (float)RockAmount);//平均每个小行星在圆环上的角度
        float Offset = (rand() % (int)(HalfWidth * 2 * 100)) / 100.0f - HalfWidth;//小行星在小行星带中的偏移[-HalfWidth,HalfWidth].(乘100再除100是为了提高精度)
        float x = cos(DeltaAngle) * Radius + Offset;//x坐标
        Offset = (rand() % (int)(HalfWidth * 2 * 100)) / 100.0f - HalfWidth;//重新获取一个随机的Offset
        float y = Offset * 0.5f;//y坐标
        Offset = (rand() % (int)(HalfWidth * 2 * 100)) / 100.0f - HalfWidth;
        float z = sin(DeltaAngle) * Radius + Offset;//z坐标
        model = glm::translate(model,glm::vec3(x,y,z));

        //获取缩放
        float scale = (rand() % 20) / 100.0f + 0.05f;//(0.05f,0.25f)之间缩放
        model = glm::scale(model,glm::vec3(scale));

        //获取旋转
        float RotateAngle = rand() % 360;
        model = glm::rotate(model,glm::radians(RotateAngle),glm::vec3((rand() % 10)) / 10.0f);

        //法线矩阵
        glm::mat3 normal = glm::mat3(1.0f);
        normal = glm::mat3(glm::transpose(glm::inverse(model)));
        
        NormalMatrix_Rock.push_back(normal);
        ModelMatrix_Rock.push_back(model);
    }


    //实例化顶点属性
    unsigned int buffer_model;
    glGenBuffers(1, &buffer_model);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_model);
    glBufferData(GL_ARRAY_BUFFER, RockAmount * sizeof(glm::mat4), &ModelMatrix_Rock[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < ObjectModel_Rock.meshes.size(); i++)
    {
        unsigned int VAO = ObjectModel_Rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        //由于OpenGL中顶点属性允许的最大数据大小是一个vec4，而我们想传入一个mat4(4个vec4),故需要四个顶点属性
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
        //由于OpenGL中顶点属性允许的最大数据大小是一个vec4，而我们想传入一个mat3(3个vec4),故需要三个顶点属性
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
            glfwSetWindowTitle(window, Title.c_str());
            OneSecond = 0.0f;
            FPSCount = 0;
        }

        //全局状态量设置
            /*设置窗口背景颜色并在每次循环后重新渲染(清除颜色缓冲和深度缓冲)*/
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
        
        //行星模型绘制
        PlaneShader.Use();
        glm::mat4 ModelMatrix_Plane = glm::mat4(1.0f);
        ModelMatrix_Plane = glm::translate(ModelMatrix_Plane,glm::vec3(0.0f,-3.0f,0.0f));
        ModelMatrix_Plane = glm::scale(ModelMatrix_Plane,glm::vec3(4.0f));
        ModelMatrix_Plane = glm::rotate(ModelMatrix_Plane,glm::radians(25.0f) * PassedTime,glm::vec3(0.0f,1.0f,0.0f));

        //计算相关
        PlaneShader.setMat4("projection", projection);
        PlaneShader.setMat4("view", view);
        PlaneShader.setMat4("model", ModelMatrix_Plane);
        PlaneShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(ModelMatrix_Plane))));
        PlaneShader.SetVec3("ViewPos", CameraPos);
        //材质
        PlaneShader.SetFloat("material.Shininess", 32.0f);
        //定向光
        PlaneShader.SetVec3("directLight.Dir", DirectLightDirection);
        PlaneShader.SetVec3("directLight.Ambient", DirectLightColor * glm::vec3(0.2f));
        PlaneShader.SetVec3("directLight.Diffuse", DirectLightColor * glm::vec3(0.5f));
        PlaneShader.SetVec3("directLight.Specular", DirectLightColor * glm::vec3(0.0f));
        //手电光
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


        //小行星模型绘制
        RockShader.Use();

        //计算相关
        RockShader.setMat4("projection", projection);
        RockShader.setMat4("view", view);
        RockShader.SetVec3("ViewPos", CameraPos);
        //RockShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(ModelMatrix_Plane))));
        //材质
        RockShader.SetFloat("material.Shininess", 32.0f);
        //定向光
        RockShader.SetVec3("directLight.Dir", DirectLightDirection);
        RockShader.SetVec3("directLight.Ambient", DirectLightColor* glm::vec3(0.2f));
        RockShader.SetVec3("directLight.Diffuse", DirectLightColor* glm::vec3(0.5f));
        RockShader.SetVec3("directLight.Specular", DirectLightColor* glm::vec3(0.0f));
        //手电光
        RockShader.SetVec3("flashLight.Pos", CameraPos);
        RockShader.SetVec3("flashLight.Dir", CameraFront);
        RockShader.SetVec3("flashLight.Diffuse", FlashLightColor* glm::vec3(0.5f));
        RockShader.SetVec3("flashLight.Specular", FlashLightColor* glm::vec3(0.0f));
        RockShader.SetFloat("flashLight.InnerAngle", InnerAngle);
        RockShader.SetFloat("flashLight.OuterAngle", OuterAngle);
        RockShader.SetFloat("flashLight.constant", 1.0f);
        RockShader.SetFloat("flashLight.linear", 0.35f);
        RockShader.SetFloat("flashLight.quadratic", 0.44f);
        //绘制
        ObjectModel_Rock.InstanceDraw(RockShader, RockAmount);


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
void mouse_callback(GLFWwindow* window, double pos_x, double pos_y)
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

        camera.ProcessMouseMove(offset_x, offset_y);
    }
}

//滚轮回调函数
void scroll_callback(GLFWwindow* window, double offset_x, double offset_y)
{
    camera.ProcessMouseScroll((float)offset_x, (float)offset_y);
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
//gl_Positon也会插值后在片段着色器中被使用，但我们不需要显式地接收gl_Positon

//顶点数组对象（VAO）在OpenGL中负责将顶点缓冲区对象（VBO）中的数据传递给顶点着色器//
//一、1.创建一个缓冲区VBO      2.告诉该缓冲区如何存储数据  3.将数据传递给VBO，
//二、1.创建一个“解释器”VAO  2.将该VAO设置为当前VAO      3.设置VAO如何解释数据      4.将数据传递给着色器

/*纹理*/
//一、纹理环绕方式：处理纹理坐标越界的情况
//二、纹理过滤：决定不同像素色块的取样方式（取临近色块或取插值色块）
//三、多级渐远纹理：当摄像机离物体很远时，很难获取高分辨率的纹理，于是预先创建一个Mipmap（由很多分辨率不断折半的原纹理组成），然后取临近分辨率或取插值分辨率

/*坐标变换*/
//局部坐标系 -> (模型变换) -> 世界坐标系 -> (视图变换) -> 观察坐标系 -> (投影变换) -> 裁剪坐标系 -> (裁剪+透视除法) -> 规范设备坐标系(NDC) -> (视口变换) -> 屏幕坐标系 -> 进行深度测试等渲染过程
//世界空间以一个固定点(0,0,0)为坐标系原点，视图空间以摄像机位置为坐标系原点，这两个空间都是线性空间(顶点的相对位置不会改变)，裁剪空间坐标原点与视锥体近平面重合(通常摄像机与近平面很接近)，是非线性空间
//投影矩阵将顶点从视图空间变换到裁剪空间(这一步叫视图变换),视图变换将顶点深度值(z坐标值)进行非线性缩放(模拟近大远小的效果:随着物体与摄像机的距离增加，物体的坐标值会以非线性的方式缩小)，故裁剪空间是一个非线性空间(原本顶点的相对位置发生了变化)
//变换到裁剪空间之后会由OpenGL自动完成裁剪和透视除法(依靠齐次坐标w)，将顶点变换到NDC中，此时x/y/z的值都在(-1,1)之间

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
//四、如果我们在想在任意顺序渲染天空盒(优化减少片段渲染量)，则需要暂时将深度检测函数设为LEAQAL,保证天空盒仅在尚未屏幕中渲染过物体的位置通过深度检测，同时将这些位置深度值改为1.0(虽然本来就是1.0)

/*几何着色器*/
//一、几何着色器处于顶点着色器和片段着色器之间(若没有显式编写几何着色器则不会使用)，用于生成新的顶点和修改已有的图元
//二、layout () in申明从顶点着色器接收的图元类型，layout () out申明输出给片段着色器的图元类型和最大顶点数
//三、EmitVertex():输出新的顶点;EndPrimitive():所有被EmitVertex输出的顶点会被合成为我们在layout () out中指定过的图形
//四、如果我们启用了几何着色器，那么从顶点着色器中out的变量必须要依靠几何着色器的间接传递才能在片段着色器中接收
//五、从顶点着色器向几何着色器传递数据都是一组一组传递的，每一组包含一个图元所有的所有顶点数据(位置、法向量、纹理坐标等)
//六、可以理解为几何着色器中又包含了一个图元顶点数目个数的顶点着色器
//七、顶点着色器中的gl_Position算出来是哪个空间的，几何着色器就在哪个空间进行下一步计算
//八、可以在顶点着色器中将gl_Positon变换到裁剪空间，也可以先在顶点着色器中将gl_Position变换到观察空间然后在几何着色器中进行其他处理再将其变换到裁剪空间
/*九、如果要在几何着色器中使用顶点(gl_Position)之间的线性关系来计算其他量(如法线)时，要确保从顶点着色器中获取的gl_Positon是观察空间中的量，计算完还要再把gl_Positon转换到裁剪空间中*/

/*实例化*/
//一、uniform数据在所有实例中共享(不使用实例化绘制n个物体:每一帧向glsl上传n个uniform值;使用实例化绘制n个物体:每一帧向glsl上传1个uniform值)
//二、在绘制多个实例时如果希望用不同的model矩阵进行位置调整，如果使用uniform数组的话可能会超出最大uniform数量，故应采用实例化数组(将model设置为顶点属性:顶点属性在绘制一个新的实例时才会被更新)
//三、使用实例化数组把模型矩阵(或其他数据)传递给顶点属性时必须在渲染循环外进行





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