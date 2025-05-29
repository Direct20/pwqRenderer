#include "light/light_dot.h"
#include "misc/err.hpp"
#include "texture/texture_cube.h"
#include "misc/global_manager.h"
using namespace glm;
void FLightDot::setUniformData(GLuint _hProgram)
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been inited.");

	char name[40];
	glUseProgram(_hProgram);

	sprintf_s(name, F_UNAME_LIGHTDOT_ENABLED, _slot);
	glUniform1i(glGetUniformLocation(_hProgram, name), enabled);

	sprintf_s(name, F_UNAME_LIGHTDOT_CLR, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &color.r);

	sprintf_s(name, F_UNAME_LIGHTDOT_INTENSITY, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &intensity.x);

	sprintf_s(name, F_UNAME_LIGHTDOT_KC, _slot);
	glUniform1f(glGetUniformLocation(_hProgram, name), Kc);

	sprintf_s(name, F_UNAME_LIGHTDOT_KL, _slot);
	glUniform1f(glGetUniformLocation(_hProgram, name), Kl);

	sprintf_s(name, F_UNAME_LIGHTDOT_KQ, _slot);
	glUniform1f(glGetUniformLocation(_hProgram, name), Kq);

	sprintf_s(name, F_UNAME_LIGHTDOT_POS, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &pos.x);

	sprintf_s(name, F_UNAME_LIGHTDOT_SHADOW, _slot);
	glUniform1i(glGetUniformLocation(_hProgram, name), shadow);

	if (ptexShadow)//指定了阴影贴图
	{
		sprintf_s(name, F_UNAME_LIGHTDOT_TEX_SHADOW, _slot);
		glUniform1i(glGetUniformLocation(_hProgram, name), F_TEXTURE_START_INDEX_LD_TEX_SHADOW + _slot);
		((FTextureCube*)ptexShadow)->_bind(F_TEXTURE_START_LD_TEX_SHADOW + _slot);
	}

	sprintf_s(name, F_UNAME_LIGHTDOT_FAR_MINUS_NEAR, _slot);
	glUniform1f(glGetUniformLocation(_hProgram, name), _far - _near);
}

void FLightDot::setShadowUniformData(GLuint _hProgram)
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been inited.");

	glUseProgram(_hProgram);

	char name[40];

	for (GLuint i = 0; i < FTextureCube::_cntFaces; i++)
	{
		sprintf_s(name, F_UNAME_SHDWLD_TRANSF_LIGHT, i);
		mat4 matrix = _genLightMatrix(i);
		glUniformMatrix4fv(glGetUniformLocation(_hProgram, name), 1, GL_FALSE, &matrix[0][0]);
	}
	glUniform3fv(glGetUniformLocation(_hProgram, F_UNAME_SHDWLD_LIGHT_POS), 1, &pos[0]);
	glUniform1f(glGetUniformLocation(_hProgram, F_UNAME_SHDWLD_FAR_MINUS_NEAR), _far - _near);
}
