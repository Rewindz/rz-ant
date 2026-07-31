cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(langtonant)

set(FETCHCONTENT_QUIET OFF)
include(FetchContent)

FetchContent_Declare(glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw.git"
  GIT_TAG 3.4
  GIT_SHALLOW ON
)

FetchContent_Declare(imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui"
  GIT_TAG "v1.92.9"
  GIT_SHALLOW ON
)

FetchContent_MakeAvailable(glfw imgui)

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_library(libimgui STATIC)
target_sources(libimgui PRIVATE ${IMGUI_SOURCES})
target_include_directories(libimgui PUBLIC
  ${imgui_SOURCE_DIR}
  ${glfw_SOURCE_DIR}/include
)


set(PROJ_SRC
  "main.cpp"
  "glad.c"
  "Menu.cpp"
)

list(TRANSFORM PROJ_SRC PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(langtonant)

target_sources(langtonant PRIVATE ${PROJ_SRC})
target_include_directories(langtonant PRIVATE
"${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(langtonant PRIVATE glfw libimgui)
