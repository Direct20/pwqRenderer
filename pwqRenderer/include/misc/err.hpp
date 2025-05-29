//////////////////////////////////////////////////////////////////////////////
// err.h - 异常处理
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <ctime>
#ifndef _FPS_ENGING_ERR_
#define _FPS_ENGING_ERR_
//异常信息类
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
	//代码
	int c = 0;
	//描述
	std::string d = "";
};
#define F_EXCEPTION_ERR_FORMAT_STRING	">>>ERROR<<<\n[Code] %X\n[Info]\n%s\n[Time] %s\n"
#define F_EXCEPTION_WAR_FORMAT_STRING   "WARNING(%X): %s\n"
#define F_MAX_TIME_STRING			100
//显示错误信息
inline void err_excep(int _c, const char* _d)
{
	time_t rawtime;
	struct tm info;
	char buffer[F_MAX_TIME_STRING];
	//获取时间字符串:
	time(&rawtime);
	localtime_s(&info, &rawtime);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &info);
	//显示异常:
	fprintf(stderr, F_EXCEPTION_ERR_FORMAT_STRING, _c, _d, buffer);
}
inline void err_excep(const ex& _e)
{
	err_excep(_e.c, _e.d.c_str());
}
//显示警告信息
inline void war_excep(int _c, const char* _d)
{
	fprintf(stderr, F_EXCEPTION_WAR_FORMAT_STRING, _c, _d);
}
inline void war_excep(const ex& _e)
{
	war_excep(_e.c, _e.d.c_str());
}

#endif //_FPS_ENGING_ERR_