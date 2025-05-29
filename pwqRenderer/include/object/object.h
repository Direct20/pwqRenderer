//////////////////////////////////////////////////////////////////////////////
//object.h - �ɻ��ƶ���
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "object/object_base.h"
#include "shader/shader_program.h"
#include "material/material.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////////
//��addVertexAttr
//��setVertices
//��setIndices
//�ɻ��ƶ�����
class FObject :public FBaseObject
{
	friend class FModel;
public:
	//��������
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
	//��������
	enum DrawType
	{
		//����������
		dtIndices = 0,
		//���������
		dtVertices
	};
	//�����һ������
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
		//���
		GLuint index = 0;
		//��Ա����
		GLint num = 3;
		//��Ա����
		GLenum type = GL_FLOAT;
		//һ����Առ�õ��ֽ���
		GLuint size = sizeof(float);
	};
	using MeshType = FShaderProgram::MeshType;
public:
	//��δ��ʼ��ʱ�˹��캯����ִ�г�ʼ������,���ֶ�����init.
	FObject()
	{
		//����֧����OpenGL��ʼ��ǰ����ȫ�ֱ���

		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
	}
	//��δ��ʼ��ʱ����
	FObject(const VertexAttrib* _attrs, GLuint _cnt)
	{
		_init();
		addVertexAttrs(_attrs, _cnt);
	}
	//!_autoreleaseʱ������destroy
	virtual ~FObject()
	{
		if (_autorelease)
			_destroy();
	}
	//��ʼ��
	void _init();

	//����
	void _destroy();

public:
	//���һ�������.
	//����,��Ա����,��Ա����,��һ��Առ���ֽ���.
	void addVertexAttr(GLuint _index, GLint _num, GLenum _type, GLuint _size)
	{
		attribs.emplace_back(_index, _num, _type, _size);
	}
	//���һ�������.(����_attr���洢)
	void addVertexAttr(const VertexAttrib& _attr)
	{
		attribs.push_back(_attr);
	}
	//������Ӷ�������,׷�ӵ�ĩβ.(����_attrs���洢)
	void addVertexAttrs(const VertexAttrib* _attrs, GLuint _cnt)
	{
		attribs.insert(attribs.end(), _attrs, &_attrs[_cnt]);
	}
	//��ȡ������ӵ�е�������Ŀ����
	std::vector<VertexAttrib>& getVertexAttrs()
	{
		return attribs;
	}

	//һ�����ö����������Ե�����.
	//_pdata������ָ����ʱ������ָ��.
	void setVertices(const void* const _pdata, GLsizeiptr _size);

	//һ�����ö���һ�����Ե�����.
	//_pdata������ָ����ʱ������ָ��.
	//_index��������
	void setVertices(const void* const _pdata, GLsizeiptr _size, AttribIndex _index);

	//������������,����������Ϊuint,�������.
	//_pdata������ָ����ʱ������ָ��.
	void setIndices(const void* const _pdata, GLsizeiptr _size);

	//���ò���
	void setMaterial(FMaterial& _mtl);


	//����
	void draw(FShaderProgram& _sp, DrawType _dt = dtIndices, GLenum _mode = GL_TRIANGLES);

	//����,_modelMatrix����_genModelMatrix����
	void draw(FShaderProgram& _sp, glm::mat4 _modelMatrix, DrawType _dt = dtIndices, GLenum _mode = GL_TRIANGLES);

	//����̬���������
	void draw_static(FShaderProgram& _sp, glm::mat4 _modelMatrix, DrawType _dt = dtIndices, GLenum _mode = GL_TRIANGLES);

	//����̬���������
	void draw_static(FShaderProgram& _sp, DrawType _dt = dtIndices, GLenum _mode = GL_TRIANGLES);

public:
	FObject(FObject& _x) = delete;
	void operator=(FObject& _x) = delete;
private:
	//VAO���
	GLuint hVAO = 0;
	//��������,ÿ��Ԫ�ش������һ�����Ե���Ϣ
	std::vector<VertexAttrib> attribs;
	//������Ŀ
	GLuint cntVertex = 0;
	//������Ŀ
	GLuint cntIndex = 0;
protected:
	//����������(��д)
	MeshType meshType = MeshType::meshStatic;
public:
	//����(��д)
	FMaterial mtl;
};