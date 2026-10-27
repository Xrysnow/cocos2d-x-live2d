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
  elseif(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930)
    # Visual Studio 2019
    set(MSVC_NUM 142)
  elseif(MSVC_VERSION GREATER_EQUAL 1930)
    # Visual Studio 2022
    set(MSVC_NUM 143)
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
    # CMake accepts either an SDK name or a full path to a versioned .sdk.
    get_filename_component(CORE_IOS_SYSROOT "${CMAKE_OSX_SYSROOT}" NAME)
    string(TOLOWER "${CORE_IOS_SYSROOT}" CORE_IOS_SYSROOT)
    if(CORE_IOS_SYSROOT MATCHES "^iphonesimulator([0-9.]+)?(\\.sdk)?$")
      set(CORE_IOS_SDK iphonesimulator)
      set(CORE_IOS_ARCH x86_64)
    elseif(CORE_IOS_SYSROOT MATCHES "^iphoneos([0-9.]+)?(\\.sdk)?$")
      set(CORE_IOS_SDK iphoneos)
      set(CORE_IOS_ARCH arm64)
    else()
      message(FATAL_ERROR "Cubism Core requires an iOS SDK: set CMAKE_OSX_SYSROOT "
        "to iphoneos, iphonesimulator or the corresponding .sdk path; "
        "got '${CMAKE_OSX_SYSROOT}'.")
    endif()

    # The bundled Core has arm64 device and x86_64 simulator archives only.
    # A single imported archive cannot serve both SDKs in a combined build.
    if(CMAKE_IOS_INSTALL_COMBINED)
      message(FATAL_ERROR "Cubism Core does not support combined device/simulator "
        "builds. Configure each iOS SDK in a separate build directory.")
    endif()
    if(CORE_IOS_SDK STREQUAL "iphonesimulator" AND NOT CMAKE_OSX_ARCHITECTURES)
      message(FATAL_ERROR "Bundled Cubism Core for iphonesimulator supports only "
        "x86_64. Set CMAKE_OSX_ARCHITECTURES=x86_64 explicitly; the SDK's default "
        "architecture may be unsupported (arm64).")
    endif()
    foreach(CORE_IOS_REQUESTED_ARCH IN LISTS CMAKE_OSX_ARCHITECTURES)
      if(NOT CORE_IOS_REQUESTED_ARCH STREQUAL CORE_IOS_ARCH)
        message(FATAL_ERROR "Bundled Cubism Core for ${CORE_IOS_SDK} supports only "
          "${CORE_IOS_ARCH}; requested '${CORE_IOS_REQUESTED_ARCH}' in "
          "CMAKE_OSX_ARCHITECTURES. A matching Core binary is required.")
      endif()
    endforeach()
    foreach(CORE_IOS_CONFIG Debug Release)
      set(CORE_IOS_LIBRARY "${CORE_PATH}/lib/ios/${CORE_IOS_CONFIG}-${CORE_IOS_SDK}/libLive2DCubismCore.a")
      if(NOT EXISTS "${CORE_IOS_LIBRARY}")
        message(FATAL_ERROR "Missing Cubism Core ${CORE_IOS_CONFIG} library: ${CORE_IOS_LIBRARY}")
      endif()
    endforeach()

    set_target_properties(Live2DCubismCore
      PROPERTIES
        IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
        IMPORTED_LOCATION
          "${CORE_PATH}/lib/ios/Release-${CORE_IOS_SDK}/libLive2DCubismCore.a"
        IMPORTED_LOCATION_DEBUG
          "${CORE_PATH}/lib/ios/Debug-${CORE_IOS_SDK}/libLive2DCubismCore.a"
        IMPORTED_LOCATION_RELEASE
          "${CORE_PATH}/lib/ios/Release-${CORE_IOS_SDK}/libLive2DCubismCore.a"
        MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
        MAP_IMPORTED_CONFIG_MINSIZEREL Release
        MAP_IMPORTED_CONFIG_NOCONFIG Release
    )
  elseif(MACOSX)
    # Respect explicit cross/universal builds; otherwise use CMake's target CPU.
    set(CORE_MACOS_ARCHITECTURES "${CMAKE_OSX_ARCHITECTURES}")
    if(NOT CORE_MACOS_ARCHITECTURES)
      set(CORE_MACOS_ARCHITECTURES "${CMAKE_SYSTEM_PROCESSOR}")
    endif()
    if(NOT CORE_MACOS_ARCHITECTURES)
      message(FATAL_ERROR "Cannot determine the macOS architecture for Cubism Core. "
        "Set CMAKE_OSX_ARCHITECTURES to x86_64, arm64 or x86_64;arm64.")
    endif()
    list(REMOVE_DUPLICATES CORE_MACOS_ARCHITECTURES)
    set(CORE_MACOS_LIBRARIES)
    foreach(CORE_MACOS_ARCH IN LISTS CORE_MACOS_ARCHITECTURES)
      if(NOT CORE_MACOS_ARCH STREQUAL "x86_64" AND NOT CORE_MACOS_ARCH STREQUAL "arm64")
        message(FATAL_ERROR "Bundled Cubism Core for macOS supports only x86_64 and arm64; "
          "requested '${CORE_MACOS_ARCH}'. Set CMAKE_OSX_ARCHITECTURES explicitly.")
      endif()
      set(CORE_MACOS_LIBRARY "${CORE_PATH}/lib/macos/${CORE_MACOS_ARCH}/libLive2DCubismCore.a")
      if(NOT EXISTS "${CORE_MACOS_LIBRARY}")
        message(FATAL_ERROR "Missing Cubism Core macOS ${CORE_MACOS_ARCH} library: ${CORE_MACOS_LIBRARY}")
      endif()
      list(APPEND CORE_MACOS_LIBRARIES "${CORE_MACOS_LIBRARY}")
    endforeach()

    list(LENGTH CORE_MACOS_ARCHITECTURES CORE_MACOS_ARCH_COUNT)
    if(CORE_MACOS_ARCH_COUNT GREATER 1)
      # Keep the supplied thin archives intact; generate the universal library in the build tree.
      if(CMAKE_LIPO)
        set(CORE_MACOS_LIPO "${CMAKE_LIPO}")
      else()
        find_program(CORE_MACOS_LIPO NAMES lipo)
      endif()
      if(NOT CORE_MACOS_LIPO)
        message(FATAL_ERROR "A macOS universal Cubism Core build requires lipo. Set CMAKE_LIPO to its path.")
      endif()
      set(CORE_MACOS_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/macos/universal/libLive2DCubismCore.a")
      add_custom_command(OUTPUT "${CORE_MACOS_LIBRARY}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/macos/universal"
        COMMAND "${CORE_MACOS_LIPO}" -create ${CORE_MACOS_LIBRARIES} -output "${CORE_MACOS_LIBRARY}"
        DEPENDS ${CORE_MACOS_LIBRARIES}
        COMMENT "Combining macOS Cubism Core architectures"
        VERBATIM
      )
      add_custom_target(Live2DCubismCore_universal DEPENDS "${CORE_MACOS_LIBRARY}")
      add_dependencies(Live2DCubismCore Live2DCubismCore_universal)
    endif()
    set_target_properties(Live2DCubismCore
      PROPERTIES
        IMPORTED_LOCATION "${CORE_MACOS_LIBRARY}"
    )
  endif()
endif()
# Set core include directory.
set_target_properties(Live2DCubismCore
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CORE_PATH}/include
)
