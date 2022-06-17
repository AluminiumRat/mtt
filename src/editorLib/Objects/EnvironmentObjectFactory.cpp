#include <mtt/editorLib/Objects/AmbientLightObject.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/Objects/EnvironmentGroup.h>
#include <mtt/editorLib/Objects/EnvironmentModel.h>
#include <mtt/editorLib/Objects/EnvironmentObjectFactory.h>
#include <mtt/editorLib/Objects/EnvironmentRootObject.h>
#include <mtt/editorLib/Objects/PointLightObject.h>
#include <mtt/editorLib/Objects/SpotLightObject.h>

using namespace mtt;

EnvironmentObjectFactory::EnvironmentObjectFactory()
{
  registerType<EnvironmentGroup, EnvironmentGroupIndex>();
  registerType<EnvironmentRootObject, RootObjectIndex>();
  registerType<BackgroundObject, BackgroundIndex>();
  registerType<AmbientLightObject, AmbientLightIndex>();
  registerType<DirectLightObject, DirectLightIndex>();
  registerType<EnvironmentModel, EnvironmentModelIndex>();
  registerType<SpotLightObject, SpotLightIndex>();
  registerType<PointLightObject, PointLightIndex>();
}
