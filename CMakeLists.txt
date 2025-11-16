cmake_minimum_required(VERSION 3.20)

project(3dentt LANGUAGES CXX)

if(EMSCRIPTEN)
    message(STATUS "Building for WebAssembly / Emscripten")
else()
    message(STATUS "Building for native platform")
endif()

if(CMAKE_CROSSCOMPILING)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()

# --- Output directories relative to the build directory ---
# Works for single-config generators (Ninja, Make, etc.)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

# --- Handle multi-config generators (Visual Studio, Xcode, etc.) ---
if(CMAKE_CONFIGURATION_TYPES)
    foreach(OUTPUTCONFIG IN LISTS CMAKE_CONFIGURATION_TYPES)
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
    endforeach()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
# --- SDL3 setup ---
# For native builds (MSVC / MinGW), use find_package
#if (NOT EMSCRIPTEN)
    find_package(SDL3 CONFIG REQUIRED)
#endif()

add_executable(app src/main.cpp)

if (EMSCRIPTEN)
    set_target_properties(app PROPERTIES
       LINK_FLAGS "-sUSE_SDL=3 -sFULL_ES3=1 -sASSERTIONS=2 -sUSE_WEBGL2=1 -sALLOW_MEMORY_GROWTH=1 -sWASM=1"
    #    LINK_FLAGS "-sALLOW_MEMORY_GROWTH=1 -sWASM=1 -sEXPORTED_FUNCTIONS=[\"_main\"] -sEXPORTED_RUNTIME_METHODS=[\"cwrap\"]"
        SUFFIX ".html"
    )
    # target_link_libraries(app PRIVATE fmt::fmt CLI11::CLI11)
    target_link_libraries(app PRIVATE fmt::fmt SDL3::SDL3 CLI11::CLI11)
else()
    target_link_libraries(app PRIVATE fmt::fmt SDL3::SDL3 CLI11::CLI11)
endif()

if (NOT EMSCRIPTEN)
    # --- Copy vcpkg DLLs next to the executable automatically ---
    if (WIN32)
        add_custom_command(TARGET app POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:app> $<TARGET_FILE_DIR:app>
            COMMAND_EXPAND_LISTS
        )
    endif()
endif()