#pragma once
#include "base/base.h"
#include "object.h"
#include <glad/glad.h>
#include "misc/def1.h"
#include "texture/texture_cube.h"
#include <glm/glm.hpp>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//天空盒类
class FSkybox :protected FObject
{
public:
	//立方体顶点数据
	const GLfloat _cube_vertices[24] =
	{
		-1,1,-1,
		-1,-1,-1,
		-1,-1,1,
		-1,1,1,
		1,1,1,
		1,-1,1,
		1,-1,-1,
		1,1,-1
	};
	//立方体索引数据
	const GLuint _cube_indices[36] =
	{
		0,1,2,
		0,3,2,
		0,3,7,
		3,4,7,
		2,3,5,
		3,4,5,
		1,2,6,
		2,5,6,
		4,5,7,
		5,6,7,
		0,7,1,
		1,7,6
	};
public:
	//库未初始化时此构造函数不执行初始化,须手动调用_init.
	FSkybox()
		:FObject()
	{

		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_initThis();
	}
	//库未初始化时报错.初始化并指定立方体贴图,将贴图绑定到采样器.
	FSkybox(FTextureCube* _ptex, FShaderProgram& _sp)
		:FObject()
	{
		auto& gm = FGlobalManager::get_instance();
		if (!gm.g_isInited)throw ex(0, F_EXC_DESCRIP_OPENGL_NOT_INITED);
		_initThis();
		ptex = _ptex;
		setUniformData(_sp);
	}
	//!_autorelease时不释放
	virtual ~FSkybox()
	{
		if (_autorelease)_destroyThis();
	}
	//初始化
	void _init()
	{
		__super::_init();
		_initThis();
	}
	//销毁
	void _destroy()
	{
		_destroyThis();
		__super::_destroy();
	}
private:
	//初始化此类
	void _initThis()
	{
		addVertexAttr(aiPosition, 3, GL_FLOAT, sizeof(GLfloat));
		setVertices(_cube_vertices, sizeof(_cube_vertices));
		setIndices(_cube_indices, sizeof(_cube_indices));
	}
	//销毁此类
	void _destroyThis()
	{
		ptex = nullptr;
	}
public:
	//绘制
	void draw(FShaderProgram& _sp)
	{
		glDepthMask(GL_FALSE);//禁用深度缓冲区写入
		glDepthFunc(GL_LEQUAL);//小于等于时通过深度测试
		__super::draw(_sp, FObject::dtIndices, GL_TRIANGLES);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
	}
	//设置Uniform变量数据
	void setUniformData(FShaderProgram& _sp)
	{
		if (!ptex)return;
		ptex->_bind(F_TEXTURE_SKYBOX);
		_sp._setInt(F_UNAME_SKYBOX_TEX, F_TEXTURE_SKYBOX_INDEX);
	}
	//获取r的引用(弧度制)
	glm::vec3& getr()
	{
		return r;
	}
	//获取p的引用
	glm::vec3& getp()
	{
		return p;
	}
	//获取s的引用
	glm::vec3& gets()
	{
		return s;
	}
public:
	//天空盒纹理(读写)
	FTextureCube* ptex = nullptr;
};
