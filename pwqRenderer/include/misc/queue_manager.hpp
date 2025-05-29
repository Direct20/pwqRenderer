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
	//����Ĭ�Ϲ������newһ������
	T* newOne()
	{
		T* p = new T();
		q.push(p);
		return p;
	}
	//���õ��������캯��newһ������
	template<typename _Param>
	T* newOne(_Param& _p)
	{
		T* p = new T(_p);
		q.push(p);
		return p;
	}
	//����˫�������캯��newһ������
	template<typename _Param1, typename _Param2>
	T* newOne(_Param1& _p1, _Param2& _p2)
	{
		T* p = new T(_p1, _p2);
		q.push(p);
		return p;
	}
	//��new�����Ķ��������������,ʹ�ù������Զ�����
	void pushOne(T* _x)
	{
		q.push(_x);
	}
	//ɾ������,�ͷ��ڴ�
	void popOne()
	{
		delete q.front();
		q.pop();
	}
public:
	FManagerQueue(FManagerQueue& _x) = delete;
	void operator=(FManagerQueue& _x) = delete;
public:
	//�����
	std::queue<T*> q;
};
