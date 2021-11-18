#pragma once

#include <fbxsdk.h>

#include <mtt/Fbx/FbxRAII.h>
#include <mtt/Utilities/Abort.h>

namespace mtt
{
  class FbxLib
  {
  public:
    FbxLib();
    FbxLib(const FbxLib&) = delete;
    FbxLib& operator = (const FbxLib&) = delete;
    virtual ~FbxLib() noexcept = default;

    inline static FbxLib& instance() noexcept;

    FbxContainer<FbxManager> fbxManager;

  private:
    static FbxLib* _instance;
    FbxContainer<FbxIOSettings> _fbxIOSettings;
  };

  inline FbxLib& FbxLib::instance() noexcept
  {
    if(_instance == nullptr) Abort("FbxLib::Instance: fbxlib is not crated");
    return *_instance;
  }
}