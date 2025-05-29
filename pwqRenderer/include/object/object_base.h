#pragma once
#include "base/base.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "collision/box_base.h"
//////////////////////////////////////////////////////////////////////////////
//����������
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
	//����λ������ģ�ͱ任����
	glm::mat4 _genModelMatrix()
	{
		using namespace glm;
		mat4 model(1.f);
		model = translate(model, p);//λ��
		model = rotate(model, r.x, vec3(1.f, 0.f, 0.f));//��תx
		model = rotate(model, r.y, vec3(0.f, 1.f, 0.f));//��תy
		model = rotate(model, r.z, vec3(0.f, 0.f, 1.f));//��תz
		model = scale(model, s);//����
		return model;
	}
public:
	//λ��(��д)
	glm::vec3 p = glm::vec3(0.f, 0.f, 0.f);
	//��ת�Ƕ�(������)(��д)
	glm::vec3 r = glm::vec3(0.f, 0.f, 0.f);
	//���ű���(��д)
	glm::vec3 s = glm::vec3(1.f, 1.f, 1.f);
	//��Χ��(��д)
	//FBoxBase* pBox = nullptr;
};
