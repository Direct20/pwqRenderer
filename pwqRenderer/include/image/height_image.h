#pragma once
#include "image/image.h"
#include <string>
#include <glm/glm.hpp>
//////////////////////////////////////////////////////////////////////////////
//�߶�ͼ��
class FHeightImage :public FImage
{
public:
	//������������
	enum FilterType
	{
		ftNearst = 0,
		ftAverage
	};
public:
	FHeightImage() {}
	//���ļ����ظ߶�ͼ
	FHeightImage(const std::string& _path)
	{
		load(_path);
	}
	//���ڴ���ظ߶�ͼ
	FHeightImage(void* _pdata, GLsizeiptr _size)
	{
		load(_pdata, _size);
	}
	virtual ~FHeightImage() {}
public:
	//���ļ����ظ߶�ͼ
	void load(const std::string& _path)
	{
		__super::load(_path, false);
	}
	//���ڴ���ظ߶�ͼ
	void load(void* _pdata, GLsizeiptr _size)
	{
		__super::load(_pdata, _size, "", false);
	}
	//@brief ȡ��������
	//@param _x [0,1]��������
	//@param _y [0,1]��������
	//@return [0,1]�߶�ֵ
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
			int rgn = lvAverage;//���˳̶�
			float ret = 0.f;
			for (int x = -rgn; x <= rgn; x++)
				for (int y = -rgn; y <= rgn; y++)
					ret += get(glm::ivec2(p + glm::vec2(x, y)));
			ret /= powf((rgn * 2.f + 1.f), 2);
			return ret / 255.f;
		}
	}
	//@brief ȡ��������
	//@param _p [0,1]��������
	//@return [0,1]�߶�ֵ
	GLfloat get(glm::vec2 _p)
	{
		return get(_p.x, _p.y);
	}
	//@brief ȡ��������
	//@param _x ��������,<0ʱ��0����,>wt-1ʱ��wt-1����
	//@param _y ��������,<0ʱ��0����,>ht-1ʱ��ht-1����
	//@return �߶�ֵ[0,255]
	GLubyte get(int _x, int _y)
	{
		assert(pdata != nullptr);
		_x = _x < 0 ? 0 : _x;
		_x = _x > wt - 1 ? wt - 1 : _x;
		_y = _y < 0 ? 0 : _y;
		_y = _y > ht - 1 ? ht - 1 : _y;

		return pdata[wt * _y + _x];
	}
	//@brief ȡ��������
	//@param _p ��������,<0ʱ��0����,>wt-1(ht-1)ʱ��wt-1(ht-1)����
	//@return �߶�ֵ[0,255]
	GLubyte get(glm::ivec2 _p)
	{
		return get(_p.x, _p.y);
	}
public:
	//ƽ������(��д)
	GLuint lvAverage = 1;
	//��������(��д)
	FilterType filter = ftAverage;
};