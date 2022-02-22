#include <mtt/editorLib/Objects/AmbientLightObject.h>
#include <mtt/editorLib/Objects/EnvironmentObjectFactory.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/Objects/EnvironmentModel.h>

using namespace mtt;

EnvironmentObjectFactory::EnvironmentObjectFactory()
{
  registerType<AmbientLightObject, AmbientLightIndex>();
  registerType<BackgroundObject, BackgroundIndex>();
  registerType<DirectLightObject, DirectLightIndex>();
  registerType<EnvironmentModel, EnvironmentModelIndex>();
}
