cmake_minimum_required(VERSION 3.0...3.25)
project(
    WebGPU-Demo # name of the project, which will also be the name of the visual studio solution if you use it
    VERSION 0.1.0 # any version number
    LANGUAGES CXX C # programming languages used by the project
)

# Set the Compiler Flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")

# Include wgpu-native directory, to define the 'wgpu-native' target
add_executable(App
	src/main.cpp
	src/app.cpp
	src/window.cpp
	src/mesh.cpp
	src/env/gpuenv.cpp
	src/graphics/instance.cpp
	src/graphics/request.cpp
	src/graphics/inspect.cpp
	src/graphics/shader.cpp
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
	src/error/callback.cpp
	src/util/freader.cpp
	src/glfw3webgpu/glfw3webgpu.c
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
	target_link_options(App PRIVATE -sASYNCIFY -sUSE_GLFW=3)
endif()

target_include_directories(App PRIVATE
	${MINGW}/include
	include
	include/env
	include/graphics
	include/structs
	include/util
	include/error
	include/glfw3webgpu
	external/include
)

target_link_directories(App PRIVATE
	${MINGW}/lib
	external/lib
)

# Add the 'webgpu' target as a dependency of our App
target_link_libraries(App PRIVATE
	gcc
	stdc++
	m
	glfw3
	wgpu_native
)

# The application's binary must find wgpu.dll or libwgpu.so at runtime,
# so we automatically copy it (it's called WGPU_RUNTIME_LIB in general)
# next to the binary.
