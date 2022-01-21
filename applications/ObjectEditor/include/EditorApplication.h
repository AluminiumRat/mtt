#pragma once

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/application/ResourceManager/CubeTextureLibrary.h>
#include <mtt/application/ResourceManager/FbxModelLibrary.h>
#include <mtt/application/ResourceManager/MMDModelLibrary.h>
#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/application/Application.h>
#include <mtt/fbx/FbxLib.h>
#include <mtt/utilities/Abort.h>

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
  mtt::MMDModelLibrary mmdModelLibrary;
  mtt::FbxModelLibrary fbxModelLibrary;

private:
  static EditorApplication* _instance;
  mtt::FbxLib _fbxLib;
};

inline EditorApplication& EditorApplication::instance() noexcept
{
  if(_instance == nullptr) mtt::Abort("EditorApplication::Instance: application is not crated");
  return *_instance;
}
