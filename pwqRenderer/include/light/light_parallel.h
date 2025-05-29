#pragma once
#include "light_base.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//////////////////////////////////////////////////////////////////////////////
//平行光源类(衰减无效)
F_REPLICABLE class FLightParallel :public FLight
{
public:
	FLightParallel()
	{
		_slot = _nextSlot[1]++;
	}
public:
	//将灯光的数据复制到着色器的第_slot个平行光源的数据中
	virtual void setUniformData(GLuint _hProgram);

	//生成灯光空间变换矩阵
	virtual glm::mat4 _genLightMatrix()
	{
		return glm::lookAt(pos, pos + direction, glm::vec3(0.f, 1.f, 0.f));
	}
	//生成投影变换矩阵
	virtual glm::mat4 _genProjectionMatrix()
	{
		return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, .1f, 30.f);
	}
	//根据方向自动生成位置
	//_ratio:距离倍数
	void autoPos(GLfloat _ratio = 10.f)
	{
		pos = (glm::vec3(0.f) - normalize(direction)) * _ratio;
	}
public:
	//方向(读写)
	glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
};