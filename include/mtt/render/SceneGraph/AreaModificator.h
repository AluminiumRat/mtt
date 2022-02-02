#pragma once

#include <algorithm>
#include <vector>

#include <mtt/render/SceneGraph/BoundedNode.h>

namespace mtt
{
  class AbstractPipeline;
  struct DrawPlanBuildInfo;

  class AreaModificator : public BoundedNode
  {
  public:
    class Consumer
    {
    public:
      virtual void resetPipeline() noexcept = 0;
    };

  protected:
    AreaModificator(size_t type);
    AreaModificator(const AreaModificator&) = delete;
    AreaModificator& operator = (const AreaModificator&) = delete;
  public:
    virtual ~AreaModificator() noexcept = default;

    inline size_t type() const noexcept;

    static size_t allocateType() noexcept;

    void addToDrawPlan(DrawPlanBuildInfo& buildInfo);

    inline void addConsumer(Consumer& consumer);
    inline void removeConsumer(Consumer& consumer) noexcept;

    virtual void setTransformMatrix(
                                  const glm::mat4& newValue) noexcept override;

  protected:
    inline void resetConsumers() noexcept;

    virtual void buildPrepareActions(DrawPlanBuildInfo& buildInfo);

  private:
    size_t _type;
    glm::mat3 _normalTransformMatrix;
    std::vector<Consumer*> _consumers;
  };

  inline size_t AreaModificator::type() const noexcept
  {
    return _type;
  }

  inline void AreaModificator::addConsumer(Consumer& consumer)
  {
    _consumers.push_back(&consumer);
  }

  inline void AreaModificator::removeConsumer(Consumer& consumer) noexcept
  {
    std::vector<Consumer*>::iterator iConsumer = std::find( _consumers.begin(),
                                                            _consumers.end(),
                                                            &consumer);
    if(iConsumer == _consumers.end()) return;
    _consumers.erase(iConsumer);
  }

  inline void AreaModificator::resetConsumers() noexcept
  {
    for(Consumer* consumer : _consumers) consumer->resetPipeline();
  }
}