#include <mtt/editorLib/PropertiesWidget/AnimationObjectWidget.h>
#include <mtt/editorLib/AnimationPlayer.h>

#include <GeneratedFiles/ui_AnimationObjectWidget.h>

using namespace mtt;

AnimationObjectWidget::AnimationObjectWidget( AnimationObject& object,
                                              AnimationPlayer& player) :
  _ui(new Ui::AnimationObjectWidget),
  _object(object),
  _player(player)
{
  _ui->setupUi(this);

  connect(_ui->playButton,
          &QPushButton::clicked,
          this,
          &AnimationObjectWidget::_play,
          Qt::DirectConnection);

  connect(_ui->stopButton,
          &QPushButton::clicked,
          this,
          &AnimationObjectWidget::_stop,
          Qt::DirectConnection);

  adjustSize();
}

AnimationObjectWidget::~AnimationObjectWidget() noexcept = default;

void AnimationObjectWidget::_play() noexcept
{
  _player.start(_object);
}

void AnimationObjectWidget::_stop() noexcept
{
  _player.stop();
}
