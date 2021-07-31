file(GLOB_RECURSE sandbox_source_files "src/*.cpp")
file(GLOB_RECURSE sandbox_header_files "src/*.h")

add_executable(Sandbox
        ${sandbox_source_files}
        ${sandbox_header_files}
        )

set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Sandbox")

target_compile_definitions(
        Sandbox

        PRIVATE
        "FF_PLATFORM_WINDOWS"
)

target_link_libraries(Sandbox PRIVATE Firefly)

file(GLOB assets "assets/*")
if (assets)
    if (UNIX)
        add_custom_command(
                TARGET Sandbox
                PRE_BUILD
                COMMAND "${CMAKE_COMMAND}" -E
                create_symlink "${CMAKE_SOURCE_DIR}/Sandbox/assets" $<TARGET_FILE_DIR:Sandbox>/assets
        )
    else ()
        add_custom_command(
                TARGET Sandbox
                POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E
                copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" $<TARGET_FILE_DIR:Sandbox>/assets
        )

    endif ()

endif ()