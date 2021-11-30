cmake_minimum_required (VERSION 3.8)

project(DrawImpl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_path(GLM_PATH
          glm/glm.hpp
          HINTS "$ENV{GLM_PATH}"
          REQUIRED)

find_path(VKMEMALLOC_PATH
          vk_mem_alloc.h
          HINTS "$ENV{VKMEMALLOC_PATH}"
          REQUIRED)

find_package(Vulkan REQUIRED)

set(CMAKE_MODULE_PATH &{CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
find_package(Shaderc REQUIRED)

include_directories("../../include"
                    ${GLM_PATH}
                    ${Vulkan_INCLUDE_DIRS}
                    ${Shaderc_INCLUDE_DIRS}
                    ${VKMEMALLOC_PATH})

set(HEADERS
    "../../include/mtt/DrawImpl/Lighting/AbstractLightApplicator.h"
    "../../include/mtt/DrawImpl/Lighting/AmbientLightApplicator.h"
    "../../include/mtt/DrawImpl/Lighting/DirectLightApplicator.h"

    "../../include/mtt/DrawImpl/Lighting/ShadowmapBuilder.h"
    "../../include/mtt/DrawImpl/Lighting/ShadowMapProvider.h"

    "../../include/mtt/DrawImpl/MeshTechniques/EmissionTechnique.h"
    "../../include/mtt/DrawImpl/MeshTechniques/InstrumentalCompositeTechnique.h"
    "../../include/mtt/DrawImpl/MeshTechniques/InstrumentalTechnique.h"
    "../../include/mtt/DrawImpl/MeshTechniques/MeshCompositeColorTechnique.h"
    "../../include/mtt/DrawImpl/MeshTechniques/OpaqueColorTechnique.h"
    "../../include/mtt/DrawImpl/MeshTechniques/SelectionMeshTechnique.h"
    "../../include/mtt/DrawImpl/MeshTechniques/ShadowmapMeshTechnique.h"
    "../../include/mtt/DrawImpl/MeshTechniques/TransparentColorTechnique.h"

    "../../include/mtt/DrawImpl/RenderPass/AdditionalColorPass.h"
    "../../include/mtt/DrawImpl/RenderPass/AmbientWeightPass.h"
    "../../include/mtt/DrawImpl/RenderPass/BackgroundPass.h"
    "../../include/mtt/DrawImpl/RenderPass/EmissionPass.h"
    "../../include/mtt/DrawImpl/RenderPass/GBufferPass.h"
    "../../include/mtt/DrawImpl/RenderPass/LightingPass.h"
    "../../include/mtt/DrawImpl/RenderPass/PeelingGPass.h"
    "../../include/mtt/DrawImpl/RenderPass/PeelingLayPass.h"
    "../../include/mtt/DrawImpl/RenderPass/PeelingMergePass.h"
    "../../include/mtt/DrawImpl/RenderPass/ShadowmapRenderPass.h"

    "../../include/mtt/DrawImpl/BackgroundDrawable.h"
    "../../include/mtt/DrawImpl/ColorFrameBuilder.h"
    "../../include/mtt/DrawImpl/constants.h"
    "../../include/mtt/DrawImpl/TransparencyPipeline.h"
    )

set(SOURCES
    "Lighting/AbstractLightApplicator.cpp"
    "Lighting/AmbientLightApplicator.cpp"
    "Lighting/DirectLightApplicator.cpp"

    "Lighting/ShadowmapBuilder.cpp"
    "Lighting/ShadowMapProvider.cpp"

    "MeshTechniques/EmissionTechnique.cpp"
    "MeshTechniques/InstrumentalCompositeTechnique.cpp"
    "MeshTechniques/InstrumentalTechnique.cpp"
    "MeshTechniques/MeshCompositeColorTechnique.cpp"
    "MeshTechniques/OpaqueColorTechnique.cpp"
    "MeshTechniques/SelectionMeshTechnique.cpp"
    "MeshTechniques/ShadowmapMeshTechnique.cpp"
    "MeshTechniques/TransparentColorTechnique.cpp"

    "RenderPass/AdditionalColorPass.cpp"
    "RenderPass/AmbientWeightPass.cpp"
    "RenderPass/BackgroundPass.cpp"
    "RenderPass/EmissionPass.cpp"
    "RenderPass/GBufferPass.cpp"
    "RenderPass/LightingPass.cpp"
    "RenderPass/PeelingGPass.cpp"
    "RenderPass/PeelingLayPass.cpp"
    "RenderPass/PeelingMergePass.cpp"
    "RenderPass/ShadowmapRenderPass.cpp"

    "BackgroundDrawable.cpp"
    "ColorFrameBuilder.cpp"
    "TransparencyPipeline.cpp"
    )

add_library(DrawImpl
            ${HEADERS}
            ${SOURCES})
