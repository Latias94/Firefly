project "Firefly"
    kind "StaticLib"

    language "C++"
    cppdialect "C++17"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ffpch.h"
    pchsource "src/ffpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "%{includeDir.glm}/glm/**.hpp", -- header based library
        "%{includeDir.glm}/glm/**.inl",

        -- stb
        "vendor/stb/stb_image.h",
        "vendor/stb/stb_image.cpp",

        "vendor/stb/stb_rectpack.h",
        "vendor/stb/stb_rectpack.cpp",

        "vendor/stb/stb_sprintf.h",
        "vendor/stb/stb_sprintf.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "src",
        "%{includeDir.spdlog}",
        "%{includeDir.GLFW}",
        "%{includeDir.glad}",
        "%{includeDir.imgui}",
        "%{includeDir.glm}",
        "%{includeDir.stb}",
    }

    links (linkLibs)

    filter "system:windows"
        staticruntime (staticRuntime)
        systemversion "latest"

        defines
        {
            "FF_BUILD_DLL",
        }

    filter "system:linux" --not working
        pic "on"
        systemversion "latest"

    filter "configurations:Debug"
        defines "FF_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "FF_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "FF_DIST"
        runtime "Release"
        optimize "on"