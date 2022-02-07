#include <stdexcept>

#include <mtt/clPipeline/Lighting/AbstractLight.h>
#include <mtt/utilities/Log.h>

#include <Objects/LightObject.h>
#include <Render/AbstractLightRenderObserver.h>

AbstractLightRenderObserver::AbstractLightRenderObserver(
                                                LightObject& object,
                                                EditorCommonData& commonData) :
  Object3DRenderObserver(object, commonData),
  _object(object),
  _infinityArea(false),
  _defferedLightApplicator(nullptr),
  _forwardLightApplicator(nullptr)
{
  connect(&_object,
          &LightObject::enabledChanged,
          this,
          &AbstractLightRenderObserver::_updateEnabled,
          Qt::DirectConnection);
  _updateEnabled();
}

void AbstractLightRenderObserver::setLightObject(
                                        mtt::clPipeline::AbstractLight& light)
{
  _clearDrawables();

  _defferedLightApplicator = light.defferedLightApplicator();
  _forwardLightApplicator = light.forwardLightModificator();

  try
  {
    if (_defferedLightApplicator != nullptr)
    {
      _defferedLightApplicator->addModificator(_enableFilter);
      positionRotateJoint().addChild(*_defferedLightApplicator);
      if(_infinityArea) registerUnculledDrawable(*_defferedLightApplicator);
      else registerCulledDrawable(*_defferedLightApplicator);
    }
    if (_forwardLightApplicator != nullptr)
    {
      positionRotateJoint().addChild(*_forwardLightApplicator);
      if(_object.enabled()) registerAreaModificator(*_forwardLightApplicator);
    }
  }
  catch (...)
  {
    _clearDrawables();
    throw;
  }
}

void AbstractLightRenderObserver::setInfinityArea(bool newValue)
{
  if(_infinityArea == newValue) return;

  _infinityArea = newValue;

  if(_defferedLightApplicator == nullptr) return;

  try
  {
    if (_infinityArea)
    {
      unregisterCulledDrawable(*_defferedLightApplicator);
      registerUnculledDrawable(*_defferedLightApplicator);
    }
    else
    {
      unregisterUnculledDrawable(*_defferedLightApplicator);
      registerCulledDrawable(*_defferedLightApplicator);
    }
  }
  catch (std::exception& error)
  {
    mtt::Log() << "AbstractLightRenderObserver::setInfinityArea: unable to registed deffered light applicator: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "AbstractLightRenderObserver::setInfinityArea: unable to registed deffered light applicator";
  }
}

void AbstractLightRenderObserver::_clearDrawables() noexcept
{
  if (_defferedLightApplicator != nullptr)
  {
    _defferedLightApplicator->removeModificator(_enableFilter);
    positionRotateJoint().removeChild(*_defferedLightApplicator);
    if (_infinityArea) unregisterUnculledDrawable(*_defferedLightApplicator);
    else unregisterCulledDrawable(*_defferedLightApplicator);
    _defferedLightApplicator = nullptr;
  }
  if (_forwardLightApplicator != nullptr)
  {
    positionRotateJoint().removeChild(*_forwardLightApplicator);
    if (_object.enabled()) unregisterAreaModificator(*_forwardLightApplicator);
    _forwardLightApplicator = nullptr;
  }
}

void AbstractLightRenderObserver::_updateEnabled() noexcept
{
  _enableFilter.setVisible(_object.enabled());
  if (_forwardLightApplicator != nullptr)
  {
    try
    {
      if(_object.enabled()) registerAreaModificator(*_forwardLightApplicator);
      else unregisterAreaModificator(*_forwardLightApplicator);
    }
    catch (std::exception& error)
    {
      mtt::Log() << "AbstractLightRenderObserver::_updateEnabled: " << error.what();
    }
    catch (...)
    {
      mtt::Log() << "AbstractLightRenderObserver::_updateEnabled: unable to register area modificator";
    }
  }
}
