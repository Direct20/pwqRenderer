#pragma once
#include <glad/glad.h>
#include <string>
#include "base/base.h"
#include "misc/err.hpp"
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//Uniform块管理类
class FUniformBlock :public FBase
{
public:
	//此构造函数不执行初始化操作,须手动调用init.
	FUniformBlock()
	{
	}
	//初始化
	FUniformBlock(GLuint _hProgram, const std::string& _blockName)
	{
		_init(_hProgram, _blockName);
	}
	virtual ~FUniformBlock()
	{
		if (_autorelease)_destroy();
	}
	//初始化
	void _init(GLuint _hProgram, const std::string& _blockName)
	{
		auto& gm = FGlobalManager::get_instance();
		if (!gm.g_isInited)throw ex("The OpenGL context hasn't been initialized.");
		if (hUBO)
		{
			war_excep(0, "The UBO had been initialized.");
			return;
		}
		glGenBuffers(1, &hUBO);//生成UBO
		glBindBufferBase(GL_UNIFORM_BUFFER, point, hUBO);//绑定UBO到绑定点point

		iBlock = glGetUniformBlockIndex(_hProgram, _blockName.c_str());//获取block索引
		glUniformBlockBinding(_hProgram, iBlock, point);//绑定Uniform块到绑定点point

		hProgram = _hProgram;
		curPt = point;
		point++;//迭代point点
	}
	//销毁
	void _destroy()
	{
		glUniformBlockBinding(hProgram, iBlock, 0);//绑定Uniform块到绑定点0
		glDeleteBuffers(1, &hUBO);
		hUBO = 0;
		hProgram = 0;
		iBlock = 0;
		curPt = 0;
	}
public:
	//设置Uniform块数据
	virtual void setData(const void* _data, GLsizeiptr _size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, hUBO);

		glBufferData(GL_UNIFORM_BUFFER, _size, _data, GL_STATIC_READ);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
public:
	FUniformBlock(FUniformBlock& _x) = delete;
	void operator=(FUniformBlock& _x) = delete;
public:
	//UBO句柄(对外只读)
	GLuint hUBO = 0;
	//着色器程序句柄(对外只读)
	GLuint hProgram = 0;
	//uniform块索引(对外只读)
	GLuint iBlock = 0;
	//当前绑定到的绑定点(对外只读)
	GLuint curPt = 0;
protected:
	//绑定点,0为无效
	static GLuint point;
};
GLuint FUniformBlock::point = 1;