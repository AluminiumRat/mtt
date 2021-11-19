#pragma once

#include <map>
#include <memory>
#include <mutex>

#include <mtt/Utilities/Abort.h>

namespace mtt
{
  template <typename ResourceType, typename DescriptionType>
  class ResourceLibrary
  {
  public:
    ResourceLibrary() = default;
    ResourceLibrary(const ResourceLibrary&) = delete;
    ResourceLibrary& operator = (const ResourceLibrary&) = delete;
    virtual ~ResourceLibrary() noexcept = default;

    /// Returns a resource that already exist in the library.
    /// Uses buildResource method if the resource does not exist
    /// Thread safe method
    inline std::shared_ptr<ResourceType> getOrCreate(
                                            const DescriptionType& description);

    /// Returns a resource that already exist in the library.
    /// Returns nullptr if the resource does not exist
    /// Thread safe method
    inline std::shared_ptr<ResourceType> get(
                            const DescriptionType& description) const noexcept;

    /// Returns false if a resource with this description already exist
    /// Thread safe method
    inline bool add(std::shared_ptr<ResourceType> resource,
                    const DescriptionType& description);
  
  protected:
    virtual std::shared_ptr<ResourceType> buildResource(
                                      const DescriptionType& description) = 0;

  private:
    using ResourceMap = std::map< DescriptionType,
                                  std::shared_ptr<ResourceType>>;
    ResourceMap _resourceMap;
    mutable std::mutex _resourceMapMutex;
  };

  template <typename ResourceType, typename DescriptionType>
  inline std::shared_ptr<ResourceType>
            ResourceLibrary<ResourceType,DescriptionType>::getOrCreate(
                                            const DescriptionType& description)
  {
    std::shared_ptr<ResourceType> resource = get(description);
    if(resource != nullptr) return resource;

    resource = buildResource(description);
    if (resource == nullptr) Abort("ResourceLibrary::getOrCreate: uploaded resource is null");

    if(add(resource, description) == true) return resource;

    resource = get(description);
    if(resource == nullptr) Abort("ResourceLibrary::getOrCreate: stored resource is null");
    return resource;
  }

  template <typename ResourceType, typename DescriptionType>
  inline std::shared_ptr<ResourceType>
            ResourceLibrary<ResourceType, DescriptionType>::get(
                              const DescriptionType& description) const noexcept
  {
    std::lock_guard<std::mutex> lock(_resourceMapMutex);
    typename ResourceMap::const_iterator iRecord =
                                                _resourceMap.find(description);
    if (iRecord == _resourceMap.end()) return nullptr;
    else return iRecord->second;
  }

  template <typename ResourceType, typename DescriptionType>
  inline bool ResourceLibrary<ResourceType, DescriptionType>::add(
                                        std::shared_ptr<ResourceType> resource,
                                        const DescriptionType& description)
  {
    std::lock_guard<std::mutex> lock(_resourceMapMutex);
  
    typename ResourceMap::const_iterator iRecord =
                                                _resourceMap.find(description);
    if (iRecord != _resourceMap.end()) return false;
    _resourceMap[description] = resource;
    return true;
  }
}