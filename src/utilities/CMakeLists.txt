cmake_minimum_required (VERSION 3.8)

project(utilities)

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
    "../../include/mtt/utilities/Abort.h"
    "../../include/mtt/utilities/Box.h"
    "../../include/mtt/utilities/clip.h"
    "../../include/mtt/utilities/CommonMeshGeometry.h"
    "../../include/mtt/utilities/lineLineNearest.h"
    "../../include/mtt/utilities/Log.h"
    "../../include/mtt/utilities/Plane.h"
    "../../include/mtt/utilities/Range.h"
    "../../include/mtt/utilities/ScopedSetter.h"
    "../../include/mtt/utilities/Segment.h"
    "../../include/mtt/utilities/Sphere.h"
    "../../include/mtt/utilities/SpinLock.h"
    "../../include/mtt/utilities/static_cast_unique_ptr.h"
    "../../include/mtt/utilities/StringRegistry.h"
    "../../include/mtt/utilities/Surface.h"
    "../../include/mtt/utilities/SurfaceMaterialData.h"
    "../../include/mtt/utilities/UID.h"
    "../../include/mtt/utilities/ViewFrustum.h"
    )

set(SOURCES
    "CommonMeshGeometry.cpp"
    "Log.cpp"
    "StringRegistry.cpp"
    "Surface.cpp"
    "UID.cpp"
    "ViewFrustum.cpp"
    )

add_library(utilities
            ${HEADERS}
            ${SOURCES})
