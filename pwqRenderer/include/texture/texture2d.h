#pragma once
#include "texture_base.h"
#include "image/image.h"
#include <string>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//������(����һ��2D����)
class FTexture2D :public FTexture
{
public:
	//��������
	enum TextureType
	{
		//������,��������Ϊ֡���帽��ʱ�Զ�����,��������������
		ttNone = -1,
		//��������
		ttAmbient = 0,
		//������
		ttDiffuse,
		//�߹�
		ttSpecular,
		//�����
		ttShininess,
		//͸����
		ttOpacity,
		//����
		ttNormal,
		//�Է���
		ttEmissive,
	};
public:
	//��δ��ʼ��ʱ�˹��캯����ִ�г�ʼ������,���ֶ�����init.
	FTexture2D()
	{
		_baseType = GL_TEXTURE_2D;
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
	}
	//��δ��ʼ��ʱ����
	FTexture2D(const std::string& _path, TextureType _type = ttDiffuse)
	{
		_baseType = GL_TEXTURE_2D;
		_init();
		load(_path, _type);
	}
	//@brief ��δ��ʼ��ʱ����,��ʼ������Ϊ֡���帽��.
	//@param _format GL_DEPTH_COMPONENT, GL_RGBA...
	FTexture2D(GLint _format, GLsizei _cx, GLsizei _cy)
	{
		_baseType = GL_TEXTURE_2D;
		_init();
		attachment(_format, _cx, _cy);
	}
	//!_autoreleaseʱ������destroy
	virtual ~FTexture2D()
	{
		if (_autorelease)_destroy();
	}
	//��ʼ��
	void _init();

	//����
	void _destroy();

public:
	//���ļ���������
	void load(const std::string& _path, TextureType _type = ttDiffuse)
	{
		FImage img(_path, true);
		load(&img, _type);
	}
	//��FImage��������
	void load(FImage* _img, TextureType _type = ttDiffuse);

	//����ɫ������������,_path�����浵
	void load(const void* _data, GLint _wt, GLuint _ht, const std::string& _path, TextureType _type = ttDiffuse);

	//����Ϊ֡���帽��
	//_format:GL_DEPTH_COMPONENT, ...
	void attachment(GLint _format, GLsizei _cx, GLsizei _cy);

	//��������ƽ������
	void setWrap(WrapType _typeS = wtRepeat, WrapType _typeT = wtRepeat)
	{
		__super::setWrap(_typeS, _typeT);
	}
public:
	//�����ļ�·��(����ֻ��)
	std::string path = "";
	//��������(����ֻ��)
	TextureType type = ttDiffuse;
};
