#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h> 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//摄像机移动类型
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//摄像机默认参数值
const float YAW = -90.0f;//偏航角
const float PITCH = 0.0f;//俯仰角
const float SPEED = 2.5f;//摄像机移动速度  
const float SENSITIVITY = 0.1F;//摄像机旋转灵敏度
const float FOV = 45.0f;//摄像机视野范围角度



class Camera
{
public:
	//摄像机参数
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;//摄像机相对坐标上方向
	glm::vec3 Right;
	glm::vec3 WorldUp;//世界坐标系上方向
	//欧拉角
	float Yaw;
	float Pitch;
	float Last_Yaw;//上一帧的Yaw值
	float Last_Pitch;//上一帧的Pitch值
	//摄像机操作
	float Speed;
	float Sensitivity;
	float Fov;
	//按键检测
	bool IfRollBack = false;//是否将Yaw和Pitch回溯到之前的帧(按下具体某个按键前的帧)

	Camera(glm::vec3 pos = glm::vec3(0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),float yaw = YAW,float pitch = PITCH) : Front(glm::vec3(0.0f,0.0f,-1.0f)),Speed(SPEED),Sensitivity(SENSITIVITY),Fov(FOV)
	{
		this->Pos = pos;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		//更新camera方向
		UpdateCameraVectors();
	}

	//获取View矩阵
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Pos,this->Pos + this->Front,this->Up);//(摄像机位置，观察目标坐标(视图矩阵中方向向量= 目标位置-摄像机位置，故加上摄像机坐标保证其移动也能维持视线方向不变，恒为摄像机朝向的前方而不是某个固定的点)，世界坐标中上方向)
	}

	//camera位置变化
	void ProcessWASD(Camera_Movement Direction,float deltaTime)
	{
		float movement = this->Speed * deltaTime;
		if (Direction == FORWARD)	this->Pos += this->Front * movement;
		if (Direction == BACKWARD)	this->Pos -= this->Front * movement;
		if (Direction == LEFT)		this->Pos -= this->Right * movement;
		if (Direction == RIGHT)		this->Pos += this->Right * movement;
	}

	//camera角度变化
	void ProcessMouseMove(float offset_x, float offset_y, bool IfFlip = false)
	{
		offset_x *= this->Sensitivity;
		offset_y *= this->Sensitivity;

		this->Yaw += offset_x;
		this->Pitch += offset_y;
		if (!IfFlip)//调整俯仰角不允许翻转相机时
		{
			if (this->Pitch >= 89.0f)  this->Pitch = 89.0f;
			if (this->Pitch <= -89.0f) this->Pitch = -89.0f;
		}

		//RollBack将Yaw和Pitch恢复到上一帧(尚未按下Alt时)的值
		if (IfRollBack)
		{
			this->Yaw = Last_Yaw;
			this->Pitch = Last_Pitch;
			IfRollBack = false;
		}
		//记录每一帧的角度（不能在RollBack前记录，否则松开Alt之后的第一帧会刷新按下Alt之前保存的角度）
		this->Last_Yaw = this->Yaw;
		this->Last_Pitch = this->Pitch;


		//更新camera方向
		UpdateCameraVectors();
	}

	//camera缩放变化
	void ProcessMouseScroll(float offset_x,float offset_y)
	{
		this->Fov -= offset_y;
		if (this->Fov >= 45.0f) this->Fov = 45.0f;
		if (this->Fov <= 1.0f) this->Fov = 1.0f;
	}



private:
	void UpdateCameraVectors()
	{
		//计算更新后的摄像机方向向量
		glm::vec3 front = glm::vec3(1.0f);
		front.x = cos(glm::radians(this->Pitch)) * cos(glm::radians(this->Yaw));
		front.y = sin(glm::radians(this->Pitch));
		front.z = cos(glm::radians(this->Pitch)) * sin(glm::radians(this->Yaw));
		this->Front = glm::normalize(front);

		//重新计算摄像机坐标系右向量和上向量
		this->Right = glm::normalize(glm::cross(this->Front,this->WorldUp));//用摄像机朝向和世界系上方向计算出摄像机右方向(不论摄像机的Up向量如何，右方向应该总是和WorldUp垂直)
		this->Up = glm::normalize(glm::cross(this->Right,this->Front));//摄像机系的上方向则是用右方向和朝向求出
	}
};

#endif
