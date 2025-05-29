#pragma once
#include <vector>
//////////////////////////////////////////////////////////////////////////////
//�����������
template <typename T>
class FManagerVector
{
public:
	FManagerVector() {}
	~FManagerVector()
	{
		for (unsigned i = 0; i < lib.size(); i++)
			delete lib[i];
	}
public:
	//����Ĭ�Ϲ������newһ������
	T* newOne()
	{
		T* p = new T();
		lib.push_back(p);
		return p;
	}
	//���õ��������캯��newһ������
	template<typename _Param>
	T* newOne(_Param& _p)
	{
		T* p = new T(_p);
		lib.push_back(p);
		return p;
	}
	//����˫�������캯��newһ������
	template<typename _Param1, typename _Param2>
	T* newOne(_Param1& _p1, _Param2& _p2)
	{
		T* p = new T(_p1, _p2);
		lib.push_back(p);
		return p;
	}
	//��new�����Ķ������������,ʹ�ù������Զ�����
	void addOne(T* _x)
	{
		lib.push_back(_x);
	}
	//ɾ������,�ͷ��ڴ�
	void deleteOne(unsigned _index)
	{
		delete lib[_index];
		lib.erase(lib.begin() + _index);
	}
public:
	FManagerVector(FManagerVector& _x) = delete;
	void operator=(FManagerVector& _x) = delete;
public:
	//�����
	std::vector<T*> lib;
};
