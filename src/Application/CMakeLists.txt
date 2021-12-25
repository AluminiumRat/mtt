cmake_minimum_required (VERSION 3.8)

project(Application)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_path(GLM_PATH
          glm/glm.hpp
          HINTS "$ENV{GLM_PATH}"
          REQUIRED)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Vulkan REQUIRED)

set(CMAKE_MODULE_PATH &{CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
find_package(Shaderc REQUIRED)

include_directories("../../include"
                    ${GLM_PATH}
                    ${Vulkan_INCLUDE_DIRS}
                    ${Shaderc_INCLUDE_DIRS}
                    ${VKMEMALLOC_PATH}
                    )

add_definitions(${Qt5Core_DEFINITIONS}
                ${Qt5Widgets_DEFINITIONS}
                )

set(HEADERS
    "../../include/mtt/Application/AsyncTasks/AbstractAsyncTask.h"
    "../../include/mtt/Application/AsyncTasks/AsyncTaskQueue.h"

    "../../include/mtt/Application/DrawModel/DrawModel.h"
    "../../include/mtt/Application/DrawModel/DrawModelAnimation.h"
    "../../include/mtt/Application/DrawModel/DrawModelAnimationTrack.h"
    "../../include/mtt/Application/DrawModel/DrawModelLoader.h"
    "../../include/mtt/Application/DrawModel/MeshControlNode.h"
    "../../include/mtt/Application/DrawModel/SkinControlNode.h"
    "../../include/mtt/Application/DrawModel/SlaveDrawModel.h"
    "../../include/mtt/Application/DrawModel/TransformTable.h"

    "../../include/mtt/Application/EditCommands/AbstractEditCommand.h"
    "../../include/mtt/Application/EditCommands/AddObjectCommand.h"
    "../../include/mtt/Application/EditCommands/CompositeCommand.h"
    "../../include/mtt/Application/EditCommands/MoveObjectCommand.h"
    "../../include/mtt/Application/EditCommands/RemoveObjectCommand.h"
    "../../include/mtt/Application/EditCommands/SetPropertyCommand.h"
    "../../include/mtt/Application/EditCommands/UndoStack.h"

    "../../include/mtt/Application/Manipulator/AbstractManipulator.h"
    "../../include/mtt/Application/Manipulator/AxisMove3DManipulator.h"
    "../../include/mtt/Application/Manipulator/Composite3DManipulator.h"
    "../../include/mtt/Application/Manipulator/Manipulator3D.h"
    "../../include/mtt/Application/Manipulator/ManipulatorController.h"
    "../../include/mtt/Application/Manipulator/PlaneMove3DManipulator.h"
    "../../include/mtt/Application/Manipulator/RingRotation3DManipulator.h"
    "../../include/mtt/Application/Manipulator/Simple3DManipulator.h"
    "../../include/mtt/Application/Manipulator/UniformScale3DManipulator.h"

    "../../include/mtt/Application/ResourceManager/CubeTextureDescription.h"
    "../../include/mtt/Application/ResourceManager/CubeTextureLibrary.h"
    "../../include/mtt/Application/ResourceManager/ResourceLibrary.h"
    "../../include/mtt/Application/ResourceManager/Texture2DDescription.h"
    "../../include/mtt/Application/ResourceManager/Texture2DLibrary.h"

    "../../include/mtt/Application/Scene/Object.h"
    "../../include/mtt/Application/Scene/object_cast.h"
    "../../include/mtt/Application/Scene/ObjectGroup.h"
    "../../include/mtt/Application/Scene/ObjectItemListModel.h"
    "../../include/mtt/Application/Scene/ObjectItemTreeModel.h"
    "../../include/mtt/Application/Scene/ObjectLink.h"
    "../../include/mtt/Application/Scene/ObjectVisitor.h"
    "../../include/mtt/Application/Scene/Scene.h"
    "../../include/mtt/Application/Scene/SpecialGroup.h"

    "../../include/mtt/Application/Widgets/AsyncTaskDialog/AsyncTaskDialog.h"
    "../../include/mtt/Application/Widgets/AsyncTaskDialog/TaskProgressWidget.h"

    "../../include/mtt/Application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/BoolPropertyWidget.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/ColorPropertyWidget.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/ColorWidget.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/FloatPropertyLinearSlider.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/FloatPropertyProgressiveSlider.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/FloatPropertySpin.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/FloatSpinConnection.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/IntPropertySpin.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/IntSpinConnection.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/NamePropertyWidget.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/ReferenceWidget.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/RotationPropertyWidget.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/StringPropertyWidget.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h"
    "../../include/mtt/Application/Widgets/PropertiesWidgets/Vec3Widget.h"

    "../../include/mtt/Application/Widgets/RenderWidget/ClickSelectHandler.h"
    "../../include/mtt/Application/Widgets/RenderWidget/OrbitalCameraController.h"
    "../../include/mtt/Application/Widgets/RenderWidget/RenderWidget.h"

    "../../include/mtt/Application/Widgets/ObjectsTreeView.h"
    "../../include/mtt/Application/Widgets/SelectObjectsDialog.h"
    "../../include/mtt/Application/Widgets/StatisticWidget.h"

    "../../include/mtt/Application/WorkCycle/RenderImageAction.h"
    "../../include/mtt/Application/WorkCycle/RenderSceneAction.h"
    "../../include/mtt/Application/WorkCycle/WorkCycle.h"

    "../../include/mtt/Application/ApplicationStatistic.h"
    "../../include/mtt/Application/Application.h"
    "../../include/mtt/Application/CommonEditData.h"
    "../../include/mtt/Application/DataStream.h"
    )

set(UI_FILES
    "Widgets/AsyncTaskDialog/TaskProgressWidget.ui"
    "Widgets/PropertiesWidgets/ColorPropertyWidget.ui"
    "Widgets/PropertiesWidgets/NamePropertyWidget.ui"
    "Widgets/PropertiesWidgets/ReferencePropertyWidget.ui"
    "Widgets/PropertiesWidgets/Vec3PropertyWidget.ui"
    "Widgets/SelectObjectsDialog.ui"
    "Widgets/StatisticWidget.ui"
    )

set(SOURCES
    "AsyncTasks/AbstractAsyncTask.cpp"
    "AsyncTasks/AsyncTaskQueue.cpp"

    "DrawModel/DrawModel.cpp"
    "DrawModel/DrawModelAnimation.cpp"
    "DrawModel/DrawModelAnimationTrack.cpp"
    "DrawModel/DrawModelLoader.cpp"
    "DrawModel/MeshControlNode.cpp"
    "DrawModel/SkinControlNode.cpp"
    "DrawModel/SlaveDrawModel.cpp"

    "EditCommands/AddObjectCommand.cpp"
    "EditCommands/CompositeCommand.cpp"
    "EditCommands/MoveObjectCommand.cpp"
    "EditCommands/RemoveObjectCommand.cpp"
    "EditCommands/UndoStack.cpp"

    "Manipulator/AbstractManipulator.cpp"
    "Manipulator/AxisMove3DManipulator.cpp"
    "Manipulator/Composite3DManipulator.cpp"
    "Manipulator/ManipulatorController.cpp"
    "Manipulator/PlaneMove3DManipulator.cpp"
    "Manipulator/RingRotation3DManipulator.cpp"
    "Manipulator/Simple3DManipulator.cpp"
    "Manipulator/UniformScale3DManipulator.cpp"

    "ResourceManager/CubeTextureLibrary.cpp"
    "ResourceManager/Texture2DLibrary.cpp"

    "Scene/Object.cpp"
    "Scene/ObjectGroup.cpp"
    "Scene/ObjectItemListModel.cpp"
    "Scene/ObjectItemTreeModel.cpp"
    "Scene/ObjectLink.cpp"
    "Scene/ObjectVisitor.cpp"
    "Scene/Scene.cpp"

    "Widgets/AsyncTaskDialog/AsyncTaskDialog.cpp"
    "Widgets/AsyncTaskDialog/TaskProgressWidget.cpp"
    "Widgets/PropertiesWidgets/ColorWidget.cpp"
    "Widgets/PropertiesWidgets/NamePropertyWidget.cpp"
    "Widgets/PropertiesWidgets/ReferenceWidget.cpp"
    "Widgets/PropertiesWidgets/Vec3Widget.cpp"
    "Widgets/RenderWidget/ClickSelectHandler.cpp"
    "Widgets/RenderWidget/OrbitalCameraController.cpp"
    "Widgets/RenderWidget/RenderWidget.cpp"
    "Widgets/ObjectsTreeView.cpp"
    "Widgets/SelectObjectsDialog.cpp"
    "Widgets/StatisticWidget.cpp"

    "WorkCycle/RenderImageAction.cpp"
    "WorkCycle/RenderSceneAction.cpp"
    "WorkCycle/WorkCycle.cpp"

    "Application.cpp"
    "CommonEditData.cpp"
    )

add_library(Application
            ${HEADERS}
            ${SOURCES}
            ${UI_FILES}
            )

target_link_libraries(Application
                      Qt5::Core
                      Qt5::Widgets
                      )
