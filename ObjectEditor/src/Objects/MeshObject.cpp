#include <mtt/Utilities/Abort.h>

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

void MeshObject::setMaterial(MaterialObject* material)
{
  if(material == nullptr) setMaterialId(mtt::UID());
  else setMaterialId(material->id());
}

void MeshObject::setMaterialId(const mtt::UID& id)
{
  try
  {
    _materialLink.setReferencedId(id);
  }
  catch(...)
  {
    mtt::Abort("MeshObject::setMaterialId: unable to set material link");
  }
}

void MeshObject::_connectMaterial(MaterialObject& material)
{
  emit materialRefChanged(&material);
}

void MeshObject::_disconnectMaterial(MaterialObject& material) noexcept
{
  emit materialRefChanged(nullptr);
}
