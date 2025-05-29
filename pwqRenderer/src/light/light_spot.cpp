#include "light/light_spot.h"
#include "misc/err.hpp"
#include "texture/texture_cube.h"
using namespace glm;

void FLightSpot::setUniformData(GLuint _hProgram)
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been inited.");

	char name[40];
	glUseProgram(_hProgram);

	sprintf_s(name, F_UNAME_LIGHTSPOT_ENABLED, _slot);
	glUniform1i(glGetUniformLocation(_hProgram, name), enabled);

	sprintf_s(name, F_UNAME_LIGHTSPOT_CLR, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &color.r);

	sprintf_s(name, F_UNAME_LIGHTSPOT_INTENSITY, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &intensity.x);

	sprintf_s(name, F_UNAME_LIGHTSPOT_KC, _slot);
	glUniform1f(glGetUniformLocation(_hProgram, name), Kc);

	sprintf_s(name, F_UNAME_LIGHTSPOT_KL, _slot);
	glUniform1f(glGetUniformLocation(_hProgram, name), Kl);

	sprintf_s(name, F_UNAME_LIGHTSPOT_KQ, _slot);
	glUniform1f(glGetUniformLocation(_hProgram, name), Kq);

	sprintf_s(name, F_UNAME_LIGHTSPOT_POS, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &pos.x);

	sprintf_s(name, F_UNAME_LIGHTSPOT_DIRECTION, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &direction.x);

	sprintf_s(name, F_UNAME_LIGHTSPOT_INNERCUTOFF, _slot);
	glUniform1f(glGetUniformLocation(_hProgram, name), innerCutOff);

	sprintf_s(name, F_UNAME_LIGHTSPOT_OUTTERCUTOFF, _slot);
	glUniform1f(glGetUniformLocation(_hProgram, name), outterCutOff);

	sprintf_s(name, F_UNAME_LIGHTSPOT_SHADOW, _slot);
	glUniform1i(glGetUniformLocation(_hProgram, name), shadow);

	//sprintf_s(name, F_UNAME_LIGHTSPOT_TEX_SHADOW, _slot);
	//glUniform1i(glGetUniformLocation(_hProgram, name), F_TEXTURE_START_INDEX_TEX_SHADOW_LS + _slot);
	//((FTexture2D*)ptexShadow)->_bind(F_TEXTURE_START_TEX_SHADOW_LS + _slot);

	sprintf_s(name, F_UNAME_LIGHTSPOT_MAT_VP, _slot);
	mat4 matVP = _genProjectionMatrix() * _genLightMatrix();
	glUniformMatrix4fv(glGetUniformLocation(_hProgram, name), 1, GL_FALSE, &matVP[0][0]);
}