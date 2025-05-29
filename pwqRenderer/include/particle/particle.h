#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
//////////////////////////////////////////////////////////////////////////////
//粒子类
class FParticle
{
public:
	FParticle() {}
	~FParticle() {}
	//更新时间
	void update()
	{

	}
public:
	GLfloat prevTime;
	//时间(<=0时生命结束)
	float t;
	//位置
	glm::vec3 p;
	//速度
	glm::vec3 v;

	GLfloat opacity;
};