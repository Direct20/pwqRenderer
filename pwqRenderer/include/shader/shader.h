#pragma once
#include "base/base.h"
#include "misc/err.hpp"
#include <glad/glad.h>
#include <string>
//////////////////////////////////////////////////////////////////////////////
//着色器类
class FShader :public FBase
{
public:
	//着色器类型
	enum ShaderType
	{
		stVertex = GL_VERTEX_SHADER,
		stFragment = GL_FRAGMENT_SHADER,
		stGeometry = GL_GEOMETRY_SHADER
	};
public:
	//此构造函数不执行初始化操作,须手动调用init.
	FShader()
	{
	}
	//初始化着色器,库未初始化时出错.
	FShader(ShaderType _type, const char* const _src)
	{
		_init(_type, _src);
	}
	//!_autorelease时不调用destroy
	virtual ~FShader()
	{
		if (_autorelease)_destroy();
	}
	//初始化着色器
	void _init(ShaderType _type, const char* const _src);

	//销毁着色器
	void _destroy();

public:
	//将着色器附加到着色器程序
	void _attach(GLuint _program)const;

public:
	//加载着色器源码
	static GLchar* _load(const std::string& _path);

public:
	FShader(FShader& _x) = delete;
	void operator=(FShader& _x) = delete;
public:
	//句柄(对外只读)
	GLuint h = 0;
};