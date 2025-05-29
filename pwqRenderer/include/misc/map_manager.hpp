#pragma once
#include <map>
template <typename Key, typename Val>
class FManagerMap
{
public:
	FManagerMap() {}
	~FManagerMap()
	{
		for (auto iter = m.begin(); iter != m.end(); ++iter)
			delete iter->second;
	}
public:
	//@brief 调用默认构造参数new一个对象
	//@return _key已经存在时返回nullptr.
	Val* newOne(Key _key)
	{
		if (m.count(_key))return nullptr;
		Val* p = new Val();
		m.emplace(_key, p);
		return p;
	}
	//@brief 调用单参数构造函数new一个对象
	//@return _key已经存在时返回nullptr.
	template<typename _Param>
	Val* newOne(Key _key, _Param& _p)
	{
		if (m.count(_key))return nullptr;
		Val* p = new Val(_p);
		m.emplace(_key, p);
		return p;
	}
	//@brief 调用双参数构造函数new一个对象
	//@return _key已经存在时返回nullptr.
	template<typename _Param1, typename _Param2>
	Val* newOne(Key _key, _Param1& _p1, _Param2& _p2)
	{
		if (m.count(_key))return nullptr;
		Val* p = new Val(_p1, _p2);
		m.emplace(_key, p);
		return p;
	}
	//@brief 将new产生的对象加入对象容器中,使用管理器自动管理
	//@return 加入成功返回true
	bool addOne(Key _key, Val* _val)
	{
		return m.emplace(_key, _val).first;
	}
	//删除对象,释放内存
	void deleteOne(Key _key)
	{
		auto iter = m.find(_key);
		if (iter != m.end())
		{
			delete m[_key];
			m.erase(iter);
		}
	}
public:
	FManagerMap(FManagerMap& _x) = delete;
	void operator=(FManagerMap& _x) = delete;
public:
	//对象库
	std::map<Key, Val*> m;
};