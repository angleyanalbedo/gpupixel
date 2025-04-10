# Basic Project Configuration
# ----------------------------------------
cmake_minimum_required(VERSION 3.10)

# 设置CMake策略CMP0177
if(POLICY CMP0177)
  cmake_policy(SET CMP0177 NEW)
endif()

# 设置安装位置为输出目录
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../output)

# Set C++ standard and compiler options
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Set cmake module path to current directory
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

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
set(OUTPUT_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../output")
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
    # Project core modules
    ${CMAKE_CURRENT_SOURCE_DIR}
    # # Third-party dependencies
    # ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include
    # ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libyuv/include
)

# Collect source files
file(GLOB SOURCE_FILES     
    "${CMAKE_CURRENT_SOURCE_DIR}/core/*"        
    "${CMAKE_CURRENT_SOURCE_DIR}/filter/*"         
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*"       
    "${CMAKE_CURRENT_SOURCE_DIR}/sink/*"                               
    "${CMAKE_CURRENT_SOURCE_DIR}/face_detector/*"                 
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*"                 
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libyuv/source/*"
)

# Collect public headers for export
file(GLOB EXPORT_HEADER 
    "${CMAKE_CURRENT_SOURCE_DIR}/core/*.h"         
    "${CMAKE_CURRENT_SOURCE_DIR}/filter/*.h"         
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"       
    "${CMAKE_CURRENT_SOURCE_DIR}/sink/*.h"                      
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h"                 
    "${CMAKE_CURRENT_SOURCE_DIR}/face_detector/*.h"      
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include/*"
)

# Collect resource files
file(GLOB RESOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/res*"  
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mars-face-kit/mod*"                     
)

message(STATUS "RESOURCE_FILES: ${RESOURCE_FILES}")
# Platform Specific Configuration
# ----------------------------------------
if(${CURRENT_OS} STREQUAL "windows")     
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    file(GLOB GLAD_SOURCE_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/src/*.c" )
    list(APPEND SOURCE_FILES ${GLAD_SOURCE_FILE})
elseif(${CURRENT_OS} STREQUAL "linux")    
    file(GLOB GLAD_SOURCE_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/src/*.c" )
    list(APPEND SOURCE_FILES ${GLAD_SOURCE_FILE})
elseif(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    file(GLOB OBJC_SOURCE_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/sink/objc/*")
    list(APPEND SOURCE_FILES ${OBJC_SOURCE_FILE})
elseif(${CURRENT_OS} STREQUAL "android")
    file(GLOB JNI_SOURCE_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/android/jni/*")
    list(APPEND SOURCE_FILES ${JNI_SOURCE_FILE})
endif()

# Library Configuration
# ----------------------------------------
# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${RESOURCE_FILES})

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
        MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        FRAMEWORK_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/
        RESOURCE "${RESOURCE_FILES}"
    )
endif()

# 安装头文件，保持目录结构
if(${CURRENT_OS} STREQUAL "macos" OR ${CURRENT_OS} STREQUAL "ios")
    # 为macOS和iOS平台设置framework头文件安装路径
    set(FRAMEWORK_INCLUDE_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.framework/Headers")
    
    # macOS/iOS平台安装头文件到framework内，保持目录结构
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/core/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/core
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/filter/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/filter
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/source/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/source
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sink/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/sink
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/utils/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/utils
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/face_detector/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/face_detector
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/third_party/glfw/include
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/third_party/glad/include
            FILES_MATCHING PATTERN "*.h")

    # 添加安装stb_image.h的命令
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb/
            DESTINATION ${FRAMEWORK_INCLUDE_DIR}/third_party/stb
            FILES_MATCHING PATTERN "*.h")
            
    # 针对macOS/iOS平台单独添加特殊目录
    install(DIRECTORY 
        ${CMAKE_CURRENT_SOURCE_DIR}/sink/objc/
        DESTINATION ${FRAMEWORK_INCLUDE_DIR}/sink/objc
        FILES_MATCHING PATTERN "*.h"
    )
    endif()
    # Windows和Linux平台安装头文件到output/include目录
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/core/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/core
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/filter/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/filter
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/source/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/source
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sink/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/sink
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/utils/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/utils
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/face_detector/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/face_detector
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/third_party/glfw/include
            FILES_MATCHING PATTERN "*.h")
            
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/third_party/glad/include
            FILES_MATCHING PATTERN "*.h")

    # 添加安装stb_image.h的命令
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/third_party/stb
            FILES_MATCHING PATTERN "*.h")

    # 针对Android平台单独添加特殊目录
    if(${CURRENT_OS} STREQUAL "android")
        install(DIRECTORY 
            ${CMAKE_CURRENT_SOURCE_DIR}/android/jni/
            DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/android/jni
            FILES_MATCHING PATTERN "*.h"
        )
    endif()


# 安装资源文件
install(DIRECTORY 
    ${CMAKE_CURRENT_SOURCE_DIR}/res/
    DESTINATION ${OUTPUT_INSTALL_PATH}/res
)

install(DIRECTORY 
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mars-face-kit/models/
    DESTINATION ${OUTPUT_INSTALL_PATH}/models
)

# 安装库文件
if(${CURRENT_OS} STREQUAL "linux")
    install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.so
        DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )
elseif(${CURRENT_OS} STREQUAL "windows")
    install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/msvc-x64/mars-face-kit.dll
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
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.so)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/${ANDROID_ABI}/libmars-face-kit.so)
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
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/libs/msvc-x64/glfw3.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/msvc-x64/mars-face-kit.lib)
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
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mnn/libs/${CURRENT_OS}/MNN.framework"
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.a"
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
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mnn/libs/${CURRENT_OS}/MNN.framework"
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mars-face-kit/libs/${CURRENT_OS}/libmars-face-kit.a"
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