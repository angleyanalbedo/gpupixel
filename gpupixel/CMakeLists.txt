################################################################################
# GPUPixel CMake配置文件
################################################################################

# 基础项目配置
################################################################################
cmake_minimum_required(VERSION 3.10)

# 定义项目名称并初始化项目
set(PROJECT_NAME "gpupixel")
project(${PROJECT_NAME})

# 设置安装位置为输出目录
set(CMAKE_INSTALL_PREFIX ${GPUPIXEL_ROOT_DIR}/output)

# 设置C++标准和编译器选项
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# 设置cmake模块路径
set(CMAKE_MODULE_PATH "${GPUPIXEL_ROOT_DIR}/gpupixel")

# 定义API导出宏（Windows DLL导出）
add_definitions(-DMYMATH_EXPORT_LIBRARY)


# 源文件配置
################################################################################
# 项目核心头文件包含目录
include_directories(
    ${GPUPIXEL_ROOT_DIR}
    ${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/libyuv/include
)

# 主要源文件目录列表
set(GPUPIXEL_SOURCE_DIRS 
    "core"
    "filter"
    "source"
    "sink"
    "face_detector"
    "utils"
    "third_party/glad/include"
    "third_party/glfw/include"
)

# 收集主要源文件
unset(SOURCE_CODE_FILES CACHE)
foreach(DIR ${GPUPIXEL_SOURCE_DIRS})
    file(GLOB DIR_SOURCES "${GPUPIXEL_ROOT_DIR}/gpupixel/${DIR}/*")
    list(APPEND SOURCE_CODE_FILES ${DIR_SOURCES})
endforeach()

# 添加第三方库源文件
file(GLOB LIBYUV_SOURCES "${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/libyuv/source/*")
list(APPEND SOURCE_CODE_FILES ${LIBYUV_SOURCES})

message(STATUS "已添加源代码文件")

# 收集资源文件
file(GLOB RESOURCE_FILES 
    "${GPUPIXEL_ROOT_DIR}/gpupixel/res*"  
    "${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mars-face-kit/mod*"
)

message(STATUS "已添加资源文件")

# 平台特定源文件配置
################################################################################
if(${CURRENT_OS} STREQUAL "windows" OR ${CURRENT_OS} STREQUAL "linux")
    # Windows和Linux添加GLAD源文件
    file(GLOB GLAD_SOURCE_FILE "${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/glad/src/*.c")
    list(APPEND SOURCE_CODE_FILES ${GLAD_SOURCE_FILE})
    
    if(${CURRENT_OS} STREQUAL "windows")
        # Windows特定配置
        set(CMAKE_SHARED_LIBRARY_PREFIX "")
    endif()
elseif(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    # macOS和iOS添加Objective-C源文件
    file(GLOB OBJC_SOURCE_FILE "${GPUPIXEL_ROOT_DIR}/gpupixel/sink/objc/*")
    list(APPEND SOURCE_CODE_FILES ${OBJC_SOURCE_FILE})
elseif(${CURRENT_OS} STREQUAL "android")
    # Android添加JNI源文件
    file(GLOB JNI_SOURCE_FILE "${GPUPIXEL_ROOT_DIR}/gpupixel/android/jni/*")
    list(APPEND SOURCE_CODE_FILES ${JNI_SOURCE_FILE})
endif()

# 头文件安装配置
################################################################################

if(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    # macOS/iOS平台使用framework结构和标准include目录
    set(FRAMEWORK_HEADER_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.framework/Headers")
    # 同时安装到framework和标准目录
    install(FILES ${GPUPIXEL_ROOT_DIR}/gpupixel/gpupixel.h
            DESTINATION ${FRAMEWORK_HEADER_DIR}/)
 
    # 同时安装到framework和标准目录
    foreach(DIR ${GPUPIXEL_SOURCE_DIRS})
        install(DIRECTORY ${GPUPIXEL_ROOT_DIR}/gpupixel/${DIR}/
                DESTINATION ${FRAMEWORK_HEADER_DIR}/${DIR}
                FILES_MATCHING PATTERN "*.h")
    endforeach()
 
endif()

# 其他平台只使用标准include目录
################################################################################
set(STANDARD_HEADER_DIR "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}")

install(FILES ${GPUPIXEL_ROOT_DIR}/gpupixel/gpupixel.h
        DESTINATION ${STANDARD_HEADER_DIR}/)

foreach(DIR ${GPUPIXEL_SOURCE_DIRS})
    install(DIRECTORY ${GPUPIXEL_ROOT_DIR}/gpupixel/${DIR}/
            DESTINATION ${STANDARD_HEADER_DIR}/gpupixel/${DIR}
            FILES_MATCHING PATTERN "*.h")
endforeach()

install(FILES ${GPUPIXEL_ROOT_DIR}/gpupixel/gpupixel.h
        DESTINATION ${STANDARD_HEADER_DIR}/)
# 资源文件安装
install(DIRECTORY 
    ${GPUPIXEL_ROOT_DIR}/gpupixel/res/
    DESTINATION ${OUTPUT_INSTALL_PATH}/res
)

install(DIRECTORY 
    ${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mars-face-kit/models/
    DESTINATION ${OUTPUT_INSTALL_PATH}/models
)

# 库配置
################################################################################
# 创建共享库
add_library(${PROJECT_NAME} SHARED ${SOURCE_CODE_FILES} ${RESOURCE_FILES})

# macOS/iOS Framework配置
if(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_PRODUCT_NAME ${PROJECT_NAME}
        COMPILE_FLAGS "-x objective-c++"
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER net.pixpark.${PROJECT_NAME}
        PRODUCT_BUNDLE_IDENTIFIER net.pixpark.${PROJECT_NAME}
        CMAKE_XCODE_ATTRIBUTE_BUILT_PRODUCTS_DIR ${PROJECT_NAME}
        MACOSX_FRAMEWORK_INFO_PLIST ${GPUPIXEL_ROOT_DIR}/gpupixel/Info.plist
        FRAMEWORK_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/
        RESOURCE "${RESOURCE_FILES}"
    )
endif()

# 第三方库安装
if(${CURRENT_OS} STREQUAL "linux")
    install(FILES
        ${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.so
        DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )
elseif(${CURRENT_OS} STREQUAL "windows")
    install(FILES
        ${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mars-face-kit/libs/${CURRENT_OS}/msvc-x64/mars-face-kit.dll
        DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )
endif()

# 安装目标库
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    FRAMEWORK DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# 平台特定库配置
################################################################################
# Linux平台配置
if(${CURRENT_OS} STREQUAL "linux")
    add_library(mars-face-kit SHARED IMPORTED)
    set_target_properties(mars-face-kit PROPERTIES IMPORTED_LOCATION
        ${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.so)

# Windows平台配置
elseif(${CURRENT_OS} STREQUAL "windows")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")

# macOS/iOS平台配置
elseif(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    # Framework属性已在前面设置
    
# Android平台配置
elseif(${CURRENT_OS} STREQUAL "android")
    add_library(mars-face-kit SHARED IMPORTED)
    set_target_properties(mars-face-kit PROPERTIES IMPORTED_LOCATION
        ${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mars-face-kit/libs/${CURRENT_OS}/${ANDROID_ABI}/libmars-face-kit.so)
endif()

# 链接库依赖
################################################################################
# Linux平台依赖
if(${CURRENT_OS} STREQUAL "linux")
    target_link_libraries(${PROJECT_NAME} PRIVATE
        GL
        glfw
        mars-face-kit
    )

# Windows平台依赖
elseif(${CURRENT_OS} STREQUAL "windows")
    target_link_libraries(${PROJECT_NAME} 
        opengl32
        ${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/glfw/libs/msvc-x64/glfw3.lib
        ${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mars-face-kit/libs/${CURRENT_OS}/msvc-x64/mars-face-kit.lib
    )

# macOS平台依赖
elseif(${CURRENT_OS} STREQUAL "macos")
    target_link_libraries(${PROJECT_NAME}
        "-framework OpenGL"
        "-framework AppKit"
        "-framework QuartzCore"
        "-framework CoreVideo"
        "-framework CoreGraphics"
        "-framework AVFoundation"
        "-framework CoreMedia"
        "-framework Metal"
        "${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mnn/libs/${CURRENT_OS}/MNN.framework"
        "${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.a"
    )

# iOS平台依赖
elseif(${CURRENT_OS} STREQUAL "ios")
    target_link_libraries(${PROJECT_NAME}
        "-framework OpenGLES"     
        "-framework UIKit"     
        "-framework QuartzCore"  
        "-framework CoreVideo"  
        "-framework CoreGraphics"
        "-framework AVFoundation"
        "-framework CoreMedia"
        "-framework Metal"
        "-framework CoreML"
        "${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mnn/libs/${CURRENT_OS}/MNN.framework"
        "${GPUPIXEL_ROOT_DIR}/gpupixel/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.a"
    )

# Android平台依赖
elseif(${CURRENT_OS} STREQUAL "android")
    target_link_libraries(${PROJECT_NAME}
        log
        android
        GLESv3
        EGL
        jnigraphics
        mars-face-kit
    )
endif()