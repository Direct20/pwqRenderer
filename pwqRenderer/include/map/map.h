//////////////////////////////////////////////////////////////////////////////
//map.h - ��ͼ
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "object/object.h"
#include "image/height_image.h"
#include <glm/glm.hpp>
#include "misc/err.hpp"
#include <glm/detail/type_vec.hpp>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//��ͼ��
class FMap :protected FObject
{
public:
	FMap() :FObject()
	{

		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_initThis();
	}
	FMap(FHeightImage* _img, glm::vec2 _rgnX, glm::vec2 _rgnY, glm::vec2 _rgnZ, glm::vec2 _precision) :FObject()
	{
		auto& gm = FGlobalManager::get_instance();
		if (!gm.g_isInited)throw ex(0, F_EXC_DESCRIP_OPENGL_NOT_INITED);
		_initThis();
		load(_img, _rgnX, _rgnY, _rgnZ, _precision);
	}
	virtual ~FMap()
	{
		if (_autorelease)_destroyThis();
	}
	//��ʼ��
	void _init()
	{
		__super::_init();
		_initThis();
	}
	//����
	void _destroy()
	{
		_destroyThis();
		__super::_destroy();
	}
private:
	void _initThis();

	void _destroyThis();

public:
	//@brief ���ظ߶�ͼ�����ɵ�ͼ
	//@param _img �߶�ͼ
	//@param _rgnX X���귶Χ,[minX,maxX]
	//@param _rgnY Y���귶Χ,[minY,maxY]
	//@param _rgnZ Z���귶Χ,[minZ,maxZ]
	//@param _precision �������ɾ���(XZƽ�������ڶ�����)
	void load(FHeightImage* _img, glm::vec2 _rgnX, glm::vec2 _rgnY, glm::vec2 _rgnZ, glm::vec2 _precision);
	
	//����
	void draw(FShaderProgram& _sp)
	{
		__super::draw(_sp, dtIndices, GL_TRIANGLES);
	}
	//@brief ��ײ���
	//@param _p ����ռ�����
	//@return 0.f��ʾ��������;<0.f��ʾ�ڵ�������,ֵ��ʾ����;>0.f�����ڵ�������,ֵ��ʾ����.
	GLfloat collision(glm::vec3 _p);

	//���ò���
	void setMaterial(FMaterial& _mtl)
	{
		__super::setMaterial(_mtl);
	}
	//��ȡ��ͼλ��
	glm::vec3& getP()
	{
		return p;
	}
	//@brief ��ȡ���θ߶�Yֵ
	//@param _xz ģ�Ϳռ�XZƽ������
	//@return �߶�ֵ��Ӧ��ģ�Ϳռ�Y����,������Χ����rgnYmin
	GLfloat _getHeight(glm::vec2 _xz);

public:
	//@brief ��ȡN��_cy�о����е�_x��_y�и�Ԫ�ص�����,��0��ʼ,���ʱ�ü�����.
	//@param _cy ����
	//@param _x ��
	//@param _y ��
	static GLuint _index(GLuint _cx, GLuint _cy, GLint _x, GLint _y);

	//���㶥��_this�ķ���
	static glm::vec3 _normal(glm::vec3 _this, glm::vec3 _l, glm::vec3 _t, glm::vec3 _r, glm::vec3 _b);

	//���㶥��_this������
	static glm::vec3 _tangent(glm::vec3 _this, glm::vec3 _pt1, glm::vec3 _pt2, glm::vec2 _uvThis, glm::vec2 _uv1, glm::vec2 _uv2);
	
public:
	//�߶�ͼ(ֻ��)
	FHeightImage* img = nullptr;
	//���귶Χ(ֻ��)
	glm::vec2 rgnX, rgnY, rgnZ;
	//XYZ�ߴ�(ֻ��)
	glm::vec3 size = glm::vec3(2.f, 2.f, 2.f);
	//XZ��������ƽ�̴���
	glm::vec2 texTile = glm::vec2(4.f, 4.f);
	//��ײ�Ƚ���Χ
	GLfloat range = 0.05f;
};
