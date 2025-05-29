#pragma once
#include "base/base.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "collision/box_base.h"
//////////////////////////////////////////////////////////////////////////////
//基本对象类
F_REPLICABLE class FBaseObject : public FBase
{
public:
	FBaseObject() {}
	FBaseObject(glm::vec3 _p, glm::vec3 _r, glm::vec3 _s)
	{
		p = _p;
		r = _r;
		s = _s;
	}
	virtual ~FBaseObject()
	{
	}
public:
	//根据位置生成模型变换矩阵
	glm::mat4 _genModelMatrix()
	{
		using namespace glm;
		mat4 model(1.f);
		model = translate(model, p);//位置
		model = rotate(model, r.x, vec3(1.f, 0.f, 0.f));//旋转x
		model = rotate(model, r.y, vec3(0.f, 1.f, 0.f));//旋转y
		model = rotate(model, r.z, vec3(0.f, 0.f, 1.f));//旋转z
		model = scale(model, s);//缩放
		return model;
	}
public:
	//位置(读写)
	glm::vec3 p = glm::vec3(0.f, 0.f, 0.f);
	//旋转角度(弧度制)(读写)
	glm::vec3 r = glm::vec3(0.f, 0.f, 0.f);
	//缩放比例(读写)
	glm::vec3 s = glm::vec3(1.f, 1.f, 1.f);
	//包围盒(读写)
	//FBoxBase* pBox = nullptr;
};
