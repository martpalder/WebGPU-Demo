cmake_minimum_required(VERSION 3.0...3.25)
project(
    WebGPU-Demo		# name of the project, which will also be the name of the visual studio solution if you use it
    VERSION 0.1.0	# any version number
    LANGUAGES C CXX	# programming languages used by the project
)

# Set the Compiler Flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")

set(INCLUDE_DIRS
    ${MINGW}/include
    ${MINGW}/lib/gcc/x86_64-w64-mingw32/13.2.0/include
    ${MINGW}/lib/gcc/x86_64-w64-mingw32/13.2.0/include/c++
    pch
    include
    include/env
    include/graphics
    include/structs
    include/managers
    include/loaders
    include/util
    include/error
    external/linmath
    external/stb
)

set(HEADER_FILES
    pch/stdafx.h
    include/app.hpp
    include/window.hpp
    include/input.hpp
    include/world.hpp
    include/actor.hpp
    include/camera.hpp
    include/graphics/instance.hpp
    include/graphics/request.hpp
    include/graphics/inspect.hpp
    include/graphics/shader.hpp
    include/graphics/mesh.hpp
    include/graphics/texture.hpp
    include/graphics/bind.hpp
    include/graphics/pipeline.hpp
    include/graphics/buffer.hpp
    include/graphics/view.hpp
    include/structs/desc.hpp
    include/structs/config.hpp
    include/structs/attach.hpp
    include/structs/state.hpp
    include/structs/attrib.hpp
    include/structs/layout.hpp
    include/env/gpuenv.hpp
    include/managers/mesh_manager.hpp
    include/loaders/obj_loader.hpp
    include/util/mymath.h
    include/util/freader.hpp
    include/error/callback.hpp
    include/glfw3webgpu/glfw3webgpu.h
    external/stb/stb_image.h
)

# Include wgpu-native directory, to define the 'wgpu-native' target
set(SOURCE_FILES
    pch/stdafx.cpp
    src/main.cpp
    src/app.cpp
    src/window.cpp
    src/input.cpp
    src/world.cpp
    src/actor.cpp
    src/camera.cpp
    src/graphics/instance.cpp
    src/graphics/request.cpp
    src/graphics/inspect.cpp
    src/graphics/shader.cpp
    src/graphics/mesh.cpp
    src/graphics/texture.cpp
    src/graphics/bind.cpp
    src/graphics/pipeline.cpp
    src/graphics/buffer.cpp
    src/graphics/view.cpp
    src/structs/desc.cpp
    src/structs/config.cpp
    src/structs/attach.cpp
    src/structs/state.cpp
    src/structs/attrib.cpp
    src/structs/layout.cpp
    src/env/gpuenv.cpp
    src/managers/mesh_manager.cpp
    src/loaders/obj_loader.cpp
    src/util/freader.cpp
    src/error/callback.cpp
    src/glfw3webgpu/glfw3webgpu.c
    external/stb/stb_image.h
)

add_executable(App
    ${SOURCE_FILES}
)

set_target_properties(App PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR OFF
)

if (MSVC)
    target_compile_options(App PRIVATE /W4)
else()
    target_compile_options(App PRIVATE -Wall -Wextra -pedantic)
endif()

if (XCODE)
    set_target_properties(App PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal"
    )
endif()

# Options that are specific to Emscripten
if (EMSCRIPTEN)
    # Generate a full web page rather than a simple WebAssembly module
    set_target_properties(App PROPERTIES SUFFIX ".html")
    set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
    # Enable the use of emscripten_sleep()
    add_library(glfw INTERFACE)
    target_link_options(App PRIVATE "-sASYNCIFY" "-sUSE_GLFW=3")
    # Add the 'emscripten' includes
    target_include_directories(App PRIVATE
        external/emscripten/include
    )
    # Add the 'emscripten' libs
    target_link_directories(App PRIVATE
        external/emscripten/lib
    )
else()
    target_include_directories(App PRIVATE
        external/glfw/include
        external/wgpu-native/include
        external/glfw3webgpu
    )
    target_link_directories(App PRIVATE
        external/glfw/lib-mingw-w64
        external/wgpu-native/lib
        external/glfw3webgpu
    )

    # Add the 'glfw3' and 'webgpu' target as a dependency of our App
    target_link_libraries(App PRIVATE
        glfw3 wgpu_native
    )
endif()

# Define the header-only library
add_library(stb_image INTERFACE)

# Specify the include directories
target_include_directories(stb_image INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h
)
target_include_directories(App PRIVATE
    ${INCLUDE_DIRS}
)

target_link_directories(App PRIVATE
    ${MINGW}/lib
    external/lib
)

# Link the libraries
target_link_libraries(App PRIVATE gcc stdc++ m stb_image)

# Add Rust Debug Macro
target_compile_definitions(App PRIVATE RUST_BACKTRACE=full)

# The application's binary must find wgpu.dll or libwgpu.so at runtime,
# so we automatically copy it (it's called WGPU_RUNTIME_LIB in general)
# next to the binary.

# Use Precompiled Headers
#target_precompile_headers(App PRIVATE "pch/stdafx.h")
