#pragma once
#include "base/base.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
//////////////////////////////////////////////////////////////////////////////
//摄像机类
F_REPLICABLE class FCamera
{
public:
	FCamera() {}
	virtual ~FCamera() {}
public:
	//旋转(角度制)
	//_dx:绕t向量旋转角度,向左看为正;
	//_dy:绕u向量旋转角度,向下看为正;
	//_dz:绕g向量旋转角度,向左倾为正
	virtual void rotateBy(GLfloat _dx, GLfloat _dy, GLfloat _dz = 0.f);

	//平移
	virtual void translateBy(glm::vec3 _d);

	//获取右向量
	glm::vec3 getRight();

	//设置Uniform数据
	virtual void setUniformData(GLuint _hProgram);

public:
	//生成视图变换矩阵
	glm::mat4 _genViewMatrix();

	//生成投影变换矩阵
	glm::mat4 _genProjectionMatrix(GLuint _cx, GLuint _cy);

public:
	//位置向量
	glm::vec3 e = glm::vec3(0.f, 0.f, 0.f);
	//观察方向向量
	glm::vec3 g = glm::vec3(0.f, 0.f, -1.f);
	//上向量
	glm::vec3 t = glm::vec3(0.f, 1.f, 0.f);
	//旋转约束最小值(x,y,z),0为无约束
	glm::vec3 constraintRmin = glm::vec3(0., -89., -45.);
	//旋转约束最大值(x,y,z),0为无约束
	glm::vec3 constraintRmax = glm::vec3(0., 89., 45.);
};