cmake_minimum_required(VERSION 3.11...3.16)

project(VIZ_ISABEL
    VERSION 0.1
    DESCRIPTION "VIZ ISABEL"
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)

file(GLOB_RECURSE VIZ_SRC "src/*.cpp" "src/*.h")
set(VIZ_INCLUDE 
    "src"
    "extern/glm"
    "extern/glad/include"
    "extern/glfw/include"
)

add_compile_definitions(SHADERS_SRC_LOC="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
add_compile_definitions(DATA_LOC="${CMAKE_CURRENT_SOURCE_DIR}/data/")

add_executable(viz ${VIZ_SRC})
target_include_directories(viz PUBLIC ${VIZ_INCLUDE})
target_link_libraries(viz PUBLIC glm)
target_link_libraries(viz PUBLIC glfw)
target_link_libraries(viz PUBLIC glad)