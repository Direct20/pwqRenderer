#include "light/light_parallel.h"
#include "misc/err.hpp"
#include "texture/texture2d.h"
#include "misc/global_manager.h"
using namespace glm;
void FLightParallel::setUniformData(GLuint _hProgram)
{
	auto& gm = FGlobalManager::get_instance();
	if (!gm.g_isInited)throw ex("OpenGL hasn't been inited.");

	char name[40];
	glUseProgram(_hProgram);

	sprintf_s(name, F_UNAME_LIGHTPARALLEL_ENABLED, _slot);
	glUniform1i(glGetUniformLocation(_hProgram, name), enabled);

	sprintf_s(name, F_UNAME_LIGHTPARALLEL_CLR, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &color.r);

	sprintf_s(name, F_UNAME_LIGHTPARALLEL_INTENSITY, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &intensity.x);

	sprintf_s(name, F_UNAME_LIGHTPARALLEL_DIRECTION, _slot);
	glUniform3fv(glGetUniformLocation(_hProgram, name), 1, &direction.x);

	sprintf_s(name, F_UNAME_LIGHTPARALLEL_SHADOW, _slot);
	glUniform1i(glGetUniformLocation(_hProgram, name), shadow);

	if (ptexShadow)
	{
		sprintf_s(name, F_UNAME_LIGHTPARALLEL_TEX_SHADOW, _slot);
		glUniform1i(glGetUniformLocation(_hProgram, name), F_TEXTURE_START_INDEX_LP_TEX_SHADOW + _slot);
		((FTexture2D*)ptexShadow)->_bind(F_TEXTURE_START_LP_TEX_SHADOW + _slot);
	}

	mat4 matVP = _genProjectionMatrix() * _genLightMatrix();
	sprintf_s(name, F_UNAME_LIGHTPARALLEL_MAT_VP, _slot);
	glUniformMatrix4fv(glGetUniformLocation(_hProgram, name), 1, GL_FALSE, &matVP[0][0]);
}
