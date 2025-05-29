#pragma once
#include "image/image.h"
#include <string>
#include <glm/glm.hpp>
//////////////////////////////////////////////////////////////////////////////
//高度图类
class FHeightImage :public FImage
{
public:
	//采样过滤类型
	enum FilterType
	{
		ftNearst = 0,
		ftAverage
	};
public:
	FHeightImage() {}
	//从文件加载高度图
	FHeightImage(const std::string& _path)
	{
		load(_path);
	}
	//从内存加载高度图
	FHeightImage(void* _pdata, GLsizeiptr _size)
	{
		load(_pdata, _size);
	}
	virtual ~FHeightImage() {}
public:
	//从文件加载高度图
	void load(const std::string& _path)
	{
		__super::load(_path, false);
	}
	//从内存加载高度图
	void load(void* _pdata, GLsizeiptr _size)
	{
		__super::load(_pdata, _size, "", false);
	}
	//@brief 取像素数据
	//@param _x [0,1]纹理坐标
	//@param _y [0,1]纹理坐标
	//@return [0,1]高度值
	GLfloat get(GLfloat _x, GLfloat _y)
	{
		glm::vec2 p = glm::vec2(wt * _x, ht * _y);
		p = glm::vec2(round(p.x), round(p.y));

		if (filter == ftNearst)
		{
			return get(glm::ivec2(p)) / 255.f;
		}
		else
		{
			int rgn = lvAverage;//过滤程度
			float ret = 0.f;
			for (int x = -rgn; x <= rgn; x++)
				for (int y = -rgn; y <= rgn; y++)
					ret += get(glm::ivec2(p + glm::vec2(x, y)));
			ret /= powf((rgn * 2.f + 1.f), 2);
			return ret / 255.f;
		}
	}
	//@brief 取像素数据
	//@param _p [0,1]纹理坐标
	//@return [0,1]高度值
	GLfloat get(glm::vec2 _p)
	{
		return get(_p.x, _p.y);
	}
	//@brief 取像素数据
	//@param _x 纹理坐标,<0时作0处理,>wt-1时作wt-1处理
	//@param _y 纹理坐标,<0时作0处理,>ht-1时作ht-1处理
	//@return 高度值[0,255]
	GLubyte get(int _x, int _y)
	{
		assert(pdata != nullptr);
		_x = _x < 0 ? 0 : _x;
		_x = _x > wt - 1 ? wt - 1 : _x;
		_y = _y < 0 ? 0 : _y;
		_y = _y > ht - 1 ? ht - 1 : _y;

		return pdata[wt * _y + _x];
	}
	//@brief 取像素数据
	//@param _p 纹理坐标,<0时作0处理,>wt-1(ht-1)时作wt-1(ht-1)处理
	//@return 高度值[0,255]
	GLubyte get(glm::ivec2 _p)
	{
		return get(_p.x, _p.y);
	}
public:
	//平均级别(读写)
	GLuint lvAverage = 1;
	//过滤类型(读写)
	FilterType filter = ftAverage;
};