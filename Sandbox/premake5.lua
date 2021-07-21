project "Sandbox"
    kind "ConsoleApp"

    language "C++"
    cppdialect "C++17"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs (clientIncludes)

    links "Firefly"

    filter "system:windows"
        staticruntime (staticRuntime)
        systemversion "latest"

        defines
        {
            "FF_PLATFORM_WINDOWS",
        }

    filter "system:linux" --not working
		pic "on"
		systemversion "latest"
		staticruntime "on"

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