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
