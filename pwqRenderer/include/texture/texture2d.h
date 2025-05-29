#pragma once
#include "texture_base.h"
#include "image/image.h"
#include <string>
#include "misc/global_manager.h"
//////////////////////////////////////////////////////////////////////////////
//纹理类(管理一张2D纹理)
class FTexture2D :public FTexture
{
public:
	//纹理类型
	enum TextureType
	{
		//无类型,当纹理作为帧缓冲附件时自动设置,不用于数组索引
		ttNone = -1,
		//环境光照
		ttAmbient = 0,
		//漫反射
		ttDiffuse,
		//高光
		ttSpecular,
		//反光度
		ttShininess,
		//透明度
		ttOpacity,
		//法线
		ttNormal,
		//自发光
		ttEmissive,
	};
public:
	//库未初始化时此构造函数不执行初始化操作,须手动调用init.
	FTexture2D()
	{
		_baseType = GL_TEXTURE_2D;
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
	}
	//库未初始化时出错
	FTexture2D(const std::string& _path, TextureType _type = ttDiffuse)
	{
		_baseType = GL_TEXTURE_2D;
		_init();
		load(_path, _type);
	}
	//@brief 库未初始化时出错,初始化纹理为帧缓冲附件.
	//@param _format GL_DEPTH_COMPONENT, GL_RGBA...
	FTexture2D(GLint _format, GLsizei _cx, GLsizei _cy)
	{
		_baseType = GL_TEXTURE_2D;
		_init();
		attachment(_format, _cx, _cy);
	}
	//!_autorelease时不调用destroy
	virtual ~FTexture2D()
	{
		if (_autorelease)_destroy();
	}
	//初始化
	void _init();

	//销毁
	void _destroy();

public:
	//从文件载入纹理
	void load(const std::string& _path, TextureType _type = ttDiffuse)
	{
		FImage img(_path, true);
		load(&img, _type);
	}
	//从FImage载入纹理
	void load(FImage* _img, TextureType _type = ttDiffuse);

	//从颜色矩阵载入纹理,_path用作存档
	void load(const void* _data, GLint _wt, GLuint _ht, const std::string& _path, TextureType _type = ttDiffuse);

	//设置为帧缓冲附件
	//_format:GL_DEPTH_COMPONENT, ...
	void attachment(GLint _format, GLsizei _cx, GLsizei _cy);

	//设置纹理平铺类型
	void setWrap(WrapType _typeS = wtRepeat, WrapType _typeT = wtRepeat)
	{
		__super::setWrap(_typeS, _typeT);
	}
public:
	//纹理文件路径(对外只读)
	std::string path = "";
	//纹理类型(对外只读)
	TextureType type = ttDiffuse;
};
