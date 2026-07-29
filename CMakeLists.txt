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

FetchContent_MakeAvailable(glfw)

set(PROJ_SRC
  "main.cpp"
  "glad.c"
)

list(TRANSFORM PROJ_SRC PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(langtonant)

target_sources(langtonant PRIVATE ${PROJ_SRC})
target_include_directories(langtonant PRIVATE
"${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(langtonant PRIVATE glfw)
