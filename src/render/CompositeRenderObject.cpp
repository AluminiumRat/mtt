#include <mtt/render/CompositeRenderObject.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

size_t CompositeRenderObject::culledDrawablesNumber() const noexcept
{
  return 0;
}

DrawableNode&
      CompositeRenderObject::culledDrawable [[noreturn]] (size_t index) noexcept
{
  Abort("CompositeRenderObject::culledDrawable: no drawables available.");
}

size_t CompositeRenderObject::areaModificatorsNumber() const noexcept
{
  return 0;
}

AreaModificator&
    CompositeRenderObject::areaModificator [[noreturn]] (size_t index) noexcept
{
  Abort("CompositeRenderObject::areaModificator: no modificators available.");
}

size_t CompositeRenderObject::unculledDrawablesNumber() const noexcept
{
  return 0;
}

Drawable&
    CompositeRenderObject::unculledDrawable [[noreturn]] (size_t index) noexcept
{
  Abort("CompositeRenderObject::unculledDrawable: no drawables available.");
}

size_t CompositeRenderObject::hudDrawablesNumber() const noexcept
{
  return 0;
}

Drawable&
        CompositeRenderObject::hudDrawable [[noreturn]] (size_t index) noexcept
{
  Abort("CompositeRenderObject::hudDrawable: no drawables available.");
}
