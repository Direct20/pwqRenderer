#pragma once
#include "texture_base.h"
#include <glad/glad.h>
#include "misc/def0.h"
#include "image/image.h"
#include <string>
#include "misc/global_manager.h"

//////////////////////////////////////////////////////////////////////////////
//������������
class FTextureCube :public FTexture
{
public:
	//��δ��ʼ��ʱ�˹��캯����ִ�г�ʼ������,���ֶ�����_init.
	FTextureCube()
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
	}
	//��δ��ʼ��ʱ����.��ʼ������������.
	FTextureCube(FImage* _imgs)
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		_init();
		load(_imgs);
	}
	//��δ��ʼ��ʱ����.��ʼ������������.
	FTextureCube(const char** _paths, bool _flip = false)
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		_init();
		load(_paths, _flip);
	}
	//��δ��ʼ��ʱ����.��ʼ������������.
	FTextureCube(const std::string* _paths, bool _flip = false)
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		_init();
		load(_paths, _flip);
	}
	//@brief ��δ��ʼ��ʱ����.��ʼ��������Ϊ֡���帽��
	//@param _format GL_DEPTH_COMPONENT, ...
	//@param _wt ���
	FTextureCube(GLint _format, GLsizei _wt)
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		_init();
		attachment(_format, _wt);
	}
	//!_autoreleaseʱ������_destroy
	virtual ~FTextureCube()
	{
		if (_autorelease)_destroy();
	}
	//��ʼ��
	void _init();

	//����
	void _destroy();

public:
	//���ļ�������ͼ
	void load(const char** _paths, bool _flip = false);

	//���ļ�������ͼ
	void load(const std::string* _paths, bool _flip = false);

	//��FImage������ͼ,_imgsӦΪ����(6)
	void load(FImage* _imgs);

	//@brief ����Ϊ֡���帽��,
	//@param _format GL_DEPTH_COMPONENT, ...
	//@param _wt ���
	void attachment(GLint _format, GLsizei _wt);

public:
	//����������
	static const GLuint _cntFaces = 6;
public:
	//����·��(+-x+-y+-z)
	std::string path[_cntFaces];
};
