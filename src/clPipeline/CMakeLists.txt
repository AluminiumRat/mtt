cmake_minimum_required (VERSION 3.8)

project(clPipeline)

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
    "../../include/mtt/clPipeline/Background/Background.h"
    "../../include/mtt/clPipeline/Background/BackgroundAreaModificator.h"
    "../../include/mtt/clPipeline/Background/BackgroundDrawable.h"
    "../../include/mtt/clPipeline/Background/BackgroundProperties.h"

    "../../include/mtt/clPipeline/Lighting/AbstractLight.h"
    "../../include/mtt/clPipeline/Lighting/AmbientLight.h"
    "../../include/mtt/clPipeline/Lighting/AbstractLightApplicator.h"
    "../../include/mtt/clPipeline/Lighting/AmbientLightApplicator.h"
    "../../include/mtt/clPipeline/Lighting/AmbientLightAreaModificator.h"
    "../../include/mtt/clPipeline/Lighting/AmbientLightData.h"
    "../../include/mtt/clPipeline/Lighting/DirectLightApplicator.h"
    "../../include/mtt/clPipeline/Lighting/ShadowmapBuilder.h"
    "../../include/mtt/clPipeline/Lighting/ShadowmapProvider.h"

    "../../include/mtt/clPipeline/MeshTechniques/EmissionTechnique.h"
    "../../include/mtt/clPipeline/MeshTechniques/FLColorTechnique.h"
    "../../include/mtt/clPipeline/MeshTechniques/InstrumentalCompositeTechnique.h"
    "../../include/mtt/clPipeline/MeshTechniques/MeshCompositeColorTechnique.h"
    "../../include/mtt/clPipeline/MeshTechniques/ModelTechniquesFactory.h"
    "../../include/mtt/clPipeline/MeshTechniques/OpaqueColorTechnique.h"
    "../../include/mtt/clPipeline/MeshTechniques/SelectionMeshTechnique.h"
    "../../include/mtt/clPipeline/MeshTechniques/ShadowmapMeshTechnique.h"
    "../../include/mtt/clPipeline/MeshTechniques/TransparentProxyTechnique.h"
    "../../include/mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h"

    "../../include/mtt/clPipeline/RenderPass/AmbientWeightPass.h"
    "../../include/mtt/clPipeline/RenderPass/BackgroundPass.h"
    "../../include/mtt/clPipeline/RenderPass/EmissionPass.h"
    "../../include/mtt/clPipeline/RenderPass/ForwardLightPass.h"
    "../../include/mtt/clPipeline/RenderPass/GBufferPass.h"
    "../../include/mtt/clPipeline/RenderPass/LightingPass.h"
    "../../include/mtt/clPipeline/RenderPass/SelectionPass.h"
    "../../include/mtt/clPipeline/RenderPass/ShadowmapPass.h"

    "../../include/mtt/clPipeline/ColorFrameBuilder.h"
    "../../include/mtt/clPipeline/constants.h"
    )

set(SOURCES
    "Background/Background.cpp"
    "Background/BackgroundAreaModificator.cpp"
    "Background/BackgroundDrawable.cpp"

    "Lighting/AbstractLightApplicator.cpp"
    "Lighting/AmbientLight.cpp"
    "Lighting/AmbientLightApplicator.cpp"
    "Lighting/AmbientLightAreaModificator.cpp"
    "Lighting/DirectLightApplicator.cpp"
    "Lighting/ShadowmapBuilder.cpp"
    "Lighting/ShadowmapProvider.cpp"

    "MeshTechniques/EmissionTechnique.cpp"
    "MeshTechniques/FLColorTechnique.cpp"
    "MeshTechniques/InstrumentalCompositeTechnique.cpp"
    "MeshTechniques/MeshCompositeColorTechnique.cpp"
    "MeshTechniques/ModelTechniquesFactory.cpp"
    "MeshTechniques/OpaqueColorTechnique.cpp"
    "MeshTechniques/SelectionMeshTechnique.cpp"
    "MeshTechniques/ShadowmapMeshTechnique.cpp"
    "MeshTechniques/TransparentProxyTechnique.cpp"
    "MeshTechniques/UnlightedColorTechnique.cpp"

    "RenderPass/AmbientWeightPass.cpp"
    "RenderPass/BackgroundPass.cpp"
    "RenderPass/EmissionPass.cpp"
    "RenderPass/ForwardLightPass.cpp"
    "RenderPass/GBufferPass.cpp"
    "RenderPass/LightingPass.cpp"
    "RenderPass/SelectionPass.cpp"
    "RenderPass/ShadowmapPass.cpp"

    "ColorFrameBuilder.cpp"
    )

add_library(clPipeline
            ${HEADERS}
            ${SOURCES})
