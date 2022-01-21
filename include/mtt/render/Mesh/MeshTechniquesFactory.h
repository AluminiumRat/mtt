#pragma once

namespace mtt
{
  class Mesh;

  class MeshTechniquesFactory
  {
  public:
    MeshTechniquesFactory() = default;
    MeshTechniquesFactory(const MeshTechniquesFactory&) = delete;
    MeshTechniquesFactory& operator = (const MeshTechniquesFactory&) = delete;
    virtual ~MeshTechniquesFactory() noexcept = default;

    virtual void setupTechniques(Mesh& mesh) = 0;
  };
}