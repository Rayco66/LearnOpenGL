#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h> 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//������ƶ�����
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//�����Ĭ�ϲ���ֵ
const float YAW = -90.0f;//ƫ����
const float PITCH = 0.0f;//������
const float SPEED = 2.5f;//������ƶ��ٶ�  
const float SENSITIVITY = 0.1F;//�������ת������
const float FOV = 45.0f;//�������Ұ��Χ�Ƕ�



class Camera
{
public:
	//���������
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;//�������������Ϸ���
	glm::vec3 Right;
	glm::vec3 WorldUp;//��������ϵ�Ϸ���
	//ŷ����
	float Yaw;
	float Pitch;
	float Last_Yaw;//��һ֡��Yawֵ
	float Last_Pitch;//��һ֡��Pitchֵ
	//���������
	float Speed;
	float Sensitivity;
	float Fov;
	//�������
	bool IfRollBack = false;//�Ƿ�Yaw��Pitch���ݵ�֮ǰ��֡(���¾���ĳ������ǰ��֡)

	Camera(glm::vec3 pos = glm::vec3(0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),float yaw = YAW,float pitch = PITCH) : Front(glm::vec3(0.0f,0.0f,-1.0f)),Speed(SPEED),Sensitivity(SENSITIVITY),Fov(FOV)
	{
		this->Pos = pos;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		//����camera����
		UpdateCameraVectors();
	}

	//��ȡView����
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Pos,this->Pos + this->Front,this->Up);//(�����λ�ã��۲�Ŀ������(��ͼ�����з�������= Ŀ��λ��-�����λ�ã��ʼ�����������걣֤���ƶ�Ҳ��ά�����߷��򲻱䣬��Ϊ����������ǰ��������ĳ���̶��ĵ�)�������������Ϸ���)
	}

	//cameraλ�ñ仯
	void ProcessWASD(Camera_Movement Direction,float deltaTime)
	{
		float movement = this->Speed * deltaTime;
		if (Direction == FORWARD)	this->Pos += this->Front * movement;
		if (Direction == BACKWARD)	this->Pos -= this->Front * movement;
		if (Direction == LEFT)		this->Pos -= this->Right * movement;
		if (Direction == RIGHT)		this->Pos += this->Right * movement;
	}

	//camera�Ƕȱ仯
	void ProcessMouseMove(float offset_x, float offset_y, bool IfFlip = false)
	{
		offset_x *= this->Sensitivity;
		offset_y *= this->Sensitivity;

		this->Yaw += offset_x;
		this->Pitch += offset_y;
		if (!IfFlip)//���������ǲ�����ת���ʱ
		{
			if (this->Pitch >= 89.0f)  this->Pitch = 89.0f;
			if (this->Pitch <= -89.0f) this->Pitch = -89.0f;
		}

		//RollBack��Yaw��Pitch�ָ�����һ֡(��δ����Altʱ)��ֵ
		if (IfRollBack)
		{
			this->Yaw = Last_Yaw;
			this->Pitch = Last_Pitch;
			IfRollBack = false;
		}
		//��¼ÿһ֡�ĽǶȣ�������RollBackǰ��¼�������ɿ�Alt֮��ĵ�һ֡��ˢ�°���Alt֮ǰ����ĽǶȣ�
		this->Last_Yaw = this->Yaw;
		this->Last_Pitch = this->Pitch;


		//����camera����
		UpdateCameraVectors();
	}

	//camera���ű仯
	void ProcessMouseScroll(float offset_x,float offset_y)
	{
		this->Fov -= offset_y;
		if (this->Fov >= 45.0f) this->Fov = 45.0f;
		if (this->Fov <= 1.0f) this->Fov = 1.0f;
	}



private:
	void UpdateCameraVectors()
	{
		//������º���������������
		glm::vec3 front = glm::vec3(1.0f);
		front.x = cos(glm::radians(this->Pitch)) * cos(glm::radians(this->Yaw));
		front.y = sin(glm::radians(this->Pitch));
		front.z = cos(glm::radians(this->Pitch)) * sin(glm::radians(this->Yaw));
		this->Front = glm::normalize(front);

		//���¼������������ϵ��������������
		this->Right = glm::normalize(glm::cross(this->Front,this->WorldUp));//����������������ϵ�Ϸ�������������ҷ���(�����������Up������Σ��ҷ���Ӧ�����Ǻ�WorldUp��ֱ)
		this->Up = glm::normalize(glm::cross(this->Right,this->Front));//�����ϵ���Ϸ����������ҷ���ͳ������
	}
};

#endif
