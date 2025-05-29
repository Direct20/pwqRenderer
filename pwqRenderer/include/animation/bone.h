#pragma once
#include "misc/def0.h"
#include "base/base.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
//////////////////////////////////////////////////////////////////////////////
//������
F_REPLICABLE class FBone
{
public:
	FBone() {}
	~FBone() {}
public:
	//ID(��д)
	GLint id = 0;
	//����(��д)
	std::string name = "";
	//��ģ�Ϳռ䵽�����ռ�ı任����(��д)
	glm::mat4 matOffset;
	//���ձ任����
	glm::mat4 matFinal;
};