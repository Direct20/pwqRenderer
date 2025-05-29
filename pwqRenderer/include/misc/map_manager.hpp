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
	//@brief ����Ĭ�Ϲ������newһ������
	//@return _key�Ѿ�����ʱ����nullptr.
	Val* newOne(Key _key)
	{
		if (m.count(_key))return nullptr;
		Val* p = new Val();
		m.emplace(_key, p);
		return p;
	}
	//@brief ���õ��������캯��newһ������
	//@return _key�Ѿ�����ʱ����nullptr.
	template<typename _Param>
	Val* newOne(Key _key, _Param& _p)
	{
		if (m.count(_key))return nullptr;
		Val* p = new Val(_p);
		m.emplace(_key, p);
		return p;
	}
	//@brief ����˫�������캯��newһ������
	//@return _key�Ѿ�����ʱ����nullptr.
	template<typename _Param1, typename _Param2>
	Val* newOne(Key _key, _Param1& _p1, _Param2& _p2)
	{
		if (m.count(_key))return nullptr;
		Val* p = new Val(_p1, _p2);
		m.emplace(_key, p);
		return p;
	}
	//@brief ��new�����Ķ���������������,ʹ�ù������Զ�����
	//@return ����ɹ�����true
	bool addOne(Key _key, Val* _val)
	{
		return m.emplace(_key, _val).first;
	}
	//ɾ������,�ͷ��ڴ�
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
	//�����
	std::map<Key, Val*> m;
};