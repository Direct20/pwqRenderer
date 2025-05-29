#include "texture/texture_base.h"

void FTexture::setWrap(WrapType _typeS, WrapType _typeT, WrapType _typeR)
{
	assert(_baseType != 0);

	_bind(GL_TEXTURE0);//绑定到插槽0
	glTexParameteri(_baseType, GL_TEXTURE_WRAP_S, _typeS);
	glTexParameteri(_baseType, GL_TEXTURE_WRAP_T, _typeT);
	glTexParameteri(_baseType, GL_TEXTURE_WRAP_R, _typeR);
	_unbind(GL_TEXTURE0);
}

void FTexture::setFilter(FilterType _min, FilterType _mag)
{
	assert(_baseType != 0);

	_bind(GL_TEXTURE0);//绑定到插槽0
	glTexParameteri(_baseType, GL_TEXTURE_MIN_FILTER, _min);
	glTexParameteri(_baseType, GL_TEXTURE_MAG_FILTER, _mag);
	_unbind(GL_TEXTURE0);
}

void FTexture::setBorderColor(glm::vec4 _color)
{
	assert(_baseType != 0);

	_bind(GL_TEXTURE0);//绑定到插槽0
	glTexParameterfv(_baseType, GL_TEXTURE_BORDER_COLOR, &_color[0]);
	_unbind(GL_TEXTURE0);
}

void FTexture::_bind(GLenum id)
{
	assert(_baseType != 0);

	glActiveTexture(id);
	glBindTexture(_baseType, h);
}

void FTexture::_unbind(GLenum id)
{
	assert(_baseType != 0);

	glActiveTexture(id);
	glBindTexture(_baseType, 0);//从插槽取出
}
