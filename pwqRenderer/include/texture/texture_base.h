#pragma once
#include "base/base.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
//////////////////////////////////////////////////////////////////////////////
//�������
class FTexture :public FBase
{
	friend class FFramebuffer;
public:
	//����������
	enum WrapType
	{
		wtRepeat = GL_REPEAT,
		wtMirror = GL_MIRRORED_REPEAT,
		wtClampToEdge = GL_CLAMP_TO_EDGE,
		wtClampToBorder = GL_CLAMP_TO_BORDER
	};
	//�����������
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
	//��������ƽ������
	void setWrap(WrapType _typeS = wtRepeat, WrapType _typeT = wtRepeat, WrapType _typeR = wtRepeat);

	//���������������
	void setFilter(FilterType _min = ftNearest, FilterType _mag = ftLinear);

	//��������߿���ɫ
	void setBorderColor(glm::vec4 _color);

	//�󶨵�ָ����,�ⲿ�����_id��F_TEXTURE_START+0��ʼ
	void _bind(GLenum id);

	//ȡ��ָ���۵İ�,�ⲿ�����_id��F_TEXTURE_START+0��ʼ
	void _unbind(GLenum id);

public:
	FTexture(FTexture& _x) = delete;
	void operator=(FTexture& _x) = delete;
public:
	//������
	GLuint h = 0;
protected:
	//������
	GLenum _baseType = 0;
};
