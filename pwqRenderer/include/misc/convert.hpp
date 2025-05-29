//////////////////////////////////////////////////////////////////////////////
//convert.h - Assimp×ª»»
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "glm/glm.hpp"
#include "assimp/matrix4x4.h"
#include <assimp/scene.h>
#include <glad/glad.h>
inline glm::vec3 convert(const aiColor3D& _x)
{
	return glm::vec3(_x.r, _x.g, _x.b);
}
inline glm::vec4 convert(const ai_real* _x)
{
	return glm::vec4(_x[0], _x[1], _x[2], _x[3]);
}
inline ai_real* convert(const glm::vec4& _x)
{
	return (ai_real*)&_x[0];
}
inline glm::mat4 convert(aiMatrix4x4 _x)
{
	_x.Transpose();
	glm::mat4 tmp;
	for (GLuint i = 0; i < 4; i++)
		tmp[i] = convert(_x[i]);
	return tmp;
}
inline aiMatrix4x4 convert(const glm::mat4& _x)
{
	glm::mat4 t= transpose(_x);
	aiMatrix4x4 tmp;
	for (GLuint i = 0; i < 4; i++)
	{
		ai_real* line=convert(t[i]);
		for (GLuint j = 0; j < 4; j++)
			tmp[i][j] = line[j];
	}
	return tmp;
}
