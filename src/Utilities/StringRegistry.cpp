#include <mtt/Utilities/StringRegistry.h>

using namespace mtt;

StringRegistry::StringRegistry() :
  _indexCursor(0)
{
}

size_t StringRegistry::getIndex(const std::string& theString)
{
  std::lock_guard<std::mutex> lock(_accessMutex);
  RegistryMap::iterator iString = _map.find(theString);
  if(iString == _map.end())
  {
    _indexCursor++;
    _map[theString] = _indexCursor;
    return _indexCursor;
  }
  else return iString->second;
}
