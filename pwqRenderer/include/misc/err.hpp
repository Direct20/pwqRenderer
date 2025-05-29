//////////////////////////////////////////////////////////////////////////////
// err.h - �쳣����
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <ctime>
#ifndef _FPS_ENGING_ERR_
#define _FPS_ENGING_ERR_
//�쳣��Ϣ��
struct ex
{
public:
	ex() { }
	ex(const std::string& _d)
	{
		c = 0;
		d = _d;
	}
	ex(int _c, const std::string& _d)
	{
		c = _c;
		d = _d;
	}
public:
	//����
	int c = 0;
	//����
	std::string d = "";
};
#define F_EXCEPTION_ERR_FORMAT_STRING	">>>ERROR<<<\n[Code] %X\n[Info]\n%s\n[Time] %s\n"
#define F_EXCEPTION_WAR_FORMAT_STRING   "WARNING(%X): %s\n"
#define F_MAX_TIME_STRING			100
//��ʾ������Ϣ
inline void err_excep(int _c, const char* _d)
{
	time_t rawtime;
	struct tm info;
	char buffer[F_MAX_TIME_STRING];
	//��ȡʱ���ַ���:
	time(&rawtime);
	localtime_s(&info, &rawtime);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &info);
	//��ʾ�쳣:
	fprintf(stderr, F_EXCEPTION_ERR_FORMAT_STRING, _c, _d, buffer);
}
inline void err_excep(const ex& _e)
{
	err_excep(_e.c, _e.d.c_str());
}
//��ʾ������Ϣ
inline void war_excep(int _c, const char* _d)
{
	fprintf(stderr, F_EXCEPTION_WAR_FORMAT_STRING, _c, _d);
}
inline void war_excep(const ex& _e)
{
	war_excep(_e.c, _e.d.c_str());
}

#endif //_FPS_ENGING_ERR_