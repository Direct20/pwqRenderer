//////////////////////////////////////////////////////////////////////////////
//material.h - 材质
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "base/base.h"
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "texture/texture2d.h"
//////////////////////////////////////////////////////////////////////////////
//材质类(管理一种材质)
F_REPLICABLE class FMaterial
{
public:
	//材质中含有的颜色数量
	static const GLuint _cntClr = 4;
	//材质中含有的纹理数量
	static const GLuint _cntTex = 7;
public:
	//材质数据
	struct MaterialData
	{
		MaterialData() {}
		MaterialData(const FMaterial& _x)
		{
			operator=(_x);
		}
		void operator=(const FMaterial& _x)
		{
			valid = _x.valid;
			shininess = _x.shininess;
			shininessStrength = _x.shininessStrength;
			opacity = _x.opacity;
			emissiveIntensity = _x.emissiveIntensity;
			for (GLuint i = 0; i < _cntClr; i++)
			{
				clr[i].x = _x.clr[i].x;
				clr[i].y = _x.clr[i].y;
				clr[i].z = _x.clr[i].z;
			}
			for (GLuint i = 0; i < _cntTex; i++)
				hasTex[i].x = _x.hasTex[i];
		}

		//材质有效性
		alignas(4) bool valid = true;
		//反光度
		alignas(4) GLfloat shininess = 32.f;
		//高光强度
		alignas(4) GLfloat shininessStrength = 1.f;
		//不透明度
		alignas(4) GLfloat opacity = 1.f;
		//自发光强度
		alignas(4) GLfloat emissiveIntensity = 1.f;
		//颜色(vec3),w为垃圾字节
		alignas(16) FMem4<GLfloat> clr[_cntClr] = {};
		//含有纹理(GLint),yzw为垃圾字节
		alignas(16) FMem4<GLint> hasTex[_cntTex] = {};
	};
public:
	//颜色类型
	enum ColorType
	{
		ctAmbient = 0,
		ctDiffuse,
		ctSpecular,
		ctEmissive,
	};
public:
	FMaterial()
	{
	}
	virtual ~FMaterial()
	{
	}
public:
	//获取着色器格式的材质数据
	const MaterialData& _getMaterialData()const;

	//将纹理绑定到槽上并为着色器赋值
	void _bindTexture(GLuint _hProgram);

public:
	//材质有效性
	bool valid = true;
	//反光度
	GLfloat shininess = 32.f;
	//高光强度
	GLfloat shininessStrength = 1.f;
	//不透明度
	GLfloat opacity = 1.f;
	//自发光强度
	GLfloat emissiveIntensity = 1.f;
	//颜色
	glm::vec3 clr[_cntClr] = {};
	//含有纹理
	bool hasTex[_cntTex] = {};
	//纹理
	FTexture* tex[_cntTex] = {};
};
