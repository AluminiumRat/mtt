cmake_minimum_required (VERSION 3.8)

project(Utilities)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_path(GLM_PATH
          glm/glm.hpp
          HINTS "$ENV{GLM_PATH}"
          REQUIRED)

include_directories("../../include"
                    ${GLM_PATH})

set(HEADERS
    "../../include/mtt/Utilities/Abort.h"
    "../../include/mtt/Utilities/Box.h"
    "../../include/mtt/Utilities/clip.h"
    "../../include/mtt/Utilities/CommonMeshGeometry.h"
    "../../include/mtt/Utilities/lineLineNearest.h"
    "../../include/mtt/Utilities/Log.h"
    "../../include/mtt/Utilities/Plane.h"
    "../../include/mtt/Utilities/ScopedSetter.h"
    "../../include/mtt/Utilities/Segment.h"
    "../../include/mtt/Utilities/Sphere.h"
    "../../include/mtt/Utilities/SpinLock.h"
    "../../include/mtt/Utilities/static_cast_unique_ptr.h"
    "../../include/mtt/Utilities/StringRegistry.h"
    "../../include/mtt/Utilities/Surface.h"
    "../../include/mtt/Utilities/SurfaceMaterialData.h"
    "../../include/mtt/Utilities/UID.h"
    "../../include/mtt/Utilities/ViewFrustum.h"
    )

set(SOURCES
    "CommonMeshGeometry.cpp"
    "Log.cpp"
    "StringRegistry.cpp"
    "Surface.cpp"
    "UID.cpp"
    "ViewFrustum.cpp"
    )

add_library(Utilities
            ${HEADERS}
            ${SOURCES})
