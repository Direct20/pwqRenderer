#pragma once
#include "light_base.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//////////////////////////////////////////////////////////////////////////////
//ƽ�й�Դ��(˥����Ч)
F_REPLICABLE class FLightParallel :public FLight
{
public:
	FLightParallel()
	{
		_slot = _nextSlot[1]++;
	}
public:
	//���ƹ�����ݸ��Ƶ���ɫ���ĵ�_slot��ƽ�й�Դ��������
	virtual void setUniformData(GLuint _hProgram);

	//���ɵƹ�ռ�任����
	virtual glm::mat4 _genLightMatrix()
	{
		return glm::lookAt(pos, pos + direction, glm::vec3(0.f, 1.f, 0.f));
	}
	//����ͶӰ�任����
	virtual glm::mat4 _genProjectionMatrix()
	{
		return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, .1f, 30.f);
	}
	//���ݷ����Զ�����λ��
	//_ratio:���뱶��
	void autoPos(GLfloat _ratio = 10.f)
	{
		pos = (glm::vec3(0.f) - normalize(direction)) * _ratio;
	}
public:
	//����(��д)
	glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
};