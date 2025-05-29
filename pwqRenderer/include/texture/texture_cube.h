#pragma once
#include "texture_base.h"
#include <glad/glad.h>
#include "misc/def0.h"
#include "image/image.h"
#include <string>
#include "misc/global_manager.h"

//////////////////////////////////////////////////////////////////////////////
//立方体纹理类
class FTextureCube :public FTexture
{
public:
	//库未初始化时此构造函数不执行初始化操作,须手动调用_init.
	FTextureCube()
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		auto& gm = FGlobalManager::get_instance();
		if (gm.g_isInited)_init();
	}
	//库未初始化时出错.初始化并加载纹理.
	FTextureCube(FImage* _imgs)
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		_init();
		load(_imgs);
	}
	//库未初始化时出错.初始化并加载纹理.
	FTextureCube(const char** _paths, bool _flip = false)
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		_init();
		load(_paths, _flip);
	}
	//库未初始化时出错.初始化并加载纹理.
	FTextureCube(const std::string* _paths, bool _flip = false)
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		_init();
		load(_paths, _flip);
	}
	//@brief 库未初始化时出错.初始化并创建为帧缓冲附件
	//@param _format GL_DEPTH_COMPONENT, ...
	//@param _wt 宽度
	FTextureCube(GLint _format, GLsizei _wt)
	{
		_baseType = GL_TEXTURE_CUBE_MAP;
		_init();
		attachment(_format, _wt);
	}
	//!_autorelease时不调用_destroy
	virtual ~FTextureCube()
	{
		if (_autorelease)_destroy();
	}
	//初始化
	void _init();

	//销毁
	void _destroy();

public:
	//从文件加载贴图
	void load(const char** _paths, bool _flip = false);

	//从文件加载贴图
	void load(const std::string* _paths, bool _flip = false);

	//从FImage加载贴图,_imgs应为数组(6)
	void load(FImage* _imgs);

	//@brief 设置为帧缓冲附件,
	//@param _format GL_DEPTH_COMPONENT, ...
	//@param _wt 宽度
	void attachment(GLint _format, GLsizei _wt);

public:
	//立方体面数
	static const GLuint _cntFaces = 6;
public:
	//纹理路径(+-x+-y+-z)
	std::string path[_cntFaces];
};
