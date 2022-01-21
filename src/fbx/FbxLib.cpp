#include <mtt/fbx/FbxLib.h>

using namespace mtt;

FbxLib* FbxLib::_instance = nullptr;

FbxLib::FbxLib() :
  _fbxIOSettings(&fbxManager(), IOSROOT)
{
  if(_instance != nullptr) Abort("FbxLib::FbxLib: FbxLib is already created");
  _instance = this;
  fbxManager().SetIOSettings(&_fbxIOSettings());
}
