#pragma once
#include "base/base.h"
#include "light_base.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//////////////////////////////////////////////////////////////////////////////
//�۹����
F_REPLICABLE class FLightSpot :public FLight
{
public:
	FLightSpot()
	{
		_slot = _nextSlot[2]++;

		innerCutOff = glm::radians(12.5f);
		outterCutOff = glm::radians(20.f);
	}
public:
	//���ƹ�����ݸ��Ƶ���ɫ���ĵ�_slot���۹�Ƶ�������
	virtual void setUniformData(GLuint _hProgram);

	//���ɵƹ�ռ�任����
	virtual glm::mat4 _genLightMatrix()
	{
		return glm::lookAt(pos, pos + direction, glm::vec3(0.f, 1.f, 0.f));
	}
	//����ͶӰ�任����
	virtual glm::mat4 _genProjectionMatrix()
	{
		return glm::mat4(1.f);
	}
public:
	//����
	glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
	//�й��(��Բ׶),������
	GLfloat innerCutOff;
	//�й��(��Բ׶),������
	GLfloat outterCutOff;
};

