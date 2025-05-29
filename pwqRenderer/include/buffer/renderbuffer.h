#pragma once
#include "base/base.h"
#include <glad/glad.h>
#include "misc/err.hpp"
#include "misc/global_manager.h"

//////////////////////////////////////////////////////////////////////////////
//渲染缓冲区类
class FRenderbuffer :public FBase
{
public:
	//库未初始化时此构造函数不执行初始化操作, 须手动调用_init.
	FRenderbuffer()
	{
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
	}
	//@brief 库未初始化时出错,初始化并创建为帧缓冲附件.
	//@param _internalFormat *GL_DEPTH24_STENCIL8
	FRenderbuffer(GLsizei _cx, GLsizei _cy, GLenum _internalFormat = GL_DEPTH24_STENCIL8)
	{
		_init();
		attachment(_cx, _cy, _internalFormat);
	}
	//!_autorelease时不调用_destroy
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
	//@brief 创建为帧缓冲附件
	//@param _internalFormat *GL_DEPTH24_STENCIL8
	void attachment(GLsizei _cx, GLsizei _cy, GLenum _internalFormat = GL_DEPTH24_STENCIL8)
	{
		_bind();
		glRenderbufferStorage(GL_RENDERBUFFER, _internalFormat, _cx, _cy);
		_unbind();
	}
	//绑定到OpenGL上下文
	void _bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, h);
	}
	//取消绑定到OpenGL上下文
	void _unbind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
public:
	FRenderbuffer(FRenderbuffer& _x) = delete;
	void operator=(FRenderbuffer& _x) = delete;
public:
	//渲染缓冲区句柄(对外只读)
	GLuint h = 0;
};
