//////////////////////////////////////////////////////////////////////////////
//object.h - 可绘制对象
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "object/object_base.h"
#include "shader/shader_program.h"
#include "material/material.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////////
//①addVertexAttr
//②setVertices
//③setIndices
//可绘制对象类
class FObject :public FBaseObject
{
	friend class FModel;
public:
	//属性索引
	enum AttribIndex
	{
		aiPosition = 0,
		aiColor,
		aiTexCoord,
		aiNormal,
		aiTangent,
		aiBoneIndex,
		aiBoneWeight
	};
	//绘制类型
	enum DrawType
	{
		//按索引绘制
		dtIndices = 0,
		//按顶点绘制
		dtVertices
	};
	//顶点的一项属性
	struct VertexAttrib
	{
		VertexAttrib() {}
		VertexAttrib(GLuint _index, GLint _num, GLenum _type, GLuint _size)
		{
			index = _index;
			num = _num;
			type = _type;
			size = _size;
		}
		//编号
		GLuint index = 0;
		//成员数量
		GLint num = 3;
		//成员类型
		GLenum type = GL_FLOAT;
		//一个成员占用的字节数
		GLuint size = sizeof(float);
	};
	using MeshType = FShaderProgram::MeshType;
public:
	//库未初始化时此构造函数不执行初始化操作,须手动调用init.
	FObject()
	{
		//用于支持在OpenGL初始化前创建全局变量

		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
	}
	//库未初始化时出错
	FObject(const VertexAttrib* _attrs, GLuint _cnt)
	{
		_init();
		addVertexAttrs(_attrs, _cnt);
	}
	//!_autorelease时不调用destroy
	virtual ~FObject()
	{
		if (_autorelease)
			_destroy();
	}
	//初始化
	void _init();

	//销毁
	void _destroy();

public:
	//添加一项顶点属性.
	//索引,成员数量,成员类型,单一成员占用字节数.
	void addVertexAttr(GLuint _index, GLint _num, GLenum _type, GLuint _size)
	{
		attribs.emplace_back(_index, _num, _type, _size);
	}
	//添加一项顶点属性.(复制_attr并存储)
	void addVertexAttr(const VertexAttrib& _attr)
	{
		attribs.push_back(_attr);
	}
	//批量添加顶点属性,追加到末尾.(复制_attrs并存储)
	void addVertexAttrs(const VertexAttrib* _attrs, GLuint _cnt)
	{
		attribs.insert(attribs.end(), _attrs, &_attrs[_cnt]);
	}
	//获取顶点所拥有的属性项目数组
	std::vector<VertexAttrib>& getVertexAttrs()
	{
		return attribs;
	}

	//一次设置顶点所有属性的数据.
	//_pdata可以是指向临时变量的指针.
	void setVertices(const void* const _pdata, GLsizeiptr _size);

	//一次设置顶点一项属性的数据.
	//_pdata可以是指向临时变量的指针.
	//_index属性索引
	void setVertices(const void* const _pdata, GLsizeiptr _size, AttribIndex _index);

	//设置索引数据,索引类型须为uint,否则出错.
	//_pdata可以是指向临时变量的指针.
	void setIndices(const void* const _pdata, GLsizeiptr _size);

	//设置材质
	void setMaterial(FMaterial& _mtl);


	//绘制
	void draw(FShaderProgram& _sp, DrawType _dt = dtIndices, GLenum _mode = GL_TRIANGLES);

	//绘制,_modelMatrix调用_genModelMatrix生成
	void draw(FShaderProgram& _sp, glm::mat4 _modelMatrix, DrawType _dt = dtIndices, GLenum _mode = GL_TRIANGLES);

	//按静态网格体绘制
	void draw_static(FShaderProgram& _sp, glm::mat4 _modelMatrix, DrawType _dt = dtIndices, GLenum _mode = GL_TRIANGLES);

	//按静态网格体绘制
	void draw_static(FShaderProgram& _sp, DrawType _dt = dtIndices, GLenum _mode = GL_TRIANGLES);

public:
	FObject(FObject& _x) = delete;
	void operator=(FObject& _x) = delete;
private:
	//VAO句柄
	GLuint hVAO = 0;
	//顶点属性,每个元素代表顶点的一项属性的信息
	std::vector<VertexAttrib> attribs;
	//顶点数目
	GLuint cntVertex = 0;
	//索引数目
	GLuint cntIndex = 0;
protected:
	//网格体类型(读写)
	MeshType meshType = MeshType::meshStatic;
public:
	//材质(读写)
	FMaterial mtl;
};