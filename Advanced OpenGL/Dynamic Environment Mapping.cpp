//#include <iostream>
//#include <vector>
//#include <chrono>
//#include <map>
//#include <filesystem>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include "HeadFile/Shader.h"
//#include "HeadFile/Mesh.h"
//#include "HeadFile/Model.h"
//
//#include "Include/glm/glm.hpp"
//#include "Include/glm/gtc/matrix_transform.hpp"
//#include "Include/glm/gtc/type_ptr.hpp"
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "Include/stb_image.h"
//
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//
//
//int WindowWidth = 1200;//���ڴ�С
//int WindowHeight = 900;
//int WindowPos_X = 300;//����(���Ͻ�)����λ��
//int WindowPos_Y = 100;
//float aspectRatio = (float)(WindowWidth / WindowHeight);//���ڳ����
//bool IsPaused = false;//�Ƿ���ͣ
//bool LastSpacePressd = false;//�����һ֡�Ƿ���space
//bool CurrentSpacePressd = false;//��⵱ǰ֡�Ƿ���space
//bool IsKey_F = false;//�Ƿ���F��
//bool LastKeyPressd_F = false;//�����һ֡�Ƿ���F��
//bool CurrentKeyPressd_F = false;//��⵱ǰ֡�Ƿ���F��
//bool IsKey_E = false;//�Ƿ���E��
//bool LastKeyPressd_E = false;//�����һ֡�Ƿ���E��
//bool CurrentKeyPressd_E = false;//��⵱ǰ֡�Ƿ���E��
//bool IsKey_M = false;//�Ƿ���M��
//bool LastKeyPressd_M = false;//�����һ֡�Ƿ���M��
//bool CurrentKeyPressd_M = false;//��⵱ǰ֡�Ƿ���M��
//bool IsAlt = false;//�Ƿ���Alt
//bool IsAltRealise = false;//�Ƿ��ɿ�Alt
//float LastTime = 0.0f;//��һ֡ʱ��
//float CurrentTime = 0.0f;//��ǰ֡ʱ��
//float PassedTime = 0.0f;//����ͣʱ������ʱ��
//float PeriodTime = 0.0f;//ÿ��֮֡���ʱ��
//int FPSCount = 0;//֡��Ŀ
//float OneSecond = 0.0f;//��¼һ����
//float LastX = 960;//����ʼλ�ã���Ⱦ�������ģ�
//float LastY = 540;
//float pitch = 0.0f;//������
//float yaw = -90.0f;//ƫ����
//float Last_pitch;//��һ֡pitch
//float last_yaw;//��һ֡yaw
//bool FirstMouse = true;//�Ƿ��״λ�ȡ�������
//float FOV = 45.0f;//��Ұ��Χ
//float rate = 0.0f;//����ɼ���
//
////���������
//glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
//glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//���������
//glm::vec3 CameraUP = glm::vec3(0.0f, 1.0f, 0.0f);
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double pos_x, double pos_y);
//void scroll_callback(GLFWwindow* window, double offset_x, double offset_y);
//void processInput(GLFWwindow* window);
//unsigned int LoadTexture(char const* path);
//unsigned int LoadCubeTexture(std::vector<std::string> Faces_Path);
//
//int main()
//{
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//
//    //����һ������
//    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetWindowPos(window, WindowPos_X, WindowPos_Y);//���ô���λ��
//
//    //��ʼ��glad
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    //�����ӿ�
//    glViewport(0, 0, WindowWidth, WindowHeight);
//
//    //������Ⱦģʽ
//    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);//�߿�ģʽ
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//���ģʽ
//
//    //������Ȳ��ԡ�ģ����ԣ�ȫ��״̬��������Ĭ��֡���廹���Խ�֡���嶼�����ò��ԣ�
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    glEnable(GL_STENCIL_TEST);
//    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);//ģ����ʧ��ʱģ��ֵ���䣬ģ����ͨ������ȼ��ʧ��ʱģ��ֵ�滻Ϊ�ο�ֵ����ͨ��ʱģ��ֵ�滻Ϊ�ο�ֵ
//
//    //�������޳�
//    //glEnable(GL_CULL_FACE);
//
//    //���ô��ڴ�С�ص�����
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//ÿ���û��ֶ��������ڴ�Сʱ�ͻ���ûص�����(GLFW���Զ�����Ƿ���Ҫ���ú��������Բ���д��ѭ���ڲ�)
//    //��ʹû���������GLFWҲ�����û��ֶ����ڴ��ڴ�С������������ǵ����ӿڴ�С�������������
//
//    //�����
//    //glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
//    glfwSetCursorPosCallback(window, mouse_callback);//glfw�Զ��������λ�ò����ݸ�mouse_callback
//    glfwSetScrollCallback(window, scroll_callback);//��������ƶ����������ݸ�scroll_callback
//
//    //������ɫ��
//    //Shader BoxShader("ShaderSource/vShader_Box.glsl", "ShaderSource/fShader_Box.glsl");
//    Shader BoxReflectShader("ShaderSource/vShader_BoxReflect.glsl", "ShaderSource/fShader_BoxReflect.glsl");
//    Shader WallShader("ShaderSource/vShader_Wall.glsl", "ShaderSource/fShader_Wall.glsl");
//    Shader LightShader("ShaderSource/vShader_Light.glsl", "ShaderSource/fShader_Light.glsl");
//    Shader GrassShader("ShaderSource/vShader_Grass.glsl", "ShaderSource/fShader_Grass.glsl");
//    Shader WindowShader("ShaderSource/vShader_Window.glsl", "ShaderSource/fShader_Window.glsl");
//    Shader ScreenShader("ShaderSource/vShader_Screen.glsl", "ShaderSource/fShader_Screen.glsl");
//    Shader SkyBoxShader("ShaderSource/vShader_SkyBox.glsl", "ShaderSource/fShader_SkyBox.glsl");
//
//    Shader ModelShader_Knight("ShaderSource/vShader_Model.glsl", "ShaderSource/fShader_Knight.glsl");
//    Shader ModelShader_JinXi("ShaderSource/vShader_Model.glsl", "ShaderSource/fShader_PersonModel.glsl");
//
//    Shader FrameShader("ShaderSource/vShader_Frame.glsl", "ShaderSource/fShader_Frame.glsl");
//
//    //��������
//    //unsigned int BoxTexture1 = LoadTexture((std::filesystem::current_path() / "source/Makima.jpg").string().c_str());//ȷ��c++17����
//    //unsigned int BoxTexture2 = LoadTexture((std::filesystem::current_path() / "source/Makima3.jpg").string().c_str());
//    unsigned int WallTexture1 = LoadTexture((std::filesystem::current_path() / "source/Wall.png").string().c_str());
//    unsigned int WallTexture2 = LoadTexture((std::filesystem::current_path() / "source/Specular Map.png").string().c_str());
//    unsigned int WallTexture3 = LoadTexture((std::filesystem::current_path() / "source/Code.jpg").string().c_str());
//    unsigned int GrassTexture1 = LoadTexture((std::filesystem::current_path() / "source/grass.png").string().c_str());
//    unsigned int WindowTexture1 = LoadTexture((std::filesystem::current_path() / "source/window.png").string().c_str());
//
//    //������������ͼ����
//    std::vector<std::string> SkyBoxFaces_Path = {
//        (std::filesystem::current_path() / "source/skybox/skybox/right.jpg").string().c_str(),
//        (std::filesystem::current_path() / "source/skybox/skybox/left.jpg").string().c_str(),
//        (std::filesystem::current_path() / "source/skybox/skybox/top.jpg").string().c_str(),
//        (std::filesystem::current_path() / "source/skybox/skybox/bottom.jpg").string().c_str(),
//        (std::filesystem::current_path() / "source/skybox/skybox/front.jpg").string().c_str(),
//        (std::filesystem::current_path() / "source/skybox/skybox/back.jpg").string().c_str()
//    };
//    unsigned int SkyBoxCMTexture = LoadCubeTexture(SkyBoxFaces_Path);
//
//
//
//    //���񶥵�
//    std::vector<Vertex> CubeVertices = {
//        //aPos                  //aNormal           //aTexture
//        // ���� (z = -1.0)
//        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},// ���½�
//        {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},// ���½�
//        {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//        {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//        {{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},// ���Ͻ�
//        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},// ���½�
//
//        // ǰ�� (z = 1.0)
//        {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// ���½�
//        {{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},// ���½�
//        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//        {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},// ���Ͻ�
//        {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// ���½�
//
//        // ���� (x = -1.0)
//        {{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
//        {{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},// ���ҽ�
//        {{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
//        {{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
//        {{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// �����
//        {{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
//
//        // ���� (x = 1.0)
//        {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
//        {{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},// ���ҽ�
//        {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
//        {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
//        {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// �����
//        {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
//
//        // ���� (y = -1.0)
//        {{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},// ���Ͻ�
//        {{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},// ���Ͻ�
//        {{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
//        {{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
//        {{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},// ���½�
//        {{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},// ���Ͻ�
//
//        // ���� (y = 1.0)
//        {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},// ���Ͻ�
//        {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},// ���Ͻ�
//        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
//        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
//        {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},// ���½�
//        {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}} // ���Ͻ�
//    };
//
//    std::vector<Vertex> WallVertices = {
//        //aPos                  //aNormal           /aTexCoord
//        // ����
//        {{-0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // ���Ͻ� (��ʱ������)
//        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},   // ���½�
//        {{0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // ���Ͻ�
//
//        {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // ���½�
//        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},   // ���½�
//        {{-0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // ���Ͻ�
//
//        // ����
//        {{-0.5f, -0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // ���½�
//        {{0.5f, -0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // ���½�
//        {{0.5f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},   // ���Ͻ�
//
//        {{0.5f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},   // ���Ͻ�
//        {{-0.5f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // ���Ͻ�
//        {{-0.5f, -0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}  // ���½�
//    };
//
//    std::vector<Vertex> SquareVertices = {
//        //aPos                 //aNormal           //aTexCoord
//        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// ���½�
//        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},// ���½�
//        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//
//        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},// ���Ͻ�
//        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}// ���½�
//    };
//
//    std::vector<Vertex> ScreenVertices = {
//        //aPos                 //aNormal           //aTexCoord
//        {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// ���½�
//        {{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},// ���½�
//        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//
//        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
//        {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},// ���Ͻ�
//        {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}// ���½�
//    };
//
//    //��������
//    std::vector<unsigned int> CubeIndices = {
//        // ����
//        0, 1, 2,
//        3, 4, 5,
//        // ǰ��
//        6, 7, 8,
//        9, 10, 11,
//        // ����
//        12, 13, 14,
//        15, 16, 17,
//        // ����
//        18, 19, 20,
//        21, 22, 23,
//        // ����
//        24, 25, 26,
//        27, 28, 29,
//        // ����
//        30, 31, 32,
//        33, 34, 35
//    };
//
//    std::vector<unsigned int> WallIndices = {
//        // ����
//        0, 1, 2,
//        3, 4, 5,
//        // ����
//        6, 7, 8,
//        9, 10, 11,
//    };
//
//    std::vector<unsigned int> SquareIndices = {
//         0,1,2,
//         3,4,5
//    };
//
//
//    //��������
//    std::vector<Texture> WallTextures =
//    {
//        {WallTexture1,"DiffuseColorSampler","source/Wall.png"},
//        {WallTexture2,"SpecularColorSampler","source/Specular Map.png"},
//        {WallTexture3,"EmissionColorSampler","source/Code.jpg"}
//    };
//
//    std::vector<Texture> GrassTextures = {
//        {GrassTexture1,"DiffuseColorSampler","source/grass.png"}
//    };
//
//    std::vector<Texture> WindowTextures = {
//        {WindowTexture1,"DiffuseColorSampler","source/window.png"}
//    };
//
//    std::vector<Texture> ScreenTextures = {};
//
//    std::vector<Texture> SkyBoxTextures = {};
//
//    std::vector<Texture> BoxReflectTextures = {};
//
//    //��������
//    //Mesh BoxMesh(BoxVertices,BoxIndices,BoxTextures);
//    Mesh BoxReflectMesh(CubeVertices, CubeIndices, BoxReflectTextures);
//    Mesh WallMesh(WallVertices, WallIndices, WallTextures);
//    Mesh GrassMesh(SquareVertices, SquareIndices, GrassTextures);
//    Mesh WindowMesh(SquareVertices, SquareIndices, WindowTextures);
//    Mesh ScreenMesh(ScreenVertices, SquareIndices, ScreenTextures);
//    Mesh SkyBoxMesh(CubeVertices, CubeIndices, SkyBoxTextures);
//
//    //����ģ��
//    Model PersonModel_Knight((std::filesystem::current_path() / "source/nanosuit_reflection/nanosuit.obj").string());
//    Model PersonModel_JinXi((std::filesystem::current_path() / "source/JinXi/01.fbx").string());
//    Model LightModel((std::filesystem::current_path() / "source/Hollow Sphere/linked_star.obj").string());
//
//
//    //����������պ�
//    unsigned int EnvironmentCubeMap = 0;
//    glGenTextures(1, &EnvironmentCubeMap);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, EnvironmentCubeMap);
//    //���Ʒ�ʽ
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//���������곬��[0.0, 1.0]��Χʱ����������ᱻ������0��1֮�䡣
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//    //���˷�ʽ
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    //ΪEnvironmentCubeMapÿһ��������ڴ�
//    for (int i = 0; i < 6; i++)
//    {
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
//    }
//
//    //����֡����
//    unsigned int G_Buffer[6];
//    glGenFramebuffers(6, G_Buffer);//����֡����
//    for (unsigned int i = 0; i < 6; i++)
//    {
//        glBindFramebuffer(GL_FRAMEBUFFER, G_Buffer[i]);//��G_Buffer
//        //������ɫ������
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, EnvironmentCubeMap, 0);//�ѻ�����պ������ӵ���ǰ�󶨵�G_Buffer��
//
//        //������Ⱦ�������(���ģ�帽��)
//        unsigned int RBO;
//        glGenRenderbuffers(1, &RBO);//������Ⱦ����
//        glBindRenderbuffer(GL_RENDERBUFFER, RBO);//��
//        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowWidth, WindowHeight);//Ϊ��Ⱦ����������洢�ռ�(��Ȼ�����24λ��ģ�建����8λ)
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);//�����ģ�帽�����ӵ���ǰ�󶨵�G_Buffer��
//        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)//���G_Buffer�Ƿ�����
//        {
//            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
//        }
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);//���G_Buffer
//    }
//
//
//
//
//
//    //��Ⱦѭ��
//    while (!glfwWindowShouldClose(window))
//    {
//        //�����������¼�
//        glfwPollEvents();//�����ص�����
//
//        //����������״̬
//        processInput(window);
//
//        //��ȡʱ��
//        CurrentTime = (float)glfwGetTime();
//        PeriodTime = CurrentTime - LastTime;
//        if (!IsPaused)
//        {
//            PassedTime += CurrentTime - LastTime;
//        }
//        LastTime = CurrentTime;
//
//        //��ʾ֡��
//        FPSCount++;
//        OneSecond += PeriodTime;
//        if (OneSecond >= 1.0f)
//        {
//            float FPS = (float)FPSCount / OneSecond;
//            std::string Title = "FPS:  " + std::to_string(FPS);
//            glfwSetWindowTitle(window, Title.c_str());
//            OneSecond = 0.0f;
//            FPSCount = 0;
//        }
//
//        for (int k = 0; k < 6; k++)
//        {
//            //ʹ��G_Buffer
//            glBindFramebuffer(GL_FRAMEBUFFER, G_Buffer[k]);
//
//            //���ô��ڱ�����ɫ����ÿ��ѭ����������Ⱦ(�����ɫ�������Ȼ���)
//            glClearColor(0.678f, 0.847f, 0.902f, 1.0f);
//            glStencilMask(0xFF);//ȷ��ģ�建�������������
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//            //��������
//            //glm::vec3 LightColor(sin(glfwGetTime() * 1.0f)/2.0f+0.5f,sin(glfwGetTime() * 1.2f)/2.0f+0.5f,sin(glfwGetTime() * 1.5f)/2.0f+0.5f);
//            glm::vec3 LightColor(1.0f);//���Դ��ɫ
//            glm::vec3 DirectColor(1.0f);//�������ɫ
//            glm::vec3 FlashColor(0.0f);//�ֵ����ɫ
//            float InnerAngle = glm::cos(glm::radians(5.0f - rate));//�ֵ����Ȧ�Ƕȣ�����ֵ��
//            float OuterAngle = glm::cos(glm::radians(10.0f + rate));//�ֵ����Ȧ�Ƕȣ�����ֵ��
//            glm::vec3 LightPos(0.9f * sin(PassedTime), 0.9f * cos(PassedTime) + 0.5f, sin(PassedTime) + 2.0f);//��Դλ��
//            glm::vec3 LightDirection(1.0f, -1.0f, -1.0f);//ƽ�йⷽ��
//
//            //��ͼ����
//            glm::vec3 CameraPos2 = glm::vec3(0.0f, 0.0f, 2.0f);//���λ�������ó�����λ��
//            glm::vec3 CameraFront2 = glm::vec3(0.0f);
//            glm::vec3 CameraUP2 = glm::vec3(0.0f, -1.0f, 0.0f);//����������������������Ϊʲô���ó�1EnvironmentCubeMap����ͻ����µߵ���������������������������
//            if (k == 0) CameraFront2 = glm::vec3(1.0f, 0.0f, 0.0f);//��
//            if (k == 1) CameraFront2 = glm::vec3(-1.0f, 0.0f, 0.0f);//��
//            if (k == 2) CameraFront2 = glm::vec3(0.0f, -1.0f, 0.0f);//��
//            if (k == 3) CameraFront2 = glm::vec3(0.0f, 1.0f, 0.0f);//��
//            if (k == 4) CameraFront2 = glm::vec3(0.0f, 0.0f, 1.0f);//��
//            if (k == 5) CameraFront2 = glm::vec3(0.0f, 0.0f, -1.0f);//ǰ
//
//
//
//            glm::mat4 view = glm::mat4(1.0f);
//            view = glm::lookAt(CameraPos2, CameraPos2 + CameraFront2, CameraUP2);//(�����λ�ã��۲�Ŀ������(��ͼ�����з�������= Ŀ��λ��-�����λ�ã��ʼ�����������걣֤���ƶ�Ҳ��ά�����߷��򲻱䣬��Ϊ����������ǰ��������ĳ���̶��ĵ�)�������������Ϸ���)
//            //ͶӰ����
//            glm::mat4 projection = glm::mat4(1.0f);
//            projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);//��Ұ��Сʱ�����彫ռ�����������Ļ������λ�ã���������ø���
//
//
//
//            //����ģ�ͻ���
//                /*��ʿģ�ͻ���*/
//            ModelShader_Knight.Use();
//            glActiveTexture(GL_TEXTURE2);
//            glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxCMTexture);
//
//            glm::mat4 model_Knight = glm::mat4(1.0f);
//            model_Knight = glm::translate(model_Knight, glm::vec3(0.5f, -1.0f, 1.0f));
//            model_Knight = glm::scale(model_Knight, glm::vec3(0.1f));
//            //�������
//            ModelShader_Knight.setMat4("projection", projection);
//            ModelShader_Knight.setMat4("view", view);
//            ModelShader_Knight.setMat4("model", model_Knight);
//            ModelShader_Knight.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model_Knight))));
//            ModelShader_Knight.SetVec3("ViewPos", CameraPos);
//            //����
//            ModelShader_Knight.SetFloat("material.Shininess", 32.0f);
//            ModelShader_Knight.SetInt("SkyBoxTexture", 2);
//            //���Դ
//            ModelShader_Knight.SetVec3("pointLight.Pos", LightPos);
//            ModelShader_Knight.SetVec3("pointLight.Ambient", LightColor * glm::vec3(0.2f));
//            ModelShader_Knight.SetVec3("pointLight.Diffuse", LightColor * glm::vec3(0.5f));
//            ModelShader_Knight.SetVec3("pointLight.Specular", LightColor * glm::vec3(1.0f));
//            ModelShader_Knight.SetFloat("pointLight.constant", 1.0f);
//            ModelShader_Knight.SetFloat("pointLight.linear", 0.35f);
//            ModelShader_Knight.SetFloat("pointLight.quadratic", 0.44f);
//            //�����
//            ModelShader_Knight.SetVec3("directLight.Dir", LightDirection);
//            ModelShader_Knight.SetVec3("directLight.Ambient", DirectColor * glm::vec3(0.2f));
//            ModelShader_Knight.SetVec3("directLight.Diffuse", DirectColor * glm::vec3(0.5f));
//            ModelShader_Knight.SetVec3("directLight.Specular", DirectColor * glm::vec3(1.0f));
//            //�ֵ��
//            ModelShader_Knight.SetVec3("flashLight.Pos", CameraPos);
//            ModelShader_Knight.SetVec3("flashLight.Dir", CameraFront);
//            ModelShader_Knight.SetVec3("flashLight.Diffuse", FlashColor * glm::vec3(0.5f));
//            ModelShader_Knight.SetVec3("flashLight.Specular", FlashColor * glm::vec3(1.0f));
//            ModelShader_Knight.SetFloat("flashLight.InnerAngle", InnerAngle);
//            ModelShader_Knight.SetFloat("flashLight.OuterAngle", OuterAngle);
//            ModelShader_Knight.SetFloat("flashLight.constant", 1.0f);
//            ModelShader_Knight.SetFloat("flashLight.linear", 0.35f);
//            ModelShader_Knight.SetFloat("flashLight.quadratic", 0.44f);
//
//            //ģ�����
//            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//            glStencilFunc(GL_ALWAYS, 0b00000001, 0xFF);//ģ��������ͨ��������ȼ��Ҳ��ͨ��������glStencilOp��������������ͼ�ε�ģ��ֵ�����滻Ϊ00000001
//            glStencilMask(0b00000001);//������������Ӧ�Ļ�����ģ��ֵ����Ϊ00000001
//            //��ʱ����ģ�建�����У�����������Ӧλ�õ�ģ��ֵΪ00000001������λ��Ϊ0
//
//            //����
//            PersonModel_Knight.Draw(ModelShader_Knight);
//
//
//
//
//            /*��ϫģ�ͻ���*/
//            ModelShader_JinXi.Use();
//
//            glm::mat4 model_JinXi = glm::mat4(1.0f);
//            model_JinXi = glm::translate(model_JinXi, glm::vec3(-0.5f, -1.0f, 1.0f));
//            model_JinXi = glm::scale(model_JinXi, glm::vec3(0.08f));
//            //�������
//            ModelShader_JinXi.setMat4("projection", projection);
//            ModelShader_JinXi.setMat4("view", view);
//            ModelShader_JinXi.setMat4("model", model_JinXi);
//            ModelShader_JinXi.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model_JinXi))));
//            ModelShader_JinXi.SetVec3("ViewPos", CameraPos);
//            //����
//            ModelShader_JinXi.SetFloat("material.Shininess", 32.0f);
//            //���Դ
//            ModelShader_JinXi.SetVec3("pointLight.Pos", LightPos);
//            ModelShader_JinXi.SetVec3("pointLight.Ambient", LightColor * glm::vec3(0.2f));
//            ModelShader_JinXi.SetVec3("pointLight.Diffuse", LightColor * glm::vec3(0.5f));
//            ModelShader_JinXi.SetVec3("pointLight.Specular", LightColor * glm::vec3(0.0f));
//            ModelShader_JinXi.SetFloat("pointLight.constant", 1.0f);
//            ModelShader_JinXi.SetFloat("pointLight.linear", 0.35f);
//            ModelShader_JinXi.SetFloat("pointLight.quadratic", 0.44f);
//            //�����
//            ModelShader_JinXi.SetVec3("directLight.Dir", LightDirection);
//            ModelShader_JinXi.SetVec3("directLight.Ambient", DirectColor * glm::vec3(0.2f));
//            ModelShader_JinXi.SetVec3("directLight.Diffuse", DirectColor * glm::vec3(0.5f));
//            ModelShader_JinXi.SetVec3("directLight.Specular", DirectColor * glm::vec3(0.0f));
//            //�ֵ��
//            ModelShader_JinXi.SetVec3("flashLight.Pos", CameraPos);
//            ModelShader_JinXi.SetVec3("flashLight.Dir", CameraFront);
//            ModelShader_JinXi.SetVec3("flashLight.Diffuse", FlashColor * glm::vec3(0.5f));
//            ModelShader_JinXi.SetVec3("flashLight.Specular", FlashColor * glm::vec3(0.0f));
//            ModelShader_JinXi.SetFloat("flashLight.InnerAngle", InnerAngle);
//            ModelShader_JinXi.SetFloat("flashLight.OuterAngle", OuterAngle);
//            ModelShader_JinXi.SetFloat("flashLight.constant", 1.0f);
//            ModelShader_JinXi.SetFloat("flashLight.linear", 0.35f);
//            ModelShader_JinXi.SetFloat("flashLight.quadratic", 0.44f);
//
//            //ģ�����
//            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//            glStencilFunc(GL_ALWAYS, 0b00000010, 0xFF);
//            glStencilMask(0b00000010);//������������Ӧ�Ļ�����ģ��ֵ����Ϊ00000010
//            //��ʱ����ģ�建�����У���model����Ӧλ�õ�ģ��ֵΪ00000010����һ��model����Ӧλ��Ϊ00000001�����������ص����֣����ص�����Ϊ00000011
//
//            //����
//            PersonModel_JinXi.Draw(ModelShader_JinXi);
//
//
//
//
//
//            /*ǽ�����*/
//            WallShader.Use();
//            glm::mat4 model2 = glm::mat4(1.0f);
//            model2 = glm::translate(model2, glm::vec3(0.0f, -0.2f, 1.5f));
//            model2 = glm::scale(model2, glm::vec3(1.5f));
//            //�������
//            WallShader.setMat4("model", model2);
//            WallShader.setMat4("view", view);
//            WallShader.setMat4("projection", projection);
//            WallShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model2))));
//            WallShader.SetVec3("ViewPos", CameraPos);
//            //����
//            WallShader.SetFloat("material.EmissionStrength", (float)(sin(PassedTime * 5.0f) * 0.25 + 0.75f));
//            WallShader.SetFloat("material.EmissionMovement", PassedTime * 1.5f);
//            WallShader.SetFloat("material.Shininess", 32.0f);
//            //���Դ
//            WallShader.SetVec3("pointLight.Pos", LightPos);
//            WallShader.SetVec3("pointLight.Ambient", LightColor * glm::vec3(0.2f));
//            WallShader.SetVec3("pointLight.Diffuse", LightColor * glm::vec3(0.5f));
//            WallShader.SetVec3("pointLight.Specular", LightColor * glm::vec3(1.0f));
//            WallShader.SetFloat("pointLight.constant", 1.0f);
//            WallShader.SetFloat("pointLight.linear", 0.35f);
//            WallShader.SetFloat("pointLight.quadratic", 0.44f);
//            //�����
//            WallShader.SetVec3("directLight.Dir", LightDirection);
//            WallShader.SetVec3("directLight.Ambient", DirectColor * glm::vec3(0.2f));
//            WallShader.SetVec3("directLight.Diffuse", DirectColor * glm::vec3(0.5f));
//            WallShader.SetVec3("directLight.Specular", DirectColor * glm::vec3(1.0f));
//            //�ֵ��
//            WallShader.SetVec3("flashLight.Pos", CameraPos);
//            WallShader.SetVec3("flashLight.Dir", CameraFront);
//            WallShader.SetVec3("flashLight.Diffuse", FlashColor * glm::vec3(0.5f));
//            WallShader.SetVec3("flashLight.Specular", FlashColor * glm::vec3(1.0f));
//            WallShader.SetFloat("flashLight.InnerAngle", InnerAngle);
//            WallShader.SetFloat("flashLight.OuterAngle", OuterAngle);
//            WallShader.SetFloat("flashLight.constant", 1.0f);
//            WallShader.SetFloat("flashLight.linear", 0.35f);
//            WallShader.SetFloat("flashLight.quadratic", 0.44f);
//
//            //ģ�����
//            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//            glStencilFunc(GL_ALWAYS, 1, 0xFF);//ǽ��ͨ����⣬������Ƭ��
//            glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//            //��ʱ����ģ�建�����У�ֻ������model����λ���з����ģ��ֵ�����ಿ�־�Ϊ0
//
//            //����
//            WallMesh.Draw(WallShader);
//
//
//
//            /*��Դ����*/
//            LightShader.Use();
//
//            glm::mat4 model3 = glm::mat4(1.0f);
//            model3 = glm::translate(model3, LightPos);
//            model3 = glm::scale(model3, glm::vec3(0.005f, 0.005f, 0.005f));
//            //�������
//            LightShader.setMat4("model", model3);
//            LightShader.setMat4("view", view);
//            LightShader.setMat4("projection", projection);
//
//            //ģ�����
//            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//            glStencilFunc(GL_ALWAYS, 1, 0xFF);//��Դͨ����⣬������Ƭ��
//            glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//            //��ʱ����ģ�建�����У�ֻ������model����λ���з����ģ��ֵ�����ಿ�־�Ϊ0
//
//            //����
//            LightModel.Draw(LightShader);
//
//
//
//            /*��������*/
//            if (IsKey_E)
//            {
//                /*�ݵ�*/
//                //����ȫ͸������(������ı߿�)������Ҫ��ϲ��ԣ�ֱ�Ӷ�����Ӧ͸���ȵ�Ƭ�μ���
//                GrassShader.Use();
//                glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//                glStencilFunc(GL_ALWAYS, 1, 0xFF);//ͨ����⣬������Ƭ��
//                glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//                std::vector<glm::vec3> vegetation;
//                vegetation.push_back(glm::vec3(-0.7f, -0.5f, 1.0f));
//                vegetation.push_back(glm::vec3(0.7f, -0.5f, 1.0f));
//                vegetation.push_back(glm::vec3(0.0f, -0.5f, 1.0f));
//
//                glm::mat4 model5 = glm::mat4(1.0f);
//                GrassShader.setMat4("projection", projection);
//                GrassShader.setMat4("view", view);
//
//                for (unsigned int i = 0; i < vegetation.size(); i++)
//                {
//                    model5 = glm::mat4(1.0f);
//                    model5 = glm::translate(model5, vegetation[i]);
//                    GrassShader.setMat4("model", model5);
//                    GrassMesh.Draw(GrassShader);
//                }
//
//                /*����*/
//                //���ڰ�͸�����壺ʹ�û�ϲ��ԣ�ȷ������Ⱦ��͸������������Ⱦ��͸�������壬����ȾԶ���İ�͸����������Ⱦ�����İ�͸������
//                WindowShader.Use();
//                glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//                glStencilFunc(GL_ALWAYS, 1, 0xFF);//ͨ����⣬������Ƭ��
//                glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//                glEnable(GL_BLEND);//���û��
//                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//                std::vector<glm::vec3> vegetation2;
//                vegetation2.push_back(glm::vec3(-0.35f, -0.5f, 1.0f));
//                vegetation2.push_back(glm::vec3(0.35f, -0.5f, 1.5f));
//                vegetation2.push_back(glm::vec3(0.0f, -0.5f, 2.0f));
//                std::map<float, glm::vec3> Sorted;
//                for (unsigned int i = 0; i < vegetation2.size(); i++)
//                {
//                    float distence = glm::length(CameraPos - vegetation2[i]);
//                    Sorted[distence] = vegetation2[i];
//                }
//                glm::mat4 model6 = glm::mat4(1.0f);
//                WindowShader.setMat4("projection", projection);
//                WindowShader.setMat4("view", view);
//
//                for (auto it = Sorted.rbegin(); it != Sorted.rend(); ++it)
//                {
//                    model6 = glm::mat4(1.0f);
//                    model6 = glm::translate(model6, it->second);
//                    WindowShader.setMat4("model", model6);
//                    WindowMesh.Draw(WindowShader);
//                }
//                glDisable(GL_BLEND);
//            }
//
//            /*��պ�*/
//            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//            glStencilFunc(GL_ALWAYS, 1, 0xFF);//ͨ��ģ����
//            glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//            glDepthFunc(GL_LEQUAL);//���ֵС�ڵ�����Ȼ�������Ƭ�α�����(�Ѿ���Ⱦ�����������λ�õ���պ�Ƭ�α���������δ��Ⱦ�����������λ��ͨ����ȼ�⣬��Ӧ��Ȼ���������Ϊ1.0(����Щ�ط���������1.0��Ϊ��Ȼ�����Ĭ��ֵΪ1.0))
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxCMTexture);
//            SkyBoxShader.Use();
//            SkyBoxShader.setMat4("view", glm::mat4(glm::mat3(view)));//ȥ��view�����е�ƽ�Ʋ��֣��������ʼ�մ�������ԭ��(�ƶ�ʱҲ��)
//            SkyBoxShader.setMat4("projection", projection);
//            SkyBoxShader.SetInt("SkyBoxCMTexture", 0);
//            SkyBoxMesh.Draw(SkyBoxShader);
//            glDepthFunc(GL_LESS);//����������ȼ�⺯��ΪLESSͨ��
//
//
//
//            /*�߿����*/
//            if (IsKey_F)
//            {
//                /*��ʿ�߿����*/
//                FrameShader.Use();
//                glm::mat4 Fmodel_Knight = glm::mat4(1.0f);
//                Fmodel_Knight = glm::translate(Fmodel_Knight, glm::vec3(0.5f, -1.0f, 1.0f));
//                Fmodel_Knight = glm::scale(Fmodel_Knight, glm::vec3(0.1001f));//���������ű�����Ҫֻ��ģ�͵����ű�����һ��㼴��
//                FrameShader.setMat4("model", Fmodel_Knight);
//                FrameShader.setMat4("projection", projection);
//                FrameShader.setMat4("view", view);
//                FrameShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(Fmodel_Knight))));
//                FrameShader.SetFloat("FixRate", 0.0001f);//��0.001��0.0001��ֱ��Ч���Ǹı������Ĵ�ϸ
//                FrameShader.SetVec3("FrameColor", glm::vec3(1.0f, 0.0f, 0.0f));
//
//                //ģ����
//                glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//                glStencilFunc(GL_NOTEQUAL, 0b00000001, 0b00000001);//����model��Ӧλ�õ�ģ��ֵ�ֱ�Ϊ00000001��00000010���ص�����Ϊ00000011�����ǵ�һλ������1�ľ�����������ֻ�и�model�ı߿���ͨ�����
//                glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//                //��ʱ����ģ�建�����У�ֻ������model����λ���з����ģ��ֵ�����ಿ�־�Ϊ0
//
//                //��ȼ��
//                glDisable(GL_DEPTH_TEST);//��ʱ������ȼ�⣬ʹ�߿���Զ��ͨ����ȼ��
//
//                //����
//                PersonModel_Knight.Draw(FrameShader);
//
//                //�ָ���ȼ��
//                glEnable(GL_DEPTH_TEST);
//
//
//
//                /*��Ϧ�߿����*/
//                FrameShader.Use();
//                glm::mat4 Fmodel_JinXi = glm::mat4(1.0f);
//                Fmodel_JinXi = glm::translate(Fmodel_JinXi, glm::vec3(-0.5f, -1.0f, 1.0f));
//                Fmodel_JinXi = glm::scale(Fmodel_JinXi, glm::vec3(0.0801f));
//                FrameShader.setMat4("model", Fmodel_JinXi);
//                FrameShader.setMat4("projection", projection);
//                FrameShader.setMat4("view", view);
//                FrameShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(Fmodel_JinXi))));
//                FrameShader.SetFloat("FixRate", 0.0001f);
//                FrameShader.SetVec3("FrameColor", glm::vec3(0.0f, 0.0f, 1.0f));
//
//                //ģ����
//                glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//                glStencilFunc(GL_NOTEQUAL, 0b00000010, 0b00000010);//����model��Ӧλ�õ�ģ��ֵ�ֱ�Ϊ00000001��00000010���ص�����Ϊ00000011�����ǵڶ�λ������1�ľ�����������ֻ�и�model�ı߿���ͨ�����
//                glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//                //��ʱ����ģ�建�����У�ֻ������model����λ���з����ģ��ֵ�����ಿ�־�Ϊ0
//
//                //��ȼ��
//                glDisable(GL_DEPTH_TEST);//��ʱ������ȼ�⣬ʹ�߿���Զ��ͨ����ȼ��
//
//                //����
//                PersonModel_JinXi.Draw(FrameShader);
//
//                //�ָ���ȼ��
//                glEnable(GL_DEPTH_TEST);
//
//            }
//        }
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//        /*������Ⱦһ����������*/
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        glm::mat4 view = glm::mat4(1.0f);
//        view = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUP);
//        //ͶӰ����
//        glm::mat4 projection = glm::mat4(1.0f);
//        projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
//
//        //���ô��ڱ�����ɫ����ÿ��ѭ����������Ⱦ(�����ɫ�������Ȼ���)
//        glClearColor(0.678f, 0.847f, 0.902f, 1.0f);
//        glStencilMask(0xFF);//ȷ��ģ�建�������������
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//        //��������
//        //glm::vec3 LightColor(sin(glfwGetTime() * 1.0f)/2.0f+0.5f,sin(glfwGetTime() * 1.2f)/2.0f+0.5f,sin(glfwGetTime() * 1.5f)/2.0f+0.5f);
//        glm::vec3 LightColor(1.0f);//���Դ��ɫ
//        glm::vec3 DirectColor(1.0f);//�������ɫ
//        glm::vec3 FlashColor(0.0f);//�ֵ����ɫ
//        float InnerAngle = glm::cos(glm::radians(5.0f - rate));//�ֵ����Ȧ�Ƕȣ�����ֵ��
//        float OuterAngle = glm::cos(glm::radians(10.0f + rate));//�ֵ����Ȧ�Ƕȣ�����ֵ��
//        glm::vec3 LightPos(0.9f * sin(PassedTime), 0.9f * cos(PassedTime) + 0.5f, sin(PassedTime) + 2.0f);//��Դλ��
//        glm::vec3 LightDirection(1.0f, -1.0f, -1.0f);//ƽ�йⷽ��
//
//
//        //����ģ�ͻ���
//            /*��ʿģ�ͻ���*/
//        ModelShader_Knight.Use();
//        glActiveTexture(GL_TEXTURE2);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxCMTexture);
//
//        glm::mat4 model_Knight = glm::mat4(1.0f);
//        model_Knight = glm::translate(model_Knight, glm::vec3(0.5f, -1.0f, 1.0f));
//        model_Knight = glm::scale(model_Knight, glm::vec3(0.1f));
//        //�������
//        ModelShader_Knight.setMat4("projection", projection);
//        ModelShader_Knight.setMat4("view", view);
//        ModelShader_Knight.setMat4("model", model_Knight);
//        ModelShader_Knight.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model_Knight))));
//        ModelShader_Knight.SetVec3("ViewPos", CameraPos);
//        //����
//        ModelShader_Knight.SetFloat("material.Shininess", 32.0f);
//        ModelShader_Knight.SetInt("SkyBoxTexture", 2);
//        //���Դ
//        ModelShader_Knight.SetVec3("pointLight.Pos", LightPos);
//        ModelShader_Knight.SetVec3("pointLight.Ambient", LightColor * glm::vec3(0.2f));
//        ModelShader_Knight.SetVec3("pointLight.Diffuse", LightColor * glm::vec3(0.5f));
//        ModelShader_Knight.SetVec3("pointLight.Specular", LightColor * glm::vec3(1.0f));
//        ModelShader_Knight.SetFloat("pointLight.constant", 1.0f);
//        ModelShader_Knight.SetFloat("pointLight.linear", 0.35f);
//        ModelShader_Knight.SetFloat("pointLight.quadratic", 0.44f);
//        //�����
//        ModelShader_Knight.SetVec3("directLight.Dir", LightDirection);
//        ModelShader_Knight.SetVec3("directLight.Ambient", DirectColor * glm::vec3(0.2f));
//        ModelShader_Knight.SetVec3("directLight.Diffuse", DirectColor * glm::vec3(0.5f));
//        ModelShader_Knight.SetVec3("directLight.Specular", DirectColor * glm::vec3(1.0f));
//        //�ֵ��
//        ModelShader_Knight.SetVec3("flashLight.Pos", CameraPos);
//        ModelShader_Knight.SetVec3("flashLight.Dir", CameraFront);
//        ModelShader_Knight.SetVec3("flashLight.Diffuse", FlashColor * glm::vec3(0.5f));
//        ModelShader_Knight.SetVec3("flashLight.Specular", FlashColor * glm::vec3(1.0f));
//        ModelShader_Knight.SetFloat("flashLight.InnerAngle", InnerAngle);
//        ModelShader_Knight.SetFloat("flashLight.OuterAngle", OuterAngle);
//        ModelShader_Knight.SetFloat("flashLight.constant", 1.0f);
//        ModelShader_Knight.SetFloat("flashLight.linear", 0.35f);
//        ModelShader_Knight.SetFloat("flashLight.quadratic", 0.44f);
//
//        //ģ�����
//        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//        glStencilFunc(GL_ALWAYS, 0b00000001, 0xFF);//ģ��������ͨ��������ȼ��Ҳ��ͨ��������glStencilOp��������������ͼ�ε�ģ��ֵ�����滻Ϊ00000001
//        glStencilMask(0b00000001);//������������Ӧ�Ļ�����ģ��ֵ����Ϊ00000001
//        //��ʱ����ģ�建�����У�����������Ӧλ�õ�ģ��ֵΪ00000001������λ��Ϊ0
//
//        //����
//        PersonModel_Knight.Draw(ModelShader_Knight);
//
//
//
//
//        /*��ϫģ�ͻ���*/
//        ModelShader_JinXi.Use();
//
//        glm::mat4 model_JinXi = glm::mat4(1.0f);
//        model_JinXi = glm::translate(model_JinXi, glm::vec3(-0.5f, -1.0f, 1.0f));
//        model_JinXi = glm::scale(model_JinXi, glm::vec3(0.08f));
//        //�������
//        ModelShader_JinXi.setMat4("projection", projection);
//        ModelShader_JinXi.setMat4("view", view);
//        ModelShader_JinXi.setMat4("model", model_JinXi);
//        ModelShader_JinXi.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model_JinXi))));
//        ModelShader_JinXi.SetVec3("ViewPos", CameraPos);
//        //����
//        ModelShader_JinXi.SetFloat("material.Shininess", 32.0f);
//        //���Դ
//        ModelShader_JinXi.SetVec3("pointLight.Pos", LightPos);
//        ModelShader_JinXi.SetVec3("pointLight.Ambient", LightColor * glm::vec3(0.2f));
//        ModelShader_JinXi.SetVec3("pointLight.Diffuse", LightColor * glm::vec3(0.5f));
//        ModelShader_JinXi.SetVec3("pointLight.Specular", LightColor * glm::vec3(0.0f));
//        ModelShader_JinXi.SetFloat("pointLight.constant", 1.0f);
//        ModelShader_JinXi.SetFloat("pointLight.linear", 0.35f);
//        ModelShader_JinXi.SetFloat("pointLight.quadratic", 0.44f);
//        //�����
//        ModelShader_JinXi.SetVec3("directLight.Dir", LightDirection);
//        ModelShader_JinXi.SetVec3("directLight.Ambient", DirectColor * glm::vec3(0.2f));
//        ModelShader_JinXi.SetVec3("directLight.Diffuse", DirectColor * glm::vec3(0.5f));
//        ModelShader_JinXi.SetVec3("directLight.Specular", DirectColor * glm::vec3(0.0f));
//        //�ֵ��
//        ModelShader_JinXi.SetVec3("flashLight.Pos", CameraPos);
//        ModelShader_JinXi.SetVec3("flashLight.Dir", CameraFront);
//        ModelShader_JinXi.SetVec3("flashLight.Diffuse", FlashColor * glm::vec3(0.5f));
//        ModelShader_JinXi.SetVec3("flashLight.Specular", FlashColor * glm::vec3(0.0f));
//        ModelShader_JinXi.SetFloat("flashLight.InnerAngle", InnerAngle);
//        ModelShader_JinXi.SetFloat("flashLight.OuterAngle", OuterAngle);
//        ModelShader_JinXi.SetFloat("flashLight.constant", 1.0f);
//        ModelShader_JinXi.SetFloat("flashLight.linear", 0.35f);
//        ModelShader_JinXi.SetFloat("flashLight.quadratic", 0.44f);
//
//        //ģ�����
//        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//        glStencilFunc(GL_ALWAYS, 0b00000010, 0xFF);
//        glStencilMask(0b00000010);//������������Ӧ�Ļ�����ģ��ֵ����Ϊ00000010
//        //��ʱ����ģ�建�����У���model����Ӧλ�õ�ģ��ֵΪ00000010����һ��model����Ӧλ��Ϊ00000001�����������ص����֣����ص�����Ϊ00000011
//
//        //����
//        PersonModel_JinXi.Draw(ModelShader_JinXi);
//
//
//
//
//        /*�������*/
//            //������ɫ������
//        BoxReflectShader.Use();
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, EnvironmentCubeMap);
//
//        //ģ�;���
//        glm::mat4 model1 = glm::mat4(1.0f);//ÿһ֡��Ҫ���´���model�������ʹ����һ֡�Ѿ��任����model
//        model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, 2.0f));
//        //model1 = glm::rotate(model1, PassedTime * glm::radians(-50.0f), glm::vec3(1.0f, 1.0f, -1.0f));//������Ӧ����ת����Ӧ��λ��
//        model1 = glm::scale(model1, glm::vec3(0.25f));
//        BoxReflectShader.setMat4("model", model1);
//        //���߾���
//        BoxReflectShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model1))));//ģ�;������Ͻ�3x3���ֵ�������ת�þ���
//        BoxReflectShader.setMat4("view", view);
//        BoxReflectShader.setMat4("projection", projection);
//        BoxReflectShader.SetVec3("ViewPos", CameraPos);
//
//        BoxReflectShader.SetInt("EnvirmentCubeMap", 0);
//
//        //ģ����
//        glStencilFunc(GL_ALWAYS, 1, 0xFF);
//        glStencilMask(0x00);
//
//        //����  
//        BoxReflectMesh.Draw(BoxReflectShader);
//
//
//        /*ǽ�����*/
//        WallShader.Use();
//        glm::mat4 model2 = glm::mat4(1.0f);
//        model2 = glm::translate(model2, glm::vec3(0.0f, -0.2f, 1.5f));
//        model2 = glm::scale(model2, glm::vec3(1.5f));
//        //�������
//        WallShader.setMat4("model", model2);
//        WallShader.setMat4("view", view);
//        WallShader.setMat4("projection", projection);
//        WallShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model2))));
//        WallShader.SetVec3("ViewPos", CameraPos);
//        //����
//        WallShader.SetFloat("material.EmissionStrength", (float)(sin(PassedTime * 5.0f) * 0.25 + 0.75f));
//        WallShader.SetFloat("material.EmissionMovement", PassedTime * 1.5f);
//        WallShader.SetFloat("material.Shininess", 32.0f);
//        //���Դ
//        WallShader.SetVec3("pointLight.Pos", LightPos);
//        WallShader.SetVec3("pointLight.Ambient", LightColor * glm::vec3(0.2f));
//        WallShader.SetVec3("pointLight.Diffuse", LightColor * glm::vec3(0.5f));
//        WallShader.SetVec3("pointLight.Specular", LightColor * glm::vec3(1.0f));
//        WallShader.SetFloat("pointLight.constant", 1.0f);
//        WallShader.SetFloat("pointLight.linear", 0.35f);
//        WallShader.SetFloat("pointLight.quadratic", 0.44f);
//        //�����
//        WallShader.SetVec3("directLight.Dir", LightDirection);
//        WallShader.SetVec3("directLight.Ambient", DirectColor * glm::vec3(0.2f));
//        WallShader.SetVec3("directLight.Diffuse", DirectColor * glm::vec3(0.5f));
//        WallShader.SetVec3("directLight.Specular", DirectColor * glm::vec3(1.0f));
//        //�ֵ��
//        WallShader.SetVec3("flashLight.Pos", CameraPos);
//        WallShader.SetVec3("flashLight.Dir", CameraFront);
//        WallShader.SetVec3("flashLight.Diffuse", FlashColor * glm::vec3(0.5f));
//        WallShader.SetVec3("flashLight.Specular", FlashColor * glm::vec3(1.0f));
//        WallShader.SetFloat("flashLight.InnerAngle", InnerAngle);
//        WallShader.SetFloat("flashLight.OuterAngle", OuterAngle);
//        WallShader.SetFloat("flashLight.constant", 1.0f);
//        WallShader.SetFloat("flashLight.linear", 0.35f);
//        WallShader.SetFloat("flashLight.quadratic", 0.44f);
//
//        //ģ�����
//        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//        glStencilFunc(GL_ALWAYS, 1, 0xFF);//ǽ��ͨ����⣬������Ƭ��
//        glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//        //��ʱ����ģ�建�����У�ֻ������model����λ���з����ģ��ֵ�����ಿ�־�Ϊ0
//
//        //����
//        WallMesh.Draw(WallShader);
//
//
//
//        /*��Դ����*/
//        LightShader.Use();
//
//        glm::mat4 model3 = glm::mat4(1.0f);
//        model3 = glm::translate(model3, LightPos);
//        model3 = glm::scale(model3, glm::vec3(0.005f, 0.005f, 0.005f));
//        //�������
//        LightShader.setMat4("model", model3);
//        LightShader.setMat4("view", view);
//        LightShader.setMat4("projection", projection);
//
//        //ģ�����
//        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//        glStencilFunc(GL_ALWAYS, 1, 0xFF);//��Դͨ����⣬������Ƭ��
//        glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//        //��ʱ����ģ�建�����У�ֻ������model����λ���з����ģ��ֵ�����ಿ�־�Ϊ0
//
//        //����
//        LightModel.Draw(LightShader);
//
//
//
//        /*��������*/
//        if (IsKey_E)
//        {
//            /*�ݵ�*/
//            //����ȫ͸������(������ı߿�)������Ҫ��ϲ��ԣ�ֱ�Ӷ�����Ӧ͸���ȵ�Ƭ�μ���
//            GrassShader.Use();
//            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//            glStencilFunc(GL_ALWAYS, 1, 0xFF);//ͨ����⣬������Ƭ��
//            glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//            std::vector<glm::vec3> vegetation;
//            vegetation.push_back(glm::vec3(-0.7f, -0.5f, 1.0f));
//            vegetation.push_back(glm::vec3(0.7f, -0.5f, 1.0f));
//            vegetation.push_back(glm::vec3(0.0f, -0.5f, 1.0f));
//
//            glm::mat4 model5 = glm::mat4(1.0f);
//            GrassShader.setMat4("projection", projection);
//            GrassShader.setMat4("view", view);
//
//            for (unsigned int i = 0; i < vegetation.size(); i++)
//            {
//                model5 = glm::mat4(1.0f);
//                model5 = glm::translate(model5, vegetation[i]);
//                GrassShader.setMat4("model", model5);
//                GrassMesh.Draw(GrassShader);
//            }
//
//            /*����*/
//            //���ڰ�͸�����壺ʹ�û�ϲ��ԣ�ȷ������Ⱦ��͸������������Ⱦ��͸�������壬����ȾԶ���İ�͸����������Ⱦ�����İ�͸������
//            WindowShader.Use();
//            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//            glStencilFunc(GL_ALWAYS, 1, 0xFF);//ͨ����⣬������Ƭ��
//            glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//            glEnable(GL_BLEND);//���û��
//            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//            std::vector<glm::vec3> vegetation2;
//            vegetation2.push_back(glm::vec3(-0.35f, -0.5f, 1.0f));
//            vegetation2.push_back(glm::vec3(0.35f, -0.5f, 1.5f));
//            vegetation2.push_back(glm::vec3(0.0f, -0.5f, 2.0f));
//            std::map<float, glm::vec3> Sorted;
//            for (unsigned int i = 0; i < vegetation2.size(); i++)
//            {
//                float distence = glm::length(CameraPos - vegetation2[i]);
//                Sorted[distence] = vegetation2[i];
//            }
//            glm::mat4 model6 = glm::mat4(1.0f);
//            WindowShader.setMat4("projection", projection);
//            WindowShader.setMat4("view", view);
//
//            for (auto it = Sorted.rbegin(); it != Sorted.rend(); ++it)
//            {
//                model6 = glm::mat4(1.0f);
//                model6 = glm::translate(model6, it->second);
//                WindowShader.setMat4("model", model6);
//                WindowMesh.Draw(WindowShader);
//            }
//            glDisable(GL_BLEND);
//        }
//
//        /*��պ�*/
//        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//        glStencilFunc(GL_ALWAYS, 1, 0xFF);//ͨ��ģ����
//        glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//        glDepthFunc(GL_LEQUAL);//���ֵС�ڵ�����Ȼ�������Ƭ�α�����(�Ѿ���Ⱦ�����������λ�õ���պ�Ƭ�α���������δ��Ⱦ�����������λ��ͨ����ȼ�⣬��Ӧ��Ȼ���������Ϊ1.0(����Щ�ط���������1.0��Ϊ��Ȼ�����Ĭ��ֵΪ1.0))
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxCMTexture);
//        SkyBoxShader.Use();
//        SkyBoxShader.setMat4("view", glm::mat4(glm::mat3(view)));//ȥ��view�����е�ƽ�Ʋ��֣��������ʼ�մ�������ԭ��(�ƶ�ʱҲ��)
//        SkyBoxShader.setMat4("projection", projection);
//        SkyBoxShader.SetInt("SkyBoxCMTexture", 0);
//        SkyBoxMesh.Draw(SkyBoxShader);
//        glDepthFunc(GL_LESS);//����������ȼ�⺯��ΪLESSͨ��
//
//
//
//        /*�߿����*/
//        if (IsKey_F)
//        {
//            /*��ʿ�߿����*/
//            FrameShader.Use();
//            glm::mat4 Fmodel_Knight = glm::mat4(1.0f);
//            Fmodel_Knight = glm::translate(Fmodel_Knight, glm::vec3(0.5f, -1.0f, 1.0f));
//            Fmodel_Knight = glm::scale(Fmodel_Knight, glm::vec3(0.1001f));//���������ű�����Ҫֻ��ģ�͵����ű�����һ��㼴��
//            FrameShader.setMat4("model", Fmodel_Knight);
//            FrameShader.setMat4("projection", projection);
//            FrameShader.setMat4("view", view);
//            FrameShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(Fmodel_Knight))));
//            FrameShader.SetFloat("FixRate", 0.0001f);//��0.001��0.0001��ֱ��Ч���Ǹı������Ĵ�ϸ
//            FrameShader.SetVec3("FrameColor", glm::vec3(1.0f, 0.0f, 0.0f));
//
//            //ģ����
//            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//            glStencilFunc(GL_NOTEQUAL, 0b00000001, 0b00000001);//����model��Ӧλ�õ�ģ��ֵ�ֱ�Ϊ00000001��00000010���ص�����Ϊ00000011�����ǵ�һλ������1�ľ�����������ֻ�и�model�ı߿���ͨ�����
//            glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//            //��ʱ����ģ�建�����У�ֻ������model����λ���з����ģ��ֵ�����ಿ�־�Ϊ0
//
//            //��ȼ��
//            glDisable(GL_DEPTH_TEST);//��ʱ������ȼ�⣬ʹ�߿���Զ��ͨ����ȼ��
//
//            //����
//            PersonModel_Knight.Draw(FrameShader);
//
//            //�ָ���ȼ��
//            glEnable(GL_DEPTH_TEST);
//
//
//
//            /*��Ϧ�߿����*/
//            FrameShader.Use();
//            glm::mat4 Fmodel_JinXi = glm::mat4(1.0f);
//            Fmodel_JinXi = glm::translate(Fmodel_JinXi, glm::vec3(-0.5f, -1.0f, 1.0f));
//            Fmodel_JinXi = glm::scale(Fmodel_JinXi, glm::vec3(0.0801f));
//            FrameShader.setMat4("model", Fmodel_JinXi);
//            FrameShader.setMat4("projection", projection);
//            FrameShader.setMat4("view", view);
//            FrameShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(Fmodel_JinXi))));
//            FrameShader.SetFloat("FixRate", 0.0001f);
//            FrameShader.SetVec3("FrameColor", glm::vec3(0.0f, 0.0f, 1.0f));
//
//            //ģ����
//            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
//            glStencilFunc(GL_NOTEQUAL, 0b00000010, 0b00000010);//����model��Ӧλ�õ�ģ��ֵ�ֱ�Ϊ00000001��00000010���ص�����Ϊ00000011�����ǵڶ�λ������1�ľ�����������ֻ�и�model�ı߿���ͨ�����
//            glStencilMask(0x00);//�����¸���������Ӧ��ģ�建����
//            //��ʱ����ģ�建�����У�ֻ������model����λ���з����ģ��ֵ�����ಿ�־�Ϊ0
//
//            //��ȼ��
//            glDisable(GL_DEPTH_TEST);//��ʱ������ȼ�⣬ʹ�߿���Զ��ͨ����ȼ��
//
//            //����
//            PersonModel_JinXi.Draw(FrameShader);
//
//            //�ָ���ȼ��
//            glEnable(GL_DEPTH_TEST);
//
//
//
//
//        }
//        //����������
//        glfwSwapBuffers(window);
//    }
//
//    //����
//    glfwTerminate();
//    return 0;
//}
//
//
////���ڴ�С�ص�����
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    if (width == 0 || height == 0) return;//����С�����ڵ�ʱ���Զ��˳�����ֹaspectRatio�õ�����Ľ��
//    glViewport(0, 0, width, height);
//
//    aspectRatio = static_cast<float>(width) / static_cast<float>(height);
//}
//
////���ص�����
//void mouse_callback(GLFWwindow* window, double pos_x, double pos_y)
//{
//    float sensitivity = 0.05f;//������
//
//    if (FirstMouse)
//    {
//        pos_x = (double)LastX;
//        pos_y = (double)LastY;
//        FirstMouse = false;
//    }
//
//    if (!glfwGetKey(window, GLFW_KEY_LEFT_ALT) && !glfwGetKey(window, GLFW_KEY_RIGHT_ALT))
//    {
//        float offset_x = ((float)pos_x - LastX) * sensitivity;
//        float offset_y = (LastY - (float)pos_y) * sensitivity;
//        LastX = (float)pos_x;
//        LastY = (float)pos_y;
//
//        pitch += offset_y;
//        if (pitch >= 89.0f) pitch = 89.0f;
//        if (pitch <= -89.0f) pitch = -89.0f;
//        yaw += offset_x;
//
//        //���¹����ɿ�Alt
//        if (IsAlt && IsAltRealise)
//        {
//            //���¹�Alt���ɿ���ָ��Ƕȵ�����Alt֮ǰ��ֵ
//            pitch = Last_pitch;
//            yaw = last_yaw;
//            IsAlt = false;
//            IsAltRealise = false;
//        }
//
//        //��¼δ����Alt��ÿһ֡�ĽǶȣ����ܷ����ж�Alt�����ɿ�ǰ�������ɿ�Alt֮��ĵ�һ֡��ˢ�°���Alt֮ǰ����ĽǶȣ�
//        Last_pitch = pitch;
//        last_yaw = yaw;
//
//        glm::vec3 Front;
//        Front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
//        Front.y = sin(glm::radians(pitch));
//        Front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
//        CameraFront = glm::normalize(Front);
//    }
//}
//
////���ֻص�����
//void scroll_callback(GLFWwindow* window, double offset_x, double offset_y)
//{
//    FOV -= (float)offset_y;
//    if (FOV >= 45.0f) FOV = 45.0f;
//    if (FOV <= 1.0f) FOV = 1.0f;
//}
//
////���뺯��
//void processInput(GLFWwindow* window)
//{
//    //���ESC
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//    {
//        glfwSetWindowShouldClose(window, GL_TRUE);
//    }
//
//    //���space
//    CurrentSpacePressd = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
//    if (!LastSpacePressd && CurrentSpacePressd)
//    {
//        IsPaused = !IsPaused;
//    }
//    LastSpacePressd = CurrentSpacePressd;
//
//    //���Alt
//    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
//    {
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//��ʾ��겢�ָ����Χ
//        IsAlt = true;
//    }
//    else
//    {
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//������겢�����Ļ��Χ�����ڴ�����
//        if (IsAlt) IsAltRealise = true;//�����¹�Alt֮���ɿ�����Ч(��֤Alt�ɿ�֮���Ա�ĳ��֡��⵽��ʱ�򣬲������Իָ��Ƕ�)
//    }
//
//    //���AWSD
//    float CameraSpeed = 3.0f * PeriodTime;
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//    {
//        CameraPos += CameraSpeed * CameraFront;
//    }
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//    {
//        CameraPos -= CameraSpeed * CameraFront;
//    }
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//    {
//        CameraPos += CameraSpeed * glm::normalize(glm::cross(CameraUP, CameraFront));//ʹ��cross(CameraUP,CameraFront)���Զ�̬�ļ����������ת�������
//    }
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//    {
//        CameraPos -= CameraSpeed * glm::normalize(glm::cross(CameraUP, CameraFront));
//    }
//
//    //������·����
//    float RateSpeed = 5.0f * PeriodTime;
//    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
//    {
//        if (rate < 4.0f)
//        {
//            rate += RateSpeed;
//        }
//    }
//    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
//    {
//        if (rate > -2.0f)
//        {
//            rate -= RateSpeed;
//        }
//    }
//
//    //���F��
//    CurrentKeyPressd_F = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);
//    if (CurrentKeyPressd_F && !LastKeyPressd_F)
//    {
//        IsKey_F = !IsKey_F;
//    }
//    LastKeyPressd_F = CurrentKeyPressd_F;
//
//    //���E��
//    CurrentKeyPressd_E = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS);
//    if (CurrentKeyPressd_E && !LastKeyPressd_E)
//    {
//        IsKey_E = !IsKey_E;
//    }
//    LastKeyPressd_E = CurrentKeyPressd_E;
//
//    //���M��
//    CurrentKeyPressd_M = (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS);
//    if (CurrentKeyPressd_M && !LastKeyPressd_M)
//    {
//        IsKey_M = !IsKey_M;
//    }
//    LastKeyPressd_M = CurrentKeyPressd_M;
//}
//
//unsigned int LoadTexture(char const* path)
//{
//    unsigned int texture;
//    glGenTextures(1, &texture);//(�������������������Ĵ洢λ��)
//    glBindTexture(GL_TEXTURE_2D, texture);//�󶨸�������󵽵�ǰ������
//
//    //���ز���������
//    int width, height, nrChannels;//�洢ͼ��Ŀ�ȣ��߶ȣ���ɫͨ����
//    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);//����ͼ���ļ�������ת��Ϊ��������
//    if (data)
//    {
//        GLenum format;
//        if (nrChannels == 1)      format = GL_RED;
//        else if (nrChannels == 3) format = GL_RGB;
//        else if (nrChannels == 4) format = GL_RGBA;
//
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);//��ʵ�ʵ����������ϴ���GPU�����������(��ɫ������Ⱦʱֻ��֪������Ԫ���������꣬������ֱ�Ӵ�����������)
//        glGenerateMipmap(GL_TEXTURE_2D);//Ϊ�󶨵Ķ�ά�������ɶ༶��������
//
//        //Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
//        if (format == GL_RGBA)
//        {   //���Ʒ�ʽ
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//���������곬��[0.0, 1.0]��Χʱ����������ᱻ������0��1֮�䡣
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        }
//        else
//        {
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //���������곬��[0.0, 1.0]��Χʱ��������ظ����������Ĳ��ֻ����¿�ʼ���������һ�˽���ӳ��
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        }
//
//        //���˷�ʽ
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_set_flip_vertically_on_load(true);//��תͼ��y��(ȫ�����ú���������Ҫ��ת��ʱ��Ҫ��������Ϊfalse)
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "FAILED TO LOAD TEXTURE\n";
//        stbi_image_free(data);
//    }
//    return texture;
//}
//
//
//unsigned int LoadCubeTexture(std::vector<std::string> Faces_Path)
//{
//    unsigned int texture;
//    glGenTextures(1, &texture);//(�������������������Ĵ洢λ��)
//    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);//�󶨸�������󵽵�ǰ������
//
//    stbi_set_flip_vertically_on_load(false);//�ر�y�ᷴת
//
//    //���ز���������
//    int width, height, nrChannels;//�洢ͼ��Ŀ�ȣ��߶ȣ���ɫͨ����
//    for (unsigned int i = 0; i < Faces_Path.size(); i++)
//    {
//        unsigned char* data = stbi_load(Faces_Path[i].c_str(), &width, &height, &nrChannels, 0);//����ͼ���ļ�������ת��Ϊ��������
//        if (data)
//        {
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//��ʵ�ʵ����������ϴ���GPU�����������(��ɫ������Ⱦʱֻ��֪������Ԫ���������꣬������ֱ�Ӵ�����������)
//
//            //Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
//            //���Ʒ�ʽ
//            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//���������곬��[0.0, 1.0]��Χʱ����������ᱻ������0��1֮�䡣
//            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//            //���˷�ʽ
//            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//            stbi_image_free(data);
//        }
//        else
//        {
//            std::cout << "FAILED TO LOAD TEXTURE\n";
//            stbi_image_free(data);
//        }
//    }
//
//    return texture;
//}
//
////������ɫ��: ������ɫ������ÿ�����㣬�������λ���Լ��������ܵ����ԣ�����ɫ�����ߵȣ���/**������ɫ����ֻ�ж��������еĵ㣨һ���Ǳ߽�㣩
////��դ�� : �ڹ�դ���׶Σ�OpenGL ����ݶ������Ļ�ռ�����ȷ�������εı߽磬�����������λ���������ڲ���Ƭ�Σ������أ���λ�á�
////Ƭ����ɫ�� : ÿ�����ɵ�Ƭ�λᴫ�ݸ�Ƭ����ɫ�������н�һ�������������ɫ�͹��յȡ�/**Ƭ����ɫ���������ͼ�ε��������ص�
////**�Ӷ�����ɫ����Ƭ����ɫ�����ݵ������ǲ�ֵ�����(vShader out FragColor(���ж�����ɫ) -> fShader in FragColor(��ÿ�����ص����ɫ))
//
////�����������VAO����OpenGL�и��𽫶��㻺��������VBO���е����ݴ��ݸ�������ɫ��//
////һ��1.����һ��������VBO      2.���߸û�������δ洢����  3.�����ݴ��ݸ�VBO��
////����1.����һ������������VAO  2.����VAO����Ϊ��ǰVAO      3.����VAO��ν�������      4.�����ݴ��ݸ���ɫ��
//
///*����*/
////һ�������Ʒ�ʽ��������������Խ������
////����������ˣ�������ͬ����ɫ���ȡ����ʽ��ȡ�ٽ�ɫ���ȡ��ֵɫ�飩
////�����༶��Զ������������������Զʱ�����ѻ�ȡ�߷ֱ��ʵ���������Ԥ�ȴ���һ��Mipmap���ɺܶ�ֱ��ʲ����۰��ԭ������ɣ���Ȼ��ȡ�ٽ��ֱ��ʻ�ȡ��ֵ�ֱ���
//
///*����任*/
////�ֲ�����ϵ -> (ģ�ͱ任) -> ��������ϵ -> (��ͼ�任) -> �۲�����ϵ -> (ͶӰ�任) -> �ü�����ϵ -> (͸�ӳ���) -> �淶�豸����ϵ -> (�ӿڱ任) -> ��Ļ����ϵ -> ������Ȳ��Ե���Ⱦ����
//
//
///*����������w*/
//// �ڲü��ռ���w�������λ�á���Զ�ü���ȼ���ó�
////1.͸�ӳ������ڽ��ü��ռ������ת��Ϊ��׼���豸���꣨NDC��ʱ��x��y �� z ��ֱ���� w����һ����ȷ����͸��Ч������ȷ�ԡ�
////2.����жϣ�����Ȳ����У�z �����ֵ���� w ���ʹ�ã�����ȷ���ĸ�ƬԪ�ǿɼ��ġ�ͨ����Ȼ������洢��һ��������ֵ��ȷ�������е����尴�վ�����ȷ��Ⱦ��
////3.�ü����ڲü������У�w �����ж϶����Ƿ��ڲü��ռ����Ч��Χ�ڣ��� - w �� w�������ĳ������� x / w��y / w �� z / w ���������Χ����ö���ᱻ�ü�����
//
///*���ڡ��ӿ��뻭��*/
////1.�����൱������ӿ��൱������еĻ�ֽ����Ⱦ��ͼ�ξ��ǻ�ֽ�ϵ����� 
////2.͸�Ӿ����еĿ�߱Ⱦ��ǻ�ֽ�����ݵĿ�߱ȣ����ô��ڴ�С����ʱ����������䣬��Ҫ�ڸı䴰�ڴ�С�ĸı�͸�Ӿ����߱�
////������������е����Ͻǵ�aPosƥ�������������(1,1),��������ͼƬ�����Ͻǵ����ؽ��������Ͻ�aPosλ�õ������غϣ��������������(0.5,0.5)�������Ͻ�aPosλ�õ����ػ�������ͼƬ�����ĵ����� 
//
///*ʹ�ò�ͬ��VAO����ɫ�����Ʋ�ͬͼ��*/
////һ�������ʹ�ò�ͬ�Ķ���������Ʋ�ͬ��ͼ�Σ�����Ҫ���ò�ͬ��VAO
////���������ʹ�ò�ͬ����Ⱦ��ʽ���Ʋ�ͬ��ͼ�Σ�����Ҫ���ò�ͬ����ɫ���������嶼��Shader.h,������Ķ�����ɫ����Ƭ����ɫ��������ͬ��
////���������������ɫ��֮�以���������ɫ�������еı���ֵ�����̳У����û������ĳ����ɫ�������еı��������������ΪĬ��ֵ0������ʣ�
////�ġ����Ƶڶ���ͼ��ʱ����Ҫ�ڶ�����ͬ��ģ�;���model2���õڶ�����ɫ��myshader2�����䴫����ɫ�����򣬵���ͼ�����͸�Ӿ�����Ҫ���ⶨ�壬ֱ����view1��projection1���ɣ��󶨵ڶ���ͼ�ε�VAO2���л���
//
///*��Ȼ���*/
////1.��Ȳ�����Ƭ����ɫ��֮��������ɫд����ɫ����֮ǰ����
////2.��Ȳ��Խ���ǰƬ�ε����ֵ����Ȼ����е����ֵ���бȽϣ�ͨ����Ȳ��Ե�Ƭ�����ֵ�����µ���Ȼ������� 
////3.���ղ�����Ȼ������е�Ƭ�β��ᱻ��Ⱦ�������ڵ������������Լ�������Ƭ�ε�ȫ�����ݣ����á���ǰ��Ȳ��ԡ��Ż���
////4.���ֵ���ȣ����۲�ռ��е�zֵ��������׶���ƽ���Զƽ���м��ĳ��ֵ���任����Ȼ������У��任��0.0��1.0֮�䣩����ʹ�÷����Է��̣�����Ļ���Ļ���z���ȸ��ߣ�����������ǰ���ϵ��
//
///*��ɫ���塢��Ȳ��ԡ�ģ�����*/
////1.��ɫ�������Ȳ��Եĸ���ͨ����������Ƭ����Ⱦ���֮��ͳһ���еģ����ֲ���ƽ�н��У�ֻ�����ֲ��Զ�ͨ����Ƭ�βŻᱻ������ɫ��������
////2.ģ�����������ÿһ��������Ⱦ�󣨵õ�Ƭ�κ󣩽��еġ�����Ⱦһ������ͽ���һ��ģ����ԡ�������Ⱦ�����幫��һ��ģ�建����
////3.ģ�建��������Ļ���Ӧ����Ļ��ÿһ�����ص㶼��ģ�建�����д���һ��8λ��ģ��ֵ��
////5.����������Ļ�����½ǻ��ƣ��������ģ��ֵҲ��ģ�建����(��Ӧ����Ļ����λ��)�����½ǡ�ģ����ʱҲֻ��������ڻ������ж�Ӧ��ģ��ֵ��
////6.ģ�建�����е�ģ��ֵĬ��Ϊ0
////7.ģ�建��������glClear(GL_STENCIL_BUFFER_BIT)���ܵ�ģ�������Ӱ�죬����Ѿ�������glStencilMask(0x00)�����޷�����
////8.ģ����ԣ�����ģ����� -> ����ģ�庯��(��������ģ����Ե�����)��ģ�����(������θ���ģ��ֵ) -> ����ģ������(����ģ��ֵ����Щλ���޸ģ�ģ�庯������������ļ�λ������) -> ��Ⱦ����(��ʱģ�建���������ģ����ԺͲ�������)
////9.���������������Ⱦ��֮������Ⱦ�߿���������ʹ�߿򲻻ᱻ��������(���ֵ��С��)����
////10.����Ȳ��ԡ�ģ������йصĲ���(���ò�����Func���������������OP����)����ȫ�ֵģ���ǰ���ĸ�֡�����������ĸ�֡�����¹���
////11.����glStencilOp()��
////(*��������͸���ڵ��￴ģ��) 
////(1).������Ⱦģ�ͺ���Ⱦ�ڵ���ʱ����ʱģ�͵�ģ�������ȼ���ͨ�������õ��ǵ������������ʵ�����������Ҫ����ΪGL_REPLACE���ܽ�ģ�Ͷ�Ӧλ�õ�ģ��ֵ����Ϊ1
////(2).������Ⱦ�ڵ������Ⱦģ��ʱ����ʱģ��ֻ��ͨ��ģ�����޷�ͨ����ȼ��,���õ��ǵڶ����������ʵڶ���������Ҫ����ΪGL_REPLACE���ܽ�ģ�Ͷ�Ӧλ�õ�ģ��ֵ����Ϊ1��������ó�GL_KEEP����ģ�Ͷ�Ӧλ��ģ��ֵ��Ϊ0                       
////(3).���벻����ģ�ͺ��ڵ������Ⱦ˳����Ȼ�ܴﵽ��Ҫ�Ľ������ڶ����͵���������������ΪGL_REPLACE
//
///*��ϲ���*/
////1.����ȫ͸������(������ı߿�)������Ҫ��ϲ��ԣ�ֱ�Ӷ�����Ӧ͸���ȵ�Ƭ�μ���
////2.���ڰ�͸�����壺ʹ�û�ϲ��ԣ�ȷ������Ⱦ��͸������������Ⱦ��͸�������壬����ȾԶ���İ�͸����������Ⱦ�����İ�͸������
//
///*֡����*/
////Ĭ��֡���壺�ɴ���ϵͳ�ṩ��֡���壬����Ļ��ʾ�����������Ⱦ��������һ��
////����֡���壺�ֶ�����G_Buffer����ֱ������Ļ��ʾ�����������м���Ⱦ����ʵ��������ȾЧ��
//// һ������ʹ��Ĭ��֡���彫���ֱ����Ⱦ����Ļ��
//// ��������ʹ��ʹ������֡���彫���(���Ǳ�����Ļ�ϻ���ֵ�����)����Ⱦ���Խ�G_Buffer����ɫ��������(��ʱ���൱�ڰ�ԭ��������Ļ�ϵ���������һ����ͼ)��
//// �ٽ�G_Buffer�е����ݴ���֮������Ⱦ����Ļ��(��Ҫ��һ���������е������ͼ)
//// 
////֡��������
////�� OpenGL �У�ͨ����Ҫ�� GPU ��׼����������λ�ã�
////һ������λ����Ϊ����Ŀ�꣺���ڶ�ȡ���ݣ�ͨ�����ں������㡢����ʹ��֮ǰ��Ⱦ����ĳ����С����磬����Ч����ʹ����Ⱦ���������Ϊ���롣
////һ������λ����Ϊд��Ŀ�꣺��֡����ĸ���������������ڴ洢��Ⱦ�����Ľ������Ⱦ��ͼ��ᱻд�뵽��������У�������ֱ����ʾ����Ļ�ϡ�
////��һ�� ֡�������Framebuffer, FBO�� �У������ж��������������
////�������������������ɫ�������������������ģ������������һ����ȸ��� �� һ��ģ�帽����
//// 
////��������
////���������Է�Ϊ�������ͣ����磺��ɫ�������������������ģ��������
////������ֻ��һ��ͳ�ƣ�ʵ���Ͼ������ɫ���ݡ�����������ݡ�ģ���������ݶ������������µ���ɫ�����������������ģ����������
////��ȸ����������������ģ�帽����ģ����������
////��ȸ��� or ģ�帽��������Ȼ�������ģ�建����������ֻ��������ȼ���ģ���⣬���������޷�������
////��������� or ģ�������������������������ܴ洢�����Ϣ��ģ����Ϣ�����ܱ�����ʹ�ã���ɺ���Ч��
//
///*��������ͼ*/
////��պУ�
////һ��ʹ��glm::mat4(glm::mat3(view))ȥ��ƽ�ƣ����������Ƿ��ƶ����������λ��ʼ��������ԭ�㣬ʼ������պ��ڲ�
////����һֱ��һ�������ƶ��������壬��ʹ��׶�����õúܴ�������Ȼ�п����Ƴ���պеķ�Χ�����������ǽ�������պе����д�룬
////��պл�ʼ�ձ������ڳ�������Զ��(���ֵΪ1.0����Զ���)�������Ḳ���������壬���Լ�ʹ�����Ƴ�����պеķ�Χ����պ���Ȼ������Ⱦʱ��������������
////����������ǵ�һ����Ⱦ��պ��ҽ������д�룬�򲻻Ḳ�Ǻ���Ⱦ�����壬���ں�����Ⱦ��������ʱ����Ҫ�������ڵ�����պ�Ƭ�Σ�
////�������ǿ��������Ⱦ��պв���OpenGLʼ����Ϊ��պе����ֵΪ1.0(��Զ���ֵ)
////�ġ������������������˳����Ⱦ��պ�(�Ż�����Ƭ����Ⱦ��)������Ҫ��ʱ����ȼ�⺯����ΪLEAQAL,��֤��պн�����δ��Ļ����Ⱦ�������λ��ͨ����ȼ�⣬ͬʱ����Щλ�����ֵ��Ϊ1.0(��Ȼ��������1.0)
//
//
//
//
//
//
//
//
////std::vector<Vertex> BoxVertices = {
////    //aPos                  //aNormal           //aTexture
////    // ���� (z = -0.5)
////    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},// ���½�
////    {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},// ���½�
////    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},// ���Ͻ�
////    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},// ���Ͻ�
////    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},// ���Ͻ�
////    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},// ���½�
////
////    // ǰ�� (z = 0.5)
////    {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// ���½�
////    {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},// ���½�
////    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
////    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},// ���Ͻ�
////    {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},// ���Ͻ�
////    {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},// ���½�
////
////    // ���� (x = -0.5)
////    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
////    {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},// ���ҽ�
////    {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
////    {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
////    {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// �����
////    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
////
////    // ���� (x = 0.5)
////    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
////    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},// ���ҽ�
////    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
////    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},// ���ҽ�
////    {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// �����
////    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},// �����
////
////    // ���� (y = -0.5)
////    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},// ���Ͻ�
////    {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},// ���Ͻ�
////    {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
////    {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
////    {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},// ���½�
////    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},// ���Ͻ�
////
////    // ���� (y = 0.5)
////    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},// ���Ͻ�
////    {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},// ���Ͻ�
////    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
////    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},// ���½�
////    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},// ���½�
////    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}// ���Ͻ�
////};
////
////std::vector<unsigned int> BoxIndices = {
////    // ����
////    0, 1, 2,
////    3, 4, 5,
////    // ǰ��
////    6, 7, 8,
////    9, 10, 11,
////    // ����
////    12, 13, 14,
////    15, 16, 17,
////    // ����
////    18, 19, 20,
////    21, 22, 23,
////    // ����
////    24, 25, 26,
////    27, 28, 29,
////    // ����
////    30, 31, 32,
////    33, 34, 35
////};
//// 
////std::vector<Texture> BoxTextures = 
////{
////    {BoxTexture1,"DiffuseColorSampler","source/Makima.jpg"},//��ͼ��ַ�ɸ��ɲ���
////    {BoxTexture2,"DiffuseColorSampler","source/Makima3.jpg"},
////    {BoxTexture1,"SpecularColorSampler","source/Makima.jpg"},
////    {BoxTexture2,"SpecularColorSampler","source/Makima3.jpg"}
////};
//// 
//// 
////     /*�������*/
////        //������ɫ������
////BoxShader.Use();
////
//////��������ɼ���
////RateControl(window, rate);
////BoxShader.SetFloat("rate", rate);
////
//////ģ�;���
////glm::mat4 model1 = glm::mat4(1.0f);//ÿһ֡��Ҫ���´���model�������ʹ����һ֡�Ѿ��任����model
////model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, 0.5f));
//////model1 = glm::rotate(model1, PassedTime * glm::radians(-50.0f), glm::vec3(1.0f, 1.0f, -1.0f));//������Ӧ����ת����Ӧ��λ��
////model1 = glm::scale(model1, glm::vec3(0.5f));
////BoxShader.setMat4("model", model1);
//////���߾���
////BoxShader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model1))));//ģ�;������Ͻ�3x3���ֵ�������ת�þ���
////BoxShader.setMat4("view", view);
////BoxShader.setMat4("projection", projection);
////BoxShader.SetVec3("ViewPos", CameraPos);
////
//////���Դ
////BoxShader.SetVec3("pointLight.Pos", LightPos);
////BoxShader.SetVec3("pointLight.Ambient", LightColor* glm::vec3(0.2f));
////BoxShader.SetVec3("pointLight.Diffuse", LightColor* glm::vec3(0.5f));
////BoxShader.SetVec3("pointLight.Specular", LightColor* glm::vec3(1.0f));
////BoxShader.SetFloat("pointLight.constant", 1.0f);
////BoxShader.SetFloat("pointLight.linear", 0.35f);
////BoxShader.SetFloat("pointLight.quadratic", 0.44f);
//////�����
////BoxShader.SetVec3("directLight.Dir", LightDirection);
////BoxShader.SetVec3("directLight.Ambient", DirectColor* glm::vec3(0.2f));
////BoxShader.SetVec3("directLight.Diffuse", DirectColor* glm::vec3(0.5f));
////BoxShader.SetVec3("directLight.Specular", DirectColor* glm::vec3(1.0f));
//////�ֵ��
////BoxShader.SetVec3("flashLight.Pos", CameraPos);
////BoxShader.SetVec3("flashLight.Dir", CameraFront);
////BoxShader.SetVec3("flashLight.Diffuse", FlashColor* glm::vec3(0.5f));
////BoxShader.SetVec3("flashLight.Specular", FlashColor* glm::vec3(1.0f));
////BoxShader.SetFloat("flashLight.InnerAngle", glm::cos(glm::radians(5.0f)));
////BoxShader.SetFloat("flashLight.OuterAngle", glm::cos(glm::radians(10.0f)));
////BoxShader.SetFloat("flashLight.constant", 1.0f);
////BoxShader.SetFloat("flashLight.linear", 0.35f);
////BoxShader.SetFloat("flashLight.quadratic", 0.44f);
//////���ò���
////BoxShader.SetFloat("material.Shininess", 32.0f);
////
//////ģ����
////glStencilFunc(GL_ALWAYS, 1, 0xFF);
////glStencilMask(0xFF);//������������Ӧ�Ļ�����ģ��ֵ����Ϊ1
//////��ʱ����ģ�建�����У������ӵĶ�Ӧλ�õ�ģ��ֵΪ1������λ��Ϊ0
////
//////����  
////BoxMesh.Draw(BoxShader);