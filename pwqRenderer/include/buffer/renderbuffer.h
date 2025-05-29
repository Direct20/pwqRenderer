#pragma once
#include "base/base.h"
#include <glad/glad.h>
#include "misc/err.hpp"
#include "misc/global_manager.h"

//////////////////////////////////////////////////////////////////////////////
//��Ⱦ��������
class FRenderbuffer :public FBase
{
public:
	//��δ��ʼ��ʱ�˹��캯����ִ�г�ʼ������, ���ֶ�����_init.
	FRenderbuffer()
	{
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
	}
	//@brief ��δ��ʼ��ʱ����,��ʼ��������Ϊ֡���帽��.
	//@param _internalFormat *GL_DEPTH24_STENCIL8
	FRenderbuffer(GLsizei _cx, GLsizei _cy, GLenum _internalFormat = GL_DEPTH24_STENCIL8)
	{
		_init();
		attachment(_cx, _cy, _internalFormat);
	}
	//!_autoreleaseʱ������_destroy
	virtual ~FRenderbuffer()
	{
		if (_autorelease)_destroy();
	}
public:
	void _init()
	{
		auto& gm = FGlobalManager::get_instance();
		if (!gm.g_isInited)throw ex("OpenGL hasn't been initialized.");
		if (!h)glGenRenderbuffers(1, &h);
		else war_excep(0, "The renderbuffer object had been inited.");
	}
	void _destroy()
	{
		if (h)glDeleteRenderbuffers(1, &h);
		h = 0;
	}
	//@brief ����Ϊ֡���帽��
	//@param _internalFormat *GL_DEPTH24_STENCIL8
	void attachment(GLsizei _cx, GLsizei _cy, GLenum _internalFormat = GL_DEPTH24_STENCIL8)
	{
		_bind();
		glRenderbufferStorage(GL_RENDERBUFFER, _internalFormat, _cx, _cy);
		_unbind();
	}
	//�󶨵�OpenGL������
	void _bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, h);
	}
	//ȡ���󶨵�OpenGL������
	void _unbind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
public:
	FRenderbuffer(FRenderbuffer& _x) = delete;
	void operator=(FRenderbuffer& _x) = delete;
public:
	//��Ⱦ���������(����ֻ��)
	GLuint h = 0;
};
