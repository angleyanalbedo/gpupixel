# Basic Project Configuration
# ----------------------------------------
cmake_minimum_required(VERSION 3.10)

# 设置安装位置为输出目录
set(CMAKE_INSTALL_PREFIX ${GPUPixel_ROOT_DIR}/output)

# Set C++ standard and compiler options
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Set cmake module path to current directory
set(CMAKE_MODULE_PATH "${GPUPixel_ROOT_DIR}/gpupixel")

# Define project name and initialize project
set(PROJECT_NAME "gpupixel")
project(${PROJECT_NAME})

# Define API export macros for Windows DLL exports
add_definitions(-DMYMATH_EXPORT_LIBRARY)

# Platform Detection
# ----------------------------------------
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
    message(FATAL_ERROR "NOT SUPPORT THIS SYSTEM")
endif()

# Output Directory Configuration
# ----------------------------------------
set(OUTPUT_INSTALL_PATH "${GPUPixel_ROOT_DIR}/output")
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY "${OUTPUT_INSTALL_PATH}/include")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_INSTALL_PATH}/library/${CURRENT_OS}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_INSTALL_PATH}/library/${CURRENT_OS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_INSTALL_PATH}/library/${CURRENT_OS}")

# Configure output directories for Debug/Release builds
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Source Files Configuration
# ----------------------------------------
# Core project headers
include_directories(
    ${GPUPixel_ROOT_DIR}
    ${GPUPixel_ROOT_DIR}/third_party/libyuv/include
)

# Collect source files
file(GLOB SOURCE_CODE_FILES     
    "${GPUPixel_ROOT_DIR}/gpupixel/core/*"        
    "${GPUPixel_ROOT_DIR}/gpupixel/filter/*"         
    "${GPUPixel_ROOT_DIR}/gpupixel/source/*"       
    "${GPUPixel_ROOT_DIR}/gpupixel/sink/*"                               
    "${GPUPixel_ROOT_DIR}/gpupixel/face_detector/*"                 
    "${GPUPixel_ROOT_DIR}/gpupixel/utils/*"                 
    "${GPUPixel_ROOT_DIR}/third_party/libyuv/source/*"
)


message(STATUS "SOURCE_CODE_FILES: ${SOURCE_CODE_FILES}")

# Collect resource files
file(GLOB RESOURCE_FILES 
    "${GPUPixel_ROOT_DIR}/gpupixel/res*"  
    "${GPUPixel_ROOT_DIR}/third_party/mars-face-kit/mod*"                     
)

message(STATUS "RESOURCE_FILES: ${RESOURCE_FILES}")
# Platform Specific Configuration
# ----------------------------------------
if(${CURRENT_OS} STREQUAL "windows")     
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    file(GLOB GLAD_SOURCE_FILE  "${GPUPixel_ROOT_DIR}/third_party/glad/src/*.c" )
    list(APPEND SOURCE_CODE_FILES ${GLAD_SOURCE_FILE})
elseif(${CURRENT_OS} STREQUAL "linux")    
    file(GLOB GLAD_SOURCE_FILE  "${GPUPixel_ROOT_DIR}/third_party/glad/src/*.c" )
    list(APPEND SOURCE_CODE_FILES ${GLAD_SOURCE_FILE})
elseif(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    file(GLOB OBJC_SOURCE_FILE  "${GPUPixel_ROOT_DIR}/gpupixel/sink/objc/*")
    list(APPEND SOURCE_CODE_FILES ${OBJC_SOURCE_FILE})
elseif(${CURRENT_OS} STREQUAL "android")
    file(GLOB JNI_SOURCE_FILE  "${GPUPixel_ROOT_DIR}/gpupixel/android/jni/*")
    list(APPEND SOURCE_CODE_FILES ${JNI_SOURCE_FILE})
endif()
 
# 安装头文件，保持目录结构
if(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    # 为macOS和iOS平台设置framework头文件安装路径
    set(FRAMEWORK_INCLUDE_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.framework/Headers")
else()
    set(FRAMEWORK_INCLUDE_DIR "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}")
endif()

# 安装主头文件
install(FILES ${GPUPixel_ROOT_DIR}/gpupixel/gpupixel.h
    DESTINATION ${FRAMEWORK_INCLUDE_DIR}/)

# 定义需要安装的头文件目录列表
set(HEADER_DIRS
    core
    face_detector
    filter
    source
    sink
    utils
)

# 对每个目录执行安装操作
foreach(DIR ${HEADER_DIRS})
    install(DIRECTORY ${GPUPixel_ROOT_DIR}/gpupixel/${DIR}/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/${DIR}
            FILES_MATCHING PATTERN "*.h")
endforeach()
 
# 安装资源文件
install(DIRECTORY 
    ${GPUPixel_ROOT_DIR}/gpupixel/res/
    DESTINATION ${OUTPUT_INSTALL_PATH}/res
)

install(DIRECTORY 
    ${GPUPixel_ROOT_DIR}/third_party/mars-face-kit/models/
    DESTINATION ${OUTPUT_INSTALL_PATH}/models
)


# Library Configuration
# ----------------------------------------
# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCE_CODE_FILES} ${RESOURCE_FILES})

# 为macOS/iOS设置Framework属性
if(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    # Configure macOS/iOS framework properties
    set_target_properties(${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_PRODUCT_NAME ${PROJECT_NAME}
        COMPILE_FLAGS "-x objective-c++"
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER net.pixpark.${PROJECT_NAME}
        PRODUCT_BUNDLE_IDENTIFIER net.pixpark.${PROJECT_NAME}
        CMAKE_XCODE_ATTRIBUTE_BUILT_PRODUCTS_DIR ${PROJECT_NAME}
        MACOSX_FRAMEWORK_INFO_PLIST ${GPUPixel_ROOT_DIR}/gpupixel/Info.plist
        FRAMEWORK_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/
        RESOURCE "${RESOURCE_FILES}"
    )
endif()

# 安装库文件
if(${CURRENT_OS} STREQUAL "linux")
    install(FILES
        ${GPUPixel_ROOT_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.so
        DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )
elseif(${CURRENT_OS} STREQUAL "windows")
    install(FILES
        ${GPUPixel_ROOT_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/msvc-x64/mars-face-kit.dll
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

# Platform specific library configuration
if(${CURRENT_OS} STREQUAL "linux")
    # Configure Linux specific settings
    add_library(mars-face-kit SHARED IMPORTED)
    set_target_properties(mars-face-kit PROPERTIES IMPORTED_LOCATION
        ${GPUPixel_ROOT_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.so)

elseif(${CURRENT_OS} STREQUAL "windows")
    # Configure Windows specific settings
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")

elseif(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    # 已经在前面设置了Framework属性，这里不再重复设置
    
elseif(${CURRENT_OS} STREQUAL "android")
    # Configure Android specific settings
    add_library(mars-face-kit SHARED IMPORTED)
    set_target_properties(mars-face-kit PROPERTIES IMPORTED_LOCATION
        ${GPUPixel_ROOT_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/${ANDROID_ABI}/libmars-face-kit.so)
endif()

# Library Dependencies
# ----------------------------------------
if(${CURRENT_OS} STREQUAL "linux")
    target_link_libraries(${PROJECT_NAME} PRIVATE
        GL
        glfw
        mars-face-kit)
elseif(${CURRENT_OS} STREQUAL "windows")
    target_link_libraries(${PROJECT_NAME} 
        opengl32
        ${GPUPixel_ROOT_DIR}/third_party/glfw/libs/msvc-x64/glfw3.lib
        ${GPUPixel_ROOT_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/msvc-x64/mars-face-kit.lib)
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
        "${GPUPixel_ROOT_DIR}/third_party/mnn/libs/${CURRENT_OS}/MNN.framework"
        "${GPUPixel_ROOT_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.a"
    )
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
        "${GPUPixel_ROOT_DIR}/third_party/mnn/libs/${CURRENT_OS}/MNN.framework"
        "${GPUPixel_ROOT_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.a"
    )
elseif(${CURRENT_OS} STREQUAL "android")
    target_link_libraries(${PROJECT_NAME}
        log
        android
        GLESv3
        EGL
        jnigraphics
        mars-face-kit)
endif()