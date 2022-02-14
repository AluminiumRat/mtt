cmake_minimum_required (VERSION 3.8)

project(render)

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
    "../../include/mtt/render/CommandQueue/CommandBuffer.h"
    "../../include/mtt/render/CommandQueue/CommandPool.h"
    "../../include/mtt/render/CommandQueue/CommandProducer.h"
    "../../include/mtt/render/CommandQueue/CommandQueue.h"
    "../../include/mtt/render/CommandQueue/Fence.h"
    "../../include/mtt/render/CommandQueue/QueueFamilyInfo.h"
    "../../include/mtt/render/CommandQueue/ScaledVolatileDescriptorPool.h"
    "../../include/mtt/render/CommandQueue/Semaphore.h"
    "../../include/mtt/render/CommandQueue/UniformMemoryPool.h"
    "../../include/mtt/render/CommandQueue/VolatileDescriptorPool.h"

    "../../include/mtt/render/Drawable/AntiscaleDrawableModificator.h"
    "../../include/mtt/render/Drawable/AutoscaleDrawableModificator.h"
    "../../include/mtt/render/Drawable/BoneMatricesSetter.h"
    "../../include/mtt/render/Drawable/Drawable.h"
    "../../include/mtt/render/Drawable/DrawableModificator.h"
    "../../include/mtt/render/Drawable/DrawableList.h"
    "../../include/mtt/render/Drawable/LODDrawableFilter.h"
    "../../include/mtt/render/Drawable/RotateToCameraModificator.h"
    "../../include/mtt/render/Drawable/SelectionDrawableModificator.h"
    "../../include/mtt/render/Drawable/UidDrawableModificator.h"
    "../../include/mtt/render/Drawable/VisibleDrawableFilter.h"

    "../../include/mtt/render/DrawPlan/AbstractAction.h"
    "../../include/mtt/render/DrawPlan/AbstractFramePlan.h"
    "../../include/mtt/render/DrawPlan/ActionMemoryHolder.h"
    "../../include/mtt/render/DrawPlan/ActionMemoryPool.h"
    "../../include/mtt/render/DrawPlan/DrawBin.h"
    "../../include/mtt/render/DrawPlan/DrawContext.h"
    "../../include/mtt/render/DrawPlan/DrawMeshAction.h"
    "../../include/mtt/render/DrawPlan/DrawPlan.h"
    "../../include/mtt/render/DrawPlan/DrawPlanBuildExtraData.h"
    "../../include/mtt/render/DrawPlan/DrawPlanBuildInfo.h"
    "../../include/mtt/render/DrawPlan/MultipleResourceSetter.h"
    "../../include/mtt/render/DrawPlan/ResourceSetter.h"
    "../../include/mtt/render/DrawPlan/StageIndex.h"
    "../../include/mtt/render/DrawPlan/UpdateResourcesAction.h"

    "../../include/mtt/render/Mesh/AbstractMeshTechnique.h"
    "../../include/mtt/render/Mesh/BaseGeometryTechnique.h"
    "../../include/mtt/render/Mesh/CompositeMeshTechnique.h"
    "../../include/mtt/render/Mesh/DepthMeshTechnique.h"
    "../../include/mtt/render/Mesh/Mesh.h"
    "../../include/mtt/render/Mesh/MeshExtraData.h"
    "../../include/mtt/render/Mesh/ModificatorProxyTechnique.h"
    "../../include/mtt/render/Mesh/MeshTechniquesFactory.h"
    "../../include/mtt/render/Mesh/MeshVariable.h"
    "../../include/mtt/render/Mesh/MonotopologyMeshTechnique.h"
    "../../include/mtt/render/Mesh/TechniquesCollection.h"
    "../../include/mtt/render/Mesh/UidMeshTechnique.h"

    "../../include/mtt/render/Pipeline/AbstractPipeline.h"
    "../../include/mtt/render/Pipeline/AbstractTexture.h"
    "../../include/mtt/render/Pipeline/Buffer.h"
    "../../include/mtt/render/Pipeline/ComputePipeline.h"
    "../../include/mtt/render/Pipeline/CubeTexture.h"
    "../../include/mtt/render/Pipeline/GraphicsPipeline.h"
    "../../include/mtt/render/Pipeline/InputAttachment.h"
    "../../include/mtt/render/Pipeline/PipelineResource.h"
    "../../include/mtt/render/Pipeline/PipelineResourceSet.h"
    "../../include/mtt/render/Pipeline/Sampler.h"
    "../../include/mtt/render/Pipeline/ShaderCache.h"
    "../../include/mtt/render/Pipeline/ShaderLoader.h"
    "../../include/mtt/render/Pipeline/ShaderModule.h"
    "../../include/mtt/render/Pipeline/StorageImageResource.h"
    "../../include/mtt/render/Pipeline/Texture2D.h"
    "../../include/mtt/render/Pipeline/VertexAttribute.h"
    "../../include/mtt/render/Pipeline/VertexIndices.h"
    "../../include/mtt/render/Pipeline/VolatileUniform.h"

    "../../include/mtt/render/RenderPass/AbstractFilter.h"
    "../../include/mtt/render/RenderPass/AbstractRenderPass.h"
    "../../include/mtt/render/RenderPass/DepthMapRenderPass.h"
    "../../include/mtt/render/RenderPass/FrameBuffer.h"
    "../../include/mtt/render/RenderPass/GeneralRenderPass.h"
    "../../include/mtt/render/RenderPass/UIDRenderPass.h"

    "../../include/mtt/render/SceneGraph/AbstractField.h"
    "../../include/mtt/render/SceneGraph/AbstractNode.h"
    "../../include/mtt/render/SceneGraph/AreaModificator.h"
    "../../include/mtt/render/SceneGraph/AreaModificatorSet.h"
    "../../include/mtt/render/SceneGraph/BoundedNode.h"
    "../../include/mtt/render/SceneGraph/BoundObserver.h"
    "../../include/mtt/render/SceneGraph/CameraNode.h"
    "../../include/mtt/render/SceneGraph/CombinedDrawableNode.h"
    "../../include/mtt/render/SceneGraph/DrawableNode.h"
    "../../include/mtt/render/SceneGraph/DrawVisitor.h"
    "../../include/mtt/render/SceneGraph/FieldArea.h"
    "../../include/mtt/render/SceneGraph/GroupNode.h"
    "../../include/mtt/render/SceneGraph/Joint.h"
    "../../include/mtt/render/SceneGraph/ProtectedGroupNode.h"
    "../../include/mtt/render/SceneGraph/SimpleDrawableNode.h"
    "../../include/mtt/render/SceneGraph/UnsortedField.h"

    "../../include/mtt/render/SceneRenderer/AbstractColorFrameBuilder.h"
    "../../include/mtt/render/SceneRenderer/AbstractFrame.h"
    "../../include/mtt/render/SceneRenderer/AbstractFrameBuilder.h"
    "../../include/mtt/render/SceneRenderer/AbstractSceneRenderer.h"
    "../../include/mtt/render/SceneRenderer/DepthFrameBuilder.h"
    "../../include/mtt/render/SceneRenderer/FrameType.h"
    "../../include/mtt/render/SceneRenderer/OneTargetFrameBuilder.h"
    "../../include/mtt/render/SceneRenderer/SceneToImageRenderer.h"
    "../../include/mtt/render/SceneRenderer/SceneToSurfaceRenderer.h"
    "../../include/mtt/render/SceneRenderer/UidFrameBuilder.h"

    "../../include/mtt/render/Utilities/DescriptorCounter.h"

    "../../include/mtt/render/CompositeRenderObject.h"
    "../../include/mtt/render/Image.h"
    "../../include/mtt/render/ImageView.h"
    "../../include/mtt/render/Lockable.h"
    "../../include/mtt/render/LogicalDevice.h"
    "../../include/mtt/render/PhysicalDevice.h"
    "../../include/mtt/render/PlainBuffer.h"
    "../../include/mtt/render/Ref.h"
    "../../include/mtt/render/RenderLibInstance.h"
    "../../include/mtt/render/RenderScene.h"
    "../../include/mtt/render/RenderSurface.h"
    "../../include/mtt/render/SwapChain.h"
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
    "Drawable/BoneMatricesSetter.cpp"
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
    "DrawPlan/DrawPlanBuildExtraData.cpp"
    "DrawPlan/DrawPlanBuildInfo.cpp"

    "Mesh/AbstractMeshTechnique.cpp"
    "Mesh/BaseGeometryTechnique.cpp"
    "Mesh/CompositeMeshTechnique.cpp"
    "Mesh/DepthMeshTechnique.cpp"
    "Mesh/Mesh.cpp"
    "Mesh/MeshExtraData.cpp"
    "Mesh/ModificatorProxyTechnique.cpp"
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
    "Pipeline/ShaderLoader.cpp"
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

    "SceneGraph/AreaModificator.cpp"
    "SceneGraph/BoundedNode.cpp"
    "SceneGraph/CameraNode.cpp"
    "SceneGraph/CombinedDrawableNode.cpp"
    "SceneGraph/DrawableNode.cpp"
    "SceneGraph/DrawVisitor.cpp"
    "SceneGraph/FieldArea.cpp"
    "SceneGraph/Joint.cpp"
    "SceneGraph/ProtectedGroupNode.cpp"
    "SceneGraph/SimpleDrawableNode.cpp"
    "SceneGraph/UnsortedField.cpp"

    "SceneRenderer/AbstractColorFrameBuilder.cpp"
    "SceneRenderer/AbstractFrameBuilder.cpp"
    "SceneRenderer/DepthFrameBuilder.cpp"
    "SceneRenderer/SceneToImageRenderer.cpp"
    "SceneRenderer/SceneToSurfaceRenderer.cpp"
    "SceneRenderer/UidFrameBuilder.cpp"

    "Utilities/DescriptorCounter.cpp"

    "CompositeRenderObject.cpp"
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

add_library(render
            ${HEADERS}
            ${SOURCES})
