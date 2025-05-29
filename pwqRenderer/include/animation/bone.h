#pragma once
#include "misc/def0.h"
#include "base/base.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
//////////////////////////////////////////////////////////////////////////////
//骨骼类
F_REPLICABLE class FBone
{
public:
	FBone() {}
	~FBone() {}
public:
	//ID(读写)
	GLint id = 0;
	//名称(读写)
	std::string name = "";
	//从模型空间到骨骼空间的变换矩阵(读写)
	glm::mat4 matOffset;
	//最终变换矩阵
	glm::mat4 matFinal;
};