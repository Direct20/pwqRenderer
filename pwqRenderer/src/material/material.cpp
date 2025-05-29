#include "material/material.h"

const FMaterial::MaterialData& FMaterial::_getMaterialData() const
{
	static MaterialData tmp;
	tmp = *this;
	return tmp;
}

void FMaterial::_bindTexture(GLuint _hProgram)
{
	for (GLuint i = 0; i < _cntTex; i++)
	{
		if (!tex[i])continue;//没有纹理
		//绑定到槽:
		tex[i]->_bind(F_TEXTURE_START + i);
		//给采样器赋值:
		char name[20];
		sprintf_s(name, F_UNAME_MATERIAL_TEXTURE, i);
		glUniform1i(glGetUniformLocation(_hProgram, name), F_TEXTURE_START_INDEX + i);
	}
}