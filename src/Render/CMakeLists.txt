cmake_minimum_required (VERSION 3.8)

project(Render)

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
    "../../include/mtt/Render/CommandQueue/CommandBuffer.h"
    "../../include/mtt/Render/CommandQueue/CommandPool.h"
    "../../include/mtt/Render/CommandQueue/CommandProducer.h"
    "../../include/mtt/Render/CommandQueue/CommandQueue.h"
    "../../include/mtt/Render/CommandQueue/Fence.h"
    "../../include/mtt/Render/CommandQueue/QueueFamilyInfo.h"
    "../../include/mtt/Render/CommandQueue/ScaledVolatileDescriptorPool.h"
    "../../include/mtt/Render/CommandQueue/Semaphore.h"
    "../../include/mtt/Render/CommandQueue/UniformMemoryPool.h"
    "../../include/mtt/Render/CommandQueue/VolatileDescriptorPool.h"

    "../../include/mtt/Render/Drawable/AntiscaleDrawableModificator.h"
    "../../include/mtt/Render/Drawable/AutoscaleDrawableModificator.h"
    "../../include/mtt/Render/Drawable/Drawable.h"
    "../../include/mtt/Render/Drawable/DrawableList.h"
    "../../include/mtt/Render/Drawable/LODDrawableFilter.h"
    "../../include/mtt/Render/Drawable/RotateToCameraModificator.h"
    "../../include/mtt/Render/Drawable/SelectionDrawableModificator.h"
    "../../include/mtt/Render/Drawable/UidDrawableModificator.h"
    "../../include/mtt/Render/Drawable/VisibleDrawableFilter.h"

    "../../include/mtt/Render/DrawPlan/AbstractAction.h"
    "../../include/mtt/Render/DrawPlan/AbstractFramePlan.h"
    "../../include/mtt/Render/DrawPlan/ActionMemoryHolder.h"
    "../../include/mtt/Render/DrawPlan/ActionMemoryPool.h"
    "../../include/mtt/Render/DrawPlan/DrawBin.h"
    "../../include/mtt/Render/DrawPlan/DrawContext.h"
    "../../include/mtt/Render/DrawPlan/DrawMeshAction.h"
    "../../include/mtt/Render/DrawPlan/DrawPlan.h"
    "../../include/mtt/Render/DrawPlan/DrawPlanBuildInfo.h"
    "../../include/mtt/Render/DrawPlan/MultipleResourceSetter.h"
    "../../include/mtt/Render/DrawPlan/ResourceSetter.h"
    "../../include/mtt/Render/DrawPlan/StageIndex.h"

    "../../include/mtt/Render/Mesh/AbstractMeshTechnique.h"
    "../../include/mtt/Render/Mesh/BaseGeometryTechnique.h"
    "../../include/mtt/Render/Mesh/CompositeMeshTechnique.h"
    "../../include/mtt/Render/Mesh/DepthMeshTechnique.h"
    "../../include/mtt/Render/Mesh/Mesh.h"
    "../../include/mtt/Render/Mesh/MeshExtraData.h"
    "../../include/mtt/Render/Mesh/MeshVariable.h"
    "../../include/mtt/Render/Mesh/MonotopologyMeshTechnique.h"
    "../../include/mtt/Render/Mesh/TechniquesCollection.h"
    "../../include/mtt/Render/Mesh/UidMeshTechnique.h"

    "../../include/mtt/Render/Pipeline/AbstractPipeline.h"
    "../../include/mtt/Render/Pipeline/AbstractTexture.h"
    "../../include/mtt/Render/Pipeline/Buffer.h"
    "../../include/mtt/Render/Pipeline/ComputePipeline.h"
    "../../include/mtt/Render/Pipeline/CubeTexture.h"
    "../../include/mtt/Render/Pipeline/GraphicsPipeline.h"
    "../../include/mtt/Render/Pipeline/InputAttachment.h"
    "../../include/mtt/Render/Pipeline/PipelineResource.h"
    "../../include/mtt/Render/Pipeline/PipelineResourceSet.h"
    "../../include/mtt/Render/Pipeline/Sampler.h"
    "../../include/mtt/Render/Pipeline/ShaderCache.h"
    "../../include/mtt/Render/Pipeline/ShaderModule.h"
    "../../include/mtt/Render/Pipeline/StorageImageResource.h"
    "../../include/mtt/Render/Pipeline/Texture2D.h"
    "../../include/mtt/Render/Pipeline/VertexAttribute.h"
    "../../include/mtt/Render/Pipeline/VertexIndices.h"
    "../../include/mtt/Render/Pipeline/VolatileUniform.h"

    "../../include/mtt/Render/RenderPass/AbstractFilter.h"
    "../../include/mtt/Render/RenderPass/AbstractRenderPass.h"
    "../../include/mtt/Render/RenderPass/DepthMapRenderPass.h"
    "../../include/mtt/Render/RenderPass/FrameBuffer.h"
    "../../include/mtt/Render/RenderPass/GeneralRenderPass.h"
    "../../include/mtt/Render/RenderPass/UIDRenderPass.h"

    "../../include/mtt/Render/SceneGraph/AbstractField.h"
    "../../include/mtt/Render/SceneGraph/AbstractNode.h"
    "../../include/mtt/Render/SceneGraph/CameraNode.h"
    "../../include/mtt/Render/SceneGraph/CombinedDrawableNode.h"
    "../../include/mtt/Render/SceneGraph/DrawableNode.h"
    "../../include/mtt/Render/SceneGraph/DrawVisitor.h"
    "../../include/mtt/Render/SceneGraph/GroupNode.h"
    "../../include/mtt/Render/SceneGraph/Joint.h"
    "../../include/mtt/Render/SceneGraph/SimpleDrawableNode.h"
    "../../include/mtt/Render/SceneGraph/UnsortedField.h"
    "../../include/mtt/Render/SceneGraph/VisitedField.h"

    "../../include/mtt/Render/SceneRenderer/AbstractColorFrameBuilder.h"
    "../../include/mtt/Render/SceneRenderer/AbstractFrame.h"
    "../../include/mtt/Render/SceneRenderer/AbstractFrameBuilder.h"
    "../../include/mtt/Render/SceneRenderer/AbstractSceneRenderer.h"
    "../../include/mtt/Render/SceneRenderer/DepthFrameBuilder.h"
    "../../include/mtt/Render/SceneRenderer/FrameType.h"
    "../../include/mtt/Render/SceneRenderer/OneTargetFrameBuilder.h"
    "../../include/mtt/Render/SceneRenderer/SceneToImageRenderer.h"
    "../../include/mtt/Render/SceneRenderer/SceneToSurfaceRenderer.h"
    "../../include/mtt/Render/SceneRenderer/UidFrameBuilder.h"

    "../../include/mtt/Render/Utilities/DescriptorCounter.h"

    "../../include/mtt/Render/Image.h"
    "../../include/mtt/Render/ImageView.h"
    "../../include/mtt/Render/Lockable.h"
    "../../include/mtt/Render/LogicalDevice.h"
    "../../include/mtt/Render/PhysicalDevice.h"
    "../../include/mtt/Render/PlainBuffer.h"
    "../../include/mtt/Render/Ref.h"
    "../../include/mtt/Render/RenderLibInstance.h"
    "../../include/mtt/Render/RenderScene.h"
    "../../include/mtt/Render/RenderSurface.h"
    "../../include/mtt/Render/SwapChain.h"
    )

set(SOURCES
    "CommandQueue/CommandBuffer.cpp"
    "CommandQueue/CommandPool.cpp"
    "CommandQueue/CommandProducer.cpp"
    "CommandQueue/CommandQueue.cpp"
    "CommandQueue/Fence.cpp"
    "CommandQueue/QueueFamilyInfo.cpp"
    "CommandQueue/ScaledVolatileDescriptorPool.cpp"
    "CommandQueue/Semaphore.cpp"
    "CommandQueue/UniformMemoryPool.cpp"
    "CommandQueue/VolatileDescriptorPool.cpp"

    "Drawable/AntiscaleDrawableModificator.cpp"
    "Drawable/AutoscaleDrawableModificator.cpp"
    "Drawable/Drawable.cpp"
    "Drawable/DrawableList.cpp"
    "Drawable/LODDrawableFilter.cpp"
    "Drawable/RotateToCameraModificator.cpp"
    "Drawable/SelectionDrawableModificator.cpp"
    "Drawable/UidDrawableModificator.cpp"
    "Drawable/VisibleDrawableFilter.cpp"

    "DrawPlan/AbstractFramePlan.cpp"
    "DrawPlan/ActionMemoryPool.cpp"
    "DrawPlan/DrawBin.cpp"
    "DrawPlan/DrawPlan.cpp"
    "DrawPlan/DrawPlanBuildInfo.cpp"

    "Mesh/AbstractMeshTechnique.cpp"
    "Mesh/BaseGeometryTechnique.cpp"
    "Mesh/CompositeMeshTechnique.cpp"
    "Mesh/DepthMeshTechnique.cpp"
    "Mesh/Mesh.cpp"
    "Mesh/MeshExtraData.cpp"
    "Mesh/MonotopologyMeshTechnique.cpp"
    "Mesh/UidMeshTechnique.cpp"

    "Pipeline/AbstractPipeline.cpp"
    "Pipeline/AbstractTexture.cpp"
    "Pipeline/Buffer.cpp"
    "Pipeline/ComputePipeline.cpp"
    "Pipeline/CubeTexture.cpp"
    "Pipeline/GraphicsPipeline.cpp"
    "Pipeline/InputAttachment.cpp"
    "Pipeline/PipelineResourceSet.cpp"
    "Pipeline/Sampler.cpp"
    "Pipeline/ShaderCache.cpp"
    "Pipeline/ShaderModule.cpp"
    "Pipeline/StorageImageResource.cpp"
    "Pipeline/Texture2D.cpp"
    "Pipeline/VertexAttribute.cpp"
    "Pipeline/VertexIndices.cpp"

    "RenderPass/AbstractFilter.cpp"
    "RenderPass/AbstractRenderPass.cpp"
    "RenderPass/DepthMapRenderPass.cpp"
    "RenderPass/FrameBuffer.cpp"
    "RenderPass/GeneralRenderPass.cpp"
    "RenderPass/UIDRenderPass.cpp"

    "SceneGraph/AbstractField.cpp"
    "SceneGraph/CameraNode.cpp"
    "SceneGraph/CombinedDrawableNode.cpp"
    "SceneGraph/DrawableNode.cpp"
    "SceneGraph/DrawVisitor.cpp"
    "SceneGraph/GroupNode.cpp"
    "SceneGraph/Joint.cpp"
    "SceneGraph/SimpleDrawableNode.cpp"
    "SceneGraph/UnsortedField.cpp"

    "SceneRenderer/AbstractColorFrameBuilder.cpp"
    "SceneRenderer/AbstractFrameBuilder.cpp"
    "SceneRenderer/DepthFrameBuilder.cpp"
    "SceneRenderer/SceneToImageRenderer.cpp"
    "SceneRenderer/SceneToSurfaceRenderer.cpp"
    "SceneRenderer/UidFrameBuilder.cpp"

    "Utilities/DescriptorCounter.cpp"

    "Image.cpp"
    "ImageView.cpp"
    "Lockable.cpp"
    "LogicalDevice.cpp"
    "PhysicalDevice.cpp"
    "PlainBuffer.cpp"
    "RenderLibInstance.cpp"
    "RenderScene.cpp"
    "RenderSurface.cpp"
    "SwapChain.cpp"
    )

add_library(Render
            STATIC
            ${HEADERS}
            ${SOURCES})
