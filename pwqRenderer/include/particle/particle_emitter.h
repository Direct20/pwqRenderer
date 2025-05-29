#pragma once
#include "base/base.h"
#include "misc/queue_manager.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "particle.h"
//////////////////////////////////////////////////////////////////////////////
//粒子发射器类
class FParticleEmitter :public FBase
{
public:
	//更新时间
	void update(GLfloat _time)
	{
		if (prevTime == 0.f)
		{
			prevTime = _time;
			return;
		}

	}
	//发射粒子
	void _emit()
	{
		FParticle* particle = q.newOne();
	}
public:
	//上一次的更新时间
	GLfloat prevTime = 0.f;
	//发射粒子时间间隔(s)
	GLfloat interval = 0.01f;
	//位置
	glm::vec3 p = glm::vec3(0.f, 5.f, 0.f);
	//发射方向
	glm::vec3 direction = glm::vec3(0.f, 1.f, 0.f);
	//粒子管理器
	FManagerQueue<FParticle> q;
};