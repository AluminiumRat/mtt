#pragma once

#include <memory>

namespace mtt
{
  template< typename ChildClass,
            typename ParentClass,
            typename ChildDeleter = std::default_delete<ChildClass>,
            typename ParentDeleter = std::default_delete<ParentClass>>
  std::unique_ptr<ChildClass, ChildDeleter> static_cast_unique_ptr(
                    std::unique_ptr<ParentClass, ParentDeleter>&& castedPointer)
  {
    return std::unique_ptr<ChildClass, ChildDeleter>(
                            static_cast<ChildClass*>(castedPointer.release()));
  }
}