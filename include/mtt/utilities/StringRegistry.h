#pragma once

#include <map>
#include <mutex>
#include <string>

namespace mtt
{
  class StringRegistry
  {
  public:
    StringRegistry();
    StringRegistry(const StringRegistry&) = delete;
    StringRegistry& operator = (const StringRegistry&) = delete;
    ~StringRegistry() noexcept = default;

    size_t getIndex(const std::string& theString);

  private:
    std::mutex _accessMutex;
    size_t _indexCursor;
    using RegistryMap = std::map<std::string, size_t>;
    RegistryMap _map;
  };
}