cmake_minimum_required (VERSION 3.8)

project(mtt)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(applications/ObjectEditor)
add_subdirectory(applications/IDGenerator)
add_subdirectory(src/utilities)
add_subdirectory(src/render)
add_subdirectory(src/fbx)
add_subdirectory(src/clPipeline)
add_subdirectory(src/dlPipeline)
add_subdirectory(src/editorLib)
add_subdirectory(src/application)
