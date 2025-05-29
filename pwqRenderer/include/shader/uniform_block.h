#pragma once
#include <glad/glad.h>
#include <string>
#include "base/base.h"
#include "misc/err.hpp"
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//Uniform�������
class FUniformBlock :public FBase
{
public:
	//�˹��캯����ִ�г�ʼ������,���ֶ�����init.
	FUniformBlock()
	{
	}
	//��ʼ��
	FUniformBlock(GLuint _hProgram, const std::string& _blockName)
	{
		_init(_hProgram, _blockName);
	}
	virtual ~FUniformBlock()
	{
		if (_autorelease)_destroy();
	}
	//��ʼ��
	void _init(GLuint _hProgram, const std::string& _blockName)
	{
		auto& gm = FGlobalManager::get_instance();
		if (!gm.g_isInited)throw ex("The OpenGL context hasn't been initialized.");
		if (hUBO)
		{
			war_excep(0, "The UBO had been initialized.");
			return;
		}
		glGenBuffers(1, &hUBO);//����UBO
		glBindBufferBase(GL_UNIFORM_BUFFER, point, hUBO);//��UBO���󶨵�point

		iBlock = glGetUniformBlockIndex(_hProgram, _blockName.c_str());//��ȡblock����
		glUniformBlockBinding(_hProgram, iBlock, point);//��Uniform�鵽�󶨵�point

		hProgram = _hProgram;
		curPt = point;
		point++;//����point��
	}
	//����
	void _destroy()
	{
		glUniformBlockBinding(hProgram, iBlock, 0);//��Uniform�鵽�󶨵�0
		glDeleteBuffers(1, &hUBO);
		hUBO = 0;
		hProgram = 0;
		iBlock = 0;
		curPt = 0;
	}
public:
	//����Uniform������
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
	//UBO���(����ֻ��)
	GLuint hUBO = 0;
	//��ɫ��������(����ֻ��)
	GLuint hProgram = 0;
	//uniform������(����ֻ��)
	GLuint iBlock = 0;
	//��ǰ�󶨵��İ󶨵�(����ֻ��)
	GLuint curPt = 0;
protected:
	//�󶨵�,0Ϊ��Ч
	static GLuint point;
};
GLuint FUniformBlock::point = 1;