find_path(FbxSDK_INCLUDE_DIRS
          NAMES fbxsdk.h
          HINTS "$ENV{FBX_SDK}/include")

if(FbxSDK_INCLUDE_DIRS)
  find_library( FbxSDK_LIBRARY_DEBUG
                NAMES libfbxsdk-md
                PATH_SUFFIXES "debug"
                REQUIRED)

  find_library( FbxSDK_LIBRARY_RELEASE
                NAMES libfbxsdk-md
                PATH_SUFFIXES "release"
                REQUIRED)

  find_library( libxml2_LIBRARY_DEBUG
                NAMES libxml2-md
                PATH_SUFFIXES "debug"
                REQUIRED)
  set(FbxSDK_LIBRARY_DEBUG ${FbxSDK_LIBRARY_DEBUG} ${libxml2_LIBRARY_DEBUG})

  find_library( libxml2_LIBRARY_RELEASE
                NAMES libxml2-md
                PATH_SUFFIXES "release"
                REQUIRED)
  set(FbxSDK_LIBRARY_RELEASE ${FbxSDK_LIBRARY_RELEASE} ${libxml2_LIBRARY_RELEASE})

  find_library( zlib_LIBRARY_DEBUG
                NAMES zlib-md
                PATH_SUFFIXES "debug"
                REQUIRED)
  set(FbxSDK_LIBRARY_DEBUG ${FbxSDK_LIBRARY_DEBUG} ${zlib_LIBRARY_DEBUG})

  find_library( zlib_LIBRARY_RELEASE
                NAMES zlib-md
                PATH_SUFFIXES "release"
                REQUIRED)
  set(FbxSDK_LIBRARY_RELEASE ${FbxSDK_LIBRARY_RELEASE} ${zlib_LIBRARY_RELEASE})

  mark_as_advanced( FbxSDK_INCLUDE_DIRS
                    FbxSDK_LIBRARY_DEBUG
                    FbxSDK_LIBRARY_RELEASE
                    libxml2_LIBRARY_DEBUG
                    libxml2_LIBRARY_RELEASE
                    zlib_LIBRARY_DEBUG
                    zlib_LIBRARY_RELEASE)

  include(SelectLibraryConfigurations)
  select_library_configurations(FbxSDK)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(FbxSDK DEFAULT_MSG FbxSDK_LIBRARIES FbxSDK_INCLUDE_DIRS)
endif()
