#pragma once

#include <optional>
#include <type_traits>

#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Mesh/MonotopologyMeshTechnique.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/utilities/Abort.h>

namespace mtt
{
  class BaseGeometryTechnique : public MonotopologyMeshTechnique
  {
  public:
    enum Features
    {
      NORMALS_FEATURE = 1,
      MATERIAL_FEATURE = 2,
      TEX_COORDS_FEATURE = 4,
      SKELETON_FEATURE = 8,
      ALBEDO_SAMPLER_FEATURE = 16,
      OPAQUE_SAMPLER_FEATURE = 32,
      SPECULAR_SAMPLER_FEATURE = 64,
      NORMAL_SAMPLER_FEATURE = 128,
      EMISSION_SAMPLER_FEATURE = 256,
      REFLECTION_SAMPLER_FEATURE = 512,
      VIEW_COORDS_IN_FRAGMENT_SHADER_FEATURE = 1024
    };

    enum PriorityOrder
    {
      NEAR_FIRST_ORDER,
      FAR_FIRST_ORDER,
      CUSTOM_ORDER
    };

  public:
    /// availableFeatures is combination of Features flags
    BaseGeometryTechnique(int availableFeatures,
                          StageIndex stage,
                          VkPrimitiveTopology topology,
                          PriorityOrder priorityOrder);
    BaseGeometryTechnique(const BaseGeometryTechnique&) = delete;
    BaseGeometryTechnique& operator = (const BaseGeometryTechnique&) = delete;
    virtual ~BaseGeometryTechnique() noexcept = default;

    virtual void registerVertexBuffer(Buffer& buffer,
                                      const std::string& name) override;
    virtual void unregisterVertexBuffer(
                                    Buffer& buffer,
                                    const std::string& name) noexcept override;

    virtual void registerUniformBuffer( Buffer& buffer,
                                        const std::string& name) override;
    virtual void unregisterUniformBuffer(
                                    Buffer& buffer,
                                    const std::string& name) noexcept override;

    virtual void registerSampler( Sampler& sampler,
                                  const std::string& name) override;
    virtual void unregisterSampler( Sampler& sampler,
                                    const std::string& name) noexcept override;

    virtual void registerVariable(AbstractMeshVariable& variable,
                                  const std::string& name) override;
    virtual void unregisterVariable(AbstractMeshVariable& variable,
                                    const std::string& name) noexcept override;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

    inline PriorityOrder priorityOrder() const noexcept;

    inline int availableFeatures() const noexcept;
    inline StageIndex stage() const noexcept;
    inline bool normalsEnabled() const noexcept;
    inline bool materialDataEnabled() const noexcept;
    inline bool albedoMappingEnabled() const noexcept;
    inline bool opaqueMappingEnabled() const noexcept;
    inline bool specularMappingEnabled() const noexcept;
    inline bool normalMappingEnabled() const noexcept;
    inline bool emissiveMappingEnabled() const noexcept;
    inline bool reflectionMappingEnabled() const noexcept;
    inline bool texCoordEnabled() const noexcept;
    inline bool skeletonEnabled() const noexcept;

  protected:
    using MatricesUniform = VolatileUniform<DrawMatrices>;
    using BoneMatrices = std::vector<glm::mat4>;
    using BoneMatricesUniform = VolatileUniform<BoneMatrices>;

    inline BoneMatricesUniform* boneMatricesUniform() noexcept;
    inline const BoneMatricesUniform* boneMatricesUniform() const noexcept;

    virtual void invalidatePipeline() noexcept;

    virtual void setIndicesBuffer(Buffer* buffer,
                                  VkIndexType indexType) noexcept override;

    virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                AbstractRenderPass& renderPass);

    virtual float getCustomPriority(
                    const DrawPlanBuildInfo& buildInfo) const noexcept;

    virtual void createRenderAction(DrawPlanBuildInfo& buildInfo,
                                    GraphicsPipeline& pipeline,
                                    MatricesUniform& matricesUniform);

    template <typename... ResourcesArguments>
    inline void createActionTemplate(
                                    DrawPlanBuildInfo& buildInfo,
                                    GraphicsPipeline& pipeline,
                                    MatricesUniform& matricesUniform,
                                    ResourcesArguments&... resourcesArguments);

  private:
    template<typename... Arguments>
    struct RemoveCVFromValue;

    template<typename... Arguments>
    struct CollectActionType;

  private:
    void _rebuildPipeline(AbstractRenderPass& renderPass);

  private:
    int _availableFeatures;
    StageIndex _stage;

    PriorityOrder _priorityOrder;

    std::optional<GraphicsPipeline> _pipeline;
    std::optional<MatricesUniform> _matricesUniform;

    Buffer* _positionsBuffer;
    Buffer* _normalBuffer;
    Buffer* _tangentBuffer;
    Buffer* _binormalBuffer;
    Buffer* _texCoordBuffer;
    Buffer* _boneIndicesBuffer;
    Buffer* _boneWeightsBuffer;

    Buffer* _materialUniformBuffer;

    Sampler* _albedoSampler;
    Sampler* _opaqueSampler;
    Sampler* _specularSampler;
    Sampler* _normalSampler;
    Sampler* _emissiveSampler;
    Sampler* _reflectionSampler;

    int _boneNumber;
    std::optional<BoneMatricesUniform> _boneMatricesUniform;

    Buffer* _boneInverseMatricesUniformBuffer;

    bool _useAlpha;
  };

  inline int BaseGeometryTechnique::availableFeatures() const noexcept
  {
    return _availableFeatures;
  }

  inline StageIndex BaseGeometryTechnique::stage() const noexcept
  {
    return _stage;
  }

  inline bool BaseGeometryTechnique::normalsEnabled() const noexcept
  {
    return  _normalBuffer != nullptr &&
            _availableFeatures & NORMALS_FEATURE;
  }

  inline bool BaseGeometryTechnique::materialDataEnabled() const noexcept
  {
    return _materialUniformBuffer != nullptr;
  }

  inline bool BaseGeometryTechnique::albedoMappingEnabled() const noexcept
  {
    return _albedoSampler != nullptr && _texCoordBuffer != nullptr;
  }

  inline bool BaseGeometryTechnique::opaqueMappingEnabled() const noexcept
  {
    return _opaqueSampler != nullptr && _texCoordBuffer != nullptr;
  }

  inline bool BaseGeometryTechnique::specularMappingEnabled() const noexcept
  {
    return _specularSampler != nullptr && _texCoordBuffer != nullptr;
  }

  inline bool BaseGeometryTechnique::normalMappingEnabled() const noexcept
  {
    return  _normalSampler != nullptr &&
            _texCoordBuffer != nullptr &&
            _tangentBuffer != nullptr &&
            _binormalBuffer != nullptr;
  }

  inline bool BaseGeometryTechnique::emissiveMappingEnabled() const noexcept
  {
    return _emissiveSampler != nullptr && _texCoordBuffer != nullptr;
  }

  inline bool BaseGeometryTechnique::reflectionMappingEnabled() const noexcept
  {
    return _reflectionSampler != nullptr && _normalBuffer != nullptr;
  }

  inline bool BaseGeometryTechnique::texCoordEnabled() const noexcept
  {
    return  albedoMappingEnabled() ||
            opaqueMappingEnabled() ||
            specularMappingEnabled() ||
            normalMappingEnabled() ||
            emissiveMappingEnabled();
  }

  inline bool BaseGeometryTechnique::skeletonEnabled() const noexcept
  {
    return  _availableFeatures & SKELETON_FEATURE &&
            _boneNumber != 0 &&
            _boneIndicesBuffer != nullptr &&
            _boneWeightsBuffer != nullptr &&
            _boneInverseMatricesUniformBuffer != nullptr;
  }

  inline BaseGeometryTechnique::BoneMatricesUniform*
                          BaseGeometryTechnique::boneMatricesUniform() noexcept
  {
    return _boneMatricesUniform.has_value() ? &_boneMatricesUniform.value():
                                              nullptr;
  }

  inline const BaseGeometryTechnique::BoneMatricesUniform*
                    BaseGeometryTechnique::boneMatricesUniform() const noexcept
  {
    return _boneMatricesUniform.has_value() ? &_boneMatricesUniform.value():
                                              nullptr;
  }

  BaseGeometryTechnique::PriorityOrder
                            BaseGeometryTechnique::priorityOrder() const noexcept
  {
    return _priorityOrder;
  }

  template< typename ResourceType,
            typename ResourceValueType,
            typename... OtherResourceArguments>
  struct BaseGeometryTechnique::RemoveCVFromValue<ResourceType,
                                                  ResourceValueType,
                                                  OtherResourceArguments...>
  {
    using Resource = ResourceType;
    using Value = std::remove_cv<ResourceValueType>::type;
    using NextRemove = RemoveCVFromValue<OtherResourceArguments...>;
  };

  template<typename... ParentResourceArguments>
  struct BaseGeometryTechnique::CollectActionType<
                                    BaseGeometryTechnique::RemoveCVFromValue<>,
                                    ParentResourceArguments...>
  {
    using ActionType = DrawMeshAction<ParentResourceArguments...>;
  };

  template<typename RemoveType, typename... ParentResourceTypes>
  struct BaseGeometryTechnique::CollectActionType<RemoveType,
                                                  ParentResourceTypes...>
  {
    using NextRemove = RemoveType::NextRemove;
    using Resource = RemoveType::Resource;
    using Value = RemoveType::Value;
    using Next = CollectActionType< NextRemove,
                                    ParentResourceTypes...,
                                    Resource,
                                    Value>;
    using ActionType = Next::ActionType;
  };

  template <typename... ResourcesArguments>
  inline void BaseGeometryTechnique::createActionTemplate(
                                      DrawPlanBuildInfo& buildInfo,
                                      GraphicsPipeline& pipeline,
                                      MatricesUniform& matricesUniform,
                                      ResourcesArguments&... resourcesArguments)
  {
    const BoneMatrices* boneMatrices = static_cast<const BoneMatrices*>(
          buildInfo.extraData.data(DrawPlanBuildExtraData::boneMatricesIndex));
    if (skeletonEnabled())
    {
      if (boneMatrices == nullptr)
      {
        Log() << "BaseGeometryTechnique::addToDrawPlan: boneMatrices is null.";
        return;
      }
      if (_boneNumber != boneMatrices->size())
      {
        Log() << "BaseGeometryTechnique::addToDrawPlan: wrong number of bone matrices.";
        return;
      }
    }

    uint32_t vertexCount = vertexCountToDraw();

    float priority;
    if(_priorityOrder == CUSTOM_ORDER) priority = getCustomPriority(buildInfo);
    else if (_priorityOrder == NEAR_FIRST_ORDER)
    {
      priority = buildInfo.getPriorityNearFirstOrder(glm::vec3(0));
    }
    else
    {
      priority = buildInfo.getPriorityFarFirstOrder(glm::vec3(0));
    }
    
    DrawBin* renderBin = buildInfo.currentFramePlan->getBin(_stage);
    if (renderBin == nullptr) Abort("BaseGeometryTechnique::createActionTemplate: render bin is not supported.");

    if(skeletonEnabled())
    {
      using DrawAction =
                    CollectActionType<RemoveCVFromValue<ResourcesArguments...>,
                                  VolatileUniform<DrawMatrices>,
                                  DrawMatrices,
                                  VolatileUniform<BoneMatrices>,
                                  BoneMatrices>::ActionType;
      renderBin->createAction<DrawAction>(priority,
                                          pipeline,
                                          buildInfo.viewport,
                                          buildInfo.scissor,
                                          vertexCount,
                                          matricesUniform,
                                          buildInfo.drawMatrices,
                                          *boneMatricesUniform(),
                                          *boneMatrices,
                                          resourcesArguments...);
    }
    else
    {
      using DrawAction =
                    CollectActionType<RemoveCVFromValue<ResourcesArguments...>,
                                      VolatileUniform<DrawMatrices>,
                                      DrawMatrices>::ActionType;
      renderBin->createAction<DrawAction>(priority,
                                          pipeline,
                                          buildInfo.viewport,
                                          buildInfo.scissor,
                                          vertexCount,
                                          matricesUniform,
                                          buildInfo.drawMatrices,
                                          resourcesArguments...);
    }
  }
}