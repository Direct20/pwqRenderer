#pragma once
#include "base/base.h"
#include "misc/err.hpp"
#include <glad/glad.h>
#include <string>
//////////////////////////////////////////////////////////////////////////////
//��ɫ����
class FShader :public FBase
{
public:
	//��ɫ������
	enum ShaderType
	{
		stVertex = GL_VERTEX_SHADER,
		stFragment = GL_FRAGMENT_SHADER,
		stGeometry = GL_GEOMETRY_SHADER
	};
public:
	//�˹��캯����ִ�г�ʼ������,���ֶ�����init.
	FShader()
	{
	}
	//��ʼ����ɫ��,��δ��ʼ��ʱ����.
	FShader(ShaderType _type, const char* const _src)
	{
		_init(_type, _src);
	}
	//!_autoreleaseʱ������destroy
	virtual ~FShader()
	{
		if (_autorelease)_destroy();
	}
	//��ʼ����ɫ��
	void _init(ShaderType _type, const char* const _src);

	//������ɫ��
	void _destroy();

public:
	//����ɫ�����ӵ���ɫ������
	void _attach(GLuint _program)const;

public:
	//������ɫ��Դ��
	static GLchar* _load(const std::string& _path);

public:
	FShader(FShader& _x) = delete;
	void operator=(FShader& _x) = delete;
public:
	//���(����ֻ��)
	GLuint h = 0;
};