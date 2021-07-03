file(GLOB_RECURSE Firefly_source_files "src/Firefly/*.cpp")
file(GLOB_RECURSE Firefly_header_files "src/Firefly/*.h")

set(FF_CROSS_PLATFORM_FILES
        "src/Firefly.h"
        "src/ffpch.cpp"
        "src/ffpch.h"

        ${Firefly_source_files}
        ${Firefly_header_files}
        )

add_library(Firefly STATIC ${FF_CROSS_PLATFORM_FILES})

target_precompile_headers(Firefly PUBLIC "src/ffpch.h")

target_include_directories(Firefly PUBLIC
        "src"
        "vendor"
        "vendor/spdlog/include"
        )

target_compile_definitions(
        Firefly

        PRIVATE
        "_CRT_SECURE_NO_WARNINGS"
        "GLFW_INCLUDE_NONE"

        PUBLIC
        $<$<CONFIG:Debug>:FF_DEBUG>
        $<$<CONFIG:Release>:FF_RELEASE>

        $<$<CONFIG:RelWithDebInfo>:FF_DEBUG>
        $<$<CONFIG:MinSizeRel>:FF_RELEASE>
)

