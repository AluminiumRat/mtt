cmake_minimum_required (VERSION 3.8)

project(dlPipeline)

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
    "../../include/mtt/dlPipeline/Lighting/AbstractLightApplicator.h"
    "../../include/mtt/dlPipeline/Lighting/AmbientLightApplicator.h"
    "../../include/mtt/dlPipeline/Lighting/DirectLightApplicator.h"

    "../../include/mtt/dlPipeline/Lighting/ShadowmapBuilder.h"
    "../../include/mtt/dlPipeline/Lighting/ShadowMapProvider.h"

    "../../include/mtt/dlPipeline/MeshTechniques/EmissionTechnique.h"
    "../../include/mtt/dlPipeline/MeshTechniques/InstrumentalCompositeTechnique.h"
    "../../include/mtt/dlPipeline/MeshTechniques/InstrumentalTechnique.h"
    "../../include/mtt/dlPipeline/MeshTechniques/MeshCompositeColorTechnique.h"
    "../../include/mtt/dlPipeline/MeshTechniques/ModelTechniquesFactory.h"
    "../../include/mtt/dlPipeline/MeshTechniques/OpaqueColorTechnique.h"
    "../../include/mtt/dlPipeline/MeshTechniques/SelectionMeshTechnique.h"
    "../../include/mtt/dlPipeline/MeshTechniques/ShadowmapMeshTechnique.h"
    "../../include/mtt/dlPipeline/MeshTechniques/TransparentColorTechnique.h"

    "../../include/mtt/dlPipeline/RenderPass/AdditionalColorPass.h"
    "../../include/mtt/dlPipeline/RenderPass/AmbientWeightPass.h"
    "../../include/mtt/dlPipeline/RenderPass/BackgroundPass.h"
    "../../include/mtt/dlPipeline/RenderPass/EmissionPass.h"
    "../../include/mtt/dlPipeline/RenderPass/GBufferPass.h"
    "../../include/mtt/dlPipeline/RenderPass/LightingPass.h"
    "../../include/mtt/dlPipeline/RenderPass/PeelingGPass.h"
    "../../include/mtt/dlPipeline/RenderPass/PeelingLayPass.h"
    "../../include/mtt/dlPipeline/RenderPass/PeelingMergePass.h"
    "../../include/mtt/dlPipeline/RenderPass/ShadowmapRenderPass.h"

    "../../include/mtt/dlPipeline/BackgroundDrawable.h"
    "../../include/mtt/dlPipeline/ColorFrameBuilder.h"
    "../../include/mtt/dlPipeline/constants.h"
    "../../include/mtt/dlPipeline/TransparencyPipeline.h"
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
    "MeshTechniques/ModelTechniquesFactory.cpp"
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

add_library(dlPipeline
            ${HEADERS}
            ${SOURCES})
