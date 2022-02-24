#pragma once

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/application/ResourceManager/CubeTextureLibrary.h>
#include <mtt/application/ResourceManager/FbxModelLibrary.h>
#include <mtt/application/ResourceManager/MMDModelLibrary.h>
#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/application/Application.h>
#include <mtt/fbx/FbxLib.h>
#include <mtt/utilities/Abort.h>

namespace mtt
{
  class EditorApplication : public Application
  {
  public:
    EditorApplication(int& argc,
                      char** argv,
                      const char* applicationName,
                      const RenderLibInstance::Version& applicationVersion);
    EditorApplication(const EditorApplication&) = delete;
    EditorApplication& operator = (const EditorApplication&) = delete;
    virtual ~EditorApplication() noexcept = default;

    inline static EditorApplication& instance() noexcept;

  public:
    AsyncTaskQueue asyncTaskQueue;
    Texture2DLibrary textureLibrary;
    CubeTextureLibrary cubeTextureLibrary;
    MMDModelLibrary mmdModelLibrary;
    FbxModelLibrary fbxModelLibrary;

  private:
    static EditorApplication* _instance;
    FbxLib _fbxLib;
  };

  inline EditorApplication& EditorApplication::instance() noexcept
  {
    if(_instance == nullptr) Abort("EditorApplication::Instance: application is not crated");
    return *_instance;
  }
}