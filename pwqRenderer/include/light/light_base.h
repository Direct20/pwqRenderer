#pragma once
#include "base/base.h"
#include <glm/glm.hpp>
#include "texture/texture_base.h"
//////////////////////////////////////////////////////////////////////////////
//�ƹ����
F_REPLICABLE class FLight
{
public:
	//����Uniform��������
	virtual void setUniformData(GLuint _hProgram) = 0;
public:
	//�Ƿ����õƹ�(��д)
	bool enabled = true;

	//λ��(��д)
	glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
	//��ɫ(��д)
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	//ǿ��(a,d,s)(��д)
	glm::vec3 intensity = glm::vec3(.2f, 1.f, 1.f);

	//��˥�� ˥��ֵF=1.0/(Kc+Kl*d+Kq*d^2)
	//������(constant)(��д)
	GLfloat Kc = 1.f;
	//һ����(linear)(��д)
	GLfloat Kl = 0.f;
	//������(quadratic)(��д)
	GLfloat Kq = 0.f;

	//�Ƿ�������Ӱ(��д)
	bool shadow = false;
	//��Ӱ��ͼ
	FTexture* ptexShadow = nullptr;
public:
	//��ɫ���еĲ��ID,Ĭ���Զ�����(��д)
	GLuint _slot = 0;
protected:
	static GLuint _nextSlot[3];
};
