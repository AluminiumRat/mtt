#include <Objects/BoneRefBatch.h>

BoneRefBatch::BoneRefBatch( std::vector<std::unique_ptr<BoneRefObject>>&& refs,
                            const QString& name,
                            bool canBeRenamed,
                            const mtt::UID& id) :
  Object(name, canBeRenamed, id)
{
  for(size_t refIndex = 0; refIndex < refs.size(); refIndex++)
  {
    if(refs[refIndex] != nullptr)
    {
      _refs.push_back(refs[refIndex].get());
      addSubobject(std::move(refs[refIndex]));
    }
  }
}
