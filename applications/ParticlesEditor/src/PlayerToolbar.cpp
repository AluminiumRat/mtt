#include <ParticlesEditorCommonData.h>
#include <PlayerToolbar.h>

PlayerToolbar::PlayerToolbar(ParticlesEditorCommonData& commonData) :
  _commonData(commonData)
{
  addAction(QIcon(":/particlesEditor/play.png"),
            tr("Play"),
            this,
            &PlayerToolbar::_play);

  addAction(QIcon(":/particlesEditor/stop.png"),
            tr("Stop"),
            this,
            &PlayerToolbar::_stop);
}

void PlayerToolbar::_play() noexcept
{
  if(_commonData.scene() == nullptr) return;

  _commonData.animationPlayer().start(
                                  _commonData.scene()->dataRoot().animation());
}

void PlayerToolbar::_stop() noexcept
{
  _commonData.animationPlayer().stop();
}
