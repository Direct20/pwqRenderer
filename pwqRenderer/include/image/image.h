#pragma once
#include "base/base.h"
#include "glad/glad.h"
#include <string>
//////////////////////////////////////////////////////////////////////////////
//ͼ����(����һ��ͼ��)
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
	//���ⲿ����ͼ��
	void load(const std::string& _path, bool _flipY = true);

	//���ڴ����ͼ��(_path�����洢)
	void load(void* _pdata, GLsizeiptr _size, const std::string& _path = "", bool _flipY = false);

	//����ͼ��
	void free();

	//����Ϊpng
	void save_PNG(const std::string& _path) = delete;
	//{
		//stbi_write_png(_path.c_str(),wt,ht,chs,pdata,0);
	//}
public:
	//����ͨ������ȡOpenGL��ʽ
	static GLint format(GLint _chs);

public:
	FImage(FImage& _x) = delete;
	void operator=(FImage& _x) = delete;
public:
	//ͼ��·��(����ֻ��)
	std::string path = "";
	//ͼ������(����ֻ��)
	GLubyte* pdata = nullptr;
	//��,��(����ֻ��)
	GLint wt = 0, ht = 0;
	//ͨ����(����ֻ��)
	GLint chs = 0;
};
