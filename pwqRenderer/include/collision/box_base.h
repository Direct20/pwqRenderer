#pragma once
#include "object/object_base.h"
#include <glm/glm.hpp>
//////////////////////////////////////////////////////////////////////////////
//��Χ�л���
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
	////��ȡ��ģ�Ϳռ��λ��
	//glm::vec3& getP()
	//{
	//	return p;
	//}
	////��ȡ��ģ�Ϳռ����ת�Ƕ�
	//glm::vec3& getR()
	//{
	//	return r;
	//}
	////��ȡ��ģ�Ϳռ������
	//glm::vec3& getS()
	//{
	//	return s;
	//}
	////���ɱ任��ģ�Ϳռ�ľ���
	//glm::mat4 _genBoxMatrix()
	//{
	//	return _genModelMatrix();
	//}
protected:
	BoxType _boxType = btNone;
};

////@brief ��ײ���
////@return ��Χ���ཻ�̶�.0.fΪ����ײ.����ֵ��>0.f.
//GLfloat collision(FBoxAABB* _pb1, FBoxAABB* _pb2)
//{
//	if (!_pb1 || !_pb2)return 0.f;
//	return 0.f;
//}