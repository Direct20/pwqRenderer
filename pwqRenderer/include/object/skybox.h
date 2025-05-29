#pragma once
#include "base/base.h"
#include "object.h"
#include <glad/glad.h>
#include "misc/def1.h"
#include "texture/texture_cube.h"
#include <glm/glm.hpp>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//��պ���
class FSkybox :protected FObject
{
public:
	//�����嶥������
	const GLfloat _cube_vertices[24] =
	{
		-1,1,-1,
		-1,-1,-1,
		-1,-1,1,
		-1,1,1,
		1,1,1,
		1,-1,1,
		1,-1,-1,
		1,1,-1
	};
	//��������������
	const GLuint _cube_indices[36] =
	{
		0,1,2,
		0,3,2,
		0,3,7,
		3,4,7,
		2,3,5,
		3,4,5,
		1,2,6,
		2,5,6,
		4,5,7,
		5,6,7,
		0,7,1,
		1,7,6
	};
public:
	//��δ��ʼ��ʱ�˹��캯����ִ�г�ʼ��,���ֶ�����_init.
	FSkybox()
		:FObject()
	{

		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_initThis();
	}
	//��δ��ʼ��ʱ����.��ʼ����ָ����������ͼ,����ͼ�󶨵�������.
	FSkybox(FTextureCube* _ptex, FShaderProgram& _sp)
		:FObject()
	{
		auto& gm = FGlobalManager::get_instance();
		if (!gm.g_isInited)throw ex(0, F_EXC_DESCRIP_OPENGL_NOT_INITED);
		_initThis();
		ptex = _ptex;
		setUniformData(_sp);
	}
	//!_autoreleaseʱ���ͷ�
	virtual ~FSkybox()
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
	//��ʼ������
	void _initThis()
	{
		addVertexAttr(aiPosition, 3, GL_FLOAT, sizeof(GLfloat));
		setVertices(_cube_vertices, sizeof(_cube_vertices));
		setIndices(_cube_indices, sizeof(_cube_indices));
	}
	//���ٴ���
	void _destroyThis()
	{
		ptex = nullptr;
	}
public:
	//����
	void draw(FShaderProgram& _sp)
	{
		glDepthMask(GL_FALSE);//������Ȼ�����д��
		glDepthFunc(GL_LEQUAL);//С�ڵ���ʱͨ����Ȳ���
		__super::draw(_sp, FObject::dtIndices, GL_TRIANGLES);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
	}
	//����Uniform��������
	void setUniformData(FShaderProgram& _sp)
	{
		if (!ptex)return;
		ptex->_bind(F_TEXTURE_SKYBOX);
		_sp._setInt(F_UNAME_SKYBOX_TEX, F_TEXTURE_SKYBOX_INDEX);
	}
	//��ȡr������(������)
	glm::vec3& getr()
	{
		return r;
	}
	//��ȡp������
	glm::vec3& getp()
	{
		return p;
	}
	//��ȡs������
	glm::vec3& gets()
	{
		return s;
	}
public:
	//��պ�����(��д)
	FTextureCube* ptex = nullptr;
};
