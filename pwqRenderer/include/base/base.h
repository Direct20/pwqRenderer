#pragma once
#include "misc/def0.h"
//////////////////////////////////////////////////////////////////////////////
//定义F对象时,只要OpenGL上下文已初始化,F对象就会自动初始化,初始化函数带参的除外.
//_autorelease=true时,对象析构时自动释放资源.默认为true;
//自动释放对象基类
F_REPLICABLE class FBase
{
protected:
	//不支持单独实例化
	FBase()
	{

	}
	//调用delete析构时同时析构子类
	virtual ~FBase()
	{

	}
public:
	//是否自动释放
	//提供含OpenGL句柄的对象的拷贝支持
	//避免临时对象被正式储存后的析构造成正式对象对应的OpenGL句柄被释放.
	bool _autorelease = true;
};
//////////////////////////////////////////////////////////////////////////////
//内存对齐专用结构
template<typename T>
struct FMem4
{
	T x;
	T y;
	T z;
	T w;
};

//////////////////////////////////////////////////////////////////////////////
//[模板]
//帧缓冲区对象(FBO)
//class FFramebuffer :public FBase
//{
//public:
	//库未初始化时此构造函数不执行初始化操作,须手动调用_init.
//FFramebuffer()
//{
//	if (g_isInited)_init();
//}
//!_autorelease时不调用_destroy
//	~FFramebuffer()
//	{
//		if (_autorelease)_destroy();
//	}
//public:
//	void _init()
//	{
//		if (!g_isInited)throw ex("OpenGL hasn't been initialized.");
//		if (!h)glGenFramebuffers(1, &h);
//		else war_excep(0, "The framebuffer object had been inited.");
//	}
//	void _destroy()
//	{
//		if (h)glDeleteFramebuffers(1, &h);
//		h = 0;
//	}
// //绑定到OpenGL上下文
// void _bind()
// {
// }
// //从OpengL上下文取消绑定
// void _unbind()
// {
// }
// public:
//FFramebuffer(FFramebuffer& _x) = delete;
//void operator=(FFramebuffer& _x) = delete;
//public:
//	//帧缓冲区句柄(对外只读)
//	GLuint h = 0;
//};