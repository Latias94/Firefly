file(GLOB_RECURSE Firefly_source_files "src/Firefly/*.cpp")
file(GLOB_RECURSE Firefly_header_files "src/Firefly/*.h")

set(FF_CROSS_PLATFORM_FILES
        "src/Firefly.h"
        "src/pch.cpp"
        "src/pch.h"

        ${Firefly_source_files}
        ${Firefly_header_files}
        )

add_library(Firefly STATIC ${FF_CROSS_PLATFORM_FILES})

target_precompile_headers(Firefly PUBLIC "src/pch.h")

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
        $<$<CONFIG:Debug>:OE_DEBUG>
        $<$<CONFIG:Release>:OE_RELEASE>

        $<$<CONFIG:RelWithDebInfo>:OE_DEBUG>
        $<$<CONFIG:MinSizeRel>:OE_RELEASE>
)

