#pragma once

#include <memory>

#include <mtt/utilities/Abort.h>

namespace mtt
{
  template <typename FbxObject>
  class FbxDelete
  {
  public:
    FbxDelete() = default;
    FbxDelete(const FbxDelete&) = default;
    FbxDelete& operator = (const FbxDelete&) = default;
    ~FbxDelete() = default;

    inline void operator() (FbxObject* fbxObject) const noexcept;
  };
  
  template <typename FbxObject>
  using FbxPtr = std::unique_ptr<FbxObject, FbxDelete<FbxObject>>;

  template <typename FbxObject>
  class FbxContainer
  {
  public:
    template <typename... Args>
    inline explicit FbxContainer(Args&&... args);
    FbxContainer(const FbxContainer&) = delete;
    FbxContainer& operator = (const FbxContainer&) = delete;
    inline ~FbxContainer() noexcept = default;

    inline FbxObject& operator()() noexcept;
    inline const FbxObject& operator()() const noexcept;

  private:
    FbxPtr<FbxObject> _fbxObject;
  };

  template <typename FbxObject>
  inline void FbxDelete<FbxObject>::operator() (
                                            FbxObject* fbxObject) const noexcept
  {
    try
    {
      fbxObject->Destroy();
    }
    catch (...)
    {
      Abort("FbxDelete: unable to destroy fbx object.");
    }
  }

  template <typename FbxObject>
  template <typename... Args>
  inline FbxContainer<FbxObject>::FbxContainer(Args&&... args) :
    _fbxObject(FbxObject::Create(args...))
  {
  }

  template <typename FbxObject>
  inline FbxObject& FbxContainer<FbxObject>::operator()() noexcept
  {
    return *_fbxObject;
  }

  template <typename FbxObject>
  inline const FbxObject& FbxContainer<FbxObject>::operator()() const noexcept
  {
    return *_fbxObject;
  }
}