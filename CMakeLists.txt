cmake_minimum_required(VERSION 3.10)

set(LIB_NAME cc_live2d)
project(${LIB_NAME})

add_library(${LIB_NAME} STATIC)
add_dependencies(${LIB_NAME} cocos2d)
use_cocos2dx_compile_define(${LIB_NAME})
use_cocos2dx_compile_options(${LIB_NAME})

add_subdirectory(Core)
target_link_libraries(${LIB_NAME} Live2DCubismCore)

set(SRC_DIR Framework/src)

target_sources(${LIB_NAME}
  PRIVATE
    CubismOffscreenSurface_CC.cpp
    CubismOffscreenSurface_CC.h
    CubismRenderer_CC.cpp
    CubismRenderer_CC.h
    L2DFramework.cpp
    L2DFramework.h
    L2DModel.cpp
    L2DModel.h
    LAppAllocator.cpp
    LAppAllocator.hpp
    LAppDefine.cpp
    LAppDefine.hpp
    LAppModel.cpp
    LAppModel.hpp
    LAppPal.cpp
    LAppPal.hpp
    ${SRC_DIR}/CubismCdiJson.cpp
    ${SRC_DIR}/CubismCdiJson.hpp
    ${SRC_DIR}/CubismDefaultParameterId.cpp
    ${SRC_DIR}/CubismDefaultParameterId.hpp
    ${SRC_DIR}/CubismFramework.cpp
    ${SRC_DIR}/CubismFramework.hpp
    ${SRC_DIR}/CubismFrameworkConfig.hpp
    ${SRC_DIR}/CubismModelSettingJson.cpp
    ${SRC_DIR}/CubismModelSettingJson.hpp
    ${SRC_DIR}/ICubismAllocator.hpp
    ${SRC_DIR}/ICubismModelSetting.hpp
    ${SRC_DIR}/Live2DCubismCore.hpp
)

# Add sub directories.
add_subdirectory(${SRC_DIR}/Effect)
add_subdirectory(${SRC_DIR}/Id)
add_subdirectory(${SRC_DIR}/Math)
add_subdirectory(${SRC_DIR}/Model)
add_subdirectory(${SRC_DIR}/Motion)
add_subdirectory(${SRC_DIR}/Physics)
target_sources(${LIB_NAME}
  PRIVATE
    ${SRC_DIR}/Rendering/CubismRenderer.cpp
    ${SRC_DIR}/Rendering/CubismRenderer.hpp
)
add_subdirectory(${SRC_DIR}/Type)
add_subdirectory(${SRC_DIR}/Utils)

# Add include path.
target_include_directories(${LIB_NAME}
  PUBLIC
    ${SRC_DIR}
  PRIVATE
    ${SRC_DIR}/Rendering
    Core/include
    ${COCOS2DX_ROOT_PATH}/cocos
    ${COCOS2DX_ROOT_PATH}/cocos/base
    ${COCOS2DX_ROOT_PATH}/cocos/platform
    ${COCOS2DX_ROOT_PATH}/external
    ${COCOS2DX_ROOT_PATH}/external/glfw3/include/win32
)

if(BUILD_LUA_LIBS)
  target_compile_definitions(${LIB_NAME} PUBLIC _USRLUASTATIC)
  target_sources(${LIB_NAME}
    PRIVATE
      lua_live2d_auto.cpp
      lua_live2d_auto.hpp
  )
  target_include_directories(${LIB_NAME}
    PRIVATE
      ${COCOS2DX_ROOT_PATH}/external/lua/luajit/include
      ${COCOS2DX_ROOT_PATH}/external/lua/tolua
  )
endif()

if(WINDOWS)
  target_include_directories(${LIB_NAME}
    PRIVATE
      ${COCOS2DX_ROOT_PATH}/external/win32-specific/gles/include/OGLES
  )
endif()

if(XCODE OR VS)
    cocos_mark_code_files(${LIB_NAME})
endif()

target_compile_definitions(${LIB_NAME} PUBLIC CSM_TARGET_COCOS)

set_target_properties(${LIB_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
)
