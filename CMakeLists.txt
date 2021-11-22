cmake_minimum_required (VERSION 3.8)

project(mtt)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/Utilities)
add_subdirectory(src/Render)
add_subdirectory(src/Fbx)
add_subdirectory(src/DrawImpl)
add_subdirectory(src/Application)
add_subdirectory(ObjectEditor)
add_subdirectory(IDGenerator)
