#pragma once
#include "base/base.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
//////////////////////////////////////////////////////////////////////////////
//纹理基类
class FTexture :public FBase
{
	friend class FFramebuffer;
public:
	//纹理环绕类型
	enum WrapType
	{
		wtRepeat = GL_REPEAT,
		wtMirror = GL_MIRRORED_REPEAT,
		wtClampToEdge = GL_CLAMP_TO_EDGE,
		wtClampToBorder = GL_CLAMP_TO_BORDER
	};
	//纹理过滤类型
	enum FilterType
	{
		ftNearest = GL_NEAREST,
		ftLinear = GL_LINEAR,
		ftNearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		ftNearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		ftLinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		ftLinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
	};
protected:
	FTexture() {}
	virtual ~FTexture() {}
public:
	//设置纹理平铺类型
	void setWrap(WrapType _typeS = wtRepeat, WrapType _typeT = wtRepeat, WrapType _typeR = wtRepeat);

	//设置纹理过滤类型
	void setFilter(FilterType _min = ftNearest, FilterType _mag = ftLinear);

	//设置纹理边框颜色
	void setBorderColor(glm::vec4 _color);

	//绑定到指定槽,外部纹理的_id从F_TEXTURE_START+0开始
	void _bind(GLenum id);

	//取消指定槽的绑定,外部纹理的_id从F_TEXTURE_START+0开始
	void _unbind(GLenum id);

public:
	FTexture(FTexture& _x) = delete;
	void operator=(FTexture& _x) = delete;
public:
	//纹理句柄
	GLuint h = 0;
protected:
	//纹理本质
	GLenum _baseType = 0;
};
