#pragma once

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace mtt
{
  class AbstractMeshVariable
  {
  public:
    AbstractMeshVariable() = default;
    AbstractMeshVariable(const AbstractMeshVariable&) = delete;
    AbstractMeshVariable& operator = (const AbstractMeshVariable&) = delete;
    virtual ~AbstractMeshVariable() noexcept = default;
  };

  template <typename DataType>
  class MeshVariable : public AbstractMeshVariable
  {
  public:
    using ValueType = DataType;

  public:
    template<typename ... Args>
    MeshVariable(Args&&... args);
    MeshVariable(const MeshVariable&) = delete;
    MeshVariable& operator = (const MeshVariable&) = delete;
    virtual ~MeshVariable() noexcept = default;

    inline const DataType& value() const;

  private:
    DataType _data;
  };

  using MeshBoolVariable = MeshVariable<bool>;
  using MeshIntVariable = MeshVariable<int>;
  using MeshFloatVariable = MeshVariable<float>;
  using MeshVec2Variable = MeshVariable<glm::vec2>;
  using MeshVec3Variable = MeshVariable<glm::vec3>;
  using MeshVec4Variable = MeshVariable<glm::vec4>;
  using MeshStringVariable = MeshVariable<std::string>;

  template <typename DataType>
  template<typename ... Args>
  MeshVariable<DataType>::MeshVariable(Args&&... args) :
    _data(args...)
  {
  }

  template <typename DataType>
  inline const DataType& MeshVariable<DataType>::value() const
  {
    return _data;
  }
};