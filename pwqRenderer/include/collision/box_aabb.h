#pragma once
#include "collision/box_base.h"
#include <glm/glm.hpp>
//////////////////////////////////////////////////////////////////////////////
//AABB��Χ����
class FBoxAABB :public FBoxBase
{
public:
	FBoxAABB()
	{
		_boxType = btAABB;
		rgnX = rgnY = rgnZ = glm::vec2(0.f, 0.f);
	}
	FBoxAABB(glm::vec2 _rgnX, glm::vec2 _rgnY, glm::vec2 _rgnZ)
	{
		_boxType = btAABB;
		rgnX = _rgnX, rgnY = _rgnY, rgnZ = _rgnZ;
	}
	virtual ~FBoxAABB() {}
public:
	//@brief �������귶Χ���ɶ���
	//@return ����8��Ԫ�ص�һά����
	glm::vec3* _genVertices()
	{
		using namespace glm;
		static vec3 ver[8];
		ver[0] = vec3(rgnX.x, rgnY.y, rgnZ.x);
		ver[1] = vec3(rgnX.y, rgnY.y, rgnZ.x);
		ver[2] = vec3(rgnX.y, rgnY.x, rgnZ.x);
		ver[3] = vec3(rgnX.x, rgnY.x, rgnZ.x);
		ver[4] = vec3(rgnX.x, rgnY.y, rgnZ.y);
		ver[5] = vec3(rgnX.y, rgnY.y, rgnZ.y);
		ver[6] = vec3(rgnX.y, rgnY.x, rgnZ.y);
		ver[7] = vec3(rgnX.x, rgnY.x, rgnZ.y);
		return ver;
	}
public:
	//���귶ΧXYZ[min,max]
	glm::vec2 rgnX, rgnY, rgnZ;
};
