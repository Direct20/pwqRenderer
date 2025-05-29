#pragma once
#include "base/base.h"
#include "misc/queue_manager.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "particle.h"
//////////////////////////////////////////////////////////////////////////////
//���ӷ�������
class FParticleEmitter :public FBase
{
public:
	//����ʱ��
	void update(GLfloat _time)
	{
		if (prevTime == 0.f)
		{
			prevTime = _time;
			return;
		}

	}
	//��������
	void _emit()
	{
		FParticle* particle = q.newOne();
	}
public:
	//��һ�εĸ���ʱ��
	GLfloat prevTime = 0.f;
	//��������ʱ����(s)
	GLfloat interval = 0.01f;
	//λ��
	glm::vec3 p = glm::vec3(0.f, 5.f, 0.f);
	//���䷽��
	glm::vec3 direction = glm::vec3(0.f, 1.f, 0.f);
	//���ӹ�����
	FManagerQueue<FParticle> q;
};