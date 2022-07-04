#include <mtt/editorLib/Objects/PointLightObject.h>
#include <mtt/editorLib/Objects/CubemapObject.h>

using namespace mtt;

PointLightObject::PointLightObject( const QString& name,
                                    bool canBeRenamed,
                                    const UID theId) :
  FadingLightObject(name, canBeRenamed, theId)
{
  UID cubemapId(id().mixedUID(15285932983551776433ull));
  std::unique_ptr<CubemapObject> filterCubemap(new CubemapObject( tr("Filter"),
                                                                  false,
                                                                  cubemapId));
  _filterCubemap = filterCubemap.get();
  addSubobject(std::move(filterCubemap));

  setDistance(5.f);
}
