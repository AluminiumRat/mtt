#pragma once

#include <mtt/Render/DrawPlan/StageIndex.h>
#include <mtt/Render/Pipeline/AbstractPipeline.h>
#include <mtt/Render/Pipeline/VertexAttribute.h>
#include <mtt/Render/Pipeline/VertexIndices.h>
#include <mtt/Render/RenderPass/AbstractRenderPass.h>

namespace mtt
{
  class GraphicsPipeline : public AbstractPipeline
  {
  public:
    GraphicsPipeline( AbstractRenderPass& renderPass,
                      const AbstractRenderPass::SubpassInfo& subpassInfo);
    GraphicsPipeline( AbstractRenderPass& renderPass,
                      StageIndex stage);
    GraphicsPipeline(const GraphicsPipeline&) = delete;
    GraphicsPipeline& operator = (const GraphicsPipeline&) = delete;
    virtual ~GraphicsPipeline()  noexcept = default;

    inline bool isCompatible(const AbstractRenderPass& pass) const noexcept;

    virtual void addShader(std::unique_ptr<ShaderModule> shaderModule) override;
    virtual std::unique_ptr<ShaderModule>
                    removeShader(ShaderModule& shaderModule) noexcept override;

    inline VkPrimitiveTopology topology() const noexcept;
    inline void setTopology(VkPrimitiveTopology newTopology);

    inline bool dynamicScissorViewport() const noexcept;
    inline void setDynamicScissorViewport(bool newValue);

    inline const VkViewport& viewport() const noexcept;
    inline void setViewport(const VkViewport& newViewport);

    inline const VkRect2D& scissor() const noexcept;
    inline void setScissor(const VkRect2D& newScissor);

    inline bool rasterizerDiscardEnable() const noexcept;
    inline void setRasterizerDiscardEnable(bool newValue);
  
    inline VkPolygonMode polygonMode() const noexcept;
    inline void setPolygonMode(VkPolygonMode newValue);

    inline float lineWidth() const noexcept;
    inline void setLineWidth(float newValue);

    inline VkCullModeFlags cullMode() const noexcept;
    inline void setCullMode(VkCullModeFlags newValue);

    inline VkFrontFace frontFace() const noexcept;
    inline void setFrontFace(VkFrontFace newValue);

    inline bool depthTestEnable() const noexcept;
    inline void setDepthTestEnable(bool newValue);

    inline bool depthWriteEnable() const noexcept;
    inline void setDepthWriteEnable(bool newValue);

    inline VkCompareOp depthCompareOp() const noexcept;
    inline void setDepthCompareOp(VkCompareOp newValue);

    inline bool depthBoundsTestEnable() const noexcept;
    inline void setDepthBoundsTestEnable(bool newValue);

    inline float minDepthBounds() const noexcept;
    inline void setMinDepthBounds(float newValue);

    inline float maxDepthBounds() const noexcept;
    inline void maxDepthBounds(float newValue);

    inline bool stencilTestEnable() const noexcept;
    inline void setStencilTestEnable(bool newValue);

    inline const VkStencilOpState& stencilFront() const noexcept;
    inline void setStencilFront(const VkStencilOpState& newValue);

    inline const VkStencilOpState& stencilBack() const noexcept;
    inline void setStencilBack(const VkStencilOpState& newValue);

    VertexAttribute& getOrCreateAttribute(const std::string& locationName);
    void removeAttribute(VertexAttribute& attribute);

    inline VertexIndices& indices() noexcept;
    inline const VertexIndices& indices() const noexcept;

    virtual void scheduleBind(CommandProducer& drawProducer) override;

  protected:
    friend class VertexAttribute;
    inline void invalidate() noexcept;

    virtual void rebuild() override;
    virtual VkPipeline createPipeline(
              const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
              VkPipelineLayout pipelineLayout) override;

  private:
    void _adjustAttributesLocationsAndBindings();

  private:
    AbstractRenderPass& _pass;
    size_t _compatabilityIndex;
    AbstractRenderPass::SubpassInfo _subpassInfo;

    using Attributes = std::vector<std::unique_ptr<VertexAttribute>>;
    Attributes _attributes;

    VertexIndices _indices;

    VkPrimitiveTopology _topology;

    bool _dynamicScissorViewport;
    VkViewport _viewport;
    VkRect2D _scissor;

    bool _rasterizerDiscardEnable;
    VkPolygonMode _polygonMode;
    float _lineWidth;
    VkCullModeFlags _cullMode;
    VkFrontFace _frontFace;
    bool _depthTestEnable;
    bool _depthWriteEnable;
    VkCompareOp _depthCompareOp;
    bool _depthBoundsTestEnable;
    float _minDepthBounds;
    float _maxDepthBounds;
    bool _stencilTestEnable;
    VkStencilOpState _stencilFront;
    VkStencilOpState _stencilBack;
  };

  inline bool GraphicsPipeline::isCompatible(
                                  const AbstractRenderPass& pass) const noexcept
  {
    if(_compatabilityIndex == 0) return false;
    return _compatabilityIndex == pass.compatabilityIndex();
  }

  inline void GraphicsPipeline::invalidate() noexcept
  {
    AbstractPipeline::invalidate();
  }

  inline VkPrimitiveTopology GraphicsPipeline::topology() const noexcept
  {
    return _topology;
  }

  inline void GraphicsPipeline::setTopology(VkPrimitiveTopology newTopology)
  {
    if(_topology == newTopology) return;
    _topology = newTopology;
    invalidate();
  }

  inline bool GraphicsPipeline::dynamicScissorViewport() const noexcept
  {
    return _dynamicScissorViewport;
  }

  inline void GraphicsPipeline::setDynamicScissorViewport(bool newValue)
  {
    if(_dynamicScissorViewport == newValue) return;
    _dynamicScissorViewport = newValue;
    invalidate();
  }

  inline const VkViewport& GraphicsPipeline::viewport() const noexcept
  {
    return _viewport;
  }

  inline void GraphicsPipeline::setViewport(const VkViewport& newViewport)
  {
    if( _viewport.x == newViewport.x &&
        _viewport.y == newViewport.y &&
        _viewport.width == newViewport.width &&
        _viewport.height == newViewport.height &&
        _viewport.minDepth == newViewport.minDepth &&
        _viewport.maxDepth == newViewport.maxDepth ) return;
    _viewport = newViewport;
    if(!_dynamicScissorViewport) invalidate();
  }

  inline const VkRect2D& GraphicsPipeline::scissor() const noexcept
  {
    return _scissor;
  }

  inline void GraphicsPipeline::setScissor(const VkRect2D& newScissor)
  {
    if( _scissor.offset.x == newScissor.offset.x &&
        _scissor.offset.y == newScissor.offset.y &&
        _scissor.extent.width == newScissor.extent.width &&
        _scissor.extent.height == newScissor.extent.height) return;
    _scissor = newScissor;
    if (!_dynamicScissorViewport) invalidate();
  }

  inline bool GraphicsPipeline::rasterizerDiscardEnable() const noexcept
  {
    return _rasterizerDiscardEnable;
  }

  inline void GraphicsPipeline::setRasterizerDiscardEnable(bool newValue)
  {
    if(_rasterizerDiscardEnable == newValue) return;
    _rasterizerDiscardEnable = newValue;
    invalidate();
  }

  inline VkPolygonMode GraphicsPipeline::polygonMode() const noexcept
  {
    return _polygonMode;
  }

  inline void GraphicsPipeline::setPolygonMode(VkPolygonMode newValue)
  {
    if(_polygonMode == newValue) return;
    _polygonMode = newValue;
    invalidate();
  }

  inline float GraphicsPipeline::lineWidth() const noexcept
  {
    return _lineWidth;
  }

  inline void GraphicsPipeline::setLineWidth(float newValue)
  {
    if(_lineWidth == newValue) return;
    _lineWidth = newValue;
    invalidate();
  }

  inline VkCullModeFlags GraphicsPipeline::cullMode() const noexcept
  {
    return _cullMode;
  }

  inline void GraphicsPipeline::setCullMode(VkCullModeFlags newValue)
  {
    if(_cullMode == newValue) return;
    _cullMode = newValue;
    invalidate();
  }

  inline VkFrontFace GraphicsPipeline::frontFace() const noexcept
  {
    return _frontFace;
  }

  inline void GraphicsPipeline::setFrontFace(VkFrontFace newValue)
  {
    if(_frontFace == newValue) return;
    _frontFace = newValue;
    invalidate();
  }

  inline bool GraphicsPipeline::depthTestEnable() const noexcept
  {
    return _depthTestEnable;
  }

  inline void GraphicsPipeline::setDepthTestEnable(bool newValue)
  {
    if(_depthTestEnable == newValue) return;
    _depthTestEnable = newValue;
    invalidate();
  }

  inline bool GraphicsPipeline::depthWriteEnable() const noexcept
  {
    return _depthWriteEnable;
  }

  inline void GraphicsPipeline::setDepthWriteEnable(bool newValue)
  {
    if (_depthWriteEnable == newValue) return;
    _depthWriteEnable = newValue;
    invalidate();
  }

  inline VkCompareOp GraphicsPipeline::depthCompareOp() const noexcept
  {
    return _depthCompareOp;
  }

  inline void GraphicsPipeline::setDepthCompareOp(VkCompareOp newValue)
  {
    if (_depthCompareOp == newValue) return;
    _depthCompareOp = newValue;
    invalidate();
  }

  inline bool GraphicsPipeline::depthBoundsTestEnable() const noexcept
  {
    return _depthBoundsTestEnable;
  }

  inline void GraphicsPipeline::setDepthBoundsTestEnable(bool newValue)
  {
    if (_depthBoundsTestEnable == newValue) return;
    _depthBoundsTestEnable = newValue;
    invalidate();
  }

  inline float GraphicsPipeline::minDepthBounds() const noexcept
  {
    return _minDepthBounds;
  }

  inline void GraphicsPipeline::setMinDepthBounds(float newValue)
  {
    if (_minDepthBounds == newValue) return;
    _minDepthBounds = newValue;
    invalidate();
  }

  inline float GraphicsPipeline::maxDepthBounds() const noexcept
  {
    return _maxDepthBounds;
  }

  inline void GraphicsPipeline::maxDepthBounds(float newValue)
  {
    if (_maxDepthBounds == newValue) return;
    _maxDepthBounds = newValue;
    invalidate();
  }

  inline bool GraphicsPipeline::stencilTestEnable() const noexcept
  {
    return _stencilTestEnable;
  }

  inline void GraphicsPipeline::setStencilTestEnable(bool newValue)
  {
    if (_stencilTestEnable == newValue) return;
    _stencilTestEnable = newValue;
    invalidate();
  }

  inline const VkStencilOpState& GraphicsPipeline::stencilFront() const noexcept
  {
    return _stencilFront;
  }

  inline void GraphicsPipeline::setStencilFront(
                                              const VkStencilOpState& newValue)
  {
    if (_stencilFront.failOp == newValue.failOp &&
        _stencilFront.passOp == newValue.passOp &&
        _stencilFront.depthFailOp == newValue.depthFailOp &&
        _stencilFront.compareOp == newValue.compareOp &&
        _stencilFront.compareMask == newValue.compareMask &&
        _stencilFront.writeMask == newValue.writeMask &&
        _stencilFront.reference == newValue.reference) return;
    _stencilFront = newValue;
    invalidate();
  }

  inline const VkStencilOpState& GraphicsPipeline::stencilBack() const noexcept
  {
    return _stencilBack;
  }

  inline void GraphicsPipeline::setStencilBack(const VkStencilOpState& newValue)
  {
    if (_stencilBack.failOp == newValue.failOp &&
        _stencilBack.passOp == newValue.passOp &&
        _stencilBack.depthFailOp == newValue.depthFailOp &&
        _stencilBack.compareOp == newValue.compareOp &&
        _stencilBack.compareMask == newValue.compareMask &&
        _stencilBack.writeMask == newValue.writeMask &&
        _stencilBack.reference == newValue.reference) return;
    _stencilBack = newValue;
    invalidate();
  }

  inline VertexIndices& GraphicsPipeline::indices() noexcept
  {
    return _indices;
  }

  inline const VertexIndices& GraphicsPipeline::indices() const noexcept
  {
    return _indices;
  }
}