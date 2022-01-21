#include <stdexcept>

#include <mtt/utilities/Log.h>

#include <Objects/LightObject.h>
#include <Render/AbstractLightRenderObserver.h>

AbstractLightRenderObserver::AbstractLightRenderObserver(
                                                LightObject& object,
                                                EditorCommonData& commonData) :
  Object3DRenderObserver(object, commonData),
  _lightObject(object),
  _lightApplicator(nullptr)
{
  _applicatorNode.registerModificator(_enableFilter);
  positionRotateJoint().addChild(_applicatorNode);
  registerCulledDrawable(_applicatorNode);

  connect(&_lightObject,
          &LightObject::enabledChanged,
          this,
          &AbstractLightRenderObserver::_updateEnabled,
          Qt::DirectConnection);
  _updateEnabled();

  connect(&_lightObject,
          &LightObject::distanceChanged,
          this,
          &AbstractLightRenderObserver::updateDistance,
          Qt::DirectConnection);

  connect(&_lightObject,
          &LightObject::colorChanged,
          this,
          &AbstractLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);

  connect(&_lightObject,
          &LightObject::baseIlluminanceChanged,
          this,
          &AbstractLightRenderObserver::_updateIlluminance,
          Qt::DirectConnection);
}

mtt::Sphere AbstractLightRenderObserver::getBoundingSphere() const noexcept
{
  return mtt::Sphere(glm::vec3(0), _lightObject.distance());
}

void AbstractLightRenderObserver::updateBounding()
{
  _applicatorNode.setLocalBoundSphere(getBoundingSphere());
}

void AbstractLightRenderObserver::setApplicator(
                          mtt::dlPipeline::AbstractLightApplicator& applicator)
{
  _lightApplicator = &applicator;
  _applicatorNode.setDrawable(&applicator, mtt::Sphere());
  updateDistance();
  _updateIlluminance();
}

void AbstractLightRenderObserver::_updateEnabled() noexcept
{
  _enableFilter.setVisible(_lightObject.enabled());
}

void AbstractLightRenderObserver::updateDistance() noexcept
{
  try
  {
    if(_lightApplicator != nullptr)
    {
      _lightApplicator->setDistance(_lightObject.distance());
    }
    updateBounding();
  }
  catch(const std::exception& error)
  {
    mtt::Log() << "AbstractLightRenderObserver::_updateDistance: unable to update distance: " << error.what();
  }
  catch(...)
  {
    mtt::Log() << "AbstractLightRenderObserver::_updateDistance: unable to update distance: unknown error";
  }
}

void AbstractLightRenderObserver::_updateIlluminance() noexcept
{
  try
  {
    if (_lightApplicator != nullptr)
    {
      _lightApplicator->setIlluminance(
                        _lightObject.color() * _lightObject.baseIlluminance());
    }
  }
  catch (const std::exception& error)
  {
    mtt::Log() << "AbstractLightRenderObserver::_updateIlluminance: unable to update illuminance: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "AbstractLightRenderObserver::_updateIlluminance: unable to update illuminance: unknown error";
  }
}
