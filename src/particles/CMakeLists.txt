cmake_minimum_required (VERSION 3.8)

project(particles)

set(CMAKE_MODULE_PATH &{CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_path(GLM_PATH
          glm/glm.hpp
          HINTS "$ENV{GLM_PATH}"
          REQUIRED)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Vulkan REQUIRED)

find_package(Shaderc REQUIRED)

find_path(VKMEMALLOC_PATH
          vk_mem_alloc.h
          HINTS "$ENV{VKMEMALLOC_PATH}"
          REQUIRED)

include_directories("../../include"
                    ${GLM_PATH}
                    ${Vulkan_INCLUDE_DIRS}
                    ${Shaderc_INCLUDE_DIRS}
                    ${VKMEMALLOC_PATH})

add_definitions(${Qt5Core_DEFINITIONS})

set(HEADERS
    "../../include/mtt/particles/DataSource/ParticlesDataSource.h"

    "../../include/mtt/particles/Drawable/ColorDrawParticlesAction.h"
    "../../include/mtt/particles/Drawable/ParticlesAbstractTechnique.h"
    "../../include/mtt/particles/Drawable/ParticlesColorTechnique.h"
    "../../include/mtt/particles/Drawable/ParticlesDrawable.h"
    "../../include/mtt/particles/Drawable/ParticlesDrawCommonData.h"
    "../../include/mtt/particles/Drawable/ParticlesProxyTechnique.h"
    "../../include/mtt/particles/Drawable/ParticlesShadowmapTechnique.h"
    "../../include/mtt/particles/Drawable/ShadowDrawParticlesAction.h"
    )

set(SOURCES
    "DataSource/ParticlesDataSource.cpp"

    "Drawable/ColorDrawParticlesAction.cpp"
    "Drawable/ParticlesAbstractTechnique.cpp"
    "Drawable/ParticlesColorTechnique.cpp"
    "Drawable/ParticlesDrawable.cpp"
    "Drawable/ParticlesDrawCommonData.cpp"
    "Drawable/ParticlesProxyTechnique.cpp"
    "Drawable/ParticlesShadowmapTechnique.cpp"
    "Drawable/ShadowDrawParticlesAction.cpp"
    )

add_library(particles
            ${HEADERS}
            ${SOURCES})

target_link_libraries(particles
                      Qt5::Core
                      )
