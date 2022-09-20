#pragma once

#include <mtt/application/Clipboard/CopyToClipboardOperation.h>

namespace mtt
{
  class CECopyToClipboardOperation : public CopyToClipboardOperation
  {
  public:
    static constexpr char environmentCategoryName[] = "CEEnvironment";

  public:
    CECopyToClipboardOperation();
    CECopyToClipboardOperation(const CECopyToClipboardOperation&) = delete;
    CECopyToClipboardOperation& operator = (
                                    const CECopyToClipboardOperation&) = delete;
    virtual ~CECopyToClipboardOperation() noexcept = default;

    virtual bool objectSupported(const Object& object) const noexcept override;
  };
}