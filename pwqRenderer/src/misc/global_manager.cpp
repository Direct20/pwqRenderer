#include "misc/global_manager.h"
#include "texture/texture2d.h"
#include "image/image.h"

FGlobalManager* FGlobalManager::_inst = nullptr;
bool FGlobalManager::g_isInited = false;
FManagerVector<class FTexture2D> FGlobalManager::g_mgrTexture;
FManagerVector<class FImage> FGlobalManager::g_mgrImage;
