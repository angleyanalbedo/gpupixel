---

title: Build
editLink: true
description: This article will introduce the compilation methods for the GPUPixel library on various system platforms.
---


# Build

This chapter will introduce the compilation methods for the GPUPixel library on various system platforms.

::: tip
Starting from [v1.3.0-beta](https://github.com/pixpark/gpupixel/releases/tag/v1.3.0-beta) version, the Vnn-related dependency libraries have been removed. The latest compiled libraries for various platforms can be found [here](https://github.com/pixpark/gpupixel/releases/latest).
:::

## iOS
Execute the following commands at the root directory of the project.

**Generate Project**
::: code-group
```bash [Arm64]
cmake -G Xcode -B build -S src -DCMAKE_TOOLCHAIN_FILE=../toolchain/ios.toolchain.cmake -DPLATFORM=OS64 -DCMAKE_BUILD_TYPE=Release
```
:::

**Compile**
::: code-group

```bash [Release]
cmake --build build --config Release
cmake --install build
```

```bash [Debug]
cmake --build build --config Debug
cmake --install build
```
:::

**Output**

The compilation output is located in the `output` path under the root directory of the project.
For iOS, you only need to use the `.framework` library under `output/libs`, which already contains header files and resource files.

## Mac
Execute the following commands at the root directory of the project.

**Generate Project**
::: code-group
```bash [Apple Silicon]
cmake -G Xcode -B build -S src -DCMAKE_TOOLCHAIN_FILE=../toolchain/ios.toolchain.cmake -DPLATFORM=MAC_ARM64 -DCMAKE_BUILD_TYPE=Release
```
```bash [Intel]
cmake -G Xcode -B build -S src -DCMAKE_TOOLCHAIN_FILE=../toolchain/ios.toolchain.cmake -DPLATFORM=MAC -DCMAKE_BUILD_TYPE=Debug
```
:::

**Compile**
::: code-group

```bash [Release]
cmake --build build --config Release
cmake --install build
```

```bash [Debug]
cmake --build build --config Debug
cmake --install build
```
:::

**Output**

The compilation output is located in the `output` path under the root directory of the project.
 
For MacOS, you only need to use the `.framework` library under `output/libs`, which already contains header files and resource files.

## Android

**Gradle Command Compile**

If the Gradle command environment has been configured, you can also use `gradlew` to compile.

```bash
./gradlew :gpupixel:assembleRelease
```

The output is located at: `src/android/java/gpupixel/build/outputs/aar`

## Windows

Windows compilation requires the following environment:

**Environment Requirements**
- Microsoft Visual Studio >= 2017
- cmake >= 3.10

**Build Environment Setup**

It is recommended to use Windows Terminal and select the corresponding VS environment tab for compilation.
![Windows Terminal](../../image/win-terminal.png)
If not using Windows Terminal, you can configure as follows:

- **64-bit compilation**: Find and click `vcvars64.bat` (x64 Native Tools Command Prompt for VS 2017) in settings to open the VS virtual environment for compiling x64 architecture programs

::: warning
Only supports generating x86_64 Release version, x86_32 is not supported currently
:::

**Generate Project**

```bash
cmake -G "NMake Makefiles" -B build -S src -DCMAKE_BUILD_TYPE=Release
```

**Compile**

```bash
cmake --build build --config Release
cmake --install build
```

**Output**

The compilation output is located in the `output` path under the root directory of the project.

## Linux (Debian or Ubuntu)

**Environment Configuration**

```bash
# install cmake 
sudo apt-get install cmake pkg-config
# install dependent lib
sudo apt-get install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev libglfw3-dev
```

**Generate Project**
::: code-group

```bash [Release]
cmake -B build -S src -DCMAKE_BUILD_TYPE=Release
```

```bash [Debug]
cmake -B build -S src -DCMAKE_BUILD_TYPE=Debug
```
:::

**Compile**

::: code-group

```bash [Release]
cmake --build build --config Release
cmake --install build
```

```bash [Debug]
cmake --build build --config Debug
cmake --install build
```
:::


**Output**

The compilation output is located in the `output` path under the root directory of the project.