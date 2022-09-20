#pragma once

#include <mtt/editorLib/Clipboard/CECopyToClipboardOperation.h>

class CopyToClipboardOperation : public mtt::CECopyToClipboardOperation
{
public:
  static constexpr char particlesCategoryName[] = "MTTParticles";

public:
    CopyToClipboardOperation();
    CopyToClipboardOperation(const CopyToClipboardOperation&) = delete;
    CopyToClipboardOperation& operator = (
                                    const CopyToClipboardOperation&) = delete;
    virtual ~CopyToClipboardOperation() noexcept = default;
};