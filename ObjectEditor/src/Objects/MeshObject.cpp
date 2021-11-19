#include <mtt/Utilities/Abort.h>

#include <Objects/MeshObject.h>

MeshObject::MeshObject(const mtt::UID& theId) :
  GeometryObject(theId),
  _boneRefs(nullptr),
  _material(nullptr)
{
  mtt::UID materialId(id().mixedUID(1096043266422623895ull));
  std::unique_ptr<MaterialObject> material(new MaterialObject(materialId));
  material->setObjectName(tr("Material"));
  _material = material.get();
  addSubobject(std::move(material));

  setBoneRefs(std::make_unique<BoneRefBatch>(
                                std::vector<std::unique_ptr<BoneRefObject>>()));
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
  refs->setObjectName(tr("Bone references"));
  addSubobject(std::move(refs));

  if(_boneRefs != nullptr) removeSubobject(*_boneRefs, true);
  _boneRefs = newRefsPtr;

  emit boneRefsChanged(*_boneRefs);
}
