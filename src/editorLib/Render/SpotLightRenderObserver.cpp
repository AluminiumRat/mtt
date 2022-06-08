#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/Objects/SpotLightObject.h>
#include <mtt/editorLib/Render/SpotLightRenderObserver.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/RenderScene.h>

#define ICON_FILE ":/editorLib/spotLight.png"
#define ICON_SIZE 32

#define CAP_SEGMENTS 32
#define BODY_SEGMENTS 6

using namespace mtt;

SpotLightRenderObserver::SpotLightRenderObserver( SpotLightObject& object,
                                                  CommonEditData& commonData,
                                                  RenderScene& renderScene) :
  AbstractLightRenderObserver(object, commonData, ICON_FILE, ICON_SIZE),
  _lightObject(object),
  _renderScene(renderScene)
{
  connect(&_lightObject,
          &SpotLightObject::distanceChanged,
          this,
          &SpotLightRenderObserver::_updateConeMesh,
          Qt::DirectConnection);
  _updateConeMesh();

  connect(&_lightObject,
          &SpotLightObject::angleChanged,
          this,
          &SpotLightRenderObserver::_updateConeMesh,
          Qt::DirectConnection);
  _updateConeMesh();
}

void SpotLightRenderObserver::_updateConeMesh() noexcept
{
  if (_lightObject.angle() <= 0.f || _lightObject.distance() <= 0.f)
  {
    hullNode().resetGeometry();
    return;
  }

  try
  {
    float radius = _lightObject.distance() * tan(_lightObject.angle() / 2.f);
    hullNode().setConeGeometry( radius,
                                _lightObject.distance(),
                                0.f,
                                BODY_SEGMENTS,
                                CAP_SEGMENTS);
  }
  catch (std::exception& error)
  {
    Log() << "DirectLightRenderObserver::_updateCylinderMesh: unable to update cylinder mesh: " << error.what();
  }
  catch (...)
  {
    Log() << "DirectLightRenderObserver::_updateCylinderMesh: unable to update cylinder mesh: unknown error.";
  }
}
