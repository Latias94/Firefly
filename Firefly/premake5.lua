project "Firefly"
    location "Firefly"
    kind "SharedLib"

    language "C++"
    cppdialect "C++17"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ffpch.h"
    pchsource "src/ffpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{includeDir.spdlog}",
        "%{includeDir.GLFW}",
        "%{includeDir.glad}"
    }

    links (linkLibs)

    filter "system:windows"
        staticruntime (staticRuntime)
        systemversion "latest"

        defines
        {
            "FF_BUILD_DLL",
            "FF_PLATFORM_WINDOWS",
            "_WIN32"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "system:linux" --not working
        pic "on"
        systemversion "latest"
        staticruntime "on"

    filter "configurations:Debug"
        defines "FF_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "FF_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "FF_DIST"
        optimize "On"