#pragma once
#include "base/base.h"
#include <glm/glm.hpp>
#include "texture/texture_base.h"
//////////////////////////////////////////////////////////////////////////////
//灯光基类
F_REPLICABLE class FLight
{
public:
	//设置Uniform变量数据
	virtual void setUniformData(GLuint _hProgram) = 0;
public:
	//是否启用灯光(读写)
	bool enabled = true;

	//位置(读写)
	glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
	//颜色(读写)
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	//强度(a,d,s)(读写)
	glm::vec3 intensity = glm::vec3(.2f, 1.f, 1.f);

	//光衰减 衰减值F=1.0/(Kc+Kl*d+Kq*d^2)
	//常数项(constant)(读写)
	GLfloat Kc = 1.f;
	//一次项(linear)(读写)
	GLfloat Kl = 0.f;
	//二次项(quadratic)(读写)
	GLfloat Kq = 0.f;

	//是否启用阴影(读写)
	bool shadow = false;
	//阴影贴图
	FTexture* ptexShadow = nullptr;
public:
	//着色器中的插槽ID,默认自动分配(读写)
	GLuint _slot = 0;
protected:
	static GLuint _nextSlot[3];
};
