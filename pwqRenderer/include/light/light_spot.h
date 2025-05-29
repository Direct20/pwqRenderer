#pragma once
#include "base/base.h"
#include "light_base.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//////////////////////////////////////////////////////////////////////////////
//聚光灯类
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
	//将灯光的数据复制到着色器的第_slot个聚光灯的数据中
	virtual void setUniformData(GLuint _hProgram);

	//生成灯光空间变换矩阵
	virtual glm::mat4 _genLightMatrix()
	{
		return glm::lookAt(pos, pos + direction, glm::vec3(0.f, 1.f, 0.f));
	}
	//生成投影变换矩阵
	virtual glm::mat4 _genProjectionMatrix()
	{
		return glm::mat4(1.f);
	}
public:
	//方向
	glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
	//切光角(内圆锥),弧度制
	GLfloat innerCutOff;
	//切光角(外圆锥),弧度制
	GLfloat outterCutOff;
};

