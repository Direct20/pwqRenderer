#pragma once
#include "object/object_base.h"
#include <glm/glm.hpp>
//////////////////////////////////////////////////////////////////////////////
//包围盒基类
class FBoxBase //:protected FBaseObject
{
protected:
	enum BoxType
	{
		btNone = 0,
		btAABB,
		btOBB,
		btSphere
	};
public:
	FBoxBase() {}
	virtual ~FBoxBase() {}
public:
	////获取在模型空间的位置
	//glm::vec3& getP()
	//{
	//	return p;
	//}
	////获取在模型空间的旋转角度
	//glm::vec3& getR()
	//{
	//	return r;
	//}
	////获取在模型空间的缩放
	//glm::vec3& getS()
	//{
	//	return s;
	//}
	////生成变换到模型空间的矩阵
	//glm::mat4 _genBoxMatrix()
	//{
	//	return _genModelMatrix();
	//}
protected:
	BoxType _boxType = btNone;
};

////@brief 碰撞检测
////@return 包围盒相交程度.0.f为无碰撞.返回值总>0.f.
//GLfloat collision(FBoxAABB* _pb1, FBoxAABB* _pb2)
//{
//	if (!_pb1 || !_pb2)return 0.f;
//	return 0.f;
//}