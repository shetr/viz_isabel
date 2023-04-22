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

add_executable(viz ${VIZ_SRC})
target_link_libraries(viz PUBLIC glm)
target_link_libraries(viz PUBLIC glfw)
target_link_libraries(viz PUBLIC glad)