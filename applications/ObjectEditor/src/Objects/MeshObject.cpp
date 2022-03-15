#include <mtt/utilities/Abort.h>

#include <Objects/MeshObject.h>

MeshObject::MeshObject( const QString& name,
                        bool canBeRenamed,
                        const mtt::UID& theId) :
  GeometryObject(name, canBeRenamed, theId),
  _materialLink(*this),
  _boneRefs(nullptr)
{
  setBoneRefs(std::make_unique<BoneRefBatch>(
                                std::vector<std::unique_ptr<BoneRefObject>>(),
                                tr("Bone references"),
                                false));
}

void MeshObject::setGeometry(mtt::CommonMeshGeometry&& newGeometry) noexcept
{
  _geometry = std::move(newGeometry);
  emit geometryChanged();
}

void MeshObject::setBoneRefs(std::unique_ptr<BoneRefBatch> refs)
{
  if(refs == nullptr) mtt::Abort("MeshObject::setBoneRefs: refs is nullptr");

  BoneRefBatch* newRefsPtr = refs.get();
  addSubobject(std::move(refs));

  if(_boneRefs != nullptr) removeSubobject(*_boneRefs, true);
  _boneRefs = newRefsPtr;

  emit boneRefsChanged(*_boneRefs);
}
