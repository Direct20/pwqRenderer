#pragma once
#include "misc/def0.h"
//////////////////////////////////////////////////////////////////////////////
//����F����ʱ,ֻҪOpenGL�������ѳ�ʼ��,F����ͻ��Զ���ʼ��,��ʼ���������εĳ���.
//_autorelease=trueʱ,��������ʱ�Զ��ͷ���Դ.Ĭ��Ϊtrue;
//�Զ��ͷŶ������
F_REPLICABLE class FBase
{
protected:
	//��֧�ֵ���ʵ����
	FBase()
	{

	}
	//����delete����ʱͬʱ��������
	virtual ~FBase()
	{

	}
public:
	//�Ƿ��Զ��ͷ�
	//�ṩ��OpenGL����Ķ���Ŀ���֧��
	//������ʱ������ʽ���������������ʽ�����Ӧ��OpenGL������ͷ�.
	bool _autorelease = true;
};
//////////////////////////////////////////////////////////////////////////////
//�ڴ����ר�ýṹ
template<typename T>
struct FMem4
{
	T x;
	T y;
	T z;
	T w;
};

//////////////////////////////////////////////////////////////////////////////
//[ģ��]
//֡����������(FBO)
//class FFramebuffer :public FBase
//{
//public:
	//��δ��ʼ��ʱ�˹��캯����ִ�г�ʼ������,���ֶ�����_init.
//FFramebuffer()
//{
//	if (g_isInited)_init();
//}
//!_autoreleaseʱ������_destroy
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
// //�󶨵�OpenGL������
// void _bind()
// {
// }
// //��OpengL������ȡ����
// void _unbind()
// {
// }
// public:
//FFramebuffer(FFramebuffer& _x) = delete;
//void operator=(FFramebuffer& _x) = delete;
//public:
//	//֡���������(����ֻ��)
//	GLuint h = 0;
//};