#pragma once

#include <memory>
#include <vector>

#include <mtt/render/Drawable/DrawableList.h>

namespace mtt
{
  class ModificatorProxyDrawable : public DrawableList
  {
  public:
    ModificatorProxyDrawable() noexcept = default;
    ModificatorProxyDrawable(const ModificatorProxyDrawable&) = delete;
    ModificatorProxyDrawable& operator = (
                                      const ModificatorProxyDrawable&) = delete;
    virtual ~ModificatorProxyDrawable() noexcept = default;

    virtual void registerAreaModificators(AreaModificatorSet& set) override;
    virtual void unregisterAreaModificators(
                                    AreaModificatorSet& set) noexcept override;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

  protected:
    virtual std::unique_ptr<Drawable>
                                createSubdrawable(AreaModificatorSet& set) = 0;

  private:
    struct SubdrawableRecord
    {
      AreaModificatorSet* set = nullptr;
      size_t registerCounter = 0;
      std::unique_ptr<Drawable> subdrawable;

      SubdrawableRecord() = default;
      SubdrawableRecord(const SubdrawableRecord&) = delete;
      SubdrawableRecord& operator = (const SubdrawableRecord&) = delete;
      SubdrawableRecord(SubdrawableRecord&&) = default;
      SubdrawableRecord& operator = (SubdrawableRecord&&) = default;
      ~SubdrawableRecord() noexcept = default;
    };
    using Subdrawables = std::vector<SubdrawableRecord>;
    Subdrawables _subdrawables;
  };
}