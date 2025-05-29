#include "shader/shader_program.h"
#include <string>
#include "misc/err.hpp"
#include "material/material.h"
#include "shader/uniform_block.h"
#include "misc/global_manager.h"
using namespace std;

void FShaderProgram::_init()
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been initialized.");
	if (!h)h = glCreateProgram();
	else war_excep(0, "The shader program has been initialized.");
}

void FShaderProgram::_destroy()
{
	if (h)glDeleteProgram(h);
	h = 0;
}

void FShaderProgram::add(FShader& _shader)
{
	_shader._attach(h);
}
void FShaderProgram::link()
{
	GLint suc;
	GLchar info[F_MAX_STATUS_INFO];

	glLinkProgram(h);
	glGetProgramiv(h, GL_LINK_STATUS, &suc);
	if (!suc)
	{
		glGetProgramInfoLog(h, sizeof(info), 0, info);
		throw ex(string("Failed to link shader program.") + info);
	}
}

void FShaderProgram::use() const
{
	glUseProgram(h);
}

void FShaderProgram::setTransfMatrices(MatrixType _type, glm::mat4 _matrix)
{
	switch (_type)
	{
	case mtModel:
		_setMat4(F_UNAME_TRANSF_MODEL, value_ptr(_matrix));
		break;
	case mtView:
		_setMat4(F_UNAME_TRANSF_VIEW, value_ptr(_matrix));
		break;
	case mtProjection:
		_setMat4(F_UNAME_TRANSF_PROJECTION, value_ptr(_matrix));
		break;
	}
}

void FShaderProgram::setMaterial(FMaterial& _mtl)
{
	if (disableMtl)return;

	static FUniformBlock uboMat;//Material Uniform块管理
	if (!uboMat.hUBO)//未初始化
		uboMat._init(h, F_UNAME_BLOCK_MATERIAL);

	FMaterial::MaterialData tmp = _mtl._getMaterialData();
	uboMat.setData(&tmp, sizeof(tmp));
	_mtl._bindTexture(h);
}

void FShaderProgram::setMeshType(MeshType _type)
{
	_setInt(F_UNAME_MESH_TYPE, _type);
}
