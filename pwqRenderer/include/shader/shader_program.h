#pragma once
#include "base/base.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//着色器程序类
class FShaderProgram :public FBase
{
public:
	//变换矩阵类型
	enum MatrixType
	{
		mtModel = 0,
		mtView,
		mtProjection
	};
	//网格体类型
	enum MeshType
	{
		meshStatic,
		meshSkeleton
	};
public:
	//库未初始化时此构造函数不执行初始化操作,须手动调用init.
	FShaderProgram(bool _disableMtl = false)
	{
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
		disableMtl = _disableMtl;
	}
	//初始化,添加着色器,链接,库未初始化时出错.
	FShaderProgram(FShader& _shd1, FShader& _shd2, bool _disableMtl = false)
	{
		_init();
		add(_shd1), add(_shd2);
		link();
		disableMtl = _disableMtl;
	}
	//初始化,添加着色器,链接,库未初始化时出错.
	FShaderProgram(FShader& _shd1, FShader& _shd2, FShader& _shd3, bool _disableMtl = false)
	{
		_init();
		add(_shd1), add(_shd2), add(_shd3);
		link();
		disableMtl = _disableMtl;
	}
	//!_autorelease时不调用destroy
	virtual ~FShaderProgram()
	{
		if (_autorelease)
			if (h)_destroy();
	}
	//初始化着色器程序
	void _init();

	//销毁着色器程序
	void _destroy();

public:
	//添加着色器
	void add(FShader& _shader);

	//链接着色器程序
	void link();

	//使用着色器程序
	void use()const;

public:
	//设置变换矩阵
	void setTransfMatrices(MatrixType _type, glm::mat4 _matrix);

	//设置片段着色器使用的材质
	void setMaterial(class FMaterial& _mtl);

	//设置网格体类型
	void setMeshType(MeshType _type);

	class FLight;
	//设置灯光数据(批量)
	void setLights(FLight* _lts, GLuint _cnt) = delete;
	//设置灯光数据(单一)
	void setLight(FLight& _lt) = delete;

	class FCamera;
	//设置摄像机数据
	void setCamera(FCamera& _cmr) = delete;
public:
	//为int型uniform变量赋值
	void _setInt(const std::string& _name, GLint _val)
	{
		use();
		glUniform1i(glGetUniformLocation(h, _name.c_str()), _val);
	}
	//为float型uniform变量赋值
	void _setFloat(const std::string& _name, GLfloat _val)
	{
		use();
		glUniform1f(glGetUniformLocation(h, _name.c_str()), _val);
	}
	//为bool型uniform变量赋值
	void _setBool(const std::string& _name, GLboolean _val)
	{
		use();
		glUniform1i(glGetUniformLocation(h, _name.c_str()), _val);
	}
	//为mat4型uniform变量赋值
	void _setMat4(const std::string& _name, const GLfloat* _val)
	{
		use();
		glUniformMatrix4fv(glGetUniformLocation(h, _name.c_str()), 1, GL_FALSE, _val);
	}
	//为mat4型uniform变量赋值
	void _setMat4(const std::string& _name, glm::mat4 _val)
	{
		_setMat4(_name, value_ptr(_val));
	}
	//为vec3型uniform变量赋值
	void _setVec3(const std::string& _name, const GLfloat* _val)
	{
		use();
		glUniform3fv(glGetUniformLocation(h, _name.c_str()), 1, _val);
	}
	//为vec3型uniform变量赋值
	void _setVec3(const std::string& _name, glm::vec3 _val)
	{
		_setVec3(_name, value_ptr(_val));
	}
public:
	FShaderProgram(FShaderProgram& _x) = delete;
	void operator=(FShaderProgram& _x) = delete;
public:
	//句柄(对外只读)
	GLuint h = 0;
	//禁用材质(读写)
	bool disableMtl = false;
};