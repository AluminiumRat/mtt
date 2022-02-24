#pragma once

#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

class ParticleField;

class FieldRenderObserver : public mtt::Object3DRenderObserver
{
  Q_OBJECT

  public:
    FieldRenderObserver(ParticleField& object, mtt::CommonEditData& commonData);
    FieldRenderObserver(const FieldRenderObserver&) = delete;
    FieldRenderObserver& operator = (const FieldRenderObserver&) = delete;
    virtual ~FieldRenderObserver() noexcept = default;

  private:
    void _updateBox() noexcept;

  private:
    ParticleField& _field;

    mtt::Mesh _boxMesh;
    mtt::SimpleDrawableNode _boxNode;
};