#pragma once
#include <queue>
template <typename T>
class FManagerQueue
{
public:
	FManagerQueue() {}
	~FManagerQueue()
	{
		while (!q.empty())
		{
			delete q.front();
			q.pop();
		}
	}
public:
	//调用默认构造参数new一个对象
	T* newOne()
	{
		T* p = new T();
		q.push(p);
		return p;
	}
	//调用单参数构造函数new一个对象
	template<typename _Param>
	T* newOne(_Param& _p)
	{
		T* p = new T(_p);
		q.push(p);
		return p;
	}
	//调用双参数构造函数new一个对象
	template<typename _Param1, typename _Param2>
	T* newOne(_Param1& _p1, _Param2& _p2)
	{
		T* p = new T(_p1, _p2);
		q.push(p);
		return p;
	}
	//将new产生的对象加入对象队列中,使用管理器自动管理
	void pushOne(T* _x)
	{
		q.push(_x);
	}
	//删除对象,释放内存
	void popOne()
	{
		delete q.front();
		q.pop();
	}
public:
	FManagerQueue(FManagerQueue& _x) = delete;
	void operator=(FManagerQueue& _x) = delete;
public:
	//对象库
	std::queue<T*> q;
};
