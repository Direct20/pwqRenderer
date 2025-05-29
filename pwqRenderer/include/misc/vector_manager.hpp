#pragma once
#include <vector>
//////////////////////////////////////////////////////////////////////////////
//对象管理器类
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
	//调用默认构造参数new一个对象
	T* newOne()
	{
		T* p = new T();
		lib.push_back(p);
		return p;
	}
	//调用单参数构造函数new一个对象
	template<typename _Param>
	T* newOne(_Param& _p)
	{
		T* p = new T(_p);
		lib.push_back(p);
		return p;
	}
	//调用双参数构造函数new一个对象
	template<typename _Param1, typename _Param2>
	T* newOne(_Param1& _p1, _Param2& _p2)
	{
		T* p = new T(_p1, _p2);
		lib.push_back(p);
		return p;
	}
	//将new产生的对象加入对象库中,使用管理器自动管理
	void addOne(T* _x)
	{
		lib.push_back(_x);
	}
	//删除对象,释放内存
	void deleteOne(unsigned _index)
	{
		delete lib[_index];
		lib.erase(lib.begin() + _index);
	}
public:
	FManagerVector(FManagerVector& _x) = delete;
	void operator=(FManagerVector& _x) = delete;
public:
	//对象库
	std::vector<T*> lib;
};
