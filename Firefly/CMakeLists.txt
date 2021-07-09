file(GLOB_RECURSE Firefly_source_files "src/Firefly/*.cpp")
file(GLOB_RECURSE Firefly_header_files "src/Firefly/*.h")

set(FF_CROSS_PLATFORM_FILES
        "src/Firefly.h"
        "src/ffpch.cpp"
        "src/ffpch.h"

        ${Firefly_source_files}
        ${Firefly_header_files}
        )

file(GLOB_RECURSE win32_platform_source_files "src/Platform/Windows/*.cpp")
file(GLOB_RECURSE win32_platform_header_files "src/Platform/Windows/*.h")

file(GLOB_RECURSE linux_platform_source_files "src/Platform/Linux/*.cpp")
file(GLOB_RECURSE linux_platform_header_files "src/Platform/Linux/*.h")

file(GLOB_RECURSE opengl_platform_source_files "src/Platform/OpenGL/*.cpp")
file(GLOB_RECURSE opengl_platform_header_files "src/Platform/OpenGL/*.h")

if(WIN32)
    set(FF_OS_FILES ${win32_platform_source_files} ${win32_platform_header_files})
elseif("${CMAKE_SYSTEM}" MATCHES "Linux")
    set(FF_OS_FILES ${linux_platform_source_files} ${linux_platform_header_files})
endif()

add_library(Firefly STATIC ${FF_CROSS_PLATFORM_FILES} ${FF_OS_FILES}
        ${opengl_platform_source_files}
        ${opengl_platform_header_files}
        )

target_precompile_headers(Firefly PUBLIC "src/ffpch.h")

target_include_directories(Firefly PRIVATE
        "vendor/GLFW/include"
        "vendor/glad/include"
        )

target_include_directories(Firefly PUBLIC
        "src"
        "vendor"
        "vendor/spdlog/include"
        "vendor/imgui"
        "vendor/glm"
        )

target_compile_definitions(
        Firefly

        PRIVATE
        "_CRT_SECURE_NO_WARNINGS"
        "GLFW_INCLUDE_NONE"
        "FF_PLATFORM_WINDOWS"

        PUBLIC
        $<$<CONFIG:Debug>:FF_DEBUG>
        $<$<CONFIG:Release>:FF_RELEASE>

        $<$<CONFIG:RelWithDebInfo>:FF_DEBUG>
        $<$<CONFIG:MinSizeRel>:FF_RELEASE>
)

target_link_libraries(Firefly PRIVATE glfw glad)
target_link_libraries(Firefly PUBLIC imgui)
