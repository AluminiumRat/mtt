#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <mtt/Render/Mesh/MeshVariable.h>
#include <mtt/Render/Pipeline/Buffer.h>
#include <mtt/Render/Pipeline/Sampler.h>
#include <mtt/Utilities/SurfaceMaterialData.h>

namespace mtt
{
  class AbstractMeshTechnique;
  class LogicalDevice;

  class MeshExtraData
  {
  public:
    using SurfaceMaterialVariableType = MeshVariable<SurfaceMaterialData>;
    inline static const char surfaceMaterialUniformName[] =
                                                      "surfaceMaterialUniform";
    inline static const char surfaceMaterialUniformBinding[] =
                                                "surfaceMaterialUniformBinding";
    inline static const char surfaceMaterialVariableName[] =
                                                      "surfaceMaterialVariable";

    inline static const char alphaInAlbedoSamplerIsOpacityVariableName[] =
                                                "alphaInAlbedoSamplerIsOpacity";

    inline static const char boneInverseMatricesUniformName[] =
                                                  "boneInverseMatricesUniform";
    inline static const char boneInverseMatricesUniformBinding[] =
                                                  "boneInverseMatricesBinding";

    inline static const char albedoSamplerName[] = "albedoTexture";
    inline static const char albedoSamplerBinding[] = "albedoTextureBinding";

    inline static const char opaqueSamplerName[] = "opaqueTexture";
    inline static const char opaqueSamplerBinding[] = "opaqueTextureBinding";

    inline static const char specularSamplerName[] = "specularTexture";
    inline static const char specularSamplerBinding[] =
                                                      "specularTextureBinding";

    inline static const char normalSamplerName[] = "normalTexture";
    inline static const char normalSamplerBinding[] = "normalTextureBinding";

    inline static const char emissionSamplerName[] = "emissionTexture";
    inline static const char emissionSamplerBinding[] =
                                                      "emissionTextureBinding";

    inline static const char reflectionSamplerName[] = "reflectionTexture";
    inline static const char reflectionSamplerBinding[] =
                                                    "reflectionTextureBinding";

    inline static const char boneNumberVariableName[] = "boneNumberVariable";
    using BoneNumberVariableType = MeshIntVariable;
    inline static const char boneNumberVariableDefine[] = "BONE_NUMBER";

    inline static const char boneMatricesVariableName[] =
                                                        "boneMatricesVariable";
    using BoneMatricesVariableType =
                                  MeshVariable<const std::vector<glm::mat4>*>;
    inline static const char boneMatricesUniformBinding[] =
                                                          "boneMatricesBinding";

  public:
    MeshExtraData(LogicalDevice& device);
    MeshExtraData(const MeshExtraData&) = delete;
    MeshExtraData& operator = (const MeshExtraData&) = delete;
    virtual ~MeshExtraData() noexcept;

    inline LogicalDevice& device() const noexcept;

    inline size_t uniformBuffersNumber() const noexcept;
    inline Buffer& uniformBuffer(size_t index) noexcept;
    inline const Buffer& uniformBuffer(size_t index) const noexcept;
    inline const Buffer* uniformBuffer(const std::string& name) const noexcept;
    inline Buffer* uniformBuffer(const std::string& name) noexcept;
    inline const std::string& uniformBufferName(size_t index) const noexcept;
    void addUniformBuffer(std::unique_ptr<Buffer> buffer,
                          const std::string& name);
    /// Returns removed buffer
    std::unique_ptr<Buffer> removeUniformBuffer(size_t index) noexcept;
    /// Returns removed buffer
    std::unique_ptr<Buffer> removeUniformBuffer(
                                              const std::string& name) noexcept;

    void setSurfaceMaterialData(const SurfaceMaterialData& newData);
    void removeSurfaceMaterialData() noexcept;

    inline size_t samplerNumber() const noexcept;
    inline Sampler& sampler(size_t index) noexcept;
    inline const Sampler& sampler(size_t index) const noexcept;
    inline const std::string& samplerName(size_t index) const noexcept;
    void addSampler(std::unique_ptr<Sampler> sampler,
                    const std::string& name);
    /// Returns removed sampler
    std::unique_ptr<Sampler> removeSampler(size_t index) noexcept;
    /// Returns removed sampler
    std::unique_ptr<Sampler> removeSampler(const std::string& name) noexcept;

    void setAlbedoSampler(std::unique_ptr<Sampler> sampler);
    void removeAlbedoSampler() noexcept;

    void setOpaqueSampler(std::unique_ptr<Sampler> sampler);
    void removeOpaqueSampler() noexcept;

    void setSpecularSampler(std::unique_ptr<Sampler> sampler);
    void removeSpecularSampler() noexcept;

    void setNormalSampler(std::unique_ptr<Sampler> sampler);
    void removeNormalSampler() noexcept;

    void setEmissionSampler(std::unique_ptr<Sampler> sampler);
    void removeEmissionSampler() noexcept;

    void setReflectionSampler(std::unique_ptr<Sampler> sampler);
    void removeReflectionSampler() noexcept;

    inline size_t variableNumber() const noexcept;
    inline AbstractMeshVariable& variable(size_t index) noexcept;
    inline const AbstractMeshVariable& variable(size_t index) const noexcept;
    inline const std::string& variableName(size_t index) const noexcept;
    void addVariable(std::unique_ptr<AbstractMeshVariable> variable,
                     const std::string& name);
    /// Returns removed variable
    std::unique_ptr<AbstractMeshVariable> removeVariable(size_t index) noexcept;
    /// Returns removed variable
    std::unique_ptr<AbstractMeshVariable> removeVariable(
                                              const std::string& name) noexcept;

    void setBoneInverseMatricesData(const std::vector<glm::mat4>& matrices);
    void removeBoneInverseMatricesData() noexcept;

    void setBoneMatricesData(const std::vector<glm::mat4>* matrices);
    void removeBoneMatricesData() noexcept;

    void setAlphaInAlbedoSamplerIsOpacity(bool value);
    void removeAlphaInAlbedoSamplerIsOpacityVariable() noexcept;

  protected:
    friend class AbstractMeshTechnique;
    virtual void registerTechnique(AbstractMeshTechnique& technique);
    virtual void unRegisterTechnique(AbstractMeshTechnique& technique) noexcept;

  private:
    LogicalDevice& _device;

    struct BufferRecord
    {
      std::string name;
      Buffer* buffer;
    };
    using Uniforms = std::vector<BufferRecord>;
    Uniforms _uniforms;

    struct SamplerRecord
    {
      std::string name;
      Sampler* sampler;
    };
    using Samplers = std::vector<SamplerRecord>;
    Samplers _samplers;

    struct VariableRecord
    {
      std::string name;
      AbstractMeshVariable* variable;
    };
    using Variables = std::vector<VariableRecord>;
    Variables _variables;

    using Techniques = std::vector<AbstractMeshTechnique*>;
    Techniques _techniques;
  };

  inline LogicalDevice& MeshExtraData::device() const noexcept
  {
    return _device;
  }

  inline size_t MeshExtraData::uniformBuffersNumber() const noexcept
  {
    return _uniforms.size();
  }

  inline Buffer& MeshExtraData::uniformBuffer(size_t index) noexcept
  {
    return *_uniforms[index].buffer;
  }

  inline const Buffer& MeshExtraData::uniformBuffer(size_t index) const noexcept
  {
    return *_uniforms[index].buffer;
  }

  inline const Buffer* MeshExtraData::uniformBuffer(
                                        const std::string& name) const noexcept
  {
    for(const BufferRecord& record : _uniforms)
    {
      if(record.name == name) return record.buffer;
    }
    return nullptr;
  }

  inline Buffer* MeshExtraData::uniformBuffer(const std::string& name) noexcept
  {
    for (BufferRecord& record : _uniforms)
    {
      if (record.name == name) return record.buffer;
    }
    return nullptr;
  }

  inline const std::string& MeshExtraData::uniformBufferName(
                                                    size_t index) const noexcept
  {
    return _uniforms[index].name;
  }

  inline size_t MeshExtraData::samplerNumber() const noexcept
  {
    return _samplers.size();
  }

  inline Sampler& MeshExtraData::sampler(size_t index) noexcept
  {
    return *_samplers[index].sampler;
  }

  inline const Sampler& MeshExtraData::sampler(size_t index) const noexcept
  {
    return *_samplers[index].sampler;
  }

  inline const std::string& MeshExtraData::samplerName(
                                                    size_t index) const noexcept
  {
    return _samplers[index].name;
  }

  inline size_t MeshExtraData::variableNumber() const noexcept
  {
    return _variables.size();
  }

  inline AbstractMeshVariable& MeshExtraData::variable(size_t index) noexcept
  {
    return *_variables[index].variable;
  }

  inline const AbstractMeshVariable& MeshExtraData::variable(
                                                    size_t index) const noexcept
  {
    return *_variables[index].variable;
  }

  inline const std::string& MeshExtraData::variableName(
                                                    size_t index) const noexcept
  {
    return _variables[index].name;
  }
}