CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(GPUPixel)

# 平台检测
################################################################################
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CURRENT_OS "linux")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CURRENT_OS "windows")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CURRENT_OS "macos")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "iOS")
    set(CURRENT_OS "ios")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    set(CURRENT_OS "android")
else()
    message(FATAL_ERROR "不支持当前操作系统: ${CMAKE_SYSTEM_NAME}")
endif()

message(STATUS "构建平台: ${CURRENT_OS}")

# 定义项目根目录变量
set(GPUPixel_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 添加是否构建demo的选项，默认不构建
option(BUILD_DEMO "Build the demo applications" OFF)

# Add src directory first
add_subdirectory(gpupixel)

# 仅当BUILD_DEMO选项开启时才添加demo目录
if(BUILD_DEMO)
    message(STATUS "Building demo applications")
    add_subdirectory(demo)
endif()