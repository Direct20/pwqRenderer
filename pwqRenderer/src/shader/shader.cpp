#include "shader/shader.h"
#include "misc/def0.h"
#include <fstream>
#include <glad/glad.h>
#include "misc/global_manager.h"
using namespace std;

void FShader::_init(ShaderType _type, const char* const _src)
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been initialized.");
	if (h)
	{
		war_excep(0, "The shader object has been initialized.");
		return;
	}

	int suc = false;
	GLchar info[F_MAX_STATUS_INFO];

	h = glCreateShader(_type);//创建着色器
	glShaderSource(h, 1, &_src, 0);//设置源码
	glCompileShader(h);//编译
	glGetShaderiv(h, GL_COMPILE_STATUS, &suc);//获取编译状态
	if (!suc)
	{
		glGetShaderInfoLog(h, sizeof(info), 0, info);//获取日志信息
		throw ex(std::string("Failed to compile shader.\n") + info);
	}
}

void FShader::_destroy()
{
	if (h)glDeleteShader(h);
	h = 0;
}

void FShader::_attach(GLuint _program) const
{
	glAttachShader(_program, h);
}
GLchar* FShader::_load(const std::string& _path)
{
	ifstream fin;
	fin.open(_path, ios::in | ios::binary);
	if (!fin.is_open())throw ex(0, "Failed to open shader source file.");
	//获取文件长度:
	fin.seekg(0, ios::end);
	GLint end = (GLint)fin.tellg();
	fin.seekg(0, ios::beg);
	GLuint len = (GLuint)(end - fin.tellg());

	static char _src[F_MAX_SHADER_SRC];
	fin.read(_src, len);//读取全部
	if (fin.bad())throw ex(0, "Failed to load shader source file.");
	_src[len] = '\0';
	fin.close();
	return _src;
}