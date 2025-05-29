#include "texture/texture_cube.h"
#include "misc/err.hpp"
#include "misc/global_manager.h"
using namespace std;
void FTextureCube::_init()
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been initialized.");
	if (!h)glGenTextures(1, &h);
	else war_excep(0, "The cube texture object had been inited.");
}

void FTextureCube::_destroy()
{
	if (h)glDeleteTextures(1, &h);
	h = 0;
}

void FTextureCube::load(const char** _paths, bool _flip)
{
	FImage imgs[6];
	for (GLuint i = 0; i < _cntFaces; i++)
		imgs[i].load(_paths[i], _flip);
	load(imgs);
}

void FTextureCube::load(const string* _paths, bool _flip)
{
	FImage imgs[6];
	for (GLuint i = 0; i < _cntFaces; i++)
		imgs[i].load(_paths[i], _flip);
	load(imgs);
}

void FTextureCube::load(FImage* _imgs)
{
	_bind(GL_TEXTURE0);
	for (GLuint i = 0; i < _cntFaces; i++)
	{
		if (_imgs[i].wt != _imgs[i].ht)
			throw ex(0, "The cube map's height is not equal to weight.");

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, FImage::format(_imgs[i].chs), _imgs[i].wt, _imgs[i].ht, 0, FImage::format(_imgs[i].chs), GL_UNSIGNED_BYTE, _imgs[i].pdata);
		//glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);//Éú³Émipmap
	}
	_unbind(GL_TEXTURE0);

	setWrap(wtClampToEdge, wtClampToEdge, wtClampToEdge);
	setFilter(ftLinear, ftLinear);
}

void FTextureCube::attachment(GLint _format, GLsizei _wt)
{
	_bind(GL_TEXTURE0);
	for (GLuint i = 0; i < _cntFaces; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _format, _wt, _wt, 0, _format, GL_FLOAT, nullptr);
	_unbind(GL_TEXTURE0);

	setWrap(wtClampToEdge, wtClampToEdge, wtClampToEdge);
	setFilter(ftLinear, ftLinear);
}
