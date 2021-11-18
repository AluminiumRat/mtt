#pragma once

#include <vector>

#include <mtt/Render/DrawPlan/DrawContext.h>
#include <mtt/Render/Pipeline/InputAttachment.h>
#include <mtt/Render/Pipeline/Texture2D.h>
#include <mtt/Render/Pipeline/VolatileUniform.h>
#include <mtt/Render/RenderPass/FrameBuffer.h>
#include <mtt/Utilities/Abort.h>

namespace mtt
{
  template <typename ResourceType, typename ValueType>
  class ResourceSetter;

  template <typename DataType>
  class ResourceSetter<VolatileUniform<DataType>, DataType>
  {
  public:
    inline ResourceSetter(VolatileUniform<DataType>& uniform,
                          const DataType& value) :
      _uniform(uniform),
      _value(value)
    {
    }

    ResourceSetter(const ResourceSetter&) = delete;
    ResourceSetter& operator = (const ResourceSetter&) = delete;
    ~ResourceSetter() noexcept = default;

    inline void setup(DrawContext& drawContext)
    {
      _uniform.setValuePtr(&_value);
    }

  private:
    VolatileUniform<DataType>& _uniform;
    DataType _value;
  };

  template <>
  class ResourceSetter<InputAttachment, uint32_t>
  {
  public:
    inline ResourceSetter(InputAttachment& resource,
                          uint32_t attachmentIndex) :
      _resource(resource),
      _attachmentIndex(attachmentIndex)
    {
    }

    ResourceSetter(const ResourceSetter&) = delete;
    ResourceSetter& operator = (const ResourceSetter&) = delete;
    ~ResourceSetter() noexcept = default;

    inline void setup(DrawContext& drawContext)
    {
      _resource.setAttachment(
                      &drawContext.frameBuffer->attachment(_attachmentIndex));
    }

  private:
    InputAttachment& _resource;
    uint32_t _attachmentIndex;
  };

  template <>
  class ResourceSetter<Texture2D, size_t>
  {
  public:
    inline ResourceSetter(Texture2D& resource,
                          size_t samplerMapIndex) :
      _resource(resource),
      _samplerMapIndex(samplerMapIndex)
    {
    }

    ResourceSetter(const ResourceSetter&) = delete;
    ResourceSetter& operator = (const ResourceSetter&) = delete;
    ~ResourceSetter() noexcept = default;

    inline void setup(DrawContext& drawContext)
    {
      _resource.setImageView(
                        drawContext.frameBuffer->samplerMap(_samplerMapIndex));
    }

  private:
    Texture2D& _resource;
    size_t _samplerMapIndex;
  };

  template <>
  class ResourceSetter<std::vector<Texture2D*>, std::vector<ImageView*>>
  {
  public:
    inline ResourceSetter(std::vector<Texture2D*> resources,
                          const std::vector<ImageView*>& values) :
      _resources(resources),
      _values(values)
    {
      if(_resources.size() != _values.size()) Abort("ResourceSetter<std::vector<Texture*>>::ResourceSetter: resources and value are of different sizes.");
    }

    ResourceSetter(const ResourceSetter&) = delete;
    ResourceSetter& operator = (const ResourceSetter&) = delete;
    ~ResourceSetter() noexcept = default;

    inline void setup(DrawContext& drawContext)
    {
      for(size_t resourceIndex = 0;
          resourceIndex < _resources.size();
          resourceIndex++)
      {
        _resources[resourceIndex]->setImageView(*_values[resourceIndex]);
      }
    }

  private:
    std::vector<Texture2D*> _resources;
    std::vector<ImageView*> _values;
  };
}