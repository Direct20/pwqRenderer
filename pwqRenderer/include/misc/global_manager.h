#pragma once
#include "misc/vector_manager.hpp"


class FGlobalManager
{
private:
	FGlobalManager()
	{

	}
	FGlobalManager(const FGlobalManager&) = delete;
	void operator=(const FGlobalManager& ) = delete;
public:
	~FGlobalManager()
	{
		if (_inst)
			delete _inst;
	}
	static FGlobalManager& get_instance()
	{
		if (!_inst)
			_inst = new FGlobalManager();
		return *_inst;
	}
public:
	static bool g_isInited;
	//纹理管理器
	static FManagerVector<class FTexture2D> g_mgrTexture;
	//图像管理器
	static FManagerVector<class FImage> g_mgrImage;
protected:
	static FGlobalManager* _inst;
};

