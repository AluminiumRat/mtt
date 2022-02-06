#pragma once

#include <tuple>

#include <vulkan/vulkan.h>

#include <mtt/render/DrawPlan/AbstractAction.h>
#include <mtt/render/DrawPlan/MultipleResourceSetter.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  template <typename... ResourceSetterParams>
  class DrawMeshAction : public AbstractAction
  {
  public:
    template<typename... SetterArgumentTypes>
    inline DrawMeshAction(GraphicsPipeline& pipeline,
                          VkViewport viewport,
                          VkRect2D scissor,
                          uint32_t vertexCount,
                          SetterArgumentTypes&... setterArguments);
    DrawMeshAction(const DrawMeshAction&) = delete;
    DrawMeshAction& operator = (const DrawMeshAction&) = delete;
    virtual ~DrawMeshAction() noexcept = default;

    inline virtual void execute(DrawContext& context) override;

  private:
    GraphicsPipeline& _pipeline;
    VkViewport _viewport;
    VkRect2D _scissor;
    uint32_t _vertexCount;

    MultipleResourceSetter<ResourceSetterParams...> _resourceSetter;
  };

  template<typename... ResourceSetterParams>
  template<typename... SetterArgumentTypes>
  inline DrawMeshAction<ResourceSetterParams...>::DrawMeshAction(
                                    GraphicsPipeline& pipeline,
                                    VkViewport viewport,
                                    VkRect2D scissor,
                                    uint32_t vertexCount,
                                    SetterArgumentTypes&... setterArguments) :
    _pipeline(pipeline),
    _viewport(viewport),
    _scissor(scissor),
    _vertexCount(vertexCount),
    _resourceSetter(setterArguments...)
  {
  }

  template <typename... ResourceSetterParams>
  inline void DrawMeshAction<ResourceSetterParams...>::execute(
                                                          DrawContext& context)
  {
    _resourceSetter.setup(context);
    
    _pipeline.setScissor(_scissor);
    _pipeline.setViewport(_viewport);

    _pipeline.scheduleBind(context.drawProducer);

    if(_pipeline.indices().attachedBuffer() != nullptr)
    {
      vkCmdDrawIndexed( context.drawProducer.bufferHandle(),
                        _vertexCount,
                        1,
                        0,
                        0,
                        0);
    }
    else vkCmdDraw( context.drawProducer.bufferHandle(),
                    _vertexCount,
                    1,
                    0,
                    0);
  }
}