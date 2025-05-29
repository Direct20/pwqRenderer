//////////////////////////////////////////////////////////////////////////////
//material.h - ����
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "base/base.h"
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "texture/texture2d.h"
//////////////////////////////////////////////////////////////////////////////
//������(����һ�ֲ���)
F_REPLICABLE class FMaterial
{
public:
	//�����к��е���ɫ����
	static const GLuint _cntClr = 4;
	//�����к��е���������
	static const GLuint _cntTex = 7;
public:
	//��������
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

		//������Ч��
		alignas(4) bool valid = true;
		//�����
		alignas(4) GLfloat shininess = 32.f;
		//�߹�ǿ��
		alignas(4) GLfloat shininessStrength = 1.f;
		//��͸����
		alignas(4) GLfloat opacity = 1.f;
		//�Է���ǿ��
		alignas(4) GLfloat emissiveIntensity = 1.f;
		//��ɫ(vec3),wΪ�����ֽ�
		alignas(16) FMem4<GLfloat> clr[_cntClr] = {};
		//��������(GLint),yzwΪ�����ֽ�
		alignas(16) FMem4<GLint> hasTex[_cntTex] = {};
	};
public:
	//��ɫ����
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
	//��ȡ��ɫ����ʽ�Ĳ�������
	const MaterialData& _getMaterialData()const;

	//������󶨵����ϲ�Ϊ��ɫ����ֵ
	void _bindTexture(GLuint _hProgram);

public:
	//������Ч��
	bool valid = true;
	//�����
	GLfloat shininess = 32.f;
	//�߹�ǿ��
	GLfloat shininessStrength = 1.f;
	//��͸����
	GLfloat opacity = 1.f;
	//�Է���ǿ��
	GLfloat emissiveIntensity = 1.f;
	//��ɫ
	glm::vec3 clr[_cntClr] = {};
	//��������
	bool hasTex[_cntTex] = {};
	//����
	FTexture* tex[_cntTex] = {};
};
