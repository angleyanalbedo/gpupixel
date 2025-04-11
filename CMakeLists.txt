CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(GPUPixel)

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