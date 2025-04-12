# Basic Project Configuration
# ----------------------------------------
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

# Set cmake module path to current directory for finding custom modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Define and initialize application project
set(APP_PROJECT_NAME "app")
project(${APP_PROJECT_NAME})

# Resource and Runtime Directory Configuration
# ----------------------------------------
link_directories(${CMAKE_INSTALL_LIBDIR})

# Include Directories Configuration
# Add all necessary include paths for header files
# ----------------------------------------
include_directories(
    # Project headers
    ${CMAKE_INSTALL_INCLUDEDIR}
)

# Add system-specific include paths
if(${CURRENT_OS} STREQUAL "windows")
    # For windows systems, use bundled GLFW
    include_directories(
        ${GPUPIXEL_ROOT_DIR}/third_party/glfw/include
    )
endif()

# Common third-party includes for all platforms
include_directories(
    ${GPUPIXEL_ROOT_DIR}/
    ${GPUPIXEL_ROOT_DIR}/third_party/stb
    ${GPUPIXEL_ROOT_DIR}/third_party/glad/include
    ${GPUPIXEL_ROOT_DIR}/third_party/imgui
    ${GPUPIXEL_ROOT_DIR}/third_party/imgui/backends
)

# Source Files Configuration
# ----------------------------------------
# Add desktop application source files
file(GLOB APP_SOURCE_FILES
    "${GPUPIXEL_ROOT_DIR}/demo/desktop/*"
)

# Add ImGui source files
file(GLOB IMGUI_SOURCE_FILES
    "${GPUPIXEL_ROOT_DIR}/third_party/imgui/*.cpp"
    "${GPUPIXEL_ROOT_DIR}/third_party/imgui/backends/imgui_impl_glfw.cpp"
    "${GPUPIXEL_ROOT_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp"
)
list(APPEND APP_SOURCE_FILES ${IMGUI_SOURCE_FILES})

# Platform Specific Configuration
# Add platform-specific source files and settings
# ----------------------------------------
if(${CURRENT_OS} STREQUAL "windows")
    # Configure MSVC runtime library
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")
    
    # Add Windows-specific library paths
    link_directories(
        ${GPUPIXEL_ROOT_DIR}/third_party/glfw/lib-mingw-w64)

    # Add GLAD source files for Windows
    file(GLOB GLAD_SOURCE_FILE "${GPUPIXEL_ROOT_DIR}/third_party/glad/src/*.c")
    list(APPEND APP_SOURCE_FILES ${GLAD_SOURCE_FILE})
elseif(${CURRENT_OS} STREQUAL "linux")
   # Add GLAD source files for Linux
   file(GLOB GLAD_SOURCE_FILE "${GPUPIXEL_ROOT_DIR}/third_party/glad/src/*.c")
   list(APPEND APP_SOURCE_FILES ${GLAD_SOURCE_FILE})
endif()

# Build Configuration
# ----------------------------------------
# Create executable from source files
add_executable(${PROJECT_NAME} ${APP_SOURCE_FILES})

# Library Linkage Configuration
# Configure platform-specific library dependencies
# ----------------------------------------
if(${CURRENT_OS} STREQUAL "linux")
    # Linux dependencies
    target_link_libraries(${PROJECT_NAME}
                        gpupixel
                        GL
                        glfw
                        dl)
    set(DEPS_LIBS ${CMAKE_INSTALL_LIBDIR}/libgpupixel.so
                      ${CMAKE_INSTALL_LIBDIR}/libmars-face-kit.so)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
elseif(${CURRENT_OS} STREQUAL "windows")
    # Windows dependencies
    set(DEPS_LIBS ${CMAKE_INSTALL_LIBDIR}/gpupixel.dll
                      ${CMAKE_INSTALL_LIBDIR}/mars-face-kit.dll)

    target_link_libraries(${PROJECT_NAME}
                        ${CMAKE_INSTALL_LIBDIR}/gpupixel.lib
                        opengl32
                        ${GPUPIXEL_ROOT_DIR}/third_party/glfw/libs/msvc-x64/glfw3.lib)

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
endif()
 
# 安装资源文件
# ----------------------------------------
# 安装资源和模型文件
install(DIRECTORY ${GPUPIXEL_ROOT_DIR}/output/res/
        DESTINATION ${CMAKE_INSTALL_BINDIR}/res)

install(DIRECTORY ${GPUPIXEL_ROOT_DIR}/output/models/
        DESTINATION ${CMAKE_INSTALL_BINDIR}/models)

install(FILES ${GPUPIXEL_ROOT_DIR}/demo/desktop/demo.png
        DESTINATION ${CMAKE_INSTALL_BINDIR})

# 安装依赖库
install(FILES ${DEPS_LIBS}
        DESTINATION ${CMAKE_INSTALL_BINDIR})

# 可执行文件安装
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}  # 对于macOS app bundle
)
