#pragma once
#include "base/base.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//��ɫ��������
class FShaderProgram :public FBase
{
public:
	//�任��������
	enum MatrixType
	{
		mtModel = 0,
		mtView,
		mtProjection
	};
	//����������
	enum MeshType
	{
		meshStatic,
		meshSkeleton
	};
public:
	//��δ��ʼ��ʱ�˹��캯����ִ�г�ʼ������,���ֶ�����init.
	FShaderProgram(bool _disableMtl = false)
	{
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
		disableMtl = _disableMtl;
	}
	//��ʼ��,�����ɫ��,����,��δ��ʼ��ʱ����.
	FShaderProgram(FShader& _shd1, FShader& _shd2, bool _disableMtl = false)
	{
		_init();
		add(_shd1), add(_shd2);
		link();
		disableMtl = _disableMtl;
	}
	//��ʼ��,�����ɫ��,����,��δ��ʼ��ʱ����.
	FShaderProgram(FShader& _shd1, FShader& _shd2, FShader& _shd3, bool _disableMtl = false)
	{
		_init();
		add(_shd1), add(_shd2), add(_shd3);
		link();
		disableMtl = _disableMtl;
	}
	//!_autoreleaseʱ������destroy
	virtual ~FShaderProgram()
	{
		if (_autorelease)
			if (h)_destroy();
	}
	//��ʼ����ɫ������
	void _init();

	//������ɫ������
	void _destroy();

public:
	//�����ɫ��
	void add(FShader& _shader);

	//������ɫ������
	void link();

	//ʹ����ɫ������
	void use()const;

public:
	//���ñ任����
	void setTransfMatrices(MatrixType _type, glm::mat4 _matrix);

	//����Ƭ����ɫ��ʹ�õĲ���
	void setMaterial(class FMaterial& _mtl);

	//��������������
	void setMeshType(MeshType _type);

	class FLight;
	//���õƹ�����(����)
	void setLights(FLight* _lts, GLuint _cnt) = delete;
	//���õƹ�����(��һ)
	void setLight(FLight& _lt) = delete;

	class FCamera;
	//�������������
	void setCamera(FCamera& _cmr) = delete;
public:
	//Ϊint��uniform������ֵ
	void _setInt(const std::string& _name, GLint _val)
	{
		use();
		glUniform1i(glGetUniformLocation(h, _name.c_str()), _val);
	}
	//Ϊfloat��uniform������ֵ
	void _setFloat(const std::string& _name, GLfloat _val)
	{
		use();
		glUniform1f(glGetUniformLocation(h, _name.c_str()), _val);
	}
	//Ϊbool��uniform������ֵ
	void _setBool(const std::string& _name, GLboolean _val)
	{
		use();
		glUniform1i(glGetUniformLocation(h, _name.c_str()), _val);
	}
	//Ϊmat4��uniform������ֵ
	void _setMat4(const std::string& _name, const GLfloat* _val)
	{
		use();
		glUniformMatrix4fv(glGetUniformLocation(h, _name.c_str()), 1, GL_FALSE, _val);
	}
	//Ϊmat4��uniform������ֵ
	void _setMat4(const std::string& _name, glm::mat4 _val)
	{
		_setMat4(_name, value_ptr(_val));
	}
	//Ϊvec3��uniform������ֵ
	void _setVec3(const std::string& _name, const GLfloat* _val)
	{
		use();
		glUniform3fv(glGetUniformLocation(h, _name.c_str()), 1, _val);
	}
	//Ϊvec3��uniform������ֵ
	void _setVec3(const std::string& _name, glm::vec3 _val)
	{
		_setVec3(_name, value_ptr(_val));
	}
public:
	FShaderProgram(FShaderProgram& _x) = delete;
	void operator=(FShaderProgram& _x) = delete;
public:
	//���(����ֻ��)
	GLuint h = 0;
	//���ò���(��д)
	bool disableMtl = false;
};