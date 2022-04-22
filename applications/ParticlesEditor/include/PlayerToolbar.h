#pragma once

#include <QtWidgets/QToolBar>

class ParticlesEditorCommonData;

class PlayerToolbar : public QToolBar
{
  Q_OBJECT

public:
  explicit PlayerToolbar(ParticlesEditorCommonData& commonData);
  PlayerToolbar(const PlayerToolbar&) = delete;
  PlayerToolbar& operator = (const PlayerToolbar&) = delete;
  virtual ~PlayerToolbar() noexcept = default;

private:
  void _play() noexcept;
  void _stop() noexcept;

private:
  ParticlesEditorCommonData& _commonData;
};
