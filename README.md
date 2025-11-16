# SDL-Emscripten-Sample

Sample on how to use SDL3 along with Vcpkg, CMake, Ninja and Emscripten

## Dependencies

### Ninja
[Ninja](https://ninja-build.org/) is a small and fast build system focused on speed.  
It is often used as the backend generator for CMake to significantly accelerate incremental builds.

### CMake
[CMake](https://cmake.org/) is a cross-platform build system generator.  
It allows you to define your project configuration once and generate builds for different platforms and toolchains—including Emscripten.

### Vcpkg
[Vcpkg](https://vcpkg.io/) is a cross-platform C/C++ package manager.  
It enables you to easily install and integrate libraries like SDL3 into your CMake project using `vcpkg install` and toolchain integration.

### Emscripten
[Emscripten](https://emscripten.org/) is a toolchain for compiling C/C++ to WebAssembly or asm.js.  
It provides an LLVM-based compiler (`emcc`) and ports of common libraries, allowing native applications—such as those using SDL3—to run in the browser.

---

# Building on Windows (Emscripten + SDL3 + CMake + Ninja + Vcpkg)

This guide explains how to build the SDL-Emscripten sample on **Windows** using **PowerShell**, **C++**, **CMake**, **Ninja**, **Vcpkg**, **SDL3**, and the **Emscripten SDK (emsdk)**.

All file paths shown are examples. Replace them with the correct paths on your machine.

---

## 1. Enable PowerShell Script Execution

```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope Process
```

---

## 2. Install & Activate Emscripten

```powershell
cd path/to/emsdk
./emsdk install latest
./emsdk activate latest
./emsdk_env.ps1
```

---

## 3. Configure the Project With CMake for Emscripten

```powershell
emcmake cmake -S . -B build-emscripten `
  -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="path/to/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" `
  -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=wasm32-emscripten `
  -DCMAKE_BUILD_TYPE=Debug `
  -G Ninja
```

---

## 4. Build the Project

```powershell
cd build-emscripten
ninja
```

---

## 5. Run the Build via Local HTTP Server

```powershell
cd bin/Debug
python -m http.server 8000
```

Now open the following in your browser:

```
http://localhost:8000/
```

---


