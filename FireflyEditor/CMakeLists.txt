file(GLOB_RECURSE firefly_editor_source_files "src/*.cpp")
file(GLOB_RECURSE firefly_editor_header_files "src/*.h")

add_executable(FireflyEditor
        ${firefly_editor_source_files}
        ${firefly_editor_header_files}
        )

target_include_directories(FireflyEditor PRIVATE
        "src"
        )

set_property(TARGET FireflyEditor PROPERTY
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/FireflyEditor")

target_link_libraries(FireflyEditor PRIVATE Firefly)

file(GLOB assets "assets/*")
if (assets)
    if (UNIX)
        add_custom_command(
                TARGET FireflyEditor
                PRE_BUILD
                COMMAND "${CMAKE_COMMAND}" -E
                create_symlink "${CMAKE_SOURCE_DIR}/FireflyEditor/assets" $<TARGET_FILE_DIR:FireflyEditor>/assets
        )
    else ()
        add_custom_command(
                TARGET FireflyEditor
                POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E
                copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" $<TARGET_FILE_DIR:FireflyEditor>/assets
        )
    endif ()
endif ()