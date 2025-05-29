#pragma once
#include "light_base.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//////////////////////////////////////////////////////////////////////////////
//点光源类
F_REPLICABLE class FLightDot :public FLight
{
public:
	FLightDot()
	{
		_slot = _nextSlot[0]++;
	}
public:
	//将灯光的数据复制到着色器的第_slot个点光源的数据中
	virtual void setUniformData(GLuint _hProgram);

	//设置阴影贴图着色器Uniform变量
	void setShadowUniformData(GLuint _hProgram);

	//生成灯光空间变换矩阵
	virtual glm::mat4 _genLightMatrix(GLuint _faceIndex)
	{
		using namespace glm;
		vec3 g[] = { vec3(1.f,0.f,0.f), vec3(-1.f,0.f,0.f) ,vec3(0.f,1.f,0.f) , vec3(0.f,-1.f,0.f) , vec3(0.f,0.f,1.f) , vec3(0.f,0.f,-1.f) };
		vec3 t[] = { vec3(0.f,-1.f,0.f),vec3(0.f,-1.f,0.f),vec3(0.f,0.f,1.f), vec3(0.f,0.f,-1.f),vec3(0.f,-1.f,0.f),vec3(0.f,-1.f,0.f) };
		return lookAt(pos, pos + g[_faceIndex], t[_faceIndex]);
	}
	//生成投影变换矩阵
	virtual glm::mat4 _genProjectionMatrix()
	{
		using namespace glm;
		return perspective(radians(90.f), 1.0f, _near, _far);
	}
private:
	const float _far = 35.f, _near = 1.f;
};