#pragma once
#include "base/base.h"
#include "camera/camera.h"
#include <glm/glm.hpp>
//����������
F_REPLICABLE class FPlayerCamera :public FCamera
{
public:
	//����
	void creep()
	{

	}
	//�¶�
	void crouch()
	{

	}
	//��Ծ
	void jump()
	{
	}
	//�ָ�����
	void normal()
	{

	}
	//����������λ��
	glm::vec3 getPosition()
	{
		return glm::vec3(e.x, e.y - height, e.z);
	}
public:
	//����������߶�
	GLfloat height = 3.f;
};