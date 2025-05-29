#include "buffer/framebuffer.h"
#include "misc/err.hpp"
using namespace glm;

void FFramebuffer::_init()
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been initialized.");
	if (!h)glGenFramebuffers(1, &h);
	else war_excep(0, "The framebuffer object had been inited.");
}

void FFramebuffer::_destroy()
{
	if (h)glDeleteFramebuffers(1, &h);
	h = 0;
	rect = vec4(0.f);
}

void FFramebuffer::attach(FTexture& _tex, GLenum _attachment, bool _noColorBuffer)
{
	_bind();
	glFramebufferTexture(type, _attachment, _tex.h, 0);
	if (_noColorBuffer)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	_unbind();
}

void FFramebuffer::attach(FRenderbuffer& _rbo, GLenum _attachment, bool _noColorBuffer)
{
	_bind();
	glFramebufferRenderbuffer(type, _attachment, GL_RENDERBUFFER, _rbo.h);
	if (_noColorBuffer)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	_unbind();
}

void FFramebuffer::begin()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw ex(0, "The framebuffer is not complete.");
	assert(rect != ivec4(0));

	glViewport(rect.x, rect.y, rect.z, rect.w);
	_bind();
}

void FFramebuffer::end()
{
	_unbind();
}

void FFramebuffer::_bind()
{
	glBindFramebuffer(type, h);
}

void FFramebuffer::_unbind()
{
	glBindFramebuffer(type, 0);
}