#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
//////////////////////////////////////////////////////////////////////////////
//������
class FParticle
{
public:
	FParticle() {}
	~FParticle() {}
	//����ʱ��
	void update()
	{

	}
public:
	GLfloat prevTime;
	//ʱ��(<=0ʱ��������)
	float t;
	//λ��
	glm::vec3 p;
	//�ٶ�
	glm::vec3 v;

	GLfloat opacity;
};