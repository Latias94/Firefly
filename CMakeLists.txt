cmake_minimum_required(VERSION 3.16)

if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

project(Firefly)

# vendor
add_subdirectory(Firefly/vendor/GLFW)
add_subdirectory(Firefly/vendor/glad)
add_subdirectory(Firefly/vendor/imgui)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Firefly)
add_subdirectory(Sandbox)
add_subdirectory(FireflyEditor)