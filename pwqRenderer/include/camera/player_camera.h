#pragma once
#include "base/base.h"
#include "camera/camera.h"
#include <glm/glm.hpp>
//玩家摄像机类
F_REPLICABLE class FPlayerCamera :public FCamera
{
public:
	//匍匐
	void creep()
	{

	}
	//下蹲
	void crouch()
	{

	}
	//跳跃
	void jump()
	{
	}
	//恢复正常
	void normal()
	{

	}
	//返回玩家足底位置
	glm::vec3 getPosition()
	{
		return glm::vec3(e.x, e.y - height, e.z);
	}
public:
	//摄像机离地面高度
	GLfloat height = 3.f;
};