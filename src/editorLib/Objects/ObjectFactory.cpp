#include <mtt/editorLib/Objects/ObjectFactory.h>

using namespace mtt;

ObjectFactory::TypeIndex ObjectFactory::getTypeIndex(
                                            const Object& object) const noexcept
{
  const QMetaObject* meta = object.metaObject();
  while (meta != nullptr)
  {
    TypeIndices::const_iterator indexIterator =
                                          _typeIndices.find(meta->className());
    if(indexIterator != _typeIndices.end()) return indexIterator->second;
    meta = meta->superClass();
  }
  return notIndex;
}

std::unique_ptr<Object> ObjectFactory::createObject(TypeIndex typeIndex,
                                                    const QString& name,
                                                    bool canBeRenamed,
                                                    const UID& uid) const
{
  Builders::const_iterator iBuilder = _builders.find(typeIndex);
  if(iBuilder == _builders.end()) return nullptr;
  return iBuilder->second(name, canBeRenamed, uid);
}
