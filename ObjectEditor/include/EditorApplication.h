#pragma once

#include <mtt/Core/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/Core/ResourceManager/CubeTextureLibrary.h>
#include <mtt/Core/ResourceManager/Texture2DLibrary.h>
#include <mtt/Core/Appliction.h>
#include <mtt/Fbx/FbxLib.h>
#include <mtt/Utilities/Abort.h>

class EditorApplication : public mtt::Application
{
public:
  EditorApplication(int& argc, char** argv);
  EditorApplication(const EditorApplication&) = delete;
  EditorApplication& operator = (const EditorApplication&) = delete;
  virtual ~EditorApplication() noexcept = default;

  inline static EditorApplication& instance() noexcept;

public:
  mtt::AsyncTaskQueue asyncTaskQueue;
  mtt::Texture2DLibrary textureLibrary;
  mtt::CubeTextureLibrary cubeTextureLibrary;

private:
  static EditorApplication* _instance;
  mtt::FbxLib _fbxLib;
};

inline EditorApplication& EditorApplication::instance() noexcept
{
  if(_instance == nullptr) mtt::Abort("EditorApplication::Instance: application is not crated");
  return *_instance;
}
