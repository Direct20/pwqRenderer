#pragma once
#include "base/base.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
//////////////////////////////////////////////////////////////////////////////
//�������
F_REPLICABLE class FCamera
{
public:
	FCamera() {}
	virtual ~FCamera() {}
public:
	//��ת(�Ƕ���)
	//_dx:��t������ת�Ƕ�,����Ϊ��;
	//_dy:��u������ת�Ƕ�,���¿�Ϊ��;
	//_dz:��g������ת�Ƕ�,������Ϊ��
	virtual void rotateBy(GLfloat _dx, GLfloat _dy, GLfloat _dz = 0.f);

	//ƽ��
	virtual void translateBy(glm::vec3 _d);

	//��ȡ������
	glm::vec3 getRight();

	//����Uniform����
	virtual void setUniformData(GLuint _hProgram);

public:
	//������ͼ�任����
	glm::mat4 _genViewMatrix();

	//����ͶӰ�任����
	glm::mat4 _genProjectionMatrix(GLuint _cx, GLuint _cy);

public:
	//λ������
	glm::vec3 e = glm::vec3(0.f, 0.f, 0.f);
	//�۲췽������
	glm::vec3 g = glm::vec3(0.f, 0.f, -1.f);
	//������
	glm::vec3 t = glm::vec3(0.f, 1.f, 0.f);
	//��תԼ����Сֵ(x,y,z),0Ϊ��Լ��
	glm::vec3 constraintRmin = glm::vec3(0., -89., -45.);
	//��תԼ�����ֵ(x,y,z),0Ϊ��Լ��
	glm::vec3 constraintRmax = glm::vec3(0., 89., 45.);
};