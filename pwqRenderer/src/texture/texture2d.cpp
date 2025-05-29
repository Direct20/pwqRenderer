#include "texture/texture2d.h"
#include "misc/err.hpp"
#include "misc/global_manager.h"
using namespace std;
using namespace glm;
void FTexture2D::_init()
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been initialized.");
	if (!h)glGenTextures(1, &h);//生成纹理
	else war_excep(0, "The texture object has been initialized.");
}

void FTexture2D::_destroy()
{
	if (h)glDeleteTextures(1, &h);
	h = 0;
	path = "";
}
void FTexture2D::load(FImage* _img, TextureType _type)
{
	_bind(GL_TEXTURE0);

	GLint formats[] = { GL_RED,GL_RG,GL_RGB,GL_RGBA };//候选通道类型
	path = _img->path;
	type = _type;

	glTexImage2D(GL_TEXTURE_2D, 0, formats[_img->chs - 1],
		_img->wt, _img->ht, 0, formats[_img->chs - 1], GL_UNSIGNED_BYTE, _img->pdata);
	glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
	_unbind(GL_TEXTURE0);

	//设置默认纹理参数:
	setWrap();
	setFilter();
}

void FTexture2D::load(const void* _data, GLint _wt, GLuint _ht, const string& _path, TextureType _type)
{
	_bind(GL_TEXTURE0);

	path = _path;
	type = _type;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _wt, _ht, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	glGenerateMipmap(GL_TEXTURE_2D);//生成mipmap
	_unbind(GL_TEXTURE0);

	//设置默认纹理参数:
	setWrap();
	setFilter();
}

void FTexture2D::attachment(GLint _format, GLsizei _cx, GLsizei _cy)
{
	_bind(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, _format, _cx, _cy, 0, _format, GL_FLOAT, nullptr);

	_unbind(GL_TEXTURE0);

	setWrap(wtClampToBorder, wtClampToBorder);
	setFilter(ftLinear, ftLinear);
	setBorderColor(vec4(1.f, 1.f, 1.f, 1.f));

	type = ttNone;
}
