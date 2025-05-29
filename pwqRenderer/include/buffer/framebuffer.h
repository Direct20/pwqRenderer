#pragma once
#include "base/base.h"
#include <glad/glad.h>
#include "texture/texture2d.h"
#include "buffer/renderbuffer.h"
#include <glm/glm.hpp>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//帧缓冲类(FBO)
class FFramebuffer :public FBase
{
public:
	//库未初始化时此构造函数不执行初始化操作,须手动调用init.
	FFramebuffer(GLenum _type = GL_FRAMEBUFFER)
	{
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
		type = _type;
	}
	//@brief 库未初始化时出错.初始化并附加纹理附件
	//@param _tex 纹理附件
	//@param _attachment *GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT,GL_COLOR_ATTACHMENTi...
	//@param _noColorBuffer 禁用颜色缓冲区
	//@param _type 帧缓冲类型 *GL_FRAMEBUFFER
	FFramebuffer(FTexture2D& _tex, GLenum _attchment = GL_DEPTH_ATTACHMENT,
		bool _noColorBuffer = false, GLenum _type = GL_FRAMEBUFFER)
	{
		_init();
		attach(_tex, _attchment, _noColorBuffer);
		type = _type;
	}
	//@brief 库未初始化时出错.初始化并附加渲染缓冲区附件
	//@param _rbo 渲染缓冲区附件
	//@param _attachment *GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT,GL_COLOR_ATTACHMENTi...
	//@param _noColorBuffer 禁用颜色缓冲区
	//@param _type 帧缓冲类型 *GL_FRAMEBUFFER
	FFramebuffer(FRenderbuffer& _rbo, GLenum _attchment = GL_DEPTH_ATTACHMENT,
		bool _noColorBuffer = false, GLenum _type = GL_FRAMEBUFFER)
	{
		_init();
		attach(_rbo, _attchment, _noColorBuffer);
		type = _type;
	}
	//!_autorelease时不调用_destroy
	virtual ~FFramebuffer()
	{
		if (_autorelease)_destroy();
	}
public:
	void _init();

	void _destroy();

public:
	//@brief 附加纹理附件
	//@param _tex 纹理附件
	//@param _attachment *GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT,GL_COLOR_ATTACHMENTi...
	//@param _noColorBuffer 禁用颜色缓冲区
	void attach(FTexture& _tex, GLenum _attachment = GL_DEPTH_ATTACHMENT, bool _noColorBuffer = false);

	//@brief 附加渲染缓冲区附件
	//@param _rbo 渲染缓冲区附件
	//@param _attachment *GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT,GL_COLOR_ATTACHMENTi...
	//@param _noColorBuffer 禁用颜色缓冲区
	void attach(FRenderbuffer& _rbo, GLenum _attachment = GL_DEPTH_ATTACHMENT, bool _noColorBuffer = false);

	//开始画
	void begin();

	//结束画
	void end();

	//绑定到OpenGL上下文
	void _bind();

	//从OpenGL上下文取消绑定
	void _unbind();

public:
	FFramebuffer(FFramebuffer& _x) = delete;
	void operator=(FFramebuffer& _x) = delete;
public:
	//帧缓冲句柄(对外只读)
	GLuint h = 0;
	//帧缓冲类型(读写)
	GLenum type = GL_FRAMEBUFFER;
	//帧缓冲绘制区域(l,t,w,h)(读写)
	glm::ivec4 rect = glm::ivec4(0);
};
