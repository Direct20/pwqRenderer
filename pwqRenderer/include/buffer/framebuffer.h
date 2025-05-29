#pragma once
#include "base/base.h"
#include <glad/glad.h>
#include "texture/texture2d.h"
#include "buffer/renderbuffer.h"
#include <glm/glm.hpp>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//֡������(FBO)
class FFramebuffer :public FBase
{
public:
	//��δ��ʼ��ʱ�˹��캯����ִ�г�ʼ������,���ֶ�����init.
	FFramebuffer(GLenum _type = GL_FRAMEBUFFER)
	{
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
		type = _type;
	}
	//@brief ��δ��ʼ��ʱ����.��ʼ��������������
	//@param _tex ������
	//@param _attachment *GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT,GL_COLOR_ATTACHMENTi...
	//@param _noColorBuffer ������ɫ������
	//@param _type ֡�������� *GL_FRAMEBUFFER
	FFramebuffer(FTexture2D& _tex, GLenum _attchment = GL_DEPTH_ATTACHMENT,
		bool _noColorBuffer = false, GLenum _type = GL_FRAMEBUFFER)
	{
		_init();
		attach(_tex, _attchment, _noColorBuffer);
		type = _type;
	}
	//@brief ��δ��ʼ��ʱ����.��ʼ����������Ⱦ����������
	//@param _rbo ��Ⱦ����������
	//@param _attachment *GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT,GL_COLOR_ATTACHMENTi...
	//@param _noColorBuffer ������ɫ������
	//@param _type ֡�������� *GL_FRAMEBUFFER
	FFramebuffer(FRenderbuffer& _rbo, GLenum _attchment = GL_DEPTH_ATTACHMENT,
		bool _noColorBuffer = false, GLenum _type = GL_FRAMEBUFFER)
	{
		_init();
		attach(_rbo, _attchment, _noColorBuffer);
		type = _type;
	}
	//!_autoreleaseʱ������_destroy
	virtual ~FFramebuffer()
	{
		if (_autorelease)_destroy();
	}
public:
	void _init();

	void _destroy();

public:
	//@brief ����������
	//@param _tex ������
	//@param _attachment *GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT,GL_COLOR_ATTACHMENTi...
	//@param _noColorBuffer ������ɫ������
	void attach(FTexture& _tex, GLenum _attachment = GL_DEPTH_ATTACHMENT, bool _noColorBuffer = false);

	//@brief ������Ⱦ����������
	//@param _rbo ��Ⱦ����������
	//@param _attachment *GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT,GL_COLOR_ATTACHMENTi...
	//@param _noColorBuffer ������ɫ������
	void attach(FRenderbuffer& _rbo, GLenum _attachment = GL_DEPTH_ATTACHMENT, bool _noColorBuffer = false);

	//��ʼ��
	void begin();

	//������
	void end();

	//�󶨵�OpenGL������
	void _bind();

	//��OpenGL������ȡ����
	void _unbind();

public:
	FFramebuffer(FFramebuffer& _x) = delete;
	void operator=(FFramebuffer& _x) = delete;
public:
	//֡������(����ֻ��)
	GLuint h = 0;
	//֡��������(��д)
	GLenum type = GL_FRAMEBUFFER;
	//֡�����������(l,t,w,h)(��д)
	glm::ivec4 rect = glm::ivec4(0);
};
