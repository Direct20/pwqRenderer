#pragma once
#include "light_base.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//////////////////////////////////////////////////////////////////////////////
//���Դ��
F_REPLICABLE class FLightDot :public FLight
{
public:
	FLightDot()
	{
		_slot = _nextSlot[0]++;
	}
public:
	//���ƹ�����ݸ��Ƶ���ɫ���ĵ�_slot�����Դ��������
	virtual void setUniformData(GLuint _hProgram);

	//������Ӱ��ͼ��ɫ��Uniform����
	void setShadowUniformData(GLuint _hProgram);

	//���ɵƹ�ռ�任����
	virtual glm::mat4 _genLightMatrix(GLuint _faceIndex)
	{
		using namespace glm;
		vec3 g[] = { vec3(1.f,0.f,0.f), vec3(-1.f,0.f,0.f) ,vec3(0.f,1.f,0.f) , vec3(0.f,-1.f,0.f) , vec3(0.f,0.f,1.f) , vec3(0.f,0.f,-1.f) };
		vec3 t[] = { vec3(0.f,-1.f,0.f),vec3(0.f,-1.f,0.f),vec3(0.f,0.f,1.f), vec3(0.f,0.f,-1.f),vec3(0.f,-1.f,0.f),vec3(0.f,-1.f,0.f) };
		return lookAt(pos, pos + g[_faceIndex], t[_faceIndex]);
	}
	//����ͶӰ�任����
	virtual glm::mat4 _genProjectionMatrix()
	{
		using namespace glm;
		return perspective(radians(90.f), 1.0f, _near, _far);
	}
private:
	const float _far = 35.f, _near = 1.f;
};