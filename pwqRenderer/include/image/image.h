#pragma once
#include "base/base.h"
#include "glad/glad.h"
#include <string>
//////////////////////////////////////////////////////////////////////////////
//图像类(管理一张图像)
class FImage :public FBase
{
public:
	FImage()
	{

	}
	FImage(const std::string& _path, bool _flipY = true)
	{
		load(_path, _flipY);
	}
	FImage(void* _pdata, GLsizeiptr _size, const std::string& _path, bool _flipY = true)
	{
		load(_pdata, _size, _path, _flipY);
	}
	virtual ~FImage()
	{
		if (_autorelease)free();
	}
public:
	//从外部加载图像
	void load(const std::string& _path, bool _flipY = true);

	//从内存加载图像(_path用作存储)
	void load(void* _pdata, GLsizeiptr _size, const std::string& _path = "", bool _flipY = false);

	//销毁图像
	void free();

	//保存为png
	void save_PNG(const std::string& _path) = delete;
	//{
		//stbi_write_png(_path.c_str(),wt,ht,chs,pdata,0);
	//}
public:
	//根据通道数获取OpenGL格式
	static GLint format(GLint _chs);

public:
	FImage(FImage& _x) = delete;
	void operator=(FImage& _x) = delete;
public:
	//图像路径(对外只读)
	std::string path = "";
	//图像数据(对外只读)
	GLubyte* pdata = nullptr;
	//宽,高(对外只读)
	GLint wt = 0, ht = 0;
	//通道数(对外只读)
	GLint chs = 0;
};
