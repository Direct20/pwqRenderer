#include "image/image.h"
#include <string>
#include "stb_image.h"
#include "misc/err.hpp"
using namespace std;

void FImage::load(const string& _path, bool _flipY)
{
	stbi_set_flip_vertically_on_load(_flipY);
	pdata = stbi_load(_path.c_str(), &wt, &ht, &chs, 0);
	if (!pdata)
	{
		throw ex("Failed to load image \"" + _path + "\".");
		stbi_image_free(pdata);
	}
	path = _path;
}

void FImage::load(void* _pdata, GLsizeiptr _size, const string& _path, bool _flipY)
{
	stbi_set_flip_vertically_on_load(_flipY);
	pdata = stbi_load_from_memory((stbi_uc*)_pdata, _size, &wt, &ht, &chs, 0);
	if (!pdata)
	{
		char buf[100];
		sprintf_s(buf, "Failed to load image from memory address %X.", (GLuint)_pdata);
		throw ex(buf);
		stbi_image_free(pdata);
	}
	path = _path;
}

void FImage::free()
{
	stbi_image_free(pdata);
	chs = 0;
	wt = ht = 0;
	pdata = nullptr;
	path = "";
}

GLint FImage::format(GLint _chs)
{
	GLint formats[] = { GL_RED,GL_RG,GL_RGB,GL_RGBA };
	return formats[_chs - 1];
}

