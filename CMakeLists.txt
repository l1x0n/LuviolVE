cmake_minimum_required(VERSION 3.28)
project(luviolVE VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

find_package(SDL3 REQUIRED CONFIG)

add_library(glad external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(imgui 
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)

add_executable(luviolVE WIN32 src/main.cpp src/window.cpp src/editorUI.cpp)

target_include_directories(luviolVE PRIVATE include)

target_link_libraries(luviolVE PRIVATE glad imgui SDL3::SDL3)
