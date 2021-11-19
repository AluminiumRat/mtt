cmake_minimum_required (VERSION 3.8)

project(Core)

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
    "../../include/mtt/Core/AsyncTasks/AbstractAsyncTask.h"
    "../../include/mtt/Core/AsyncTasks/AsyncTaskQueue.h"

    "../../include/mtt/Core/EditCommands/AbstractEditCommand.h"
    "../../include/mtt/Core/EditCommands/AddObjectCommand.h"
    "../../include/mtt/Core/EditCommands/CompositeCommand.h"
    "../../include/mtt/Core/EditCommands/MoveObjectCommand.h"
    "../../include/mtt/Core/EditCommands/RemoveObjectCommand.h"
    "../../include/mtt/Core/EditCommands/SetPropertyCommand.h"
    "../../include/mtt/Core/EditCommands/UndoStack.h"

    "../../include/mtt/Core/Manipulator/AbstractManipulator.h"
    "../../include/mtt/Core/Manipulator/AxisMove3DManipulator.h"
    "../../include/mtt/Core/Manipulator/Composite3DManipulator.h"
    "../../include/mtt/Core/Manipulator/Manipulator3D.h"
    "../../include/mtt/Core/Manipulator/ManipulatorController.h"
    "../../include/mtt/Core/Manipulator/PlaneMove3DManipulator.h"
    "../../include/mtt/Core/Manipulator/RingRotation3DManipulator.h"
    "../../include/mtt/Core/Manipulator/Simple3DManipulator.h"
    "../../include/mtt/Core/Manipulator/UniformScale3DManipulator.h"

    "../../include/mtt/Core/ResourceManager/CubeTextureDescription.h"
    "../../include/mtt/Core/ResourceManager/CubeTextureLibrary.h"
    "../../include/mtt/Core/ResourceManager/ResourceLibrary.h"
    "../../include/mtt/Core/ResourceManager/Texture2DDescription.h"
    "../../include/mtt/Core/ResourceManager/Texture2DLibrary.h"

    "../../include/mtt/Core/Scene/Object.h"
    "../../include/mtt/Core/Scene/object_cast.h"
    "../../include/mtt/Core/Scene/ObjectGroup.h"
    "../../include/mtt/Core/Scene/ObjectItemListModel.h"
    "../../include/mtt/Core/Scene/ObjectItemTreeModel.h"
    "../../include/mtt/Core/Scene/ObjectLink.h"
    "../../include/mtt/Core/Scene/ObjectVisitor.h"
    "../../include/mtt/Core/Scene/Scene.h"
    "../../include/mtt/Core/Scene/SpecialGroup.h"

    "../../include/mtt/Core/Widgets/AsyncTaskDialog/AsyncTaskDialog.h"
    "../../include/mtt/Core/Widgets/AsyncTaskDialog/TaskProgressWidget.h"

    "../../include/mtt/Core/Widgets/PropertiesWidgets/BoolPropertyWidget.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/ColorPropertyWidget.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/ColorWidget.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/FloatPropertyLinearSlider.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/FloatPropertyProgressiveSlider.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/FloatPropertySpin.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/IntPropertySpin.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/ReferencePropertyWidget.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/ReferenceWidget.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/RotationPropertyWidget.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/StringPropertyWidget.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/Vec3PropertyWidget.h"
    "../../include/mtt/Core/Widgets/PropertiesWidgets/Vec3Widget.h"

    "../../include/mtt/Core/Widgets/RenderWidget/ClickSelectHandler.h"
    "../../include/mtt/Core/Widgets/RenderWidget/OrbitalCameraController.h"
    "../../include/mtt/Core/Widgets/RenderWidget/RenderWidget.h"

    "../../include/mtt/Core/Widgets/ObjectsTreeView.h"
    "../../include/mtt/Core/Widgets/SelectObjectsDialog.h"
    "../../include/mtt/Core/Widgets/StatisticWidget.h"

    "../../include/mtt/Core/WorkCycle/RenderImageAction.h"
    "../../include/mtt/Core/WorkCycle/RenderSceneAction.h"
    "../../include/mtt/Core/WorkCycle/WorkCycle.h"

    "../../include/mtt/Core/ApplicationStatistic.h"
    "../../include/mtt/Core/Appliction.h"
    "../../include/mtt/Core/CommonEditData.h"
    )

set(UI_FILES
    "Widgets/AsyncTaskDialog/TaskProgressWidget.ui"
    "Widgets/PropertiesWidgets/ColorPropertyWidget.ui"
    "Widgets/PropertiesWidgets/ReferencePropertyWidget.ui"
    "Widgets/PropertiesWidgets/Vec3PropertyWidget.ui"
    "Widgets/SelectObjectsDialog.ui"
    "Widgets/StatisticWidget.ui"
    )

set(SOURCES
    "AsyncTasks/AbstractAsyncTask.cpp"
    "AsyncTasks/AsyncTaskQueue.cpp"

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

add_library(Core
            ${HEADERS}
            ${SOURCES}
            ${UI_FILES}
            )

target_link_libraries(Core
                      Qt5::Core
                      Qt5::Widgets
                      )
