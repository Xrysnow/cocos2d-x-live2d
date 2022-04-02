cmake_minimum_required(VERSION 3.6)

project(CubismCore)
add_library(Live2DCubismCore STATIC IMPORTED GLOBAL)

set(CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if(ANDROID)
  set_target_properties(Live2DCubismCore
    PROPERTIES
      IMPORTED_LOCATION
        ${CORE_PATH}/lib/android/${ANDROID_ABI}/libLive2DCubismCore.a
  )
elseif(LINUX)
  set_target_properties(Live2DCubismCore
    PROPERTIES
      IMPORTED_LOCATION
        ${CORE_PATH}/lib/linux/x86_64/libLive2DCubismCore.a
  )
elseif(WINDOWS)
  # Detect compiler.
  if(MSVC_VERSION MATCHES 1900)
    # Visual Studio 2015
    set(MSVC_NUM 140)
  elseif(MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS 1920)
    # Visual Studio 2017
    set(MSVC_NUM 141)
  elseif(MSVC_VERSION GREATER_EQUAL 1920)
    # Visual Studio 2019
    set(MSVC_NUM 142)
  elseif(MSVC)
    message(FATAL_ERROR "Unsupported Visual C++ compiler used.")
  else()
    message(FATAL_ERROR "Unsupported compiler used.")
  endif()

  # Detect core crt.
  if(CORE_CRL_MD)
    set(CRT MD)
  else()
    set(CRT MT)
  endif()

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_target_properties(Live2DCubismCore
      PROPERTIES
        IMPORTED_LOCATION_DEBUG
          ${CORE_PATH}/lib/windows/x86_64/${MSVC_NUM}/Live2DCubismCore_${CRT}d.lib
        IMPORTED_LOCATION_RELEASE
          ${CORE_PATH}/lib/windows/x86_64/${MSVC_NUM}/Live2DCubismCore_MD.lib
    )
  else()
    set_target_properties(Live2DCubismCore
      PROPERTIES
        IMPORTED_LOCATION_DEBUG
          ${CORE_PATH}/lib/windows/x86/${MSVC_NUM}/Live2DCubismCore_${CRT}d.lib
        IMPORTED_LOCATION_RELEASE
          ${CORE_PATH}/lib/windows/x86/${MSVC_NUM}/Live2DCubismCore_MD.lib
    )
  endif()
elseif(APPLE)
  if(IOS)
    # iPhone Simulator (i386) is not support on Cubism Core.
    if(IOS_PLATFORM STREQUAL "SIMULATOR")
      message(FATAL_ERROR
        "Cubism Core does not support i386 iOS architecture."
      )
    endif()

    set_target_properties(Live2DCubismCore
      PROPERTIES
        IMPORTED_LOCATION_DEBUG
          ${CORE_PATH}/lib/ios/Debug-iphoneos/libLive2DCubismCore.a
        IMPORTED_LOCATION_RELEASE
          ${CORE_PATH}/lib/ios/Release-iphoneos/libLive2DCubismCore.a
    )
  elseif(MACOSX)
    set_target_properties(Live2DCubismCore
      PROPERTIES
        IMPORTED_LOCATION
          ${CORE_PATH}/lib/macos/libLive2DCubismCore.a
    )
  endif()
endif()
# Set core include directory.
set_target_properties(Live2DCubismCore
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CORE_PATH}/include
)
