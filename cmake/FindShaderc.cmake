find_path(Shaderc_INCLUDE_DIRS
          NAMES shaderc/shaderc.h
          HINTS "$ENV{VULKAN_SDK}/include")

if(Shaderc_INCLUDE_DIRS)
  find_library( Shaderc_LIBRARY_DEBUG
                NAMES shadercd
                HINTS "$ENV{VULKAN_SDK}/lib"
                REQUIRED)

  find_library( Shaderc_LIBRARY_RELEASE
                NAMES shaderc
                HINTS "$ENV{VULKAN_SDK}/lib"
                REQUIRED)

  find_library( Shaderccombined_LIBRARY_DEBUG
                NAMES shaderc_combinedd
                HINTS "$ENV{VULKAN_SDK}/lib"
                REQUIRED)
  set(Shaderc_LIBRARY_DEBUG ${Shaderc_LIBRARY_DEBUG} ${Shaderccombined_LIBRARY_DEBUG})

  find_library( Shaderccombined_LIBRARY_RELEASE
                NAMES shaderc_combined
                HINTS "$ENV{VULKAN_SDK}/lib"
                REQUIRED)
  set(Shaderc_LIBRARY_RELEASE ${Shaderc_LIBRARY_RELEASE} ${Shaderccombined_LIBRARY_RELEASE})

  find_library( Glslang_LIBRARY_DEBUG
                NAMES glslangd
                HINTS "$ENV{VULKAN_SDK}/lib"
                REQUIRED)
  set(Shaderc_LIBRARY_DEBUG ${Shaderc_LIBRARY_DEBUG} ${Glslang_LIBRARY_DEBUG})

  find_library( Glslang_LIBRARY_RELEASE
                NAMES glslang
                HINTS "$ENV{VULKAN_SDK}/lib"
                REQUIRED)
  set(Shaderc_LIBRARY_RELEASE ${Shaderc_LIBRARY_RELEASE} ${Glslang_LIBRARY_RELEASE})

  find_library( Shadercutil_LIBRARY_DEBUG
                NAMES shaderc_utild
                HINTS "$ENV{VULKAN_SDK}/lib"
                REQUIRED)
  set(Shaderc_LIBRARY_DEBUG ${Shaderc_LIBRARY_DEBUG} ${Shadercutil_LIBRARY_DEBUG})

  find_library( Shadercutil_LIBRARY_RELEASE
                NAMES shaderc_util
                HINTS "$ENV{VULKAN_SDK}/lib"
                REQUIRED)
  set(Shaderc_LIBRARY_RELEASE ${Shaderc_LIBRARY_RELEASE} ${Glslang_LIBRARY_RELEASE})

  mark_as_advanced( Shaderc_LIBRARY_DEBUG
                    Shaderc_LIBRARY_RELEASE
                    Shaderccombined_LIBRARY_DEBUG
                    Shaderccombined_LIBRARY_RELEASE
                    Glslang_LIBRARY_DEBUG
                    Glslang_LIBRARY_RELEASE
                    Shadercutil_LIBRARY_DEBUG
                    Shadercutil_LIBRARY_RELEASE)

  include(SelectLibraryConfigurations)
  select_library_configurations(Shaderc)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Shaderc DEFAULT_MSG Shaderc_LIBRARIES Shaderc_INCLUDE_DIRS)
