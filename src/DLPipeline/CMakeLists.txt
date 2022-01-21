cmake_minimum_required (VERSION 3.8)

project(DLPipeline)

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
    "../../include/mtt/DLPipeline/Lighting/AbstractLightApplicator.h"
    "../../include/mtt/DLPipeline/Lighting/AmbientLightApplicator.h"
    "../../include/mtt/DLPipeline/Lighting/DirectLightApplicator.h"

    "../../include/mtt/DLPipeline/Lighting/ShadowmapBuilder.h"
    "../../include/mtt/DLPipeline/Lighting/ShadowMapProvider.h"

    "../../include/mtt/DLPipeline/MeshTechniques/EmissionTechnique.h"
    "../../include/mtt/DLPipeline/MeshTechniques/InstrumentalCompositeTechnique.h"
    "../../include/mtt/DLPipeline/MeshTechniques/InstrumentalTechnique.h"
    "../../include/mtt/DLPipeline/MeshTechniques/MeshCompositeColorTechnique.h"
    "../../include/mtt/DLPipeline/MeshTechniques/ModelTechniquesFactory.h"
    "../../include/mtt/DLPipeline/MeshTechniques/OpaqueColorTechnique.h"
    "../../include/mtt/DLPipeline/MeshTechniques/SelectionMeshTechnique.h"
    "../../include/mtt/DLPipeline/MeshTechniques/ShadowmapMeshTechnique.h"
    "../../include/mtt/DLPipeline/MeshTechniques/TransparentColorTechnique.h"

    "../../include/mtt/DLPipeline/RenderPass/AdditionalColorPass.h"
    "../../include/mtt/DLPipeline/RenderPass/AmbientWeightPass.h"
    "../../include/mtt/DLPipeline/RenderPass/BackgroundPass.h"
    "../../include/mtt/DLPipeline/RenderPass/EmissionPass.h"
    "../../include/mtt/DLPipeline/RenderPass/GBufferPass.h"
    "../../include/mtt/DLPipeline/RenderPass/LightingPass.h"
    "../../include/mtt/DLPipeline/RenderPass/PeelingGPass.h"
    "../../include/mtt/DLPipeline/RenderPass/PeelingLayPass.h"
    "../../include/mtt/DLPipeline/RenderPass/PeelingMergePass.h"
    "../../include/mtt/DLPipeline/RenderPass/ShadowmapRenderPass.h"

    "../../include/mtt/DLPipeline/BackgroundDrawable.h"
    "../../include/mtt/DLPipeline/ColorFrameBuilder.h"
    "../../include/mtt/DLPipeline/constants.h"
    "../../include/mtt/DLPipeline/TransparencyPipeline.h"
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

add_library(DLPipeline
            ${HEADERS}
            ${SOURCES})
